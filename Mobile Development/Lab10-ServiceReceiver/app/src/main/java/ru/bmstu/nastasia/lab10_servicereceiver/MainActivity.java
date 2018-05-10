package ru.bmstu.nastasia.lab10_servicereceiver;

import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    DummyBroadcastReceiver broadcastReceiver;
    Intent service;
    String action = "ru.bmstu.lab10_servicereceiver.ACTION_1";
    TextView messageTV;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        service = new Intent(this, DummyService.class);
        service.putExtra(DummyService.Keys.KEY_1, "Hello world from Main");
        startService(service);

        broadcastReceiver = new DummyBroadcastReceiver(this);
        IntentFilter intentFilter = new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION);
        intentFilter.addAction(Intent.ACTION_AIRPLANE_MODE_CHANGED);
        intentFilter.addAction(action);
        registerReceiver(broadcastReceiver, intentFilter);

        messageTV = (TextView)findViewById(R.id.message);
    }

    @Override
    protected void onPause() {
        super.onPause();

//        unregisterReceiver(broadcastReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        stopService(service);
        unregisterReceiver(broadcastReceiver);
    }
}
