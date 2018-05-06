package grammar_parser;

import lex_analyze.Token;
import semantic_analize.Interpreter;
import syntax_analyze.ParseNode;
import syntax_analyze.rules.Rules;
import syntax_analyze.symbols.Nonterm;
import syntax_analyze.ParseTree;
import syntax_analyze.rules.RHS;
import syntax_analyze.symbols.Symbol;
import syntax_analyze.symbols.Term;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by nastasia on 23.04.17.
 */
public class GrammarInterpreter extends Interpreter {
    private ArrayList<String> terms = new ArrayList<>();//    = syntax_analyze.GrammarStructure.terms;
    private ArrayList<String> nonterms = new ArrayList<>();// = syntax_analyze.GrammarStructure.nonterms;
    private Nonterm           axiom = new Nonterm("S");//    = syntax_analyze.GrammarStructure.axiom;
    private RHS[][] q = null;// = syntax_analyze.GrammarStructure.q;
    private ParseTree tree = null;// = new syntax_analyze.ParseTree(axiom);
    private HashMap<String, Rules> grammar_list = new HashMap<>();

    public GrammarInterpreter (ParseTree parse_tree) {
        super(parse_tree);
        tree = parse_tree;
        interpretTree();
        checkForUndefinedNonterms();

        System.out.println("TERMS: " + terms);
        System.out.println("NONTERMS: " + nonterms);
        System.out.println("AXIOM: " + axiom);
        System.out.println("GRAMMAR: " + grammar_list + "\n");
    }

    public CompilerGenerator getCompilerGenerator() {
        return new CompilerGenerator(terms, nonterms, axiom, grammar_list);
    }

    private void addNonterm(Token token) {
        if (nonterms.contains(token.getImage())) {
            System.out.println("*** Nonterminal <" + token.getImage() + "> " +
                    "defined twice at " + token.coordsToString() + " ***");
            System.exit(1);
        }
        nonterms.add(token.getImage());
        grammar_list.put(token.getImage(), new Rules());
    }

    private void checkForUndefinedNonterms() {
        boolean error = false;
        for (Map.Entry<String, Rules> entry: grammar_list.entrySet()) {
            Rules rule = entry.getValue();
            if (rule.isEmpty()) {
                System.out.println("*** No rules found for nonterminal <" + entry.getKey() + "> ***");
                error = true;
            }
            for (RHS chunk: rule) {
                for (Symbol symbol: chunk) {
                    if (symbol instanceof Nonterm && !nonterms.contains(symbol.getType())) {
                        System.out.println("*** Undefined nonterminal <" + symbol.getType() + "> " +
                                "at " + symbol.coordsToString() + " ***");
                        error = true;
                    }
                }
            }
        }
        if (error) {
            System.exit(2);
        }
    }

    //-------------- СПИСОК НЕТЕРМИНАЛОВ ------------------------------

//  DEF ::= D_PREF AXIOM D_SUFF
    private void scanDEF(ParseNode def) {
        scanDPREF((ParseNode)def.getChildAt(0));
        scanAXIOM((ParseNode)def.getChildAt(1));
        scanDSUCC((ParseNode)def.getChildAt(2));
    }
//  AXIOM ::= '{' N '}'
    private void scanAXIOM(ParseNode axiom) {
        ParseNode axiom_name = (ParseNode)axiom.getChildAt(1);
        Token symbol = (Token)axiom_name.getSymbol();
        addNonterm(symbol);
        this.axiom = new Nonterm(symbol.getImage());
    }

//  D_PREF ::= N ',' D_PREF | eps
    private void scanDPREF(ParseNode prefix) {
        while (!prefix.isLeaf()) {
            Symbol symbol = ((ParseNode)prefix.getChildAt(0)).getSymbol();
            addNonterm((Token)symbol);
            prefix = (ParseNode)prefix.getChildAt(2);
        }
    }

//  D_SUFF ::= ',' N D_SUFF | eps
    private void scanDSUCC(ParseNode suffix) {
        while (!suffix.isLeaf()) {
            Symbol symbol = ((ParseNode)suffix.getChildAt(1)).getSymbol();
            addNonterm((Token)symbol);
            suffix = (ParseNode)suffix.getChildAt(2);
        }
    }

    //-------------- СПИСОК ПРАВИЛ ------------------------------
//    R_LST ::= R R_LST | eps
    private void scanRulesList(ParseNode rule) {
        while (!rule.isLeaf()) {
            scanRule((ParseNode)rule.getChildAt(0));
            rule = (ParseNode)rule.getChildAt(1);
        }
    }

//    R  ::= '[' N ':' E Elst ']'
    private void scanRule(ParseNode rule) {
        String N = ((Token)(rule.getSymbolAt(1))).getImage();
        if (grammar_list.containsKey(N)) {
            RHS E = scanExpr((ParseNode)rule.getChildAt(3));
            Rules Elst = scanExprList((ParseNode)rule.getChildAt(4));
            Rules rules = new Rules(E);
            rules.addAll(Elst);
            Rules union_rules_list = grammar_list.get(N);
            union_rules_list.addAll(rules);
            grammar_list.put(N, union_rules_list);
        } else {
            Token tok = (Token)(rule.getSymbolAt(1));
            System.out.println("*** A rule for undefined nonterminal <" + tok.getImage() + "> "+
                    "at " + tok.coordsToString() + " ***");
        }
    }

//    Elst  ::= ':' E Elst | eps
    private Rules scanExprList(ParseNode expr) {
        Rules rules = new Rules();
        while (!expr.isLeaf()) {
            rules.add(scanExpr((ParseNode) expr.getChildAt(1)));
            expr = (ParseNode) expr.getChildAt(2);
        }
        return rules;
    }

//    E ::= SYM SYM_LST | '@'
    private RHS scanExpr(ParseNode rule) {
        if (rule.getChildCount() == 1) {
            RHS res = new RHS(RHS.EPSILON);
            res.setCoords(rule.getSymbolAt(0).getStart());
            return res;
        } else {
            Symbol symbol = parseSymbol((ParseNode)rule.getChildAt(0));
            RHS res = new RHS(symbol);
            res.setCoords(symbol.getStart());
            res.addAll(scanSymbolList((ParseNode)rule.getChildAt(1)));
            return res;
        }
    }

//    SYM_LST ::= SYM SYM_LST | eps
    private RHS scanSymbolList(ParseNode node) {
        RHS res = new RHS();
        while (!node.isLeaf()) {
            Symbol sym = parseSymbol((ParseNode)node.getChildAt(0));
            res.add(sym);
            node = (ParseNode)node.getChildAt(1);
        }
        return res;
    }
    //    SYM ::= N | T
    private Symbol parseSymbol(ParseNode node) {
        Token sym = (Token)node.getSymbolAt(0);
        String image = sym.getImage();
//        System.out.println(sym.toString());
        if (sym.getType().equals(GrammarStructure.TERMINAL)) {
            image = image.replaceAll("[\\s\"]+", "");
            if (image.isEmpty()) image = " "; //Видимо, терминалом был пробельный символ
            if (!terms.contains(image)) terms.add(image);
            return new Term(image, sym.getStart(), sym.getFollow());
        } else {
            return new Nonterm(image, sym.getStart(), sym.getFollow());
        }

    }

    //-------------- ЗАПУСК РАЗБОРА ДЕРЕВА ------------------------------
    // S ::= DEF R_LST
    private void interpretS(ParseNode root) {
        scanDEF((ParseNode)root.getChildAt(0));
        scanRulesList((ParseNode)root.getChildAt(1));
    }

    private void interpretTree() {
        interpretS((ParseNode)tree.getRoot());
    }
}
