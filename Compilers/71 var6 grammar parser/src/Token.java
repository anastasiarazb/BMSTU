/**
 * Created by nastasia on 20.04.17.
 */
public class Token extends Term {
    private String image;
    private Coords start, follow;

    public Token(String type, String image, Coords start, Coords follow) {
        super(type);
        this.image = image;//image.replaceAll("[ \\t\"]*", "");
        this.start = start;
        this.follow = follow;
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
        this.start  = Coords.undefined();
        this.follow = Coords.undefined();
    }

    @Override
    public String toString(){
        return String.format("Token %s %s-%s <%s>",
                super.toString(), start.toString(), follow.toString(), image);
    }

    @Override
    public String toDot() {
        return String.format("[label=\"%s\"][color=red]\n", toString().replaceAll("\"", "\\\\\""));
    }

    public String getImage() {
        return image;
    }

    public Coords getStart() {
        return start;
    }

    public Coords getFollow() {
        return follow;
    }
}