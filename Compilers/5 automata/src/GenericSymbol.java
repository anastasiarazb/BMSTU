
public enum GenericSymbol {
    F, U, N, L, E, T, I, LETTER, DIGIT, COLON, APOSTROPHE, WHITESPACE, NEWLINE, BACKSLASH, OTHER, EOF;

    static GenericSymbol getGenericClass(char c) {
        switch (c) {
            case 'f': return F;
            case 'u': return U;
            case 'n': return N;
            case 'l': return L;
            case 'e': return E;
            case 't': return T;
            case 'i': return I;
            case ':':  return COLON;
            case '\\': return BACKSLASH;
            case '`':  return APOSTROPHE;
            case ' ':
            case '\t': return WHITESPACE;
            case '\n': return NEWLINE;
            case '\0': return EOF;
        }
        if (Character.isDigit(c)) {
            return DIGIT;
        }
        if (Character.isAlphabetic(c)) {
            return LETTER;
        }
        return  OTHER;
    }
}
