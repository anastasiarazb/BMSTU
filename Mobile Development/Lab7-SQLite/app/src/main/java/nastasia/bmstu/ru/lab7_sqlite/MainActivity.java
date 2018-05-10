package nastasia.bmstu.ru.lab7_sqlite;

import android.content.ContentValues;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
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

import nastasia.bmstu.ru.lab7_sqlite.adapter.ClickListener;
import nastasia.bmstu.ru.lab7_sqlite.adapter.ListAdapter;
import nastasia.bmstu.ru.lab7_sqlite.adapter.RecyclerTouchListener;
import nastasia.bmstu.ru.lab7_sqlite.common.RequestCodes;
import nastasia.bmstu.ru.lab7_sqlite.data.CheckedString;
import nastasia.bmstu.ru.lab7_sqlite.data.DBContract;
import nastasia.bmstu.ru.lab7_sqlite.data.DBHelper;
import nastasia.bmstu.ru.lab7_sqlite.data.SortBy;
import nastasia.bmstu.ru.lab7_sqlite.preferences.SettingsActivity;

public class MainActivity extends AppCompatActivity {

    ArrayList<CheckedString> notesList;
    ListAdapter listAdapter;
    SQLiteDatabase database;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getPrefs();
        setContentView(R.layout.activity_main);
        DBHelper dbHelper = new DBHelper(this);
        database = dbHelper.getReadableDatabase();
//        dbHelper.onUpgrade(database, 0, 0);
        notesList   = new ArrayList<>();
        readAllData();
        initWidgets();
        sortNotes();
        listAdapter.notifyDataSetChanged();
        Log.d("onCreate()",  notesList.toString());
    }

    private void insertData(final CheckedString string) {
        ContentValues values = string.toContentValues();
        database.insert(DBContract.DBEntry.TABLE_NAME, null, values);
    }

    private void readAllData() {
        String sortOrder = SortBy.toSQLQuery(sortByItem);
        Cursor cursor =  database.query(
                DBContract.DBEntry.TABLE_NAME, null, null, null, null, null, sortOrder);
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
        int number = string.getNumber();
        boolean success
                = database.delete(DBContract.DBEntry.TABLE_NAME, string.whereClauseQuery(), null) > 0;
        for (CheckedString entry: notesList) {
            if (entry.getNumber() > number) {
                entry.setNumber(entry.getNumber() - 1);
                database.update(DBContract.DBEntry.TABLE_NAME,entry.toContentValues(),
                        DBContract.DBEntry.TIME + " = ? ",
                        new String[] {String.valueOf(entry.getTimestamp())});
            }
        }
        return success;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        recreate();
    }

    private void synchronizeDBtoList() {
        database.delete(DBContract.DBEntry.TABLE_NAME, null, null);
        for (int i = 0; i < notesList.size(); ++i) {
            notesList.get(i).setNumber(i);
            insertData(notesList.get(i));
        }
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
                CheckedString newEntry =
                        new CheckedString(newNote, System.currentTimeMillis(), notesList.size());
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
                database.update(DBContract.DBEntry.TABLE_NAME,
                        modified.toContentValues(), modified.whereClauseQuery(), null);
                if (sortByItem.equals(SortBy.CHECKED) || sortByItem.equals(SortBy.UNCHECKED)) {
                    sortNotes();
                }
                listAdapter.notifyDataSetChanged();
//                Toast.makeText(MainActivity.this, "Click on " + position, Toast.LENGTH_LONG).show();
            }

            @Override
            public void onLongClick(View view, int position) {
//                onClick(view, position);
//                Toast.makeText(MainActivity.this, "Long press on position :"+position,
//                        Toast.LENGTH_LONG).show();
            }
        }));
        new ItemTouchHelper(
                new ItemTouchHelper.SimpleCallback(0,
                        ItemTouchHelper.LEFT | ItemTouchHelper.RIGHT) {
                    @Override
                    public int getMovementFlags(RecyclerView recyclerView,
                                                RecyclerView.ViewHolder viewHolder) {
                        int dragFlags = ItemTouchHelper.UP | ItemTouchHelper.DOWN;
                        int swipeFlags = ItemTouchHelper.START | ItemTouchHelper.END;
                        return makeMovementFlags(dragFlags, swipeFlags);
                    }

                    @Override
                    public boolean isLongPressDragEnabled() {
                        return true;
                    }

                    @Override
                    public boolean onMove(RecyclerView recyclerView, RecyclerView.ViewHolder vh, RecyclerView.ViewHolder target) {
                        int fromPosition = vh.getAdapterPosition();
                        int toPosition   = target.getAdapterPosition();
                        if (fromPosition < toPosition) {
                            for (int i = fromPosition; i < toPosition; i++) {
                                Collections.swap(notesList, i, i + 1);
                                CheckedString.swapNumbers(notesList.get(i), notesList.get(i + 1));
                            }
                        } else {
                            for (int i = fromPosition; i > toPosition; i--) {
                                Collections.swap(notesList, i, i - 1);
                                CheckedString.swapNumbers(notesList.get(i), notesList.get(i - 1));
                            }
                        }

//                        listAdapter.notifyDataSetChanged();
                        synchronizeDBtoList();
                        listAdapter.notifyItemMoved(fromPosition, toPosition);
                        return true;
                    }

                    @Override
                    public void onSelectedChanged(RecyclerView.ViewHolder viewHolder, int actionState) {
                        if (actionState != ItemTouchHelper.ACTION_STATE_IDLE) {
                            if (viewHolder instanceof ListAdapter.InfoString) {
                                ((ListAdapter.InfoString) viewHolder).setSelectedColor();
                            }
                        }
                        super.onSelectedChanged(viewHolder, actionState);
                    }

                    @Override
                    public void clearView(RecyclerView rv, RecyclerView.ViewHolder viewHolder) {
                        if (viewHolder instanceof ListAdapter.InfoString) {
                            ((ListAdapter.InfoString) viewHolder).setDefaultColor();
                        }
                        super.clearView(rv, viewHolder);
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

    private void shuffleNotes() {
        Collections.shuffle(notesList);
        database.delete(DBContract.DBEntry.TABLE_NAME, null, null);
        for (int i = 0; i < notesList.size(); ++i) {
            notesList.get(i).setNumber(i);
            insertData(notesList.get(i));
        }
        listAdapter.notifyDataSetChanged();

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
                database.delete(DBContract.DBEntry.TABLE_NAME, null, null);
                notesList.clear();
                listAdapter.notifyDataSetChanged();
                return true;
            case R.id.shuffleButton:
                shuffleNotes();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }



    // ----------------------- onStart/onResume/onDestroy/onPause ---------------------------------

    // TODO: убрать лишние синхронизации
    @Override
    protected void onPause() {
        super.onPause();
        synchronizeDBtoList();
    }
}
