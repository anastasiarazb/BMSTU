package syntax_analyze.rules;

import grammar_parser.Compilable;
import lex_analyze.Coords;
import syntax_analyze.symbols.Symbol;
import syntax_analyze.symbols.Term;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;

/**
 * Created by nastasia on 20.04.17.
 */
public class RHS extends ArrayList<Symbol> implements Compilable{

    private Coords coords  = Coords.undefined();

    public RHS(Symbol... symbols) {
        super(Arrays.asList(symbols));
    }

    public static final RHS EPSILON = new RHS();
    public static final RHS ERROR = null;

    public static boolean isEpsilon(RHS rhs) {
        return rhs.isEmpty();
    }

    public static boolean isError(RHS rhs) {
        return rhs == null;
    }

    public RHS() {
        super();
    }

    public RHS(Collection<? extends Symbol> c) {
        super(c);
    }

    public RHS reverse() {
        RHS rule = new RHS();
        rule.ensureCapacity(this.size());
        for (int i = size() - 1; i >= 0; --i) {
            rule.add(get(i));
        }
        return rule;
    }

    public void setCoords(Coords c) {
        coords = c;
    }

    public Coords getCoords() {
        return coords;
    }

    //--------------------- Вывод в консоль ---------------------
    public String printConstructor() {
        if (this.equals(EPSILON)) {
            return "RHS.EPSILON";
        }
        StringBuilder res = new StringBuilder("new RHS(");
        if (!isEmpty()) {
            res.append("\n                ").append(get(0).printConstructor());
        }
        for (int i = 1; i < size(); ++i) {
            res.append(",\n                ").append(get(i).printConstructor());
        }
        res.append("\n                )");
        return  res.toString();
    }
}