package syntax_analyze;

import lex_analyze.Scanner;
import lex_analyze.Token;
import syntax_analyze.rules.RHS;
import syntax_analyze.symbols.Nonterm;
import syntax_analyze.symbols.Symbol;
import syntax_analyze.symbols.Term;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Stack;

/**
 * Created by nastasia on 12.04.17.
 */

public class Parser {

    protected ArrayList<String> terms;//    = grammar_parser.GrammarStructure.terms;
    protected ArrayList<String> nonterms;// = grammar_parser.GrammarStructure.nonterms;
    protected Nonterm axiom;//    = grammar_parser.GrammarStructure.axiom;
    protected RHS[][] q;// = grammar_parser.GrammarStructure.q;
    private ParseTree parse_tree = null;// = new syntax_analyze.ParseTree(axiom);
    protected StringBuilder log = new StringBuilder();

    public Parser() {
    }

    public Parser(ArrayList<String> terms, ArrayList<String> nonterms, Nonterm axiom, RHS[][] q) {
        this.terms    = terms;
        this.nonterms = nonterms;
        this.axiom    = axiom;
        this.q = q;
    }

    public String getLog() {
        return log.toString();
    }

    private RHS delta(Nonterm N, Term T) {
        return q[nonterms.indexOf(N.getType())][terms.indexOf(T.getType())];
    }

    private void printError(Token tok, Symbol expected) {
        System.out.println("***ERROR: " + expected.toString() + " expected, got: " + tok.toString());
    }

    public ParseTree parse(Scanner scanner) {
        log.setLength(0);
        Stack<Symbol> stack = new Stack<>();
        stack.push(new Term(Term.EOF));
        stack.push(axiom);
        parse_tree = new ParseTree(axiom);
        Token tok = scanner.nextToken();
        do {
            log.append(stack.toString()).append("-----------").append(tok.toString()).append('\n');
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
                if (RHS.isError(nextRule)) {
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

    public void dumpLogToFile(String path) {
        File parse_log = new File(path);
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(parse_log))) {
            writer.write(log.toString());
        } catch (IOException e) {
            System.err.printf("file %s cannot be read\n", path);
        }
    }

}