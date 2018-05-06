import syntax_analyze.rules.RHS;
import syntax_analyze.Parser;
import syntax_analyze.symbols.Nonterm;
import syntax_analyze.symbols.Term;

import java.util.ArrayList;
import java.util.Arrays;

public class StaticCompiler {
    public final static ArrayList<String> terms = staticTermList();
    public final static ArrayList<String> nonterms = staticNontermList();
    public final static Nonterm axiom = new Nonterm("S");
    public final static RHS[][] q = staticDelta();

    public static Parser getParser() {
        return new Parser(terms, nonterms, axiom, q);
    }

    private static ArrayList<String> staticNontermList() {
        return new ArrayList<>(Arrays.asList(
                "DEF", "S", "R'", "DPREF", "AXIOM", "DSUFF", "R", "E'", "E", "SYM'", "SYM"
        ));
    }
    private static ArrayList<String> staticTermList() {
        return new ArrayList<>(Arrays.asList(
                "{", "N", "}", ",", "[", ":", "]", "@", "T", "$"
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
        q[0][0] = new RHS(
                new Nonterm("DPREF"),
                new Nonterm("AXIOM"),
                new Nonterm("DSUFF")
                );
        q[0][1] = new RHS(
                new Nonterm("DPREF"),
                new Nonterm("AXIOM"),
                new Nonterm("DSUFF")
                );
        q[1][0] = new RHS(
                new Nonterm("DEF"),
                new Nonterm("R'")
                );
        q[1][1] = new RHS(
                new Nonterm("DEF"),
                new Nonterm("R'")
                );
        q[2][4] = new RHS(
                new Nonterm("R"),
                new Nonterm("R'")
                );
        q[2][9] = RHS.EPSILON;
        q[3][0] = RHS.EPSILON;
        q[3][1] = new RHS(
                new Term("N"),
                new Term(","),
                new Nonterm("DPREF")
                );
        q[4][0] = new RHS(
                new Term("{"),
                new Term("N"),
                new Term("}")
                );
        q[5][3] = new RHS(
                new Term(","),
                new Term("N"),
                new Nonterm("DSUFF")
                );
        q[5][4] = RHS.EPSILON;
        q[5][9] = RHS.EPSILON;
        q[6][4] = new RHS(
                new Term("["),
                new Term("N"),
                new Term(":"),
                new Nonterm("E"),
                new Nonterm("E'"),
                new Term("]")
                );
        q[7][5] = new RHS(
                new Term(":"),
                new Nonterm("E"),
                new Nonterm("E'")
                );
        q[7][6] = RHS.EPSILON;
        q[8][1] = new RHS(
                new Nonterm("SYM"),
                new Nonterm("SYM'")
                );
        q[8][7] = new RHS(
                new Term("@")
                );
        q[8][8] = new RHS(
                new Nonterm("SYM"),
                new Nonterm("SYM'")
                );
        q[9][1] = new RHS(
                new Nonterm("SYM"),
                new Nonterm("SYM'")
                );
        q[9][5] = RHS.EPSILON;
        q[9][6] = RHS.EPSILON;
        q[9][8] = new RHS(
                new Nonterm("SYM"),
                new Nonterm("SYM'")
                );
        q[10][1] = new RHS(
                new Term("N")
                );
        q[10][8] = new RHS(
                new Term("T")
                );
        return q;
    }
}
