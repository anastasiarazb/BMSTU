package ru.bmstu.nastasia.lab102_broadcastreceivercall;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.sendButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intentReceiver = new Intent();
                intentReceiver.setAction("ru.bmstu.lab10_servicereceiver.ACTION_1");
                intentReceiver.putExtra("key1", "Hello from Main from lab10");
                sendBroadcast(intentReceiver);
            }
        });
    }
}
