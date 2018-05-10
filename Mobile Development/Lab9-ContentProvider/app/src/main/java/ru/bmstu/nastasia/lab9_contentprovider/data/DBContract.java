package ru.bmstu.nastasia.lab9_contentprovider.data;

import android.database.Cursor;
import android.provider.BaseColumns;

public class DBContract {
    private DBContract() {}

    public static class DBEntry implements BaseColumns {
        public static final String TABLE_NAME = "notes";
        public static final String TIME       = "add_time";
        public static final String CONTENT    = "content";
        public static final String IS_CHECKED = "is_checked";

        public static class ColumnNumbers {
            final int content, checked, time;

            public ColumnNumbers(final Cursor cursor) {
                checked = cursor.getColumnIndex(DBContract.DBEntry.IS_CHECKED);
                content = cursor.getColumnIndex(DBContract.DBEntry.CONTENT);
                time    = cursor.getColumnIndex(DBContract.DBEntry.TIME);
            }
        }
    }
}