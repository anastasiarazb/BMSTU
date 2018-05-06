package calculator;

import lex_analyze.Coords;
import lex_analyze.Scanner;
import lex_analyze.Token;

/**
 * Created by nastasia on 01.05.17.
 */
public class ArithmeticScanner extends Scanner {
    public ArithmeticScanner(String filepath) {
        super(filepath, ArithmeticStructure.staticRegExpressions());
    }
    @Override
    protected Token returnToken (String type) {
        Coords last = coord;
        coord = coord.shift(image.length());
        log.append(type).append(' ').append(last.toString()).append('-').append(coord.toString())
                .append(": <").append(image).append(">\n");
        //Отличается от базового сканера тем, что вместо типа возвращает образ. Нужно потому, что нельзя именованным
        //группам давать имена, совпадающие со скобками и прочими спецсимволами
        return new Token(image, image, last, coord);
    }
}
