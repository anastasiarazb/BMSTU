package nastasia.bmstu.ru.lab7_big.data;

public enum Currency {
    USD, EUR, GBP, JPY, RUB;

    @Override
    public String toString() {
        switch (this) {
            case USD: return "usd";
            case EUR: return "eur";
            case GBP: return "gbp";
            case RUB: return "rub";
            case JPY: return "jpy";
            default:  return "usd";
        }
    }

    public static Currency getByPosition(int index) {
        return (index < values().length)
                ? values()[index]
                : USD;
    }

    public int pos() {
        switch (this) {
            case USD: return 0;
            case EUR: return 1;
            case GBP: return 2;
            case RUB: return 4;
            case JPY: return 3;
            default:  return 0;
        }
    }
}