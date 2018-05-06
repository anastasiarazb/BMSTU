import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;

/**
 * Created by nastasia on 12.04.17.
 */

public class Parser {

    private ArrayList<String> terms;//    = GrammarStructure.term_list;
    private ArrayList<String> nonterms;// = GrammarStructure.nonterm_list;
    private Nonterm           axiom;//    = GrammarStructure.axiom;
    private RHS[][] q;// = GrammarStructure.q;
    private ParseTree parse_tree = null;// = new ParseTree(axiom);

    public Parser() {
    }

    public Parser(ArrayList<String> terms, ArrayList<String> nonterms, Nonterm axiom, RHS[][] q) {
        this.terms    = terms;
        this.nonterms = nonterms;
        this.axiom    = axiom;
        this.q = q;
    }

    private RHS delta(Nonterm N, Term T) {
        return q[nonterms.indexOf(N.name)][terms.indexOf(T.name)];
    }

    private void printError(Token tok, Symbol expected) {
        System.out.println("***ERROR: " + expected.toString() + " expected, got: " + tok.toString());
    }

    public ParseTree parse(Scanner scanner) {
        Stack<Symbol> stack = new Stack<>();
        stack.push(new Term(Term.EOF));
        stack.push(axiom);
        parse_tree = new ParseTree(axiom);
        Token tok = scanner.nextToken();
        do {
            System.out.print(stack);
            System.out.println("-----------" + tok);
            Symbol X = stack.pop();
            if (X instanceof Term) {
                if (X.equals(tok)) {
                    parse_tree.setToken(tok);
                    tok = scanner.nextToken();
                } else {
                    printError(tok, X);
                    return parse_tree;
                }
            } else {
                RHS nextRule = delta((Nonterm)X, tok);
                if (nextRule instanceof Error) {
                    printError(tok, X);
                    return parse_tree;
                } else {
                    stack.addAll(nextRule.reverse());
                    parse_tree.add(nextRule);
                }
            }
        } while (!stack.empty());
        return parse_tree;
    }

    public ParseTree getParseTree() {
        return parse_tree;
    }

    public static void main(String args[]) {

        Scanner sc = new Scanner(args[0], GrammarStructure.reg_expressions);
        Parser parser = new GrammarParser();
        parser.parse(sc);
        File dotfile = new File("parse_graph.dot");
        try {
            Files.write(dotfile.toPath(), parser.getParseTree().toDot().getBytes());
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", dotfile.toPath());
        }
//        System.out.println(parser.getParseTree().toDot());
//        for (Token tok = sc.nextToken(); !tok.name.equals(Term.EOF); tok = sc.nextToken()) {
//            System.out.println(tok.toString());
//        }

    }
}