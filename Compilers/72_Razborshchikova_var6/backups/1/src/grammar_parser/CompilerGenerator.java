package grammar_parser;

import syntax_analyze.rules.RHS;
import syntax_analyze.rules.Rules;
import syntax_analyze.symbols.Nonterm;
import syntax_analyze.symbols.Symbol;
import syntax_analyze.symbols.Term;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;


/**
 * Created by nastasia on 24.04.17.
 */
public class CompilerGenerator {
    private ArrayList<String> terms;//    = syntax_analyze.GrammarStructure.terms;
    private ArrayList<String> nonterms;// = syntax_analyze.GrammarStructure.nonterms;
    private Nonterm axiom = new Nonterm("S");//    = syntax_analyze.GrammarStructure.axiom;
    private HashMap<String, Rules> grammar_list;

    private HashMap<String, HashSet<String>> FIRSTS = new HashMap<>();
    private HashMap<String, HashSet<String>> FOLLOWS = new HashMap<>();

    public CompilerGenerator(ArrayList<String> terms, ArrayList<String> nonterms, Nonterm axiom,
                             HashMap<String, Rules> grammar_list) {
        this.terms = terms;
        this.nonterms = nonterms;
        this.grammar_list = grammar_list;
        this.axiom = axiom;
        for (String t: nonterms) {
            FIRSTS.put (t, new HashSet<>());
            FOLLOWS.put(t, new HashSet<>());
        }
        buildFirst();
        buildFollow();
        checkLL1();
    }

    //------------------------- FIRST -------------------------

    private HashSet<String> computeFirst(RHS chunk) {
        HashSet<String> res = new HashSet<>();
        if (chunk.equals(RHS.EPSILON)) { //F[eps] = eps
            res.add(Term.EPSILON);
            return res;
        }
        for (Symbol symbol: chunk) {
            if (symbol instanceof Term) { //F[au] = a
                res.add(symbol.getType());
//                System.out.println("    Term -> " + symbol.toString() + res.toString());
                return res;
            }
            //F[Xu] = eps not in F[X] ? F[x] : (F[x]\eps UNION F[u])
            HashSet<String> symbol_first = FIRSTS.get(symbol.getType());
            if (!symbol_first.contains(Term.EPSILON)) {
                res.addAll(symbol_first);
//                System.out.println("    Nonterm no eps -> "+ symbol.toString()  + res.toString());
                return res;
            } else {
                HashSet<String> copy = new HashSet<>(symbol_first);
                copy.remove(Term.EPSILON);
                res.addAll(copy);
            }
        }
        res.add(Term.EPSILON);
//        System.out.println("    ALL EPS? -> " + res.toString());
        return res;
    }

    private void buildFirst() {
        for (Map.Entry<String, Rules> pair: grammar_list.entrySet()) {
            for (RHS chunk: pair.getValue()) {
                if (chunk.isEmpty()) continue;
                Symbol symbol = chunk.get(0);
                if (symbol instanceof Term) { //F[au] = a
                    FIRSTS.get(pair.getKey()).add(symbol.getType());
                }
            }
        }
//        System.out.println("FIRSTS prepare: " + FIRSTS.toString());
        boolean changed;
        do {
            changed = false;
            for (Map.Entry<String, Rules> pair: grammar_list.entrySet()) {
                for (RHS chunk: pair.getValue()) {
//                    System.out.println("Nonterm: " + pair.getKey());
                    changed |= (FIRSTS.get(pair.getKey())).addAll(computeFirst(chunk));
                }
            }
        } while (changed);
        System.out.println("FIRST: " + FIRSTS.toString());
    }

    //------------------------- FOLLOW -------------------------

    private void buildFollow() {
        FOLLOWS.get(axiom.getType()).add(Term.EOF); //Follow[S] = $
        for (Rules rule: grammar_list.values()) {
            for (RHS chunk: rule) { //Если есть правило вывода X -> uYv
                if (chunk.isEmpty()) continue;
//                System.out.println(rule);
                for (int i = 0; i < chunk.size() - 1; ++i) {
                    Symbol symbol = chunk.get(i);
//                    System.out.println("-- " + symbol.getType());
                    if (symbol instanceof Nonterm) {
                        HashSet<String> sublist_first = computeFirst(new RHS(chunk.subList(i+1, chunk.size()))); //все символы из First[v]
                        sublist_first.remove(Term.EPSILON);             //за исключением epsilon
//                        System.out.println("---- " + chunk.get(i+1).getType() + sublist_first);
                        FOLLOWS.get(symbol.getType()).addAll(sublist_first); // добавить в FOLLOW[Y]
                    }
                }
            }
        }
//        System.out.println("FOLLOW prepare: " + FOLLOWS.toString());
        // Пока ничего нельзя будет добавить ни к какому множеству
        //FOLLOW(X): eсли есть правило вывода X → uY или X → uYv, где
        //FIRST(v) содержит ε (т.е. v ⇒* ε), то все символы из FOLLOW(X) добавить к FOLLOW(Y).
        boolean changed;
        do {
            changed = false;
            for (Map.Entry<String, Rules> pair: grammar_list.entrySet()) {
                String X = pair.getKey();
//                System.out.println(pair.toString());
                for (RHS chunk: pair.getValue()) { //Если есть правило вывода X -> ...
                    if (chunk.isEmpty()) continue;
                    int last_elem = chunk.size() - 1;
                    Symbol Y = chunk.get(last_elem);
                    if (Y instanceof Nonterm) { //если есть правило вывода X → uY
                        changed |= FOLLOWS.get(Y.getType()).addAll(FOLLOWS.get(X)); //FOLLOW(Y) += FOLLOW(X).
                    } else {
                        continue;
                    }
                    //Пока правило разбивается на X → uYv, где FIRST(v) содержит ε, добавлять FOLLOW(X) к FOLLOW(Y).
                    for (int i = last_elem-1; i >= 0; --i) {
                        Y = chunk.get(i);
//                        System.out.println("--- " + Y.toString());
                        if (Y instanceof Term) break;
                        HashSet<String> sublist_first = computeFirst(new RHS(chunk.subList(i+1, chunk.size())));
//                        System.out.println("------ " + sublist_first.toString());
                        if (sublist_first.contains(Term.EPSILON)) { //FIRST(v) содержит ε
                            changed |= FOLLOWS.get(Y.getType()).addAll(FOLLOWS.get(X)); //FOLLOW(Y) += FOLLOW(X).
                        } else {
                            break;
                        }
                    }
                }
            }

        } while (changed);
        System.out.println("FOLLOW: " + FOLLOWS.toString());
    }

    //--------------------- Проверка на LL(1) ---------------------
/*
Определение. Грамматика G является LL(1) тогда и только тогда, когда для любых двух правил
вида A → u | v выполняется следующее:
1. FIRST (u) ∩ FIRST (v) = ∅;
2. если v ⇒* ε, то FIRST (u) ∩ FOLLOW (A) = ∅.
*/

    private void checkLL1() {
        boolean error = false;
        for (Map.Entry<String, Rules> entry: grammar_list.entrySet()) {
            String A = entry.getKey();
            HashSet<String> follow_A = FOLLOWS.get(A);
            Rules rules = entry.getValue();
            for (int i = 0; i < rules.size() - 2; ++i) {
                RHS u = rules.get(i);
                HashSet<String> first_u = computeFirst(u);
                for (int j = i + 1; j < rules.size() - 1; ++j) {
                    RHS v = rules.get(j);
                    HashSet<String> first_v = computeFirst(u);
                    HashSet<String> intersection = new HashSet<>(first_u);
                    intersection.retainAll(first_v);
                    if (!intersection.isEmpty()) {
                        error = true;
                        System.out.println("** Grammar is not LL(1): FIRST (u) ∩ FIRST (v) != ∅ for " +
                        u + " at " + u.getCoords().toString() +  " and " + v + " at " + v.getCoords().toString());
                    }
                    HashSet<String> intersection_uA = new HashSet<>(first_u);
                    HashSet<String> intersection_vA = new HashSet<>(first_v);
                    intersection_uA.retainAll(follow_A);
                    intersection_vA.retainAll(follow_A);
                    if (first_v.contains(Term.EPSILON) && !intersection_uA.isEmpty()) {
                        System.out.println("** Grammar is not LL(1): at " + v.getCoords().toString()+ " " + v + " ⇒* ε" +
                                        "and FIRST (" + u + " at " + u.getCoords().toString() +
                                        ") ∩ FOLLOW ("+ A + ") != ∅");
                        error = true;
                    }
                    if (first_u.contains(Term.EPSILON) && !intersection_vA.isEmpty()) {
                        System.out.println("** Grammar is not LL(1): at " + u.getCoords().toString()+ " " + u + " ⇒* ε and" +
                                "FIRST (" + v  + " at " + v.getCoords().toString()+ " " +
                                ") ∩ FOLLOW ("+ A + ") != ∅");
                        error = true;
                    }

                }
            }
        }
        if (error) System.exit(3);
    }

}
