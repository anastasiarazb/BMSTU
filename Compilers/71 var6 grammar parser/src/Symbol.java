import javax.print.DocFlavor;
import java.util.ArrayList;

/**
 * Created by nastasia on 20.04.17.
 */

//enum NontermType {
//    S, AXIOM, DEF, N_LST, R_LST, R, E_LST, E, SYM_LST, SYM
//}
//
//enum String { //","    "{"     "}"      "["        "]"   "@"  $
//    N, T, COLON, COMMA, LBRACE, RBRACE, LBRACKET, RBRACKET, AT, EOF, ERROR, EPSILON
//}

class Symbol {

    protected String name;

    @Override
    public String toString() {
        return name;
    }

    @Override
    public boolean equals (Object o) {
        return ((o instanceof Symbol) && name.equals(((Symbol)o).name))
                || ((o instanceof String) && name.equals(o));
    }

    public String toDot() {
        return "";
    }
}

class Term extends Symbol {
    final static String EOF = "$";
    public Term (String type) {
        super.name = type;
    }

    public static ArrayList<Term> asTermList(String ... names) {
        ArrayList<Term> list = new ArrayList<>();
        for(String name: names) {
            list.add(new Term(name));
        }
        return list;
    }

    public String toDot() {
        return String.format("[label=\"%s\"][color=black]\n", name);
    }
}

