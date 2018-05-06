package grammar_parser;

import syntax_analyze.Parser;
import syntax_analyze.rules.RHS;
import syntax_analyze.rules.Rules;
import syntax_analyze.symbols.Nonterm;
import syntax_analyze.symbols.Symbol;
import syntax_analyze.symbols.Term;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.*;


/**
 * Created by nastasia on 24.04.17.
 */
public class CompilerGenerator extends Parser {
//    private ArrayList<String> terms;//    = syntax_analyze.GrammarStructure.terms;
//    private ArrayList<String> nonterms;// = syntax_analyze.GrammarStructure.nonterms;
//    private Nonterm axiom = new Nonterm("S");//    = syntax_analyze.GrammarStructure.axiom;
//    private RHS[][] q;

    protected HashMap<String, Rules> grammar_list;

    private HashMap<String, HashSet<String>> FIRSTS = new HashMap<>();
    private HashMap<String, HashSet<String>> FOLLOWS = new HashMap<>();
    public CompilerGenerator(ArrayList<String> terms, ArrayList<String> nonterms, Nonterm axiom,
                             HashMap<String, Rules> grammar_list) {
        this.terms    = terms;
        this.nonterms = nonterms;
        this.grammar_list = grammar_list;
        this.axiom = axiom;
        init();
    }

    protected CompilerGenerator() {

    }

    public CompilerGenerator(CompilerGenerator obj) {
        this.terms        = new ArrayList<>(obj.terms);
        this.nonterms     = new ArrayList<>(obj.nonterms);
        this.grammar_list = new HashMap<>(obj.grammar_list);
        this.axiom = obj.axiom;
        init();
    }

    private void init() {
        for (String t: nonterms) {
            FIRSTS.put (t, new HashSet<>());
            FOLLOWS.put(t, new HashSet<>());
        }
        buildFirst();
        buildFollow();
        checkLL1();
        buildDelta();
        log.append("Terms: ").append(terms.toString()).append('\n')
                .append("Nonterms: ").append(nonterms.toString()).append('\n')
                .append("Axiom: ").append(axiom.toString()).append('\n')
                .append("FIRST: ").append(FIRSTS.toString()).append('\n')
                .append("FOLLOWS: ").append(FOLLOWS.toString()).append('\n');
        for (int i = 0; i < nonterms.size(); ++i) {
            for (int j = 0; j < terms.size(); ++j) {
                log.append(String.format("q[%s][%s] = %s\n", nonterms.get(i), terms.get(j),
                        q[i][j] != null ? q[i][j].toString() : "ERROR"));
            }
        }
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
//        System.out.println("FIRST: " + FIRSTS.toString());
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
//        System.out.println("FOLLOW: " + FOLLOWS.toString());
    }

    //--------------------- Проверка на LL(1) ---------------------
/*
Определение. Грамматика G является LL(1) тогда и только тогда, когда для любых двух правил
вида A → u | v выполняется следующее:
1. FIRST (u) ∩ FIRST (v) = ∅;
2. если v ⇒* ε, то FIRST (u) ∩ FOLLOW (A) = ∅.
*/

private boolean checkFirstUFollowA(RHS u, HashSet<String> first_u,
                                   RHS v, HashSet<String> first_v,
                                   String A, HashSet<String> follow_A) {

    HashSet<String> intersection_uA = new HashSet<>(first_u);
    intersection_uA.retainAll(follow_A);
    if (first_v.contains(Term.EPSILON) && !intersection_uA.isEmpty()) {
        StringBuilder local_log = new StringBuilder();
        local_log.append("** Grammar is not LL(1): at " ).append(v.getCoords().toString()).append(' ')
                .append(v.toString()).append(" ⇒* ε")
                .append("and FIRST (").append(u.toString()).append(" at ").append(u.getCoords().toString()).append(' ')
                .append(") ∩ FOLLOW (").append(A).append(") != ∅").append('\n');
        local_log.append("*** FIRST ").append(u.toString()).append(" = ").append(first_u.toString());
        local_log.append("*** FOLLOW ").append(A).append(" = ").append(follow_A.toString()).append('\n');
        System.out.print(local_log);
        log.append(local_log);
//                        System.out.println("*** FIRST");
        return true;
    }
    return false;
}

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
                    HashSet<String> first_v = computeFirst(v);
                    HashSet<String> intersection = new HashSet<>(first_u);
                    intersection.retainAll(first_v);
                    if (!intersection.isEmpty()) {
                        error = true;
                        System.out.println("** Grammar is not LL(1): FIRST (u) ∩ FIRST (v) != ∅ for " +
                        u + " at " + u.getCoords().toString() +  " and " + v + " at " + v.getCoords().toString());
                        System.out.println("*** FIRST "+ u +" = " + computeFirst(u));
                        System.out.println("*** FIRST "+ v +" = " + computeFirst(v));
                    }
                    error |= checkFirstUFollowA(u, first_u, v, first_v, A, follow_A);
                    error |= checkFirstUFollowA(v, first_v, u, first_u, A, follow_A);

//                    HashSet<String> intersection_uA = new HashSet<>(first_u);
//                    HashSet<String> intersection_vA = new HashSet<>(first_v);
//                    intersection_uA.retainAll(follow_A);
//                    intersection_vA.retainAll(follow_A);
//                    if (first_v.contains(Term.EPSILON) && !intersection_uA.isEmpty()) {
//                        System.out.println("** Grammar is not LL(1): at " + v.getCoords().toString()+ " " + v + " ⇒* ε" +
//                                        "and FIRST (" + u + " at " + u.getCoords().toString() +
//                                        ") ∩ FOLLOW ("+ A + ") != ∅");
//                        System.out.println("*** FIRST " + u +" = " + first_u);
//                        System.out.println("*** FOLLOW "+ A +" = " + follow_A);
////                        System.out.println("*** FIRST");
//                        error = true;
//                    }
//                    if (first_u.contains(Term.EPSILON) && !intersection_vA.isEmpty()) {
//                        System.out.println("** Grammar is not LL(1): at " + u.getCoords().toString()+ " " + u + " ⇒* ε and" +
//                                "FIRST (" + v  + " at " + v.getCoords().toString()+ " " +
//                                ") ∩ FOLLOW ("+ A + ") != ∅");
//                        System.out.println("*** FIRST " + v +" = " + first_v);
//                        System.out.println("*** FOLLOW "+ A +" = " + follow_A);
//                        error = true;
//                    }

                }
            }
        }
        if (error) System.exit(3);
    }

    //--------------------- Построение таблицы предсказывающего разбора ---------------------

    private void buildDelta() {
        if (!terms.contains(Term.EOF)) {
            terms.add(Term.EOF);
        }
        int m = nonterms.size();
        int n = terms.size();
        q = new RHS[m][n];
        for (RHS[] line : q) {
            Arrays.fill(line, RHS.ERROR);
        }
        for (Map.Entry<String, Rules> pair: grammar_list.entrySet()) {
            String X = pair.getKey();
            for (RHS rule: pair.getValue()) {
                HashSet<String> chunk_first = computeFirst(rule);
                for (String a: chunk_first) {
                    if (!a.equals(Term.EPSILON)) {
                        q[nonterms.indexOf(X)][terms.indexOf(a)] = rule;
                    } else {
                        for (String b: FOLLOWS.get(X)) {
                            q[nonterms.indexOf(X)][terms.indexOf(b)] = RHS.EPSILON;
                        }
                    }
                }
            }
        }
//        for (int i = 0; i < m; ++i) {
//            for (int j = 0; j < n; ++j) {
//                System.out.printf("rules.put(%s, new Rules(new RHS(", nonterms.get(i));
//            }
//        }
//        for (int i = 0; i < m; ++i) {
//            for (int j = 0; j < n; ++j) {
//                System.out.printf("q[%s][%s] = %s\n", nonterms.get(i), terms.get(j),
//                q[i][j] != null ? q[i][j].toString() : "ERROR");
//            }
//        }

    }

    //--------------------- Вывод в консоль ---------------------
    private StringBuilder printList(ArrayList<String> list)
    {
        StringBuilder res = new StringBuilder(
                "        return new ArrayList<>(Arrays.asList(\n                "
        );
        if (!list.isEmpty()) {
            res.append('"').append(list.get(0)).append('"');
        }
        for (int i = 1; i < list.size(); ++i) {
            res.append(", ").append('"').append(list.get(i)).append('"');
        }
        res.append("\n        ));\n");
        return res;
    }

    public String printCompilerClass(String classname) {
        StringBuilder res = new StringBuilder(
                "import syntax_analyze.rules.RHS;\n" +
                        "import syntax_analyze.Parser;\n" +
                        "import syntax_analyze.symbols.Nonterm;\n" +
                        "import syntax_analyze.symbols.Term;\n\n" +
                        "import java.util.ArrayList;\n" +
                        "import java.util.Arrays;\n\n" +
                        "public class " + classname + " {\n" +
                        "    public final static ArrayList<String> terms = staticTermList();\n" +
                        "    public final static ArrayList<String> nonterms = staticNontermList();\n" +
                        "    public final static Nonterm axiom = " + axiom.printConstructor() + ";\n" +
                        "    public final static RHS[][] q = staticDelta();\n\n" +
                        "    public static Parser getParser() {\n" +
                        "        return new Parser(terms, nonterms, axiom, q);\n" +
                        "    }\n\n"
        );
        //Список нетерминалов
        res.append("    private static ArrayList<String> staticNontermList() {\n")
                .append(printList(nonterms))
                .append("    }\n");

        //Список терминалов
        res.append("    private static ArrayList<String> staticTermList() {\n")
                .append(printList(terms))
                .append("    }\n");;

        //Таблица переходов

        res.append("    private static RHS[][] staticDelta() {\n" +
                "        ArrayList<String> T = terms;\n" +
                "        ArrayList<String> N = nonterms;\n" +
                "        int m = N.size();\n" +
                "        int n = T.size();\n" +
                "        RHS[][] q = new RHS[m][n];\n" +
                "        for (RHS[] line: q) {\n" +
                "            Arrays.fill(line, RHS.ERROR);\n" +
                "        }\n");
        for (int i = 0; i < q.length; ++ i) {
            for (int j = 0; j < q[0].length; ++j) {
                if (!RHS.isError(q[i][j])) {
                    res.append(String.format("        q[%d][%d] = ", i, j))
                            .append(q[i][j].printConstructor()).append(";\n");
                }
            }
        }
        res.append("        return q;\n    }\n");
        //Конец класса
        res.append("}\n");
//        System.out.print(res);
        return res.toString();
    }

    public void toJava(String path) {
        File javafile = new File(path);
        try {
            Files.write(javafile.toPath(), printCompilerClass(javafile.getName().replace(".java", "")).getBytes());
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", javafile.toPath());
        }
    }

}
