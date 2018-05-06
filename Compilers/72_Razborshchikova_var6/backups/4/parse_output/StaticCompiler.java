import syntax_analyze.rules.RHS;
import syntax_analyze.Parser;
import syntax_analyze.symbols.Nonterm;
import syntax_analyze.symbols.Term;

import java.util.ArrayList;
import java.util.Arrays;

public class StaticCompiler {
    public final static ArrayList<String> terms = staticTermList();
    public final static ArrayList<String> nonterms = staticNontermList();
    public final static Nonterm axiom = new Nonterm("E");
    public final static RHS[][] q = staticDelta();

    public static Parser getParser() {
        return new Parser(terms, nonterms, axiom, q);
    }

    private static ArrayList<String> staticNontermList() {
        return new ArrayList<>(Arrays.asList(
                "E", "E'", "T", "T'", "F"
        ));
    }
    private static ArrayList<String> staticTermList() {
        return new ArrayList<>(Arrays.asList(
                "+", "*", "n", "(", ")", "$"
        ));
    }
    private static RHS[][] staticDelta() {
        ArrayList<String> T = terms;
        ArrayList<String> N = nonterms;
        int m = N.size();
        int n = T.size();
        RHS[][] q = new RHS[m][n];
        for (RHS[] line: q) {
            Arrays.fill(line, RHS.ERROR);
        }
        q[0][2] = new RHS(
                new Nonterm("T"),
                new Nonterm("E'")
                );
        q[0][3] = new RHS(
                new Nonterm("T"),
                new Nonterm("E'")
                );
        q[1][0] = new RHS(
                new Term("+"),
                new Nonterm("T"),
                new Nonterm("E'")
                );
        q[1][4] = RHS.EPSILON;
        q[1][5] = RHS.EPSILON;
        q[2][2] = new RHS(
                new Nonterm("F"),
                new Nonterm("T'")
                );
        q[2][3] = new RHS(
                new Nonterm("F"),
                new Nonterm("T'")
                );
        q[3][0] = RHS.EPSILON;
        q[3][1] = new RHS(
                new Term("*"),
                new Nonterm("F"),
                new Nonterm("T'")
                );
        q[3][4] = RHS.EPSILON;
        q[3][5] = RHS.EPSILON;
        q[4][2] = new RHS(
                new Term("n")
                );
        q[4][3] = new RHS(
                new Term("("),
                new Nonterm("E"),
                new Term(")")
                );
        return q;
    }
}
