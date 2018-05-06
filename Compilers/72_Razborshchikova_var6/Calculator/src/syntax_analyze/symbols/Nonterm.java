package syntax_analyze.symbols;

import lex_analyze.Coords;

/**
 * Created by nastasia on 23.04.17.
 */
public class Nonterm extends Symbol {

    public Nonterm (String type) {
        super(type);
    }

    public Nonterm (String type, Coords start, Coords follow) {
        super(type, start, follow);
    }

    @Override
    public String toString() {
        return "<" + super.toString() + ">";
    }

    public String toDot() {
        return String.format("[label=\"%s\"][color=green]\n", getType());
    }

    public String printConstructor() {
        return "new Nonterm(\"" + getType() + "\")";
    }

}