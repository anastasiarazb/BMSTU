package ru.bmstu.nastasia.lab9_contentprovider.data;

import android.content.ContentValues;
import android.database.Cursor;

public class CheckedString {
    private boolean      checked;
    private final String content;
    private final long   timestamp;

    public static boolean DEFAULT_CHECK = false;
    public static int     DEFAULT_CHECK_INT = 0;

    public CheckedString(String content, long timestamp) {
        this.checked = false;
        this.content = content;
        this.timestamp = timestamp;
    }

    private CheckedString(String content, boolean checked, long timestamp) {
        this.checked = checked;
        this.content = content;
        this.timestamp = timestamp;
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

        return values;
    }

    public static CheckedString getFromCursor(Cursor cursor,
                                              DBContract.DBEntry.ColumnNumbers col_idx) {
        String  content = cursor.getString(col_idx.content);
        int     checked = cursor.getInt(col_idx.checked);
        long    time    = cursor.getLong(col_idx.time);
        return new CheckedString(content, 0 != checked, time);
    }

    public String whereClauseQuery() {
        return DBContract.DBEntry.TIME + " = " + timestamp;
    }
}