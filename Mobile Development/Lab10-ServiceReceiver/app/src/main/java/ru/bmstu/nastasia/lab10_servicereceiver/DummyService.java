package ru.bmstu.nastasia.lab10_servicereceiver;

import android.app.IntentService;
import android.content.Intent;
import android.support.annotation.Nullable;
import android.util.Log;

public class DummyService extends IntentService {

    public static class Keys {
        final static public String KEY_1 = "Key1";
    }
    private final static String TAG = DummyService.class.getSimpleName();

    public DummyService() {
        super("DummyService");
    }

    @Override
    protected void onHandleIntent(@Nullable Intent intent) {
        String message;
        if (intent != null && intent.getStringExtra(Keys.KEY_1) != null) {
            message = intent.getStringExtra(Keys.KEY_1);
            Log.d(TAG, message);
        } else {
            Log.d(TAG, "message is null");
        }

        try {
            while (true) {
                Log.d(TAG, "service is working");
                Thread.sleep(5000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            Thread.currentThread().interrupt();
        }
    }

    @Override
    public void onCreate() {
        Log.d(TAG, "onCreate()");

        super.onCreate();
    }

    @Override
    public void onStart(@Nullable Intent intent, int startId) {
        Log.d(TAG, "onStart()");

        super.onStart(intent, startId);
    }

    @Override
    public int onStartCommand(@Nullable Intent intent, int flags, int startId) {
        Log.d(TAG, "onStartCommand()");

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "onDestroy()");

        super.onDestroy();
    }
}