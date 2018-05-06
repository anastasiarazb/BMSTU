import java.io.File;
import java.io.IOException;
import java.nio.file.Files;

/**
 * Created by nastasia on 12.04.17.
 */
public class Scanner {

    String text;
    Coords coord;

    public Scanner(String filepath) {
        File file = new File(filepath);
        try {
            text = new String(Files.readAllBytes(file.toPath()));
            text = text.replaceAll("\r", ""); // Чтобы единственным символом перевода строки был \n
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", file.toPath());
        }
        coord = Coords.start();
    }

    public CodePoint getChar() {
        if (coord.getPos() >= text.length()) {
            return new CodePoint(coord, '\0');
        }
        char c = text.charAt(coord.getPos());
        CodePoint code_point = new CodePoint(coord, c);
        coord = (c == '\n') ? coord.newline() : coord.shift(1);
        return code_point;
    }

    public Coords getCoord() {
        return coord;
    }
}