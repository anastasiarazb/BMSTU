import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by nastasia on 10.03.17.
 */
public class var12_StringsNumsIdents_find_i {
    // Регулярные выражения
    final static  String ident    = "[?*|][?*|0-9]*";
    final static  String number   = "[01]+b|[0-9]+";
    final static  String string   = "`([^`]|``)*`"; //Любое кол-во
    final static  String blank    = "[ \t]+";
    final static  String newline  = "\r\n|[\r\n]";//"\\s^\\p{Blank}";

//    \G The end of the previous match - работает
//    ^	  The beginning of a line - не работает
    public static String makeGroup(String name, String expr) {
        return "(?<" + name + ">\\G" + expr + ")";
    }

    public static void main(String args[])
    {
        File file = new File(args[0]);
        String text = "";
        try {
            text = new String(Files.readAllBytes(file.toPath()));
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", file.toPath());
        }

        String pattern = makeGroup("ident",   ident)   + "|" +
                makeGroup("number",  number)  + "|" +
                makeGroup("string",  string)  + "|" +
                makeGroup("blank",   blank)   + "|" +
                makeGroup("newline", newline);

        // Компиляция регулярного выражения
        Pattern p = Pattern.compile(pattern, Pattern.DOTALL | Pattern.MULTILINE);
        Matcher m = p.matcher(text).useAnchoringBounds(true);
        int i = 0;
        Coords coord = Coords.start();
        String token;
        boolean skip = false;

        while (i < text.length()) {
            if (m.find(i)) { //Почему-то find(i) не так работал, как ожидалось. Игнорировал (^ххх), не видел строку.
                if (m.start() != i) {
                    if (!skip) {
                        System.out.printf("---- 1-st branch: i = %d != m.start = %d, m.end = %d\n", i, m.start(), m.end());
                        System.out.println("->" + m.group() + "<-");
                        System.out.println("syntax error: " + coord.toString());
                        skip = true;
                    }
                    coord = coord.shift(1);
                    ++i;
                    continue;
                }
//                System.out.println("i = " + i);
                if ((token = m.group("ident")) != null) {
                    System.out.println("IDENT " + coord.toString() + ": " + token);
                    coord = coord.shift(token.length());
                    skip = false;
                    i += token.length();
                } else if ((token = m.group("number")) != null) {
                    System.out.println("NUMBER " + coord.toString() + ": " + token);
                    coord = coord.shift(token.length());
                    skip = false;
                    i += token.length();
                } else if ((token = m.group("string")) != null) {
                    System.out.println("STRING " + coord.toString() + ": " + token);
                    String[] substrings = token.split("\n");
                    String last_substr = substrings[substrings.length-1];
                    coord = coord.shift(substrings.length - 1, last_substr.length(), token.length());
                    skip = false;
                    i += token.length();
                } else if ((token = m.group("blank")) != null) {
                    coord = coord.shift(token.length());
                    skip = false;
                    i += token.length();
                } else if ((token = m.group("newline")) != null) {
                    coord = coord.newline();
                    skip = false;
                    i += token.length();
                } else {
                    System.out.println("error error error");
                    return;
                }
            } else {
                if (!skip) {
                    System.out.printf("---- 2-nd branch: i = %d, match is not found\n", i);
                    System.out.println("syntax error: " + coord.toString());
                    skip = true;
                }
                ++i;
                coord = coord.shift(1);
            }
        }
    }
}
