package syntax_analyze.symbols;

import lex_analyze.Coords;

/**
 * Created by nastasia on 20.04.17.
 */

public class Symbol {

    protected String type;

    protected Coords start, follow;

    public String getType() {
        return type;
    }

    protected Symbol(String type)
    {
        this.type = type;
        this.start  = Coords.undefined();
        this.follow = Coords.undefined();
    }

    protected Symbol (String type, Coords start, Coords follow)
    {
        this.type = type;
        this.start  = start;
        this.follow = follow;
    }

    @Override
    public String toString() {
        return type;
    }

    @Override
    public boolean equals (Object o) {
        return ((o instanceof Symbol) && type.equals(((Symbol)o).type))
                || ((o instanceof String) && type.equals(o));
    }

    public String toDot() {
        return "";
    }

    public String coordsToString() {
        return start.toString() + "-" + follow.toString();
    }

    public Coords getStart() {
        return start;
    }

    public Coords getFollow() {
        return follow;
    }

}
