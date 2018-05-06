import calculator.ArithmeticInterpreter;
import calculator.ArithmeticScanner;
import grammar_parser.CompilerGenerator;
import grammar_parser.GrammarInterpreter;
import grammar_parser.GrammarStructure;
import lex_analyze.Scanner;
import syntax_analyze.Parser;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;

/**
 * Created by nastasia on 25.04.17.
 */
public class ApplicationMain {
    public static void main(String args[]) {

        if (args.length < 3) {
            System.out.println("usage: command line arguments <path to grammar file> <path to arithmetic expression> <value of n>\nExample: data/file.txt data/expr.txt 3");
            System.exit(-2);
        }

        Scanner sc = new Scanner(args[0], GrammarStructure.reg_expressions);
        Parser parser = GrammarStructure.GrammarParser();
        parser.parse(sc);
        File dotfile = new File("parse_output/parse_graph.dot");
        try {
            Files.write(dotfile.toPath(), parser.getParseTree().toDot().getBytes());
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", dotfile.toPath());
        }
        parser.dumpLogToFile("parse_output/parse_log.txt");
        GrammarInterpreter gr = new GrammarInterpreter(parser.getParseTree());
//        System.out.println(parser.getParseTree().toDot());
//        for (lex_analyze.Token tok = sc.nextToken(); !tok.getType().equals(syntax_analyze.symbols.Term.EOF); tok = sc.nextToken()) {
//            System.out.println(tok.toString());
//        }
        CompilerGenerator cg = gr.getCompilerGenerator();
        parser = new Parser(cg);
        cg.dumpLogToFile("parse_output/compiler_gen_log.txt");
        sc = new ArithmeticScanner(args[1]);
        parser.parse(sc);
        parser.dumpLogToFile("parse_output/arithm_parse_log.txt");
//        for (lex_analyze.Token tok = sc.nextToken(); !tok.getType().equals(syntax_analyze.symbols.Term.EOF); tok = sc.nextToken()) {
//            System.out.println(tok.toString());
//        }
        sc.dumpLogToFile("parse_output/arithm_scan_log.txt");
        dotfile = new File("parse_output/arithm_parse_graph.dot");
        try {
            Files.write(dotfile.toPath(), parser.getParseTree().toDot().getBytes());
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", dotfile.toPath());
        }
        int n = Integer.parseInt(args[2]);
        ArithmeticInterpreter evaluator = new ArithmeticInterpreter(parser.getParseTree(), n);
        System.out.print(evaluator.getResult());
    }
}
