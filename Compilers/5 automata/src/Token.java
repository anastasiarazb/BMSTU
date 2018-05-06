
public class Token {
    protected Coords start, follow;
    protected String image;

    public Token (Coords start, Coords current, String image) {
        this.start  = start;
        this.follow = current;
        this.image  = image;
    }
}
