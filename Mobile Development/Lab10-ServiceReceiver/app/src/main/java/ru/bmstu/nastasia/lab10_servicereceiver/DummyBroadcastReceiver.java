package ru.bmstu.nastasia.lab10_servicereceiver;


import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

public class DummyBroadcastReceiver extends BroadcastReceiver {

    MainActivity activity;

    private final static String TAG = "___BroadcastReceiver";

    public class Keys {
        final public static String KEY_1 = "key1";
    }

    public DummyBroadcastReceiver(MainActivity activity) {
        super();
        this.activity = activity;
    }

    public DummyBroadcastReceiver() {
        super();
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        Toast.makeText(context, "___onReceive", Toast.LENGTH_SHORT).show();
        if (intent == null) {
            Log.d(TAG, "intent = null");
            return;
        }
        StringBuilder keys = new StringBuilder();
        if (intent.getExtras().keySet() != null) {
            for (String key : intent.getExtras().keySet()) {
                keys.append(key).append(" ");
            }
        }
        Log.d(TAG, intent.getAction() + "\n" + keys);

        if (activity == null) {
            Log.d(TAG, "activity = null");
            return;
        }
        if (intent.getAction().equals("android.intent.action.AIRPLANE_MODE")) {
            if (intent.hasExtra("state")) {
                boolean mode = intent.getBooleanExtra("state", true);
                Log.d(TAG, "airplane mode = " + mode);
                activity.messageTV.setText(String.format("airplane mode = %s", mode ? "ON" : "OFF"));
            }
            return;
        }
        if (intent.getAction().equals("ru.bmstu.lab10_servicereceiver.ACTION_1")) {
            if (intent.hasExtra(Keys.KEY_1)) {
                String message = intent.getStringExtra(Keys.KEY_1);
                Log.d(TAG, message);
                activity.messageTV.setText(message);
            }
            return;
        }

        if (intent.getAction().equals("android.net.conn.CONNECTIVITY_CHANGE")) {
            Log.d(TAG, "WTF I receive this??");
        }
    }
}
