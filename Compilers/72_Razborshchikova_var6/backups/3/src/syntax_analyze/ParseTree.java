package syntax_analyze;

import lex_analyze.Token;
import syntax_analyze.rules.RHS;
import syntax_analyze.symbols.Nonterm;

import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

/**
 * Created by nastasia on 21.04.17.
 */

public class ParseTree implements TreeModel {
    private ParseNode root;
    private ParseNode current;
    private int current_number;

    public ParseTree(Nonterm axiom) {
        root = new ParseNode(axiom);
        current = root;
        current_number = 0;
        update();
    }

    private void update() {
        ++current_number;
        current.setNumber(current_number);
    }

    public void add(RHS rule) {
        if (!rule.isEmpty()) {
            current.addChildren(rule);
            current = (ParseNode)current.getChildAt(0);
        } else {
            current = current.succ();
        }
        update();
    }

    public void setToken(Token token) {
        current.setToken(token);
        current = current.succ();
        update();
    }

    public String toDot() {
        return "digraph {\n" + root.toDot() + "}\n";
    }

    @Override
    public Object getRoot()
    {
        return root;
    }

    public Object getChild(Object parent, int index)
    {
        return ((ParseNode)parent).getChildAt(index);
    }

    @Override
    public int getChildCount(Object parent) {
        return ((ParseNode)parent).getChildCount();
    }

    @Override
    public boolean isLeaf(Object node) {
        return ((ParseNode)node).isLeaf();
    }

    @Override
    public void valueForPathChanged(TreePath path, Object newValue) {

    }

    @Override
    public int getIndexOfChild(Object parent, Object child) {
        return ((ParseNode)parent).getIndex((TreeNode)child);
    }

    @Override
    public void addTreeModelListener(TreeModelListener l) {

    }

    @Override
    public void removeTreeModelListener(TreeModelListener l) {

    }
}

