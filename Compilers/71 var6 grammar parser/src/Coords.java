/**
 * Created by nastasia on 12.04.17.
 */
public class Coords {
    private final int row, col, pos;

    public int getRow() {
        return row;
    }

    public int getCol() {
        return col;
    }

    public int getPos() {
        return pos;
    }

    private Coords(int row, int col, int pos) {
        this.row = row;
        this.col = col;
        this.pos = pos;
    }

    public static Coords undefined() {
        return new Coords(-1, -1, -1);
    }

    public static Coords start() {
        return new Coords(1, 1, 0);
    }

    public Coords shift(int positions) {
        return new Coords(row, col + positions, pos + positions);
    }

    public Coords newline(int n) {
        return (n > 0) ? new Coords(row + n, 1 , pos + n) :
                new Coords(row, col, pos);
    }

    public Coords shift(int lines, int positions, int token_len) {
        return new Coords(row + lines,
                (lines > 0) ? 1 + positions : col + positions, //Либо смещать на той же строке, либо на новой
                pos + token_len);
    }

    public Coords newline() {
        return new Coords(row + 1, 1, pos + 1);
    }

    @Override
    public String toString() {
        return pos > -1 ? String.format("(%d, %d, %d)", row, col, pos) : "?";
    }
}
