package syntax_analyze.rules;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by nastasia on 23.04.17.
 */
public class Rules extends ArrayList<RHS> {

    public Rules(RHS... rules) {
        super(Arrays.asList(rules));
    }
}
