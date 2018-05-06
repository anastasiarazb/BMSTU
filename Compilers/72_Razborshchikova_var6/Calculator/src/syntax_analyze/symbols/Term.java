package syntax_analyze.symbols;

import lex_analyze.Coords;

import java.util.ArrayList;

/**
 * Created by nastasia on 23.04.17.
 */
public class Term extends Symbol{
    public final static String EOF = "$";
    public final static String EPSILON = "";
    public final static Term eps = new Term(EPSILON);
    public Term (String type) {
        super(type);
    }

    public Term (String type, Coords start, Coords follow) {
        super(type, start, follow);
    }

    public static ArrayList<Term> asTermList(String ... names) {
        ArrayList<Term> list = new ArrayList<>();
        for(String name: names) {
            list.add(new Term(name));
        }
        return list;
    }

    @Override
    public String toString() {
        return "\"" + super.toString() + "\"";
    }

    public String toDot() {
        return String.format("[label=\"%s\"][color=black]\n", getType());
    }

    public String printConstructor() {
        return "new Term(\"" + getType() + "\")";
    }
}
