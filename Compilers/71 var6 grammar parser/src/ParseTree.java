import javax.swing.event.TreeModelListener;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import java.util.ArrayList;
import java.util.Enumeration;

/**
 * Created by nastasia on 21.04.17.
 */

class ParseNode implements TreeNode {
    private Symbol symbol;
    private int number = 0;
    private ArrayList<ParseNode> children = new ArrayList<>();
    private ParseNode parent = null;

    public void setNumber(int number) {
        this.number = number;
    }

    public ParseNode(Symbol symbol) {
        this.symbol = symbol;
    }

    public ParseNode(Symbol symbol, ParseNode parent) {
        this.symbol = symbol;
        this.parent = parent;
    }

    public void setToken(Token token) {
        symbol = token;
    }

    public void addChildren(RHS nodes) {
        for (Symbol s: nodes) {
            children.add(new ParseNode(s, this));
        }
    }

    private boolean isTheMostRightChild() {
        return (parent.getIndex(this) == (parent.children.size() - 1));
    }

    private boolean isRoot () {
        return parent == null;
    }

    public ParseNode succ() {
        if (isRoot()) return this;
        ParseNode res = this.parent;
        ParseNode prev = this;
        while(!res.isRoot() && prev.isTheMostRightChild()) {
//            System.out.println("UP " + res.symbol + " " +res.number);
            prev = res;
            res = res.parent;
        }

        if (res.isRoot() && prev.isTheMostRightChild()) {
            return this; //Эта ветка и так самая правая, другого саксессора не будет
        }
        res = res.children.get(res.getIndex(prev) + 1);

        while (!res.isLeaf()) {
//            System.out.println("DOWN " + res.symbol + " " +res.number);
            res = res.children.get(0);
        }
//        System.out.println("RET " + res.symbol + " " + res.number + "\n-----\n");
        return res;
    }

    @Override
    public TreeNode getChildAt(int childIndex) {
        return children.get(childIndex);
    }

    @Override
    public int getChildCount() {
        return children.size();
    }

    @Override
    public TreeNode getParent() {
        return parent;
    }

    @Override
    public int getIndex(TreeNode node) {
        if (node instanceof ParseNode) {
            return children.indexOf(node);
        }
        else return -1;
    }

    @Override
    public boolean getAllowsChildren() {
        return !(symbol instanceof Token);
    }

    @Override
    public boolean isLeaf() {
        return children.isEmpty();
    }

    @Override
    public Enumeration children() {
        return (Enumeration)children;
    }

    public String toDot() {
        String res = String.valueOf(number) + " " + symbol.toDot();
        for (ParseNode child: children) {
            res += child.toDot();
            res += String.valueOf(number) + "->" + String.valueOf(child.number) + "\n";
        }
        return res;
    }
}

class ParseTree implements TreeModel {
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

