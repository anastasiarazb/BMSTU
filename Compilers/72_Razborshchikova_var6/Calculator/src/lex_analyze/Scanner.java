package lex_analyze;

import syntax_analyze.symbols.Symbol;
import syntax_analyze.symbols.Term;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
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
    protected Coords coord;
    protected String image = "";
    protected StringBuilder log = new StringBuilder();

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
            System.exit(0);
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

    public void dumpLogToFile(String path) {
        File parse_log = new File(path);
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(parse_log))) {
            writer.write(log.toString());
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", path);
        }
    }

    public String getText() {
        return text;
    }

    protected Token returnToken (String type) {
        Coords last = coord;
        coord = coord.shift(image.length());
        log.append(type).append(' ').append(last.toString()).append('-').append(coord.toString())
                .append(": <").append(image).append(">\n");
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
                log.append(String.format("*** m.start(%d) != coord.getPos(%d) syntax error: ",
                        m.start(), coord.getPos())).append(coord.toString()).append('\n');
                System.out.println(String.format("*** m.start(%d) != coord.getPos(%d) syntax error: ",
                        m.start(), coord.getPos()) + coord.toString());
                System.exit(-1);
//                coord = coord.shift(m.start() - coord.getPos());
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
            log.append("*** !m.find() syntax error: ").append(coord.toString()).append('\n');
            log.append("*** !m.find() syntax error: ").append(coord.toString()).append('\n');
            System.out.println("*** !m.find() syntax error: " + coord.toString());
            return new Token(Term.EOF, coord);
        }
    }
}