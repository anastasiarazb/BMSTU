package calculator;

import java.util.HashMap;

/**
 * Created by nastasia on 26.04.17.
 */
public class ArithmeticStructure {
    public final static HashMap<String, String> reg_expressions = staticRegExpressions();

    public static HashMap<String, String> staticRegExpressions() {
        HashMap<String, String> exprs = new HashMap<>();
        exprs.put("n", "n");
        exprs.put("PLUS", "\\+");
        exprs.put("STAR", "\\*");
        exprs.put("LPAREN", "\\(");
        exprs.put("RPAREN", "\\)");
        return exprs;
    }
}
