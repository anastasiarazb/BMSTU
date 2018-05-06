import javax.print.DocFlavor;
import java.util.ArrayList;

/**
 * Created by nastasia on 20.04.17.
 */

class Nonterm extends Symbol {

    public Nonterm (String type) {
        super.name = type;
    }

    @Override
    public String toString() {
        return "<" + super.toString() + ">";
    }

    public String toDot() {
        return String.format("[label=\"%s\"][color=green]\n", name);
    }

}
