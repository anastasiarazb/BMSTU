import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

/**
 * Created by nastasia on 21.04.17.
 */
class GrammarStructure { //GrammarParser extends Parser, Parser(terms, nonterms, axiom, q), GrammarParser()

    final static ArrayList<String> term_list = staticTermList();
    final static ArrayList<String> nonterm_list = staticNontermList();
    final static Nonterm axiom = new Nonterm("S");
    final static HashMap<String, String> reg_expressions = staticRegExpressions();
    final static HashMap<String, Rules> grammar_list = staticGrammarRulesList();
    final static RHS[][] q = staticDelta();

    private static ArrayList<String> staticTermList() {
        return new ArrayList<>(Arrays.asList(
                "N", "T", "COLON", "COMMA", "LBRACKET", "RBRACKET", "LBRACE", "RBRACE", "AT", Term.EOF
        ));
    }

    private static ArrayList<String> staticNontermList() {
        return new ArrayList<>(Arrays.asList(
                "S", "DEF", "RULES", "AXIOM", "D_SUFF", "D_PREF", "R_LST", "R", "E_LST", "E", "SYM_LST", "SYM"
        ));
    }

    private static HashMap<String, String> staticRegExpressions() {
        HashMap<String, String> exprs = new HashMap<>();
        exprs.put("N", "[A-Z]+'?");
        exprs.put("T", "\"\\s*([^\\s\"]|\\[nr\"tb])\\s*\"");
        exprs.put("COLON", "\\:");
        exprs.put("COMMA", ",");
        exprs.put("LBRACE", "\\{");
        exprs.put("RBRACE", "\\}");
        exprs.put("LBRACKET", "\\[");
        exprs.put("RBRACKET", "\\]");
        exprs.put("AT", "\\@");
        return exprs;
    }

    private static HashMap<String, Rules> staticGrammarRulesList() {
        HashMap<String, Rules> rules = new HashMap<>();
        rules.put("S", new Rules(new RHS(
                new Nonterm("DEF"),
                new Nonterm("R_LST")
        )));
        rules.put("DEF",
                new Rules(new RHS(
                        new Nonterm("D_PREF"),
                        new Nonterm("AXIOM"),
                        new Nonterm("D_SUFF")
                )));
        rules.put("AXIOM", new Rules(new RHS(
                new Term("LBRACE"),
                new Term("N"),
                new Term("RBRACE")
        )));
        rules.put("D_PREF", new Rules(
                new RHS(
                        new Term("N"),
                        new Term("COMMA"),
                        new Nonterm("D_PREF")
                ),
                new Epsilon()
        ));
        rules.put("D_SUFF", new Rules(
                new RHS(
                        new Term("COMMA"),
                        new Term("N"),
                        new Nonterm("D_SUFF")
                ),
                new Epsilon()
        ));
        rules.put("R_LST", new Rules(
                new RHS(
                        new Nonterm("R"),
                        new Nonterm("R_LST")
                ),
                new Epsilon()
        ));
        rules.put("R", new Rules(
                new RHS(
                        new Term("LBRACKET"),
                        new Term("N"),
                        new Term("COLON"),
                        new Nonterm("E"),
                        new Nonterm("E_LST"),
                        new Term("RBRACKET")
                )
        ));
        rules.put("E_LST", new Rules(
                new RHS(
                        new Term("COLON"),
                        new Nonterm("E"),
                        new Nonterm("E_LST")
                ),
                new Epsilon()
        ));
        rules.put("E", new Rules(
                new RHS(
                        new Nonterm("SYM"),
                        new Nonterm("SYM_LST")
                ),
                new RHS(new Term("AT"))
        ));
        rules.put("SYM_LST", new Rules(
                new RHS(
                        new Nonterm("SYM"),
                        new Nonterm("SYM_LST")
                ),
                new Epsilon()
        ));
        rules.put("SYM", new Rules(
                new RHS(new Term("N")),
                new RHS(new Term("T"))
        ));

//        System.out.println(rules.toString());
        return rules;
    }

    private static RHS[][] staticDelta() {
        ArrayList<String> T = term_list;
        ArrayList<String> N = nonterm_list;
        HashMap<String, Rules> rules = grammar_list;
        int m = N.size();
        int n = T.size();
        RHS[][] q = new RHS[m][n];
        for (RHS[] line: q) {
            Arrays.fill(line, new Error());
        }
        q[N.indexOf("S")][T.indexOf("LBRACE")] = rules.get("S").get(0);
        q[N.indexOf("S")][T.indexOf("N")]      = rules.get("S").get(0);
        q[N.indexOf("AXIOM")][T.indexOf("LBRACE")] = rules.get("AXIOM").get(0);
        q[N.indexOf("DEF")][T.indexOf("LBRACE")] = rules.get("DEF").get(0);
        q[N.indexOf("DEF")][T.indexOf("N")]      = rules.get("DEF").get(0);
        q[N.indexOf("D_SUFF")][T.indexOf("COMMA")]    = rules.get("D_SUFF").get(0);
        q[N.indexOf("D_SUFF")][T.indexOf("LBRACKET")] = rules.get("D_SUFF").get(1);
        q[N.indexOf("D_SUFF")][T.indexOf(Term.EOF)]   = rules.get("D_SUFF").get(1);
        q[N.indexOf("D_PREF")][T.indexOf("N")]    = rules.get("D_PREF").get(0);
        q[N.indexOf("D_PREF")][T.indexOf("LBRACE")] = rules.get("D_PREF").get(1);

        q[N.indexOf("R_LST")][T.indexOf("LBRACKET")] = rules.get("R_LST").get(0);
        q[N.indexOf("R_LST")][T.indexOf(Term.EOF)]   = rules.get("R_LST").get(1);
        q[N.indexOf("R")][T.indexOf("LBRACKET")] = rules.get("R").get(0);
        q[N.indexOf("E_LST")][T.indexOf("COLON")]    = rules.get("E_LST").get(0);
        q[N.indexOf("E_LST")][T.indexOf("RBRACKET")] = rules.get("E_LST").get(1);
        q[N.indexOf("E")][T.indexOf("N")]  = rules.get("E").get(0);
        q[N.indexOf("E")][T.indexOf("T")]  = rules.get("E").get(0);
        q[N.indexOf("E")][T.indexOf("AT")] = rules.get("E").get(1);
        q[N.indexOf("SYM_LST")][T.indexOf("N")]        = rules.get("SYM_LST").get(0);
        q[N.indexOf("SYM_LST")][T.indexOf("T")]        = rules.get("SYM_LST").get(0);
        q[N.indexOf("SYM_LST")][T.indexOf("RBRACKET")] = rules.get("SYM_LST").get(1);
        q[N.indexOf("SYM_LST")][T.indexOf("COLON")]    = rules.get("SYM_LST").get(1);
        q[N.indexOf("SYM")][T.indexOf("N")]    = rules.get("SYM").get(0);
        q[N.indexOf("SYM")][T.indexOf("T")]    = rules.get("SYM").get(1);

//        for (int i = 0; i < m; ++i) {
//            for (int j = 0; j < n; ++j) {
//                System.out.println(String.format("[%d][%d] %s", i, j, q[i][j].toString()));
//            }
//        }
        return q;
    }
}

