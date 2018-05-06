import calculator.ArithmeticInterpreter;
import calculator.ArithmeticScanner;
import lex_analyze.Scanner;
import syntax_analyze.Parser;

import java.io.File;

/**
 * Created by nastasia on 03.05.17.
 */
public class CalculatorMain {
    public static void main(String args[]) {

        if (args.length < 2) {
            System.out.println("usage: command line arguments " +
                    "<path to arithmetic expression> <value of n>" +
                    "\nExample: java CalculatorMain ../data/expr.txt 3");
            System.exit(-2);
        }

        String expr_src = args[0];
        String arithmetic_expr_variable = args[1];

        //Разбор арифметического выражения
        Scanner scanner = new ArithmeticScanner(expr_src);
        Parser parser = StaticCompiler.getParser();
        parser.parse(scanner);
//        scanner.dumpLogToFile("parse_output" + File.separator + "expr_scan_log.txt");
//        parser.dumpLogToFile("parse_output" + File.separator + "expr_parse_log.txt");
//        parser.dumpTreeToFile("parse_output" + File.separator + "expr_parse_graph.dot");
        int n = Integer.parseInt(arithmetic_expr_variable);
        //Вычисление арифметического выражения
        ArithmeticInterpreter evaluator = new ArithmeticInterpreter(parser.getParseTree(), n);
        String expression = scanner.getText();
        System.out.println(expression.replaceAll("\n", "") + " = " + evaluator.getResult() + ", where n = " + n);

//        System.out.println(parser.getParseTree().toDot());
//        for (lex_analyze.Token tok = sc.nextToken(); !tok.getType().equals(syntax_analyze.symbols.Term.EOF); tok = sc.nextToken()) {
//            System.out.println(tok.toString());
//        }
    }
}
