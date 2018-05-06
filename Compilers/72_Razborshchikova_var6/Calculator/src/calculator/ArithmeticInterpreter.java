package calculator;

import semantic_analize.Interpreter;
import syntax_analyze.ParseNode;
import syntax_analyze.ParseTree;

/**
 * Created by nastasia on 26.04.17.
 */
public class ArithmeticInterpreter extends Interpreter {
    private int n;
    private int result;
    private ParseTree tree = null;

    public ArithmeticInterpreter(ParseTree parseTree, int n)
    {
        super(parseTree);
        this.tree = parseTree;
        this.n = n;
        interpretTree();
    }

    public int evalFor(int n) {
        this.n = n;
        interpretTree();
        return result;
    }

    public int getResult() {
        return result;
    }

    //-------------- ЗАПУСК РАЗБОРА ДЕРЕВА ------------------------------

    private void interpretTree() {
        result = scanE((ParseNode)tree.getRoot());
    }
    // [ E : T E' ]
    private int scanE(ParseNode root) {
        return
        scanT ((ParseNode)root.getChildAt(0))
        +
        scanE1((ParseNode)root.getChildAt(1));
    }

    //[ E' : "+" T E' : @ ]
    private int scanE1(ParseNode node) {
        int res = 0;
        while (node.getChildCount() == 3) {
            res += scanT((ParseNode)node.getChildAt(1));
            node = (ParseNode)node.getChildAt(2);
        }
        return res;
    }

    //[ T : F T' ]
    private int scanT(ParseNode node) {
        return scanF ((ParseNode)node.getChildAt(0))
                * scanT1((ParseNode)node.getChildAt(1));
    }

    //[ T' : "*" F T' : @ ]
    private int scanT1(ParseNode node) {
        int res = 1;
        while (node.getChildCount() == 3) {
            res *= scanF((ParseNode)node.getChildAt(1));
            node = (ParseNode)node.getChildAt(2);
        }
        return res;
    }

    //[  F : "n" : "(" E ")"]
    private int scanF(ParseNode node) {
        if (node.getChildCount() == 3) {
            return scanE((ParseNode)node.getChildAt(1));
        } else {
            return n;
        }
    }
}
