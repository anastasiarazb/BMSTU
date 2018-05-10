package ru.bmstu.nastasia.lab9_contentprovider.preferences;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.preference.PreferenceScreen;
import android.preference.SwitchPreference;

import ru.bmstu.nastasia.lab9_contentprovider.R;

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
            }
        }
    }



    @Override
    public void onDestroy() {
        super.onDestroy();
        mSharedPreferences.unregisterOnSharedPreferenceChangeListener(this);
    }
}