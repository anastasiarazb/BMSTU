package nastasia.bmstu.ru.lab4_intent;

import android.app.SearchManager;
import android.content.Intent;
import android.net.Uri;
import android.provider.ContactsContract;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


class Requests {
    public static final int REQUEST_CODE = 0;
    public static final int ANSWER_CODE  = 1;
    public static final int MAP_REQ_CODE = 2;
    public static final int MAP_ANSW_CODE = 3;
    public static final String EXTRA_KEY   = "ValueToPrint";
    public static final String ANSWER_KEY  = "string";
}

public class MainActivity extends AppCompatActivity {

    TextView tv;
    Button button1, button2, button3, button4, button5;

    EditText phone, email, message;


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (data == null) {return;}

        switch(requestCode){
            case Requests.REQUEST_CODE:
                switch (resultCode) {
                    case Requests.ANSWER_CODE:
                        if (data.hasExtra(Requests.ANSWER_KEY)) {
                            String message = data.getStringExtra(Requests.ANSWER_KEY);
                            tv.setText("The last web search request:\n " + message);
                        }
                        break;
                }
                break;
            case Requests.MAP_REQ_CODE:
                switch (resultCode) {
                    case Requests.MAP_ANSW_CODE:
                        break;
                }
                break;
        }

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tv = (TextView)findViewById(R.id.textView);
        button1 = (Button) findViewById(R.id.button1);
        button2 = (Button) findViewById(R.id.button2);
        button3 = (Button) findViewById(R.id.button3);
        button4 = (Button) findViewById(R.id.button4);
        button5 = (Button) findViewById(R.id.button5);

        phone  = (EditText) findViewById(R.id.editText);
        email  = (EditText) findViewById(R.id.editText2);
        message = (EditText) findViewById(R.id.editText3);

        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String query = message.getText().toString();
                Intent childActivityIntent = new Intent(MainActivity.this, ExplicitActivity.class)
                                .putExtra(Requests.EXTRA_KEY, query);
                if (childActivityIntent.resolveActivity(getPackageManager()) != null) {
                    startActivityForResult(childActivityIntent, Requests.REQUEST_CODE);
                }
            }
        });

        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String address = email.getText().toString();
                String body    = message.getText().toString();

                Intent childActivityIntent = new Intent(Intent.ACTION_SEND)
                        .setType("text/plain")
                        .putExtra(Intent.EXTRA_EMAIL, new String [] {address} )
                        .putExtra(Intent.EXTRA_SUBJECT, body);
                if (childActivityIntent.resolveActivity(getPackageManager()) != null) {
                    startActivity(childActivityIntent);
                }
            }
        });

        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String number = phone.getText().toString();

                Intent childActivityIntent = new Intent(Intent.ACTION_DIAL)
                        .setData(Uri.parse("tel:" + number));
                if (childActivityIntent.resolveActivity(getPackageManager()) != null) {
                    startActivity(childActivityIntent);
                }
            }
        });

        button4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String sms = message.getText().toString();
                String number = phone.getText().toString();

                Intent childActivityIntent = new Intent(Intent.ACTION_SENDTO)
                        .setData(Uri.parse("smsto:" + number))
                        .putExtra("sms_body", sms);
                if (childActivityIntent.resolveActivity(getPackageManager()) != null) {
                    startActivity(childActivityIntent);
                }
            }
        });

        button5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String query = message.getText().toString();

                Intent childActivityIntent = new Intent(Intent.ACTION_SEARCH)
                        .putExtra(SearchManager.QUERY, query);
                if (childActivityIntent.resolveActivity(getPackageManager()) != null) {
                    startActivity(childActivityIntent);
                }
            }
        });



    }
}
