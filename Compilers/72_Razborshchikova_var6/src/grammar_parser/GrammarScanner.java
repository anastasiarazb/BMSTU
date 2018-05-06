package grammar_parser;

import calculator.ArithmeticStructure;
import lex_analyze.Coords;
import lex_analyze.Scanner;
import lex_analyze.Token;

import java.util.HashMap;

/**
 * Created by nastasia on 01.05.17.
 */
public class GrammarScanner extends Scanner {

    public final static HashMap<String, String> reg_expressions = staticRegExpressions();
    public final static String NONTERMINAL = "N";
    public final static String TERMINAL = "T";

    private static HashMap<String, String> staticRegExpressions() {
        HashMap<String, String> exprs = new HashMap<>();
        exprs.put(NONTERMINAL, "[A-Z]+'?");
        exprs.put(TERMINAL, "\"\\s*([^\\s\"]|\\[nr\"tb])\\s*\"");
        exprs.put("COLON", "\\:");
        exprs.put("COMMA", ",");
        exprs.put("LBRACE", "\\{");
        exprs.put("RBRACE", "\\}");
        exprs.put("LBRACKET", "\\[");
        exprs.put("RBRACKET", "\\]");
        exprs.put("AT", "\\@");
        return exprs;
    }
    public GrammarScanner(String filepath) {
        super(filepath, reg_expressions);
    }
    @Override
    protected Token returnToken (String type) {
        Coords last = coord;
        coord = coord.shift(image.length());
        log.append(type).append(' ').append(last.toString()).append('-').append(coord.toString())
                .append(": <").append(image).append(">\n");
        if (type.equals(TERMINAL) || type.equals(NONTERMINAL)) {
            return new Token(type, image, last, coord);
        }
        //Отличается от базового сканера тем, что вместо типа возвращает образ. Нужно потому, что нельзя именованным
        //группам давать имена, совпадающие со скобками и прочими спецсимволами
        return new Token(image, image, last, coord);
    }
}
