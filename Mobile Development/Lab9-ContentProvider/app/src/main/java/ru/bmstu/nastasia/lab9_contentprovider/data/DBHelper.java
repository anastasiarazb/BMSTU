package ru.bmstu.nastasia.lab9_contentprovider.data;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class DBHelper extends SQLiteOpenHelper {
    static final String DB_NAME    = "nastasia_notes.db";
    static final    int DB_VERSION = 1;

    public DBHelper(Context context) {
        super(context, DB_NAME, null, DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase sqLiteDatabase) {
        String createTableQuery = "CREATE TABLE " + DBContract.DBEntry.TABLE_NAME + " ( "
                + DBContract.DBEntry._ID        + " INTEGER PRIMARY KEY AUTOINCREMENT, "
                + DBContract.DBEntry.CONTENT    + " TEXT NOT NULL, "
                + DBContract.DBEntry.IS_CHECKED + " INTEGER NOT NULL, "
                + DBContract.DBEntry.TIME       + " LONG NOT NULL UNIQUE);";
        sqLiteDatabase.execSQL(createTableQuery);
    }

    @Override
    public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i1) {
        String dropTableQuery = "DROP TABLE " + DBContract.DBEntry.TABLE_NAME + ";";
        sqLiteDatabase.execSQL(dropTableQuery);
        onCreate(sqLiteDatabase);
    }

}