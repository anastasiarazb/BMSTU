package nastasia.bmstu.ru.lab7_big.activity;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.preference.PreferenceManager;
import android.util.Log;
import android.widget.LinearLayout;

import java.util.Locale;

import nastasia.bmstu.ru.lab7_big.R;

/*
* Этого кода действительно достаточно для некоторых приложений, поскольку как только пользователь изменяет предпочтение, система сохраняет изменения в файле SharedPreferences по умолчанию, который другие компоненты вашего приложения могут читать, когда требуется проверить пользовательские настройки.
* https://developer.android.com/guide/topics/ui/settings.html*/

public class SettingsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);//getBaseContext());
        setLocale(this, prefs);
        setContentView(R.layout.activity_settings);
        LinearLayout bgElement = (LinearLayout) findViewById(R.id.activity_settings);
//        bgElement.getBackground().setColorFilter(Color.parseColor(getPreferenceColor(this, prefs)), PorterDuff.Mode.DARKEN);
        bgElement.setBackgroundColor(getPreferenceColor(this, prefs));
//        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        // Display the fragment as the main content.
        getFragmentManager().beginTransaction()
                .replace(android.R.id.content, new SettingsFragment())
                .commit();
    }

    public static int getPreferenceColor(Context context, SharedPreferences preferences) {
        String content = preferences.getString(context.getString(R.string.color_key), "FFFFFF")
                .toLowerCase();
        try {
            int raw = Integer.parseInt(content, 16);
            content = String.format("%06X", raw);
            return Color.parseColor("#" + content);
        } catch (Exception ex) {
            Log.d("SettingsAFAIL___", content);
            return Color.WHITE;
        }
    }

    public static void setLocale(Context context, SharedPreferences preferences) {
        String locale = preferences.getString(context.getString(R.string.lang_key), "en");
        Log.d("setLocaleSettings______", locale);
        Resources resources = context.getResources();
        Configuration configuration = resources.getConfiguration();
        Log.d("SystemLocalSettings__", Locale.getDefault().toString());
        configuration.setLocale(new Locale(locale));
    }

}
