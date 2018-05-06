import java.util.ArrayList;

/**
 * Created by nastasia on 22.04.17.
 */
public class GrammarParser extends Parser {
    GrammarParser () {
        super(GrammarStructure.term_list, GrammarStructure.nonterm_list, GrammarStructure.axiom, GrammarStructure.q);
    }

    //Пока заглушки
    Parser genParser() {
        ParseTree tree = super.getParseTree();
        if (tree == null) {
            System.out.println("[GrammarParser.genParser]: no parse tree");
        }
        ArrayList<String> terms = GrammarStructure.term_list;
        ArrayList<String> nonterms = GrammarStructure.nonterm_list;
        Nonterm axiom = GrammarStructure.axiom;
        RHS[][] parse_table = GrammarStructure.q;
        return new Parser(terms, nonterms, axiom, parse_table);
    }
}
