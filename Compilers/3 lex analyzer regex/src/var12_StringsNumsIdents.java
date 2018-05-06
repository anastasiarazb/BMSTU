import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by nastasia on 26.02.17.
 */
public class var12_StringsNumsIdents {
    // Регулярные выражения
    final static  String ident    = "[?*|][?*|0-9]*";
    final static  String number   = "[01]+b|[0-9]+";
    final static  String string   = "`([^`]|``)*`"; //Любое кол-во
    final static  String blank    = "[ \t]+";
    final static  String newline  = "\r\n|[\r\n]";//"\\s^\\p{Blank}";

    public static String makeGroup(String name, String expr) {
        return "(?<" + name + ">" + expr + ")";
    }

    public static void main(String args[]) {
        File file = new File(args[0]);
        String text = "";
        try {
            text = new String(Files.readAllBytes(file.toPath()));
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", file.toPath());
        }

        String pattern = makeGroup("ident", ident) + "|" +
                makeGroup("number", number) + "|" +
                makeGroup("string", string) + "|" +
                makeGroup("blank", blank) + "|" +
                makeGroup("newline", newline);

        // Компиляция регулярного выражения
        Pattern p = Pattern.compile(pattern, Pattern.DOTALL);
        Matcher m = p.matcher(text);
        Coords coord = Coords.start();
        String token;

        while (coord.getPos() < text.length()) {
            if (m.find()) { //Почему-то find(i) не так работал, как ожидалось. Игнорировал (^ххх), не видел строку.
                if (m.start() != coord.getPos()) {
                    System.out.println("syntax error: " + coord.toString());
                    coord = coord.shift(m.start() - coord.getPos());
                }
                if ((token = m.group("ident")) != null) {
                    System.out.println("IDENT " + coord.toString() + ": " + token);
                    coord = coord.shift(token.length());
                } else if ((token = m.group("number")) != null) {
                    System.out.println("NUMBER " + coord.toString() + ": " + token);
                    coord = coord.shift(token.length());
                } else if ((token = m.group("string")) != null) {
                    System.out.println("STRING " + coord.toString() + ": " + token);
                    String[] lines = token.split("\n");
                    String last_substr = lines[lines.length - 1];
                    coord = coord.shift(lines.length - 1, last_substr.length(), token.length());
                } else if ((token = m.group("blank")) != null) {
                    coord = coord.shift(token.length());
                } else if ((token = m.group("newline")) != null) {
                    coord = coord.newline();
                    coord = coord.shift(token.length() - 1);
                } else {
                    System.out.println("error error error");
                    return;
                }

                if (coord.getPos() != m.end()) { //Для самопроверки
                    System.out.println("Invalid counter: coord.getPos != m.end ("+
                            coord.getPos() + " != " + m.end() + ")");
                }
            } else {
                System.out.println("syntax error: " + coord.toString());
                return;
            }
        }
    }

//    public static void main(String args[])
//    {
//        File file = new File(args[0]);
//        String text = "";
//        try {
//            text = new String(Files.readAllBytes(file.toPath()));
//        } catch (IOException e) {
//            System.err.printf("file %s cannot be read\n", file.toPath());
//        }
//
//        String pattern = makeGroup("ident",   ident)   + "|" +
//                makeGroup("number",  number)  + "|" +
//                makeGroup("string",  string)  + "|" +
//                makeGroup("blank",   blank)   + "|" +
//                makeGroup("newline", newline);
//
//        // Компиляция регулярного выражения
//        Pattern p = Pattern.compile(pattern, Pattern.DOTALL | Pattern.MULTILINE);
//        Matcher m = p.matcher(text);
//        int i = 0;
//        Coords coord = Coords.start();
//        String token;
//
//        while (i < text.length()) {
//            if (m.find()) { //Почему-то find(i) не так работал, как ожидалось. Игнорировал (^ххх), не видел строку.
//                if (m.start() != i) {
//                    System.out.println("syntax error: " + coord.toString());
//                    coord = coord.shift(m.start() - i);
//                }
//                if ((token = m.group("ident")) != null) {
//                    System.out.println("IDENT " + coord.toString() + ": " + token);
//                } else if ((token = m.group("number")) != null) {
//                    System.out.println("NUMBER " + coord.toString() + ": " + token);
//                } else if ((token = m.group("string")) != null) {
//                    System.out.println("STRING " + coord.toString() + ": " + token);
//                    String[] lines = token.split("\n");
//                    token = lines[lines.length-1];
//                    coord = coord.newline(lines.length - 1);
//                } else if (m.group("newline") != null) {
//                    coord = coord.newline();
//                    token = "";
//                } else if ((token = m.group("blank")) == null) {
//                    System.out.println("error error error");
//                    return;
//                }
//                coord = coord.shift(token.length());
//                i = m.end();
//            } else {
//                System.out.println("syntax error: " + coord.toString());
//                return;
//            }
//        }

//    public static void main(String args[])
//    {
//        File file = new File(args[0]);
//        String text = "";
//        try {
//            text = new String(Files.readAllBytes(file.toPath()));
//        } catch (IOException e) {
//            System.err.printf("file %s cannot be read\n", file.toPath());
//        }
//
//        String pattern = makeGroup("ident",   ident)   + "|" +
//                         makeGroup("number",  number)  + "|" +
//                         makeGroup("string",  string)  + "|" +
//                         makeGroup("blank",   blank)   + "|" +
//                         makeGroup("newline", newline);
//
//        // Компиляция регулярного выражения
//        Pattern p = Pattern.compile(pattern, Pattern.DOTALL | Pattern.MULTILINE);
//        Matcher m;
//        Coords coord = Coords.start();
//        boolean skip = false;
//        String token;
//
//        while (text.length() > 0) {
//            m = p.matcher(text);
//            if (m.lookingAt()) { //Почему-то find(i) не так работал, как ожидалось. Игнорировал (^ххх), не видел строку.
//                if ((token = m.group("ident")) != null) {
//                    System.out.println("IDENT " + coord.toString() + ": " + token);
//                    coord = coord.shift(token.length());
//                    skip = false;
//                    text = text.substring(token.length());
//                } else if ((token = m.group("number")) != null) {
//                    System.out.println("NUMBER " + coord.toString() + ": " + token);
//                    coord = coord.shift(token.length());
//                    skip = false;
//                    text = text.substring(token.length());
//                } else if ((token = m.group("string")) != null) {
//                    System.out.println("STRING " + coord.toString() + ": " + token);
//                    String[] substrings = token.split("\n");
//                    String last_substr = substrings[substrings.length-1];
//                    coord = coord.shift(substrings.length - 1, last_substr.length());
//                    skip = false;
//                    text = text.substring(token.length());
//                } else if ((token = m.group("blank")) != null) {
//                    coord = coord.shift(token.length());
//                    skip = false;
//                    text = text.substring(token.length());
//                } else if ((token = m.group("newline")) != null) {
//                    coord = coord.newline();
//                    skip = false;
//                    text = text.substring(token.length());
//                } else {
//                    System.out.println("error error error");
//                    return;
//                }
//
//            } else {
//                if (!skip) {
//                    skip = true;
//                    System.out.println("syntax error: " + coord.toString());
//                }
//                coord = coord.shift(1);
//                text = text.substring(1);
//            }
//        }
//    }
}

/*
Строковые литералы: ограничены обратными кавычками, могут занимать несколько строчек текста, для включения обратной
кавычки она удваивается.

Числовые литералы: десятичные литералы представляют собой последовательности десятичных цифр, двоичные -
последовательности нулей и единиц, оканчивающиеся буквой "b".

Идентификаторы: последовательности десятичных цифр и знаков "?", "*" и "|", не начинающиеся с цифры.

Чтение из файла в utf-8, вычисление текущих координат в обрабатываемом тексте. В результате работы программы в
стандартный поток вывода должны выдаваться описания распознаваемых лексем в формате

Тэг (координаты): значение

Например

IDENT (1, 2): count
ASSIGN (1, 8): ^=
NUMBER (1, 11) : 100

Лексемы во входном файле могут разделяться пробельными символами (пробел, горизонтальная табуляция, маркеры окончания
строки), а могут быть записаны слитно, если это не приводит к противоречиям.

Идентификаторы и Числовые литералы не могут содержать внутри себя пробельных символов, если в задании не указано
иного (1, 14). Комментарии, строковые и символьные литералы могут содержать внутри себя пробельные символы.

Входной файл может содержать ошибки, при которых лексический анализатор должен выдавать сообщением с указанием
координаты:

syntax error (10, 2)

Восстановление: из входного потока пропускаются все подряд идущие символы до нахождения следующей лексемы.*/
