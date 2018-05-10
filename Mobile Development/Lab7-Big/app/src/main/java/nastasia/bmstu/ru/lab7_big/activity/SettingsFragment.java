package nastasia.bmstu.ru.lab7_big.activity;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.preference.PreferenceScreen;
import android.preference.SwitchPreference;
import android.util.Log;
import android.widget.Toast;

import nastasia.bmstu.ru.lab7_big.R;

public class SettingsFragment extends PreferenceFragment
        implements SharedPreferences.OnSharedPreferenceChangeListener
{

    /*Внимание! Когда вы вызываете приемник registerOnSharedPreferenceChangeListener(), диспетчер предпочтений не сохраняет строгую ссылку на приемник. Вы должны сохранить строгую ссылку на приемник, в противном случае она будет чувствительной к очистке памяти. Мы рекомендуем хранить ссылку на приемник в данных экземпляра объекта , который будет существовать, пока вам нужен приемник.*/
    private SharedPreferences mSharedPreferences;
    public static final String KEY_PREF_SYNC_CONN = "pref_syncConnectionType";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Load the preferences from an XML resource
        addPreferencesFromResource(R.xml.pref_settings);

        mSharedPreferences = PreferenceManager.getDefaultSharedPreferences(getActivity());
        mSharedPreferences.registerOnSharedPreferenceChangeListener(this);
        EditTextPreference editTextPreference =
                (EditTextPreference) findPreference(getString(R.string.color_key));
        editTextPreference.setOnPreferenceChangeListener(new Preference.OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object newValue) {
                Log.d("changed__", newValue.toString());
                if (preference.getKey().equals(getString(R.string.color_key))) {
                    String value = (String) newValue;
                    try {
                        int raw = Integer.parseInt(value, 16);
                        value = String.format("%06X", raw);
                        Color.parseColor("#" + value);
                        return true;
                    } catch (Exception ex) {
                        Log.d("SettingsAFAIL___", value);
                        Toast.makeText(getActivity(), "Input 6 hexadecimal digits. Default #FFFFFF", Toast.LENGTH_LONG).show();
                        return false;
                    }
                }
                return true;
            }
        });
        PreferenceScreen screen = getPreferenceScreen();
        int count = screen.getPreferenceCount();
        for (int i = 0; i < count; ++i) {
            Preference p = screen.getPreference(i);
            if (! (p instanceof CheckBoxPreference)
                    && ! (p instanceof SwitchPreference)) {
                String value = mSharedPreferences.getString(p.getKey(), "");
                setPreferenceSummary(p, value);
            }
        }
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
//        if (!key.equals(KEY_PREF_SYNC_CONN)) {
//            return;
//        }
        Preference preference = findPreference(key);

        if (preference != null) {

            if (preference instanceof CheckBoxPreference
                    || preference instanceof SwitchPreference) {
                return;
            }

            String string = mSharedPreferences.getString(preference.getKey(), "");
            setPreferenceSummary(preference, string);

        }
        getActivity().recreate();
    }

    private void setPreferenceSummary(Preference preference, String value) {
        if (preference instanceof ListPreference) {
            ListPreference listPreference = (ListPreference) preference;

            int index = listPreference.findIndexOfValue(value);

            if (index >= 0) {
                preference.setSummary(listPreference.getEntries()[index]);
                return;
            }
        } else if (preference instanceof EditTextPreference) {
            if (preference.getKey().equals(getString(R.string.color_key))) {
                preference.setSummary("#" + value.toUpperCase());
//                try {
//                    int raw = Integer.parseInt(value, 16);
//                    value = String.format("%06X", raw);
//                    Color.parseColor("#" + value);
//                    preference.setSummary("#" + value);
//                } catch (Exception ex) {
//                    Log.d("SettingsAFAIL___", value);
//                    preference.setSummary("#FFFFFF");
//                    Toast.makeText(getActivity(), "Input 6 hexadecimal digits. Default #FFFFFF", Toast.LENGTH_LONG).show();
//                    return;
//                }
            };
//            preference.setSummary(value);
        }
    }



    @Override
    public void onDestroy() {
        super.onDestroy();
        mSharedPreferences.unregisterOnSharedPreferenceChangeListener(this);
    }

//    @Override
//    public View onCreateView(LayoutInflater inflater, ViewGroup container,
//                             Bundle savedInstanceState) {
//        // Inflate the layout for this fragment
//        return inflater.inflate(R.layout.activity_settings, container, false);
//    }


}