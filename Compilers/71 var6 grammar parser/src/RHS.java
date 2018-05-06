import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by nastasia on 20.04.17.
 */
class RHS extends ArrayList<Symbol> {
    public RHS(Symbol ... symbols) {
        super(Arrays.asList(symbols));
    }

    public RHS() {
        super();
    }

    public RHS reverse() {
        RHS rule = new RHS();
        rule.ensureCapacity(this.size());
        for (int i = size() - 1; i >= 0; --i) {
            rule.add(get(i));
        }
        return rule;
    }
}

class Error extends RHS {
    @Override
    public String toString() {
        return "Error";
    }
}

class Epsilon extends RHS {

}

class Rules extends ArrayList<RHS> {
    public Rules(RHS... rules) {
        super(Arrays.asList(rules));
    }
}