package nastasia.bmstu.ru.lab7_sqlite.preferences;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.preference.PreferenceManager;

import nastasia.bmstu.ru.lab7_sqlite.R;

/*
* Этого кода действительно достаточно для некоторых приложений, поскольку как только пользователь изменяет предпочтение, система сохраняет изменения в файле SharedPreferences по умолчанию, который другие компоненты вашего приложения могут читать, когда требуется проверить пользовательские настройки.
* https://developer.android.com/guide/topics/ui/settings.html*/

public class SettingsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);//getBaseContext());

        setContentView(R.layout.activity_settings);
//        LinearLayout bgElement = (LinearLayout) findViewById(R.id.activity_settings);

        // Display the fragment as the main content.
        getFragmentManager().beginTransaction()
                .replace(android.R.id.content, new SettingsFragment())
                .commit();
    }

}
