package nastasia.bmstu.ru.lab7_sqlite.data;

import java.util.Comparator;

abstract public class SortBy {

    public enum Items {
        TIME, ALPHA, UNCHECKED, CHECKED, NUMBER;

        static Items getFromString(String raw) {
            if (raw.equals("time")) {
                return TIME;
            }

            if (raw.equals("alpha")) {
                return ALPHA;
            }

            if (raw.equals("unchecked")) {
                return UNCHECKED;
            }

            if (raw.equals("checked")) {
                return CHECKED;
            }

            if (raw.equals("number")) {
                return CHECKED;
            }

            return TIME;
        }
    }

    // Map to R.arrays values
    public static final String TIME  = "time";
    public static final String ALPHA = "alpha";
    public static final String UNCHECKED = "unchecked";
    public static final String CHECKED   = "checked";
    public static final String NUMBER    = "number";

    public static final String DEFAULT = TIME;

    private static int compareByAlpha(CheckedString string1, CheckedString string2) {
        return string1.getContent().compareTo(string2.getContent());
    }

    private static int compareByTime(CheckedString string1, CheckedString string2) {
        return Long.compare(string1.getTimestamp(), string2.getTimestamp());
    }

    private static int compareByChecked(CheckedString string1, CheckedString string2) {
        return (string1.isChecked() == string2.isChecked())
                ? compareByTime(string1, string2)
                : (string1.isChecked() ? -1 : 1);
    }

    private static int compareByUnchecked(CheckedString string1, CheckedString string2) {
        return (string1.isChecked() == string2.isChecked())
                ? compareByTime(string1, string2)
                : (string1.isChecked() ? 1 : -1);
    }

    private static int compareByNumber(CheckedString string1, CheckedString string2) {
        return Integer.compare(string1.getNumber(), string2.getNumber());
    }

    public static Comparator<CheckedString> getComparatorBy(final Items item) {
        return new Comparator<CheckedString>() {
            @Override
            public int compare(CheckedString o1, CheckedString o2) {
                switch (item) {
                    case TIME:
                        return compareByTime(o1, o2);
                    case ALPHA:
                        return compareByAlpha(o1, o2);
                    case UNCHECKED:
                        return compareByUnchecked(o1, o2);
                    case CHECKED:
                        return compareByChecked(o1, o2);
                    case NUMBER:
                        return compareByNumber(o1, o2);
                    default:
                        return 0;
                }
            }
        };
    }

    public static Comparator<CheckedString> getComparatorBy(final String item) {
        if (item.equals(TIME)) {
            return new Comparator<CheckedString>() {
                @Override
                public int compare(CheckedString o1, CheckedString o2) {
                    return compareByTime(o1, o2);
                }
            };
        }
        if (item.equals(ALPHA)) {
            return new Comparator<CheckedString>() {
                @Override
                public int compare(CheckedString o1, CheckedString o2) {
                    return compareByAlpha(o1, o2);
                }
            };
        }
        if (item.equals(UNCHECKED)) {
            return new Comparator<CheckedString>() {
                @Override
                public int compare(CheckedString o1, CheckedString o2) {
                    return compareByUnchecked(o1, o2);
                }
            };
        }
        if (item.equals(CHECKED)) {
            return new Comparator<CheckedString>() {
                @Override
                public int compare(CheckedString o1, CheckedString o2) {
                    return compareByChecked(o1, o2);
                }
            };
        }
        if (item.equals(NUMBER)) {
            return new Comparator<CheckedString>() {
                @Override
                public int compare(CheckedString o1, CheckedString o2) {
                    return compareByNumber(o1, o2);
                }
            };
        }

        return null;

//        return new Comparator<CheckedString>() {
//            @Override
//            public int compare(CheckedString o1, CheckedString o2) {
//                if (item.equals(TIME))
//                    return compareByTime(o1, o2);
//                if (item.equals(ALPHA))
//                    return compareByAlpha(o1, o2);
//                if (item.equals(UNCHECKED))
//                    return compareByUnchecked(o1, o2);
//                if (item.equals(CHECKED))
//                    return compareByChecked(o1, o2);
//                if (item.equals(NUMBER))
//                    return compareByNumber(o1, o2);
//                return 0;
//            }
//        };
    }

    public static String toSQLQuery(final String sortByItem) {
        if (sortByItem.equals(TIME))
            return DBContract.DBEntry.TIME;
        if (sortByItem.equals(ALPHA))
            return DBContract.DBEntry.CONTENT;
        if (sortByItem.equals(UNCHECKED))
            return DBContract.DBEntry.IS_CHECKED + " ASC";
        if (sortByItem.equals(CHECKED))
            return DBContract.DBEntry.IS_CHECKED + " DESC";
        if (sortByItem.equals(NUMBER))
            return DBContract.DBEntry.NUMBER;
        return null;
    }
}