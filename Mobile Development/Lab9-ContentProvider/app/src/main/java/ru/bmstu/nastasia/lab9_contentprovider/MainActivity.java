package ru.bmstu.nastasia.lab9_contentprovider;

import android.content.ContentValues;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Collections;

import ru.bmstu.nastasia.lab9_contentprovider.adapter.ClickListener;
import ru.bmstu.nastasia.lab9_contentprovider.adapter.ListAdapter;
import ru.bmstu.nastasia.lab9_contentprovider.adapter.RecyclerTouchListener;
import ru.bmstu.nastasia.lab9_contentprovider.common.RequestCodes;
import ru.bmstu.nastasia.lab9_contentprovider.data.CheckedString;
import ru.bmstu.nastasia.lab9_contentprovider.data.DBContract;
import ru.bmstu.nastasia.lab9_contentprovider.data.SortBy;
import ru.bmstu.nastasia.lab9_contentprovider.preferences.SettingsActivity;

public class MainActivity extends AppCompatActivity {

    ArrayList<CheckedString> notesList;
    ListAdapter listAdapter;

    final static Uri notesContentUri = Uri.parse("content://ru.bmstu.nastasia.lab9_contentprovider.provider/notes/#");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getPrefs();
        setContentView(R.layout.activity_main);
        //TODO: доставать из базы
        notesList   = new ArrayList<>();
        readAllData();
        initWidgets();
        sortNotes();
        listAdapter.notifyDataSetChanged();
        Log.d("onCreate()",  notesList.toString());
    }

    private void insertData(final CheckedString string) {
        ContentValues values = string.toContentValues();
        Log.d("insertData_____", notesContentUri.toString());
        getContentResolver().insert(notesContentUri, values);
    }

    private void readAllData() {
        String sortOrder = SortBy.toSQLQuery(sortByItem);
        Cursor cursor =  getContentResolver().query(
                notesContentUri, null, null, null, sortOrder);
        DBContract.DBEntry.ColumnNumbers columnNumbers =
                new DBContract.DBEntry.ColumnNumbers(cursor);
        notesList.clear();
        Log.d("readAllData___", cursor.getCount() + "cursor count");
        for (int i = 0; i < cursor.getCount(); ++i) {
            cursor.moveToPosition(i);
            notesList.add(CheckedString.getFromCursor(cursor, columnNumbers));
        }
        cursor.close();
    }

    private boolean deleteData(final CheckedString string) {
        return getContentResolver().delete(notesContentUri, string.whereClauseQuery(), null) > 0;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        recreate();
    }



    // ----------------------- Widgets ---------------------------------

    RecyclerView notesListRV;
    EditText     newNoteET;
    Button       saveButton;

    String       sortByItem = SortBy.DEFAULT;

    private void initWidgets() {
        initRecyclerView();

        newNoteET   = (EditText)     findViewById(R.id.newNoteET);
        saveButton  = (Button)       findViewById(R.id.saveButton);
        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String newNote = newNoteET.getText().toString();
                if (newNote.length() == 0) return;
                CheckedString newEntry = new CheckedString(newNote, System.currentTimeMillis());
                notesList.add(newEntry);
                insertData(newEntry);
                Log.d("on click___", newNote + '\n' + notesList);
                sortNotes();
                listAdapter.notifyDataSetChanged();
                newNoteET.setText(null);
            }
        });
    }

    private void initRecyclerView() {
        notesListRV = (RecyclerView) findViewById(R.id.notesRV);
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);
        notesListRV.setLayoutManager(linearLayoutManager);
        notesListRV.setLayoutManager(linearLayoutManager);
        listAdapter = new ListAdapter(notesList);
        notesListRV.setAdapter(listAdapter);
        notesListRV.setHasFixedSize(true);
        notesListRV.addOnItemTouchListener(new RecyclerTouchListener(this,
                notesListRV, new ClickListener() {
            @Override
            public void onClick(View view, final int position) {
                CheckedString modified = notesList.get(position);
                modified.switchChecked();
                getContentResolver().update(notesContentUri,
                        modified.toContentValues(), modified.whereClauseQuery(), null);
                if (sortByItem.equals(SortBy.CHECKED) || sortByItem.equals(SortBy.UNCHECKED)) {
                    sortNotes();
                }
                listAdapter.notifyDataSetChanged();
//                Toast.makeText(MainActivity.this, "Click on " + position, Toast.LENGTH_LONG).show();
            }

            @Override
            public void onLongClick(View view, int position) {
                onClick(view, position);
//                Toast.makeText(MainActivity.this, "Long press on position :"+position,
//                        Toast.LENGTH_LONG).show();
            }
        }));
        new ItemTouchHelper(
                new ItemTouchHelper.SimpleCallback(0,
                        ItemTouchHelper.LEFT | ItemTouchHelper.RIGHT) {
                    @Override
                    public boolean onMove(RecyclerView recyclerView, RecyclerView.ViewHolder vh, RecyclerView.ViewHolder target) {
                        return false;
                    }

                    @Override
                    public void onSwiped(RecyclerView.ViewHolder viewHolder, int swipeDir) {
                        int pos = viewHolder.getAdapterPosition();
                        CheckedString removed = notesList.remove(pos);
                        listAdapter.notifyDataSetChanged();
                        deleteData(removed);
                        Toast.makeText(getBaseContext(),
                                getString(R.string.notify_deleted)
                                        + " \"" + removed.getContent() + "\"",
                                Toast.LENGTH_LONG).show();
                    }
                }
        ).attachToRecyclerView(notesListRV);
    }

    private void sortNotes() {
        Collections.sort(notesList, SortBy.getComparatorBy(sortByItem));
    }

    private void getPrefs() {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
        sortByItem = prefs.getString(getString(R.string.key_sort_by_item), SortBy.DEFAULT);
    }

    // ----------------------- Menu ---------------------------------------------------------------
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.settingsButton:
                startActivityForResult(new Intent(this, SettingsActivity.class),
                        RequestCodes.SETTINGS.ordinal());
                return true;
            case R.id.deleteAllButton:
                Toast.makeText(this, R.string.setting_del_all, Toast.LENGTH_LONG).show();
                getContentResolver().delete(notesContentUri, null, null);
                notesList.clear();
                listAdapter.notifyDataSetChanged();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }



    // ----------------------- onStart/onResume/onDestroy/onPause ---------------------------------
}
