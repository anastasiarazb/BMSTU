package ru.bmstu.nastasia.lab8_filemanager;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    EditText pathField;
    String   homePath;
    Button   goToListButton;

    enum StorageState {
        MOUNTED, READ_ONLY, EVERYTHING_IS_BAD
    }

    StorageState storageState;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        String state = Environment.getExternalStorageState();
        if (state.equals(Environment.MEDIA_MOUNTED_READ_ONLY)) {
            storageState = StorageState.READ_ONLY;
        } else if (state.equals(Environment.MEDIA_MOUNTED)) {
            storageState = StorageState.MOUNTED;
        } else {
            storageState = StorageState.EVERYTHING_IS_BAD;
            return;
        }
//        Toast.makeText(this, "Storage state is " + storageState.name(), Toast.LENGTH_LONG).show();
        homePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;
        try {
            homePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getCanonicalPath();
            homePath = this.getFilesDir().getCanonicalPath();
        } catch (IOException ex) {
            Log.e("MainActivity", ex.getMessage());
        }
//        homePath = "/sdcard/";
//        homePath = "/storage/0/emulated/30E2-BE02/";

        File home = new File(homePath);
//        Toast.makeText(this, "" + home.isDirectory(), Toast.LENGTH_LONG).show();
        initWidgets();
        createDummyFiles();
    }

    private void initWidgets() {
        pathField = (EditText) findViewById(R.id.pathField);
        goToListButton = (Button) findViewById(R.id.buttonGoToHome);
        goToListButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                Intent childActivityIntent = new Intent(MainActivity.this, FileList.class);
//                startActivityForResult(childActivityIntent, FileList.REQUEST_CODE);
                if (storageState == StorageState.MOUNTED) {
                    Intent childActivityIntent = new Intent(MainActivity.this, FileList.class)
                            .putExtra(FileList.OPEN_DIR_KEY, homePath)
                            .putExtra(FileList.BUTTON_MODE_KEY, FileList.ButtonMode.SAVE_PATH);
                        startActivityForResult(childActivityIntent, FileList.REQUEST_CODE);
                } else {
                    Toast.makeText(getBaseContext(), "Sorry, access denied", Toast.LENGTH_LONG).show();
                    Log.i("goToListButton", "Storage state is " + storageState);
                }
            }
        });
        findViewById(R.id.saveButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String path = pathField.getText().toString();
                File file = new File(path);

                if (!file.isAbsolute()) {
                    Toast.makeText(getBaseContext(), "Please, input absolute path", Toast.LENGTH_LONG).show();
                    Log.i("saveButton", "Absolute path needed, entered " + path);
                    return;
                }
                if (storageState == StorageState.MOUNTED) {
                    Intent childActivityIntent = new Intent(MainActivity.this, FileList.class)
                            .putExtra(FileList.OPEN_DIR_KEY,    homePath)
                            .putExtra(FileList.SAVED_PATH_KEY,  file.getAbsolutePath())
                            .putExtra(FileList.BUTTON_MODE_KEY, FileList.ButtonMode.PASTE);

                    startActivityForResult(childActivityIntent, FileList.REQUEST_CODE);

                } else {
                    Toast.makeText(getBaseContext(), "Sorry, access denied", Toast.LENGTH_LONG).show();
                }
            }
        });
    }

    private void createDummyFiles() {
        String filename = "myfile";
        String string = "Hello world!";
        FileOutputStream outputStream;

        try {
            outputStream = openFileOutput(filename, Context.MODE_PRIVATE);
            outputStream.write(string.getBytes());
            outputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
//        super.onActivityResult(requestCode, resultCode, data);
        if (data == null) {
            Log.d("ActivityResult____", "NULL data, request = " + requestCode + " resultCode = " + resultCode);
            return;
        }
        Log.d("ActivityResult____", "For result");
        if (requestCode == FileList.REQUEST_CODE) {
            Log.d("ActivityResult____", "   Request Code ");
            if(resultCode == FileList.ANSWER_CODE_SUCCESS){
                Log.d("ActivityResult____", "       Answer success");
                if (data.hasExtra(FileList.SAVED_PATH_KEY)) {
                    Log.d("ActivityResult____", "           Saved path key");
                    String result = data.getStringExtra(FileList.SAVED_PATH_KEY);
                    Log.d("ActivityResult____", result);
                    pathField.setText(result);
                }
            }
            if (resultCode == FileList.ANSWER_CODE_FAIL) {
                Log.d("ActivityResult____", "       Answer failed");
                //Write your code if there's no result
            }
        }
    }//onActivityResult

    @Override
    protected void onResume() {
        super.onResume();
        Log.d("onResume", "__________");

    }
}
