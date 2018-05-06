package grammar_parser;

import calculator.ArithmeticStructure;
import lex_analyze.Coords;
import lex_analyze.Scanner;
import lex_analyze.Token;

/**
 * Created by nastasia on 01.05.17.
 */
public class GrammarScanner extends Scanner {
    public GrammarScanner(String filepath) {
        super(filepath, GrammarStructure.reg_expressions);
    }
    @Override
    protected Token returnToken (String type) {
        Coords last = coord;
        coord = coord.shift(image.length());
        log.append(type).append(' ').append(last.toString()).append('-').append(coord.toString())
                .append(": <").append(image).append(">\n");
        if (type.equals(GrammarStructure.TERMINAL) || type.equals(GrammarStructure.NONTERMINAL)) {
            return new Token(type, image, last, coord);
        }
        //Отличается от базового сканера тем, что вместо типа возвращает образ. Нужно потому, что нельзя именованным
        //группам давать имена, совпадающие со скобками и прочими спецсимволами
        return new Token(image, image, last, coord);
    }
}
