package syntax_analyze.rules;

import grammar_parser.Compilable;
import syntax_analyze.symbols.Symbol;
import syntax_analyze.symbols.Term;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by nastasia on 23.04.17.
 */
public class Rules extends ArrayList<RHS> implements Compilable{

    public Rules(RHS... rules) {
        super(Arrays.asList(rules));
    }

    public String printConstructor() {
        StringBuilder res = new StringBuilder("new Rules(");
        if (!isEmpty()) {
            res.append("\n        ").append(get(0).printConstructor());
        }
        for (int i = 1; i < size(); ++i) {
            res.append(",\n        ").append(get(i).printConstructor());
        }
        res.append("\n)\n");
        return  res.toString();
    }
}
