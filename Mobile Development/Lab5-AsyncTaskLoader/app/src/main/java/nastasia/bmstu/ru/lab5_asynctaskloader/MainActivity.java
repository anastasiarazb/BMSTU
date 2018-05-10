package nastasia.bmstu.ru.lab5_asynctaskloader;

import android.content.res.Configuration;
import android.content.res.Resources;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.Loader;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Locale;

public class MainActivity extends AppCompatActivity
        implements android.support.v4.app.LoaderManager.LoaderCallbacks<JSONObject> {

    City city = City.MOSCOW;
    Spinner citySpinner;

    static String lastLocale;
    static boolean isSuccess = false;
    static String weather;
    static int    message_id = R.string.please_update;

    final static String TAG = "MainActivity";


    // =====================================================================================
    //                                  Loader
    // =====================================================================================

//    @Override
//    Loader<JSONObject> onCreateLoader(int id, Bundle args) {
//        Loader<JSONObject> loader = null;
//        // условие можно убрать, если вы используете только один загрузчик
//        if (id == WeatherAsyncTask.ASYNC_LOADER) {
//            loader = new WeatherAsyncTask(this, args);
//            Log.d(TAG, "onCreateLoader");
//        }
//        return loader;
//    }

    @Override
    public Loader<JSONObject> onCreateLoader(int id, Bundle bundle) { // = onPreExecute
         Loader<JSONObject> loader = null;
        // условие можно убрать, если вы используете только один загрузчик
        if (id == WeatherAsyncTask.ASYNC_LOADER) {
            loader = new WeatherAsyncTask(this, bundle);
            Log.d(TAG, "onCreateLoader");
        }
        return loader;
    }

    @Override
    public void onLoadFinished(Loader<JSONObject> loader, JSONObject jsonObject) { // = onPostExecute
        try {
            String message;
            if (jsonObject == null) {
                setErrorMessage(R.string.data_not_loaded);
            } else {
                Weather weather = new Weather(jsonObject);
                message = weather.toString();
                setWeather(message);
            }

        }catch (JSONException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void onLoaderReset(Loader<JSONObject> loader) {

    }

    // =====================================================================================
    //                                 Menu
    // =====================================================================================
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.updateButton:
                Toast.makeText(this, R.string.upd, Toast.LENGTH_LONG).show();
                citySpinner = (Spinner)findViewById(R.id.citySpinner);
                int index = (citySpinner.getSelectedItemPosition() == -1) ? 0 : citySpinner.getSelectedItemPosition();
                city = (index == 0) ? City.MOSCOW : City.LONDON;
                Bundle asyncTaskParams = new Bundle();
                asyncTaskParams.putSerializable(WeatherAsyncTask.BUNDLE_PARAM_KEY,
                        WeatherAsyncTask.buildURL(city));
                LoaderManager loaderManager = getSupportLoaderManager();
                Loader<JSONObject> loader = loaderManager.getLoader(WeatherAsyncTask.ASYNC_LOADER);
                if (loader == null) {
                    loaderManager.initLoader(WeatherAsyncTask.ASYNC_LOADER, asyncTaskParams, this);
                } else {
                    loaderManager.restartLoader(WeatherAsyncTask.ASYNC_LOADER, asyncTaskParams, this);
                }
                return true;
            case R.id.switchLang:
                Toast.makeText(this, R.string.switch_lang, Toast.LENGTH_LONG).show();
                Resources resources = getResources();
                Configuration configuration = resources.getConfiguration();
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                    lastLocale = configuration.getLocales().get(0).getLanguage();
                } else {
                    lastLocale = configuration.locale.getLanguage();
                }
                if (lastLocale.equals("ru")) {
                    lastLocale = "en";
                } else {
                    lastLocale = "ru";
                }
                setLocale(lastLocale);
                recreate();
//                showHelp();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    // =====================================================================================
    //                                Setters
    // =====================================================================================

    public void setLocale(String locale) {
        Resources resources = getResources();
        Configuration configuration = resources.getConfiguration();
        configuration.setLocale(new Locale(locale));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            resources.updateConfiguration(configuration, getResources().getDisplayMetrics());
        } else {
            resources.updateConfiguration(configuration, null);
        }
    }

    void setWeather(String message) {
        TextView textView = (TextView)findViewById(R.id.Weather);
        textView.setText(message);
        weather = message;
        isSuccess = true;
    }

    void setErrorMessage(int string_id) {
        weather = null;
        message_id = string_id;
        isSuccess = false;
        TextView textView = (TextView)findViewById(R.id.Weather);
        textView.setText(message_id);
    }

    // =====================================================================================
    //                        onCreate, onPause, onStop, onDestroy
    // =====================================================================================

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "***************** onCreate\n" + weather);
        citySpinner = (Spinner)findViewById(R.id.citySpinner);
        if (lastLocale != null) {
            setLocale(lastLocale);
        }

        setContentView(R.layout.activity_main);
        if (weather != null) {
            TextView textView = (TextView)findViewById(R.id.Weather);
            if (textView != null) {
                textView.setText(weather);
            }
        } else if (!isSuccess) {
            TextView textView = (TextView)findViewById(R.id.Weather);
            if (textView != null) {
                textView.setText(getString(message_id));
            }
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "***************** onStart\n" + weather);
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "***************** onStop\n"+ weather);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "***************** onDestroy\n"+ weather);
    }
}
