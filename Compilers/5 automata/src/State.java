public abstract class State {

    protected Coords start, follow;
    protected StringBuilder image;

    public String getContent() {
        return start + " - " + follow + ":    " + image;
    }

    public State() {}

    public abstract State processChar(Scanner cs);

    public State (Coords start, Coords current, StringBuilder image) {
        this.start  = start;
        this.follow = current;
        this.image  = image;
    }
}

class StringToken extends State {
    @Override
    public String toString() {
        return "STRING " + getContent();
    }
    @Override
    public State processChar(Scanner cs) {
        return this;
    }
}

class WhitespaceToken extends State {
    @Override
    public String toString() {
        return "WHITESPACE " + getContent();
    }

    @Override
     processChar(Scanner sc) {
        CodePoint s = sc.getChar();
        if (s.getType() == GenericSymbol.WHITESPACE) {
            image += s.
            return processChar(sc);
        }
    }
}