package ru.bmstu.nastasia.lab8_filemanager;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;

public class FileList extends ListActivity {

    static class ListItem {
        final String  filename;
        final String  absPath;
        final boolean isDir;

        @Override
        public String toString() {
            return isDir
                    ? " > " + filename
                    : filename;
        }

        public ListItem(String path) {
            File file = new File(path);
            filename = file.getName();
            absPath  = file.getAbsolutePath();
            isDir    = file.isDirectory();
        }

        public ListItem(final File file) {
            filename = file.getName();
            absPath  = file.getAbsolutePath();
            isDir    = file.isDirectory();
        }

        static String formatName (String path) {
            File file = new File(path);
            String filename = file.getName();
            if (file.isDirectory()) {
                filename = " > " + filename;
            }
            return filename;
        }

        static String formatName (final File file) {
            String filename = file.getName();
            if (file.isDirectory()) {
                filename = " > " + filename;
            }
            return filename;
        }
    }

//    static class Adapter extends ArrayAdapter<ListItem> {
//        public Adapter (Context context, int id, ListItem [] src) {
//            super(context, id, src);
//        }
////                this, android.R.layout.simple_list_item_1, values);)
//    }

    enum ButtonMode {
        SAVE_PATH, PASTE;

        @Override
        public String toString() {
            return this == PASTE
                    ? "Paste"
                    : "Save";
        }

        public ButtonMode switchMode() {
            return this == SAVE_PATH
                    ? PASTE
                    : SAVE_PATH;
        }
    }

    public static final String OPEN_DIR_KEY    = "OPEN_DIR_KEY";
    public static final String SAVED_PATH_KEY  = "SAVED_PATH_KEY";
    public static final String BUTTON_MODE_KEY = "BUTTON_MODE_KEY";
    public static final int    REQUEST_CODE        = 2;
    public static final int    ANSWER_CODE_SUCCESS = 1;
    public static final int    ANSWER_CODE_FAIL    = -1;

    File   currentDir;
    String savedPath;
    ButtonMode mode;
    ArrayAdapter<ListItem> adapter;
    TextView header;
    ListItem selectedItem;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_list);
        Intent parentIntent = getIntent();
        final Button button = (Button) findViewById(R.id.button);
        header = (TextView) findViewById(R.id.currentDir);
        // Read params
        if (parentIntent.hasExtra(OPEN_DIR_KEY)
                && parentIntent.hasExtra(BUTTON_MODE_KEY)) {
            String filepath = parentIntent.getStringExtra(OPEN_DIR_KEY);
            currentDir = new File(filepath);
            mode = (ButtonMode)parentIntent.getSerializableExtra(BUTTON_MODE_KEY);
        } else {
            setFail("Key \"" + OPEN_DIR_KEY + "\" and \"" + BUTTON_MODE_KEY + "\" not found");
        }

        if (mode == ButtonMode.PASTE) {
            if (parentIntent.hasExtra(SAVED_PATH_KEY)) {
                savedPath = parentIntent.getStringExtra(SAVED_PATH_KEY);
            } else {
                setFail("Key \"" + SAVED_PATH_KEY + "\"  not found though mode is " + mode.name());
            }
        }

        button.setText(mode.toString());

        // Set button listener


        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                setResult(50, new Intent());
//                finish();
                if (mode == ButtonMode.PASTE) {
                    if (!currentDir.canWrite()) {
                        Toast.makeText(getBaseContext(), "Sorry, destination access denied", Toast.LENGTH_LONG).show();
                        return;
                    }
                    File copied = new File(savedPath);
                    if (!copied.canRead()) {
                        Toast.makeText(getBaseContext(), "Sorry, source access denied", Toast.LENGTH_LONG).show();
                        return;
                    }
                    File newPastedFile = new File(currentDir, copied.getName());
                    try {
                        copyFile(copied, newPastedFile);
                    } catch (IOException ex) {
                        Toast.makeText(getBaseContext(), "Error occured during copying", Toast.LENGTH_LONG).show();
                        Log.e("FileList.copyFile()", "Error occured during copying\n" + ex.getMessage());
                        return;
                    }
                    updateDirContent();
                    mode = ButtonMode.PASTE;
                    button.setText(mode.toString());

                } else {
                    //TODO: считывание из списка, сохранение
                    setSavedPath(selectedItem.absPath);
                    Toast.makeText(getBaseContext(), "File selected, return to main window", Toast.LENGTH_LONG).show();
                }
            }
        });

        Button upButton = findViewById(R.id.upButton);
        upButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (currentDir.getParentFile() == null) {
                    Toast.makeText(getBaseContext(), "No parent directory", Toast.LENGTH_LONG).show();
                    return;
                }
                currentDir = currentDir.getParentFile();
                updateDirContent();
            }
        });

        // File navigation
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_activated_2, android.R.id.text1);
        setListAdapter(adapter);
        updateDirContent();


    }

    private void updateDirContent() {
        getListView().setItemChecked(-1, true);
        if (!currentDir.isDirectory()) {
            Toast.makeText(this,
                    currentDir.getAbsolutePath() + " is not a directory", Toast.LENGTH_LONG).show();

        }
        if (!currentDir.canRead()) {
            Toast.makeText(this,
                    currentDir.getAbsolutePath() + " no permissions to read", Toast.LENGTH_LONG).show();
            adapter.clear();
            adapter.notifyDataSetChanged();
            header.setText(currentDir.getAbsolutePath());
            return;
        }
        File[] list = currentDir.listFiles();
        if (list == null) {
            Toast.makeText(this,
                    currentDir.getAbsolutePath() + " returns null files list", Toast.LENGTH_LONG).show();
            adapter.clear();
            adapter.notifyDataSetChanged();
            header.setText(currentDir.getAbsolutePath());
            return;
        }
        ListItem[] files = new ListItem[list.length];
        for (int i = 0; i < files.length; ++i) {
            files[i] = new ListItem(list[i]);
//            Log.d("updateDirContent___", files[i].toString());
        }
        header.setText(currentDir.getAbsolutePath());
        adapter.clear();
        adapter.addAll(files);
        adapter.notifyDataSetChanged();
    }

    private void setFail(String message) {
        setResult(ANSWER_CODE_FAIL);
        Log.wtf("FileList", message);
        finish();
    }

    private void setSavedPath(String filename) {
        Intent answer = getIntent()
                .putExtra(SAVED_PATH_KEY, filename);
        setResult(ANSWER_CODE_SUCCESS, answer);
        Log.d("setSaved_______", ANSWER_CODE_SUCCESS + " " + filename);
        finish();
    }

    @Override
    protected void onListItemClick (ListView l,
                          View v,
                          int position,
                          long id) {
        ListItem item = (ListItem)l.getAdapter().getItem(position);
        Log.d("onClick_________", item.toString() + " " + item.isDir + " " + item.absPath);
        if (item.isDir) {
            currentDir = new File(item.absPath);
            l.setItemChecked(-1, true);
            updateDirContent();
        } else {
            Toast.makeText(this, "File " + item.absPath + " selected ", Toast.LENGTH_LONG).show();
            selectedItem = item;
        }
    }

    private void copyFile(File sourceFile, File destFile) throws IOException {
        if (!destFile.getParentFile().exists()) {
            if (!destFile.getParentFile().mkdirs()) {
                Toast.makeText(this, "Can't get parent directory", Toast.LENGTH_LONG).show();
                return;
            }
        }

        if (!destFile.exists()) {
            if (!destFile.createNewFile()) {
                Toast.makeText(this, "Can't create new file", Toast.LENGTH_LONG).show();
                return;
            }
        }

        FileChannel source = null;
        FileChannel destination = null;

        try {
            source = new FileInputStream(sourceFile).getChannel();
            destination = new FileOutputStream(destFile).getChannel();
            long destFreeSize = destFile.getUsableSpace();
            long srcSize = source.size();
            if (destFreeSize < srcSize) {
                Toast.makeText(this, "Not enough free space", Toast.LENGTH_LONG).show();
            }
            destination.transferFrom(source, 0, source.size());
        } finally {
            if (source != null) {
                source.close();
            }
            if (destination != null) {
                destination.close();
            }
        }
    }

}
