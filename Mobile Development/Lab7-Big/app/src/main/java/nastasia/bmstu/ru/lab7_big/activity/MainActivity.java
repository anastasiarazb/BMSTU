package nastasia.bmstu.ru.lab7_big.activity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.Loader;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.preference.PreferenceManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import nastasia.bmstu.ru.lab7_big.R;
import nastasia.bmstu.ru.lab7_big.adapter.ClickListener;
import nastasia.bmstu.ru.lab7_big.adapter.ListAdapter;
import nastasia.bmstu.ru.lab7_big.adapter.RecyclerTouchListener;
import nastasia.bmstu.ru.lab7_big.data.CoinStat;
import nastasia.bmstu.ru.lab7_big.data.Currency;
import nastasia.bmstu.ru.lab7_big.data.CurrencyAsyncLoader;

public class MainActivity extends AppCompatActivity
        implements LoaderManager.LoaderCallbacks<JSONObject> {

    public static class Keys {
        public static final String CURRENCY_INFO_KEY = "CURRENCY_INFO";;
        public static final String SELECTED_CURRENCY_KEY = "SELECTED_CURRENCY";
        public static final String CRYPTO_CURRENCY_KEY   = "CRYPTO_CURRENCY_KEY";
    }

    public enum RequestCode {
        SETTINGS
    }

    public final static String TAG = "MainActivity";

    private JSONObject currencyInfo;
    private Currency   selectedCurrency = Currency.USD;


    // ----------------------------- Loader -------------------------


    @Override
    public Loader<JSONObject> onCreateLoader(int id, Bundle bundle) { // = onPreExecute
        Loader<JSONObject> loader = null;
        // условие можно убрать, если вы используете только один загрузчик
        if (id == CurrencyAsyncLoader.LOADER_ID) {
            loader = new CurrencyAsyncLoader(this, bundle);
            Log.d(TAG, "onCreateLoader");
        }
//        loader.forceLoad();
        return loader;
    }

    @Override
    public void onLoadFinished(Loader<JSONObject> loader, JSONObject jsonObject) { // = onPostExecute
        currencyInfo = jsonObject;
        progressBar.setVisibility(ProgressBar.INVISIBLE);
        setCurrencyInfo();
    }

    @Override
    public void onLoaderReset(Loader<JSONObject> loader) {

    }

    public ProgressBar getProgressBar() {
        return progressBar;
    }

    //------------------------------------------------------------------


    private RecyclerView mItemsList;
    private ListAdapter  mListAdapter;
    private Spinner      currencySpinner;
    private EditText     cryptoCurrency;

    private ProgressBar progressBar;

    String locale;
    boolean full_precision;
    int     background_color;
    boolean show_google_button;
    private static List<CoinStat> currency_data;


    // ----------------------------- Menu -------------------------
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }

    void update() {
        String cryptoCur = cryptoCurrency.getText().toString();
        if (TextUtils.isEmpty(cryptoCur)) {
            cryptoCur = "bitcoin";
        }
        int index = (currencySpinner.getSelectedItemPosition() == -1)
                ? 0 : currencySpinner.getSelectedItemPosition();
        selectedCurrency = Currency.getByPosition(index);
        LoaderManager loaderManager = getSupportLoaderManager();
        progressBar.setVisibility(ProgressBar.VISIBLE);
        Bundle params = new Bundle();
        params.putString(Keys.CRYPTO_CURRENCY_KEY, cryptoCur);
        Loader<JSONObject> loader = loaderManager.getLoader(CurrencyAsyncLoader.LOADER_ID);
        if (loader == null) {
            loaderManager.initLoader(CurrencyAsyncLoader.LOADER_ID, params, this);
        } else {
            loaderManager.restartLoader(CurrencyAsyncLoader.LOADER_ID, params, this);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
       recreate();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        final String FUNC_TAG = "onOptionsItemSelected";
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.updateButton:
                Toast.makeText(this, R.string.upd, Toast.LENGTH_LONG).show();
                update();
                setCurrencyInfo();
                return true;
            case R.id.settingsButton:
                Toast.makeText(this, R.string.switch_lang, Toast.LENGTH_LONG).show();
                startActivityForResult(new Intent(this, SettingsActivity.class),
                        RequestCode.SETTINGS.ordinal());
//                getPrefs();
//                setLocale();
//                recreate();
//                Resources resources = getResources();
//                Configuration configuration = resources.getConfiguration();
//                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
//                    locale = configuration.getLocales().get(0).getLanguage();
//                } else {
//                    locale = configuration.locale.getLanguage();
//                }
//                if (locale.equals("ru")) {
//                    locale = "en";
//                } else {
//                    locale = "ru";
//                }
//                setLocale(locale);
//                recreate();
//                showHelp();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public void setLocale() {
        Log.d("setLocale______", locale);
        Resources resources = getResources();
        Configuration configuration = resources.getConfiguration();
        Log.d("System locale__", Locale.getDefault().toString());
        configuration.setLocale(new Locale(locale));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            resources.updateConfiguration(configuration, getResources().getDisplayMetrics());
        } else {
            resources.updateConfiguration(configuration, null);
        }
    }

    // ----------------------------- Activity -------------------------

    void setCurrencyInfo() {
        try {
            if (currencyInfo == null) {
                setErrorMessage(R.string.data_not_loaded);
            } else {
                ArrayList<CoinStat> stat_list  = CoinStat.parse(currencyInfo, selectedCurrency);
                mListAdapter.setData(stat_list, full_precision);
                mListAdapter.notifyDataSetChanged();
                Log.d(TAG, currencyInfo.toString());
            }

        }catch (JSONException ex) {
            ex.printStackTrace();
        }
    }

    void setErrorMessage(int string_id) {
        List<CoinStat> errorMessage = new ArrayList<>(1);
        errorMessage.add(new CoinStat("Error", getString(string_id), null));
        mListAdapter.setData(errorMessage, full_precision);
        mListAdapter.notifyDataSetChanged();
        Log.d("[MAIN]", "" + mListAdapter.getItemCount() + " " + mListAdapter.getData().toString());
    }

    private ArrayAdapter<CharSequence> spinnerAdapter() {
        return new ArrayAdapter<CharSequence>(getBaseContext(), android.R.layout.simple_spinner_item,
                getResources().getStringArray(R.array.currency_list)) {
            @Override
            public View getView(int position, View convertView, ViewGroup parent) {
                View view = super.getView(position, convertView, parent);
                TextView textView = (TextView) view.findViewById(android.R.id.text1);
                // do whatever you want with this text view
//                textView.setBackgroundColor(background_color);
                return view;
            }
        };
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        getPrefs();
        setLocale();
        super.onCreate(savedInstanceState);
        // Загрузить значения по умолчанию
        //PreferenceManager.setDefaultValues(this, R.xml.pref_settings, false);

        setContentView(R.layout.activity_main);
        LinearLayout bgElement = (LinearLayout) findViewById(R.id.main_layout);
//        bgElement.getBackground().setColorFilter(Color.parseColor(getPreferenceColor(this, prefs)), PorterDuff.Mode.DARKEN);
        bgElement.setBackgroundColor(background_color);

        progressBar = (ProgressBar) findViewById(R.id.downloadProgressBar);
//        progressBar.setMax(CurrencyAsyncLoader.MAX_COUNT);
//        CurrencyAsyncLoader.mActivity = new WeakReference<MainActivity>(this);

        if (savedInstanceState != null) {
            currencyInfo = null;
            if (savedInstanceState.containsKey(Keys.CURRENCY_INFO_KEY)) {
                try {
                    String currency_info_string = savedInstanceState.getString(Keys.CURRENCY_INFO_KEY);
                    currencyInfo = (currency_info_string == null)
                            ? null
                            : new JSONObject(savedInstanceState.getString(Keys.CURRENCY_INFO_KEY));
                } catch (JSONException ex) {
                    ex.printStackTrace();
                }
            }
            if (savedInstanceState.containsKey(Keys.SELECTED_CURRENCY_KEY)) {
                selectedCurrency =
                        (Currency)savedInstanceState.getSerializable(Keys.SELECTED_CURRENCY_KEY);
            }
        }
        mItemsList = (RecyclerView) findViewById(R.id.weather_rv);
        currencySpinner = (Spinner) findViewById(R.id.currency_spinner);
        currencySpinner.setAdapter(spinnerAdapter());
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);
        mItemsList.setLayoutManager(linearLayoutManager);
        try {
            if (currencyInfo == null) {
                List<CoinStat> errorMessage = new ArrayList<>(1);
                errorMessage.add(new CoinStat("Error", getString(R.string.please_update), null));
                mListAdapter = new ListAdapter(errorMessage, full_precision);
                setErrorMessage(R.string.data_not_loaded);
            } else {
                ArrayList<CoinStat> stat_list  = CoinStat.parse(currencyInfo, selectedCurrency);
                mListAdapter = new ListAdapter(stat_list, full_precision);
                mListAdapter.notifyDataSetChanged();
                Log.d(TAG, currencyInfo.toString());
                setCurrencyInfo();
            }

        }catch (JSONException ex) {
            ex.printStackTrace();
        }
        mItemsList.setAdapter(mListAdapter);
        mItemsList.addOnItemTouchListener(new RecyclerTouchListener(this,
                mItemsList, new ClickListener() {
            @Override
            public void onClick(View view, final int position) {
                Toast.makeText(MainActivity.this, "Click on " + position, Toast.LENGTH_LONG).show();
                //Values are passing to activity & to fragment as well
//                Toast.makeText(MainActivity.this, "Single Click on position :"+position, Toast.LENGTH_SHORT).show();
                Log.d("onItemTouchListener__", "OK, currency_info = " + currencyInfo);
                if (currencyInfo == null) return;
                String info = DetailedInfo.getDetailedInfoFromJSON(currencyInfo, position);
                Intent childActivityIntent = new Intent(MainActivity.this, DetailedInfo.class)
                        .putExtra(Keys.CURRENCY_INFO_KEY, info);
                Log.d("INFO___", info);
                Log.d("onItemTouchListener__", "OK, intent = " + childActivityIntent);
                if (childActivityIntent.resolveActivity(getPackageManager()) != null) {
                    startActivity(childActivityIntent);
                }
            }

            @Override
            public void onLongClick(View view, int position) {
                onClick(view, position);
//                Toast.makeText(MainActivity.this, "Long press on position :"+position,
//                        Toast.LENGTH_LONG).show();
            }
        }));

        Button googleButton = (Button) findViewById(R.id.google_button);
        LinearLayout button_layout = (LinearLayout) findViewById(R.id.button_layout);
//        bgElement.getBackground().setColorFilter(Color.parseColor(getPreferenceColor(this, prefs)), PorterDuff.Mode.DARKEN);
        bgElement.setBackgroundColor(background_color);
        if (show_google_button) {
            googleButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    String url = "https://finance.google.com/finance/converter";
                    Intent childActivityIntent = new Intent(Intent.ACTION_VIEW)
                            .setData(Uri.parse(url));
                    if (childActivityIntent.resolveActivity(getPackageManager()) != null) {
                        startActivity(childActivityIntent);
                    }
                }
            });
            button_layout.setVisibility(View.VISIBLE);
//            googleButton.setVisibility(Button.VISIBLE);
        } else {
            button_layout.setVisibility(View.GONE);
//            googleButton.setVisibility(Button.GONE);
        }

        cryptoCurrency = (EditText) findViewById(R.id.cryptoCurrencyTV);
        
        setCurrencyInfo();
        mItemsList.setHasFixedSize(true);
    }

    @Override
    protected void onStart() { // Background -> Visible
        super.onStart();
        getPrefs();
        Log.d("onStart______", locale);
        setLocale();

    }

    @Override
    protected void onResume() { // Visible -> Active
        super.onResume();
        getPrefs();
        setLocale();
    }


    @Override
    protected void onPause() { // Active -> Visible
        super.onPause();
    }

    @Override
    protected void onStop() { // Visible -> Background через onStop->onRestart->onStart
        //Вызов onSaveInstanceState(Bundle outstate) до
        super.onStop();
    }

    @Override
    protected void onDestroy() { // Background -> небытие
        //Вызов onSaveInstanceState(Bundle outstate) до
        super.onDestroy();
    }

    @Override
    protected void onSaveInstanceState(Bundle outstate) {
        super.onSaveInstanceState(outstate);
//        String currency_info_string =  (currencyInfo == null)
//                ? null
//                : currencyInfo.toString();
//        outstate.putString(Keys.CURRENCY_INFO_KEY, currency_info_string);
        outstate.putSerializable(Keys.SELECTED_CURRENCY_KEY, selectedCurrency);
//        String locale;
//        Resources resources = getResources();
//        Configuration configuration = resources.getConfiguration();
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
//            locale = configuration.getLocales().get(0).getLanguage();
//        } else {
//            locale = configuration.locale.getLanguage();
//        }
//        outstate.putSerializable(Keys.LOCALE_KEY, locale);
//        // – после onPause
//        // – до onStop, onDestroy
    }

    private void getPrefs() {
        // Get the xml/preferences.xml preferences
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);//getBaseContext());
        locale = prefs.getString(getString(R.string.lang_key), "en");
//        full_precision = prefs.getBoolean(getString(R.string.title_instant_update), false);
        full_precision = prefs.getBoolean(getString(R.string.instant_update_key), false);
        background_color = SettingsActivity.getPreferenceColor(this, prefs);
        show_google_button = prefs.getBoolean(getString(R.string.google_button_key), true);

    }
}
