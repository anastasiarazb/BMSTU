package nastasia.bmstu.ru.lab5_asynctaskloader;

import android.support.v4.content.AsyncTaskLoader;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

enum City {
    LONDON, MOSCOW, UNKNOWN;

    @Override
    public String toString() {
        switch (this) {
            case LONDON:
                return "London,uk";
            case MOSCOW:
                return "Moscow,ru";
            default:
                return "unknown";
        }
    }
}

public class WeatherAsyncTask extends AsyncTaskLoader<JSONObject> {

    //    public final static String url = "https://openweathermap.org/current";
    //http://api.openweathermap.org/data/2.5/weather?q=London&APPID=5dc8a4faa39fc2514667c093c4bc1ab7

    /*

{"coord":{"lon":-0.13,"lat":51.51},"weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],"base":"stations","main":{"temp":289.7,"pressure":1018,"humidity":72,"temp_min":288.15,"temp_max":291.15},"visibility":10000,"wind":{"speed":5.1,"deg":220},"clouds":{"all":90},"dt":1505920800,"sys":{"type":1,"id":5091,"message":0.0034,"country":"GB","sunrise":1505886277,"sunset":1505930501},"id":2643743,"name":"London","cod":200}
    */
    final static String source = "http://api.openweathermap.org/data/2.5/weather";
    final static String api_key = "5dc8a4faa39fc2514667c093c4bc1ab7"; //value 2
    final static String appid   = "APPID";  //key 2
    final static String key_city = "q";

    final static String BUNDLE_PARAM_KEY = "address";
    final static int ASYNC_LOADER = 777;

    URL url;


    public static URL buildURL(City city) {
        Uri uri = Uri.parse(source).buildUpon()
                .appendQueryParameter(key_city, city.toString())
                .appendQueryParameter(appid, api_key)
                .build();
        URL url = null;
        try {
            url = new URL(uri.toString());
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
        return url;
    }

    private static final String TAG = "WeatherAsyncTask";

    private MainActivity activity;


    public WeatherAsyncTask(MainActivity context, Bundle args) {
        super(context);
        this.activity = context;
        url = (URL)args.getSerializable(BUNDLE_PARAM_KEY);
    }

    @Override
    public JSONObject loadInBackground() { // = doInBackground
        HttpURLConnection connection;
        try {
            if (url == null) {
                Log.e(TAG, "Unable to readData: argument " + BUNDLE_PARAM_KEY
                        + " not found in Bundle args.\r\n");
                return null;
            }
            connection = (HttpURLConnection)url.openConnection();
        } catch (IOException ex) {
            Log.e(TAG, "Unable to open connection:\n"+ex.getMessage());
            Toast.makeText(activity, R.string.cannot_connect, Toast.LENGTH_LONG).show();
            ex.printStackTrace();
            return null;
        }
        try {
            InputStream in = new BufferedInputStream(connection.getInputStream());
            BufferedReader reader = new BufferedReader(new InputStreamReader(in, "UTF-8"), 8);
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line).append("\n");
            }
            in.close();
            return new JSONObject(sb.toString());
        } catch (JSONException | IOException ex){
            Log.e(TAG, "Unable to readData:\n"+ex.getMessage());
            ex.printStackTrace();
            return null;
        } finally {
            connection.disconnect();
        }
    }

    @Override
    public void forceLoad() {
        Log.d(TAG, "forceLoad");
        super.forceLoad();
    }

    @Override
    protected void onStartLoading() {
        if (url == null) {
            Log.e(TAG, "onStartLoading: url is null");
            return;
        }
        super.onStartLoading();
        Log.d(TAG, "onStartLoading");
        forceLoad();
    }

    @Override
    protected void onStopLoading() {
        super.onStopLoading();
        Log.d(TAG, "onStopLoading");
    }

}