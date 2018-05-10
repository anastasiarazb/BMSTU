package nastasia.bmstu.ru.lab7_big.data;

import android.net.Uri;
import android.os.Bundle;
import android.support.v4.content.AsyncTaskLoader;
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

import nastasia.bmstu.ru.lab7_big.R;
import nastasia.bmstu.ru.lab7_big.activity.MainActivity;

public class CurrencyAsyncLoader extends AsyncTaskLoader<JSONObject> {

    private static final String TAG = "CurrencyAsyncLoader";
    public  static int   LOADER_ID = 1;

    final static String source = "https://coinmarketcap.northpole.ro/history.json";

//    static WeakReference<MainActivity> mActivity;
//    private static final int SLEEP_TIME = 200;
//    public  static final int MAX_COUNT = 100;

    private MainActivity activity;
    private String currency;


    public static URL getURL(String coin) {
        Uri uri = Uri.parse(source).buildUpon()
                .appendQueryParameter("coin", coin)
                .appendQueryParameter("period", "14days")
                .appendQueryParameter("format", "array")
                .build();
        URL url = null;
        try {
            url = new URL(uri.toString());
        } catch (MalformedURLException e) {
            e.printStackTrace();
        }
        return url;
    }

    public CurrencyAsyncLoader(MainActivity context, Bundle args) {
        super(context);
        this.activity = context;
        if (args != null && args.containsKey(MainActivity.Keys.CRYPTO_CURRENCY_KEY)) {
            currency = args.getString(MainActivity.Keys.CRYPTO_CURRENCY_KEY);
        } else {
            currency = "bitcoin";
        }
//        mActivity = new WeakReference<MainActivity>(activity);
    }

    @Override
    public JSONObject loadInBackground() { // = doInBackground
        HttpURLConnection connection;
//        mActivity.get().getProgressBar().setVisibility(ProgressBar.VISIBLE);
        try {
            connection = (HttpURLConnection)getURL(currency).openConnection();
        } catch (IOException ex) {
            Log.e(TAG, "Unable to open connection:\n"+ex.getMessage());
            Toast.makeText(activity, R.string.cannot_connect, Toast.LENGTH_LONG).show();
            ex.printStackTrace();
            return null;
        }
//        for (int i = 0; i < MAX_COUNT; i++) {
//            try {
//                Thread.sleep(SLEEP_TIME);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//            Log.d(getClass().getSimpleName(), "Progress value is " + i);
//            Log.d(getClass().getSimpleName(), "getActivity is " + getContext());
//            Log.d(getClass().getSimpleName(), "this is " + this);
//
//            final int progress = i;
//            if (mActivity.get() != null) {
//                mActivity.get().runOnUiThread(new Runnable() {
//
//                    @Override
//                    public void run() {
//                        mActivity.get().getProgressBar().setProgress(progress);
//                    }
//                });
//            }
//        }
        try {
            InputStream in = new BufferedInputStream(connection.getInputStream());
            BufferedReader reader = new BufferedReader(new InputStreamReader(in, "UTF-8"), 8);
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line).append("\n");
            }
            in.close();
//            Log.d(TAG, sb.toString());
//            mActivity.get().getProgressBar().setVisibility(ProgressBar.INVISIBLE);
            return new JSONObject(sb.toString());
        } catch (JSONException | IOException ex){
            Log.e(TAG, "Unable to readData:\n"+ex.getMessage());
            ex.printStackTrace();
//            mActivity.get().getProgressBar().setVisibility(ProgressBar.INVISIBLE);
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