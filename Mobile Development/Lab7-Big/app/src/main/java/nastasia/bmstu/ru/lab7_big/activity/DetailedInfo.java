package nastasia.bmstu.ru.lab7_big.activity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.preference.PreferenceManager;
import android.util.Log;
import android.widget.LinearLayout;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import nastasia.bmstu.ru.lab7_big.R;

public class DetailedInfo extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detailed_info);
        Intent parentIntent = getIntent();
        if (parentIntent.hasExtra(MainActivity.Keys.CURRENCY_INFO_KEY)) {
            Log.d("Detailed_info__", "OKOK");
            String info = parentIntent.getStringExtra(MainActivity.Keys.CURRENCY_INFO_KEY);
            TextView textView = (TextView) findViewById(R.id.detailed_TV);
            textView.setText(info);
        }
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);//getBaseContext());
        LinearLayout bgElement = (LinearLayout) findViewById(R.id.detailed_layout);
        bgElement.setBackgroundColor(SettingsActivity.getPreferenceColor(this, prefs));

    }

    public static String getDetailedInfoFromJSON(JSONObject source, int line) {
        try {
            JSONObject history = source.getJSONObject("history");
            JSONArray date_keys = history.names();
            JSONObject date_info = history.getJSONObject(date_keys.getString(line));
            return date_info.toString(2);
        } catch (JSONException ex) {
            return null;
        }
    }

}
