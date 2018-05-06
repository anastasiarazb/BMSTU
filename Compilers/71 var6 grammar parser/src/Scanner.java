import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by nastasia on 12.04.17.
 */
public class Scanner {
    // Регулярные выражения
    final private static String NEWLINE = "newline";
    final private static String BLANK   = "blank";
    final private static String newline_epxr = "\\R"; //"\\r\\n|[\\r\\n]";
    final private static String blank_expr = "[ \\t]+";//"\\p{Space}"; //"[ \\t]+";
    //final static String[] terms = {":", ",", "{", "}", "[", "]", "@"};

    private HashMap<String, String> regexp = new HashMap<>();

    private String text = "";
    private Pattern p ;
    private Matcher m;
    private Coords coord;
    private String image = "";

    public static String makeGroup(String name, String expr) {
        return "(?<" + name + ">(" + expr + "))";
    }

    public String setPattern() {
        String res =
                makeGroup(BLANK, blank_expr) + "|" +
                makeGroup(NEWLINE, newline_epxr);
        for (Map.Entry<String, String> e: regexp.entrySet()) {
            res += "|" + makeGroup(e.getKey(), e.getValue());
        }
        return res;
    }

    public Scanner(String filepath, HashMap<String, String> termsexpr) {
        File file = new File(filepath);
        try {
            text = new String(Files.readAllBytes(file.toPath()));
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", file.toPath());
        }
        regexp = termsexpr;
        String pattern = setPattern();

        // Компиляция регулярного выражения
        p = Pattern.compile(pattern, Pattern.DOTALL);
        m = p.matcher(text);
        coord = Coords.start();
    }

    private boolean isType(String type) {
        return (image = m.group(type)) != null;
    }

    private Token returnToken (String type) {
        Coords last = coord;
        coord = coord.shift(image.length());
//        System.out.println(type.toString() + last.toString() + "-" + coord.toString() + ": <" + image + ">");
        return new Token(type, image, last, coord);
    }

    public Token nextToken() {
        if (coord.getPos() >= text.length()) {
            return new Token(Term.EOF, coord);
        }
        String image;
        if (m.find()) {
            if (m.start() != coord.getPos()) {
                System.out.println(String.format("*** m.start(%d) != coord.getPos(%d) syntax error: ",
                        m.start(), coord.getPos()) + coord.toString());
                coord = coord.shift(m.start() - coord.getPos());
            }
            for (String s: regexp.keySet()) {
                if (isType(s)) {
                    return returnToken(s);
                }
            }
            if ((image = m.group(BLANK)) != null) {
                coord = coord.shift(image.length());
//                System.out.println("blank_expr" + coord.toString() + ": " + image);
                return nextToken();
            }
            if ((image = m.group(NEWLINE)) != null) {
                coord = coord.newline();
                coord = coord.shift(image.length() - 1);
                return nextToken();
            } else {
                System.out.println("*** unhandled case at " + coord.toString() + " " + text.substring(coord.getPos()));
                return nextToken();
            }
        } else {
            System.out.println("*** !m.find() syntax error: " + coord.toString());
            return new Token(Term.EOF, coord);
        }
    }
}