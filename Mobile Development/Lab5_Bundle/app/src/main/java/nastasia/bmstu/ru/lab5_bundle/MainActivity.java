package nastasia.bmstu.ru.lab5_bundle;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {
    static final String TAG = "MainActivity";

    private static final String COUNTER_BUNDLE_KEY = "counter";
    private static final String SOURCE_BUNDLE_KEY  = "source";

    private static int    static_counter = 0;
    private static String static_source;
    
    private int local_saved_counter;
    private int local_counter;

    private EditText editStaticSaved;
    private EditText editBundleSaved;

    private String output() {
        return new StringBuilder()
                .append(" static counter = ").append(static_counter)
                .append(";\r\n local saved counter = ").append(local_saved_counter)
                .append(";\r\n local unsaved = ").append(local_counter)
                .toString();
    }

    private void incrementCounters()
    {
        ++static_counter;
        ++local_saved_counter;
        ++local_counter;
    }

    private void printToLog(String func_name) {
        Log.i(TAG,  "\n\n  *****************  " + func_name + ": " + output());
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) { // небытие -> Background
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        editStaticSaved = (EditText)findViewById(R.id.savedStaticText);
        editBundleSaved = (EditText)findViewById(R.id.savedBundleText);
        local_saved_counter = 0;
        editStaticSaved.setText(static_source);

        // В поле unsaved сохраняеся текст, если в onSaveInstanceState вызвать super(...)

        if (savedInstanceState != null) {
            Log.i(TAG, " ************************* onCreate: Bundle is not null");
            if (savedInstanceState.containsKey(SOURCE_BUNDLE_KEY)) {
                String text = savedInstanceState.getString(SOURCE_BUNDLE_KEY)
                        + " (FROM CUSTOM onSaveInstanceState)";
                // Добавка (FROM CUSTOM onSaveInstanceState) печатается, если в onSaveInstanceState
                // не вызывать super(...). В этом случае пустым остается и поле "Non saved"
                // Почему даже если вызвать onCreate(savedInstanceState) до всего, печатается значение,
                // сохраненное андроидом?
                editBundleSaved.setText(text);
            } else {
                // Срочка ниже не печатается в поле, но есть в логах,
                // если в onSaveInstanceState есть только super(...)
                // Она печатается, если в onSaveInstanceState закомментить все.
                editBundleSaved.setText("Bundle is not null, but doesn't contain custom keys");
                Log.i(TAG, " ************************* Bundle is not null, but doesn't contain custom keys");
            }
            if (savedInstanceState.containsKey(COUNTER_BUNDLE_KEY)) {
                local_saved_counter = savedInstanceState.getInt(COUNTER_BUNDLE_KEY);
            } else {
                local_saved_counter = 0;
            }
        } else {
            Log.i(TAG, " ************************* onCreate: Bundle is null");
        }

        printToLog("onCreate");
        incrementCounters();
//        super.onCreate(savedInstanceState);
    }

    @Override
    protected void onStart() { // Background -> Visible
        super.onStart();
        printToLog("onStart");
        incrementCounters();
    }

    @Override
    protected void onResume() { // Visible -> Active
        super.onResume();
        printToLog("onResume");
//        incrementCounters();
    }


    @Override
    protected void onPause() { // Active -> Visible
        super.onPause();
        printToLog("onPause");
//        incrementCounters();
        //Вызов onSaveInstanceState(Bundle outstate) после
    }

    @Override
    protected void onStop() { // Visible -> Background через onStop->onRestart->onStart
        //Вызов onSaveInstanceState(Bundle outstate) до
        super.onStop();
        printToLog("onStop");
        static_source = editStaticSaved.getText().toString();
        incrementCounters();
    }

    @Override
    protected void onDestroy() { // Background -> небытие
        //Вызов onSaveInstanceState(Bundle outstate) до
        super.onDestroy();
        printToLog("onDestroy");
        static_source = editStaticSaved.getText().toString();
        incrementCounters();
    }

    @Override
    protected void onSaveInstanceState(Bundle outstate) {
//        super.onSaveInstanceState(outstate);
//        // – после onPause
//        // – до onStop, onDestroy
        outstate.putInt(COUNTER_BUNDLE_KEY, local_saved_counter);
        String local_saved_source = editBundleSaved.getText().toString();
        outstate.putString(SOURCE_BUNDLE_KEY, local_saved_source);

    }

}
