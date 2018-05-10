package nastasia.bmstu.ru.lab4_intent;

import android.app.SearchManager;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class ExplicitActivity extends AppCompatActivity {

    TextView textView;
    EditText editText;
    Button   button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_explicit);

        textView = (TextView) findViewById(R.id.textView2);
        editText = (EditText) findViewById(R.id.searchQuery);
        button   = (Button)   findViewById(R.id.buttonSearch);

        Intent parentIntent = getIntent();



        if (parentIntent.hasExtra(Requests.EXTRA_KEY)) {
            String value = parentIntent.getStringExtra(Requests.EXTRA_KEY);
            editText.setHint("Maybe you want to search for:\n" + String.valueOf(value));

        }

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String query = editText.getText().toString();
                Intent intent = new Intent(Intent.ACTION_WEB_SEARCH);
                intent.putExtra(SearchManager.QUERY, query);
                if (intent.resolveActivity(getPackageManager()) != null) {
                    startActivity(intent);
                }
                Intent answer = new Intent()
                        .putExtra(Requests.ANSWER_KEY, query);
                setResult(Requests.ANSWER_CODE, answer);
                finish();
            }
        });
    }

}
