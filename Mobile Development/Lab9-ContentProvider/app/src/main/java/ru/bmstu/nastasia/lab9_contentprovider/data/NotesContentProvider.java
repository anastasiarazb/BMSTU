package ru.bmstu.nastasia.lab9_contentprovider.data;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.util.Log;

public class NotesContentProvider extends ContentProvider {

    private static final int MULTIPLE_ROWS = 1;
    private static final int SINGLE_ROW    = 2;

    private static String AUTHORITY = "ru.bmstu.nastasia.lab9_contentprovider.provider";

    public static Uri NOTES_CONTENT_URI = Uri.parse("content://"
            + AUTHORITY + "/" + DBContract.DBEntry.TABLE_NAME);


    private static final UriMatcher sUriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
    static {
        sUriMatcher.addURI(AUTHORITY, DBContract.DBEntry.TABLE_NAME,        MULTIPLE_ROWS);
        sUriMatcher.addURI(AUTHORITY, DBContract.DBEntry.TABLE_NAME + "/#" ,SINGLE_ROW);
    }

    DBHelper dbHelper;

    @Override
    public boolean onCreate() {
        dbHelper = new DBHelper(getContext());
        return true;
    }

    @Nullable
    @Override
    public Cursor query(@NonNull Uri uri, @Nullable String[] projection,
                        @Nullable String selection, @Nullable String[] selectionArgs,
                        @Nullable String sortOrder) {

        switch (sUriMatcher.match(uri)) {
            case MULTIPLE_ROWS:
                if (TextUtils.isEmpty(sortOrder)) sortOrder = SortBy.toSQLQuery(SortBy.DEFAULT);
                break;
            case SINGLE_ROW:
                selection = selection + " AND _ID = " + uri.getLastPathSegment();
                break;
            default:
                throw new IllegalStateException("Unknown URI " + uri + " with match " + sUriMatcher.match(uri));
        }
        SQLiteDatabase database = dbHelper.getWritableDatabase();
        Cursor cursor = database.query(DBContract.DBEntry.TABLE_NAME, projection, selection,
                selectionArgs, null, null, sortOrder);
            // просим ContentResolver уведомлять этот курсор
        // об изменениях данных в CONTACT_CONTENT_URI
        cursor.setNotificationUri(getContext().getContentResolver(), NOTES_CONTENT_URI);
        return cursor;

    }

    // MIME-type строки. Вызывается из приложений 3-й стороны для того, чтобы понять, каким
    // приложением открывать результат
    @Nullable
    @Override
    public String getType(@NonNull Uri uri) {
        final int match = sUriMatcher.match(uri);
        switch (match) {
            case MULTIPLE_ROWS:
                return "vnd.android.cursor.dir";
            case SINGLE_ROW:
                return "vnd.android.cursor.item";
            default:
                throw new IllegalStateException("Unknown URI " + uri + " with match " + match);
        }
    }

    @Nullable
    @Override
    public Uri insert(@NonNull Uri uri, @Nullable ContentValues values) {
        if (values == null) return null;
        Log.d("NotesProvider.insert___", uri.toString());
        final int match = sUriMatcher.match(uri);
        Log.d("NotesProvider.insert___", uri.toString());
//        if (match != SINGLE_ROW)
//            throw new IllegalArgumentException("Wrong URI: " + uri);
        if (!values.containsKey(DBContract.DBEntry.IS_CHECKED)) {
            values.put(DBContract.DBEntry.IS_CHECKED, CheckedString.DEFAULT_CHECK_INT);
        }
        if (!values.containsKey(DBContract.DBEntry.TIME)) {
            values.put(DBContract.DBEntry.TIME, System.currentTimeMillis());
        }
        SQLiteDatabase database = dbHelper.getWritableDatabase();
        long insertedRowId = database.insert(DBContract.DBEntry.TABLE_NAME, null, values);
        Uri resultUri = ContentUris.withAppendedId(NOTES_CONTENT_URI, insertedRowId);
        // уведомляем ContentResolver, что данные по адресу resultUri изменились
        getContext().getContentResolver().notifyChange(resultUri, null);
        return resultUri;
    }

    @Override
    public int delete(@NonNull Uri uri, @Nullable String selection, @Nullable String[] selectionArgs) {
        switch (sUriMatcher.match(uri)) {
            case MULTIPLE_ROWS:
                break;
            case SINGLE_ROW:
                String id = uri.getLastPathSegment();
                if (TextUtils.isEmpty(selection)) {
                    selection = DBContract.DBEntry._ID + " = " + id;
                } else {
                    selection = selection + " AND " + DBContract.DBEntry._ID + " = " + id;
                }
                break;
            default:
                throw new IllegalArgumentException("Wrong URI: " + uri);
        }
        SQLiteDatabase database = dbHelper.getWritableDatabase();
        int cnt = database.delete(DBContract.DBEntry.TABLE_NAME, selection, selectionArgs);
        getContext().getContentResolver().notifyChange(uri, null);
        return cnt;
    }

    @Override
    public int update(@NonNull Uri uri, @Nullable ContentValues values, @Nullable String selection,
                      @Nullable String[] selectionArgs) {
        switch (sUriMatcher.match(uri)) {
            case MULTIPLE_ROWS:
                break;
            case SINGLE_ROW:
                String id = uri.getLastPathSegment();
                if (TextUtils.isEmpty(selection)) {
                    selection = DBContract.DBEntry._ID + " = " + id;
                } else {
                    selection = selection + " AND " + DBContract.DBEntry._ID + " = " + id;
                }
                break;
            default:
                throw new IllegalArgumentException("Wrong URI: " + uri);
        }
        SQLiteDatabase database = dbHelper.getWritableDatabase();
        int cnt = database.update(DBContract.DBEntry.TABLE_NAME, values, selection, selectionArgs);
        getContext().getContentResolver().notifyChange(uri, null);
        return cnt;
    }
}