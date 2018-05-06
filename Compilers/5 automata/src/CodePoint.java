/**
 * Created by nastasia on 09.06.17.
 */
public class CodePoint {
    Coords coords;
    char   symbol;

    public CodePoint(Coords coords, char symbol) {
        this.coords = coords;
        this.symbol = symbol;
    }

    public Coords getCoords() {
        return coords;
    }

    public char getSymbol() {
        return symbol;
    }

    public GenericSymbol getType() {
        return GenericSymbol.getGenericClass(symbol);
    }
}
