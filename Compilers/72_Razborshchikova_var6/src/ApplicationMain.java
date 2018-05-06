import calculator.ArithmeticInterpreter;
import calculator.ArithmeticScanner;
import grammar_parser.CompilerGenerator;
import grammar_parser.GrammarInterpreter;
import grammar_parser.GrammarScanner;
import grammar_parser.GrammarStructure;
import lex_analyze.Scanner;
import syntax_analyze.Parser;

import java.io.File;

/**
 * Created by nastasia on 25.04.17.
 */
public class ApplicationMain {
    public static void main(String args[]) {

        if (args.length < 4) {
            System.out.println("usage: command line arguments " +
                    "<path to self grammar> <path to ariphmetic grammar> <path to arithmetic expression> <value of n>" +
                    "\nExample: java ApplicationMain ../data/grammar.txt ../data/file.txt ../data/expr.txt 3");
            System.exit(-2);
        }

        String self_grammar_src = args[0];
        String arithm_grammar_src = args[1];
        String expr_src = args[2];
        String arithmetic_expr_variable = args[3];

        //Считывание собственной грамматики с помощью статических таблиц
        Scanner scanner = new Scanner(self_grammar_src, GrammarScanner.reg_expressions);
        Parser parser = GrammarStructure.getParser();
        parser.parse(scanner);
        parser.dumpTreeToFile("parse_output" + File.separator + "self_parse_graph.dot");
        parser.dumpLogToFile("parse_output" + File.separator + "self_parse_log.txt");
        scanner.dumpLogToFile("parse_output" + File.separator + "self_scan_log.txt");
        //Формирование списков терминалов/нетерминалов, правил
        GrammarInterpreter gr = new GrammarInterpreter(parser.getParseTree());
        //Формирование новой таблицы переходов
        CompilerGenerator cg = gr.getCompilerGenerator();
        cg.dumpLogToFile("parse_output" + File.separator + "self_compiler_gen_log.txt");
        //Можно визуально сравить статические правила и таблицу переходов в
        //grammar_parser.GrammarStructure.staticGrammarRulesList() grammar_parser.GrammarStructure.staticDelta()
        //и содержимое файла parse_output/self_compiler_gen_log.txt

        //Считывание грамматики арифметических выражений
        scanner = new GrammarScanner(arithm_grammar_src);
        parser  = new Parser(cg);
        parser.parse(scanner);
        parser.dumpTreeToFile("parse_output" + File.separator + "arithm_parse_graph.dot");
        parser.dumpLogToFile("parse_output" + File.separator + "arithm_parse_log.txt");
        //Формирование списков терминалов/нетерминалов, правил
        gr = new GrammarInterpreter(parser.getParseTree());
        //Формирование таблицы переходов
        cg = gr.getCompilerGenerator();
        cg.dumpLogToFile("parse_output" + File.separator + "arithm_compiler_gen_log.txt");
        //Парсер арифметических выражений
        parser = new Parser(cg);
        //Разбор арифметического выражения
        scanner = new ArithmeticScanner(expr_src);
        parser.parse(scanner);
        scanner.dumpLogToFile("parse_output" + File.separator + "expr_scan_log.txt");
        parser.dumpLogToFile("parse_output" + File.separator + "expr_parse_log.txt");
        parser.dumpTreeToFile("parse_output" + File.separator + "expr_parse_graph.dot");
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
