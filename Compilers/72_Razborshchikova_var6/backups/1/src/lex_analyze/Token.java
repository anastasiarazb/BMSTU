package lex_analyze;

import syntax_analyze.symbols.Term;

/**
 * Created by nastasia on 20.04.17.
 */
public class Token extends Term {
    private String image;

    public Token(String type, String image, Coords start, Coords follow) {
        super(type, start, follow);
        this.image = image;//image.replaceAll("[ \\t\"]*", "");
    }

    public Token(String type, Coords start) {
        super(type);
        this.image = "";
        this.start = start;
        this.follow = start;
    }

    public Token(String type) {
        super(type);
        this.image  = "";
    }

    @Override
    public String toString(){
        return String.format("lex_analyze.Token %s %s-%s <%s>",
                super.toString(), start.toString(), follow.toString(), image);
    }

    @Override
    public String toDot() {
        return String.format("[label=\"%s\"][color=red]\n", toString().replaceAll("\"", "\\\\\""));
    }

    public String getImage() {
        return image;
    }
}