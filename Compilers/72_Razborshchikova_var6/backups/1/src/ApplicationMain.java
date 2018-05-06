import grammar_parser.CompilerGenerator;
import grammar_parser.GrammarInterpreter;
import grammar_parser.GrammarParser;
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

        Scanner sc = new Scanner(args[0], GrammarStructure.reg_expressions);
        Parser parser = new GrammarParser();
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
//        for (lex_analyze.Token tok = sc.nextToken(); !tok.Type.equals(syntax_analyze.symbols.Term.EOF); tok = sc.nextToken()) {
//            System.out.println(tok.toString());
//        }
        CompilerGenerator cg = gr.getCompilerGenerator();

    }
}
