package nastasia.bmstu.ru.lab7_sqlite.data;

import android.content.ContentValues;
import android.database.Cursor;

public class CheckedString {
    private boolean      checked;
    private int          number;
    private final String content;
    private final long   timestamp;

    public CheckedString(String content, long timestamp, int number) {
        this.checked = false;
        this.content = content;
        this.timestamp = timestamp;
        this.number    = number;
    }

    private CheckedString(String content, boolean checked, long timestamp, int number) {
        this.checked = checked;
        this.content = content;
        this.timestamp = timestamp;
        this.number    = number;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void switchChecked() {
        this.checked = !this.checked;
    }

    public boolean isChecked() {
        return checked;
    }

    public String getContent() {
        return content;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int idx) {
        number = idx;
    }

    public static void swapNumbers(CheckedString o1, CheckedString o2) {
        int temp = o1.number;
        o1.number = o2.number;
        o2.number = o1.number;
    }

    @Override
    public String toString() {
        return "[\"" + content + "\" " + content + " " + timestamp + "]";
    }

    // -------------------------------- Database mapping ----------------------------------



    public ContentValues toContentValues() {
        ContentValues values = new ContentValues();

        values.put(DBContract.DBEntry.CONTENT,    content);
        values.put(DBContract.DBEntry.IS_CHECKED, checked ? 1 : 0);
        values.put(DBContract.DBEntry.TIME,       timestamp);
        values.put(DBContract.DBEntry.NUMBER,     number);

        return values;
    }

    public static CheckedString getFromCursor(Cursor cursor,
                                              DBContract.DBEntry.ColumnNumbers col_idx) {
        String  content = cursor.getString(col_idx.content);
        int     checked = cursor.getInt(col_idx.checked);
        long    time    = cursor.getLong(col_idx.time);
        int     number  = cursor.getInt(col_idx.number);
        return new CheckedString(content, 0 != checked, time, number);
    }

    public String whereClauseQuery() {
        return DBContract.DBEntry.TIME + " = " + timestamp;
    }
}