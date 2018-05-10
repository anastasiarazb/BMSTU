package ru.bmstu.nastasia.lab92_contentreceiver;

import android.Manifest;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    TextView textView;
    EditText enterText;
    Button   notesButton;
    Button   alarmButton;
    Button   saveButton;

    public static final int CONTACTS_PERMISSION_REQUEST = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textView  = (TextView)findViewById(R.id.textView);
        enterText = (EditText)findViewById(R.id.editText);
        notesButton  = (Button)findViewById(R.id.updateButton);
        alarmButton  = (Button)findViewById(R.id.alarmButton);
        saveButton   = (Button)findViewById(R.id.saveButton);
        final Uri notesContentUri = Uri.parse("content://ru.bmstu.nastasia.lab9_contentprovider.provider/notes");

        notesButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                StringBuilder res = new StringBuilder();
                Cursor cursor = getContentResolver().query(notesContentUri, null, null, null, null);
                for (int i = 0; i < cursor.getCount(); ++i) {
                    cursor.moveToPosition(i);
                    res.append(cursor.getString(cursor.getColumnIndex("content"))).append('\n');
                }
                cursor.close();
                textView.setText(res.toString());
            }
        });

        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String text = enterText.getText().toString();
                ContentValues values = new ContentValues();

                values.put("content",    text);
                getContentResolver().insert(notesContentUri, values);
            }
        });

        alarmButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (ContextCompat.checkSelfPermission(getBaseContext(),
                        Manifest.permission.READ_CONTACTS)
                        != PackageManager.PERMISSION_GRANTED) {
                    requestPermission();
                    if (ContextCompat.checkSelfPermission(getBaseContext(),
                            Manifest.permission.READ_CONTACTS)
                            == PackageManager.PERMISSION_GRANTED) {
                        getContacts();
                    }
                } else {
                    getContacts();
                }
            }
        });
    }

    private void getContacts() {
        String [] mProjection = new String[]
                {
                        ContactsContract.Profile._ID,
                        ContactsContract.Profile.DISPLAY_NAME_PRIMARY,
                        ContactsContract.Profile.LOOKUP_KEY,
                        ContactsContract.Profile.PHOTO_THUMBNAIL_URI
                };
        mProjection = new String[] {
                ContactsContract.Contacts._ID,
                ContactsContract.Contacts.DISPLAY_NAME
        };

// Retrieves the profile from the Contacts Provider
        Cursor mProfileCursor =
                getContentResolver().query(
                        ContactsContract.Contacts.CONTENT_URI,
//                        ContactsContract.Profile.CONTENT_URI,
                        mProjection ,
                        null,
                        null,
                        null);

        if (mProfileCursor == null) {
            Log.d("Cursor is ", "null");
            return;
        }
        Log.d("Cursor is ", mProfileCursor.toString()  +" sizeof " + mProfileCursor.getCount());
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < mProfileCursor.getCount(); ++i) {
            mProfileCursor.moveToPosition(i);
            sb.append("[").append(mProfileCursor.getInt(0)).append("] ")
                    .append(mProfileCursor.getString(1)).append("\n");

        }
        mProfileCursor.close();
        Log.d("SB is ", sb.toString());
        textView.setText(sb.toString());
    }

    private void requestPermission() {
// Should we show an explanation?
        if (ActivityCompat.shouldShowRequestPermissionRationale(MainActivity.this,
                Manifest.permission.READ_CONTACTS)) {

            // Show an explanation to the user *asynchronously* -- don't block
            // this thread waiting for the user's response! After the user
            // sees the explanation, try again to request the permission.
            new AlertDialog.Builder(this)
                    .setMessage("Calendar permission needed")
                    .setPositiveButton("Grant", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            ActivityCompat.requestPermissions(MainActivity.this,
                                    new String[]{Manifest.permission.READ_CONTACTS},
                                    CONTACTS_PERMISSION_REQUEST);
                        }
                    }).show();

        } else {

            // No explanation needed, we can request the permission.

            ActivityCompat.requestPermissions(MainActivity.this,
                    new String[]{Manifest.permission.READ_CONTACTS},
                    CONTACTS_PERMISSION_REQUEST);

            // MY_PERMISSIONS_REQUEST_READ_CONTACTS is an
            // app-defined int constant. The callback method gets the
            // result of the request.
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull  String permissions[],
                                           @NonNull  int[]  grantResults) {
        switch (requestCode) {
            case CONTACTS_PERMISSION_REQUEST: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {

                    // permission was granted, yay! Do the
                    // contacts-related task you need to do.
                    Log.d("Permission", "granted");

                } else {

                    Log.d("Permission", "denied");
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                }
                return;
            }

            // other 'case' lines to check for other
            // permissions this app might request
        }
    }

}
