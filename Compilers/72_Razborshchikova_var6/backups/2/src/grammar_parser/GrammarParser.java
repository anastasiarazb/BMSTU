package grammar_parser;

import syntax_analyze.ParseTree;
import syntax_analyze.Parser;
import syntax_analyze.rules.RHS;
import syntax_analyze.symbols.Nonterm;

import java.util.ArrayList;

/**
 * Created by nastasia on 22.04.17.
 */
public class GrammarParser extends Parser {
    public GrammarParser () {
        super(GrammarStructure.terms, GrammarStructure.nonterms, GrammarStructure.axiom, GrammarStructure.q);
    }

    //Пока заглушки
    public Parser genParser() {
        ParseTree tree = super.getParseTree();
        if (tree == null) {
            System.out.println("[grammar_parser.GrammarParser.genParser]: no parse tree");
        }
        ArrayList<String> terms = GrammarStructure.terms;
        ArrayList<String> nonterms = GrammarStructure.nonterms;
        Nonterm axiom = GrammarStructure.axiom;
        RHS[][] parse_table = GrammarStructure.q;
        return new Parser(terms, nonterms, axiom, parse_table);
    }
}
