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
public class JustParseGrammar {
    public static void main(String args[]) {

        if (args.length < 1) {
            System.out.println("usage: command line arguments " +
                    "<path to grammar>" +
                    "\nExample: java JustParseGrammar ../data/file1.txt");
            System.exit(-2);
        }

        String grammar_src = args[0];

        //Считывание собственной грамматики с помощью статических таблиц
        Scanner scanner = new GrammarScanner(grammar_src);
        Parser parser = GrammarStructure.getParser();
        parser.parse(scanner);
        parser.dumpTreeToFile("parse_output" + File.separator + "parse_graph.dot");
        parser.dumpLogToFile("parse_output" + File.separator + "parse_log.txt");
        //Формирование списков терминалов/нетерминалов, правил
        GrammarInterpreter gr = new GrammarInterpreter(parser.getParseTree());
        //Формирование новой таблицы переходов
        CompilerGenerator cg = gr.getCompilerGenerator();
        cg.dumpLogToFile("parse_output" + File.separator + "compiler_gen_log.txt");
        cg.toJava("parse_output/GrammarStructure.java");
//        System.out.print(cg.printCompilerClass("parse_output/StaticCompiler.java"));
//        System.out.println(parser.getParseTree().toDot());
//        for (lex_analyze.Token tok = sc.nextToken(); !tok.getType().equals(syntax_analyze.symbols.Term.EOF); tok = sc.nextToken()) {
//            System.out.println(tok.toString());
//        }
    }
}
