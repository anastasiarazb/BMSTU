package nastasia.bmstu.ru.lab3_recyclerview;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.GestureDetector;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {

    public static interface ClickListener{
        public void onClick(View view,int position);
        public void onLongClick(View view,int position);
    }

    class RecyclerTouchListener implements RecyclerView.OnItemTouchListener{

        private ClickListener clicklistener;
        private GestureDetector gestureDetector;

        public RecyclerTouchListener(Context context, final RecyclerView recycleView, final ClickListener clicklistener){

            this.clicklistener=clicklistener;
            gestureDetector=new GestureDetector(context,new GestureDetector.SimpleOnGestureListener(){
                @Override
                public boolean onSingleTapUp(MotionEvent e) {
                    return true;
                }

                @Override
                public void onLongPress(MotionEvent e) {
                    View child=recycleView.findChildViewUnder(e.getX(),e.getY());
                    if(child!=null && clicklistener!=null){
                        clicklistener.onLongClick(child,recycleView.getChildAdapterPosition(child));
                    }
                }
            });
        }

        @Override
        public boolean onInterceptTouchEvent(RecyclerView rv, MotionEvent e) {
            View child=rv.findChildViewUnder(e.getX(),e.getY());
            if(child!=null && clicklistener!=null && gestureDetector.onTouchEvent(e)){
                clicklistener.onClick(child,rv.getChildAdapterPosition(child));
            }

            return false;
        }

        @Override
        public void onTouchEvent(RecyclerView rv, MotionEvent e) {

        }

        @Override
        public void onRequestDisallowInterceptTouchEvent(boolean disallowIntercept) {

        }
    }

    private City city = City.MOSCOW;

    private RecyclerView mItemsList;
    private ListAdapter mListAdapter;

    static boolean firstlyCreated = true;

    static String lastLocale;
    private static List<LineItem> weather_data;


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
                Spinner citySpinner = (Spinner)findViewById(R.id.citySpinner);
                int index = (citySpinner.getSelectedItemPosition() == -1) ? 0 : citySpinner.getSelectedItemPosition();
                city = (index == 0) ? City.MOSCOW : City.LONDON;
                new WeatherAsyncTask(this).execute(WeatherAsyncTask.buildURL(city));
                firstlyCreated = false;
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

    void setWeather(List<LineItem> weather) {
        weather_data = weather;
        mListAdapter.setData(weather_data);
        mListAdapter.notifyDataSetChanged();
        Log.d("[MAIN]", "" + mListAdapter.getItemCount() + " " + mListAdapter.getData().toString());
//        TextView textView = (TextView)findViewById(R.id.Weather);
//        textView.setText(message);
    }

    void setErrorMessage(int string_id) {
        List<LineItem> errorMessage = new ArrayList<>(1);
        errorMessage.add(new LineItem(getString(string_id)));
        setWeather(errorMessage);
//        TextView textView = (TextView)findViewById(R.id.Weather);
//        textView.setText(getString(string_id));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (lastLocale != null) {
            setLocale(lastLocale);
        }
        setContentView(R.layout.activity_main);

        if (firstlyCreated) {
            weather_data = new ArrayList<>(1);
            weather_data.add(new LineItem(LineItem.no_image, getString(R.string.please_update)));
//            firstlyCreated = false;
//
//            Spinner citySpinner = (Spinner)findViewById(R.id.citySpinner);
//            int index = (citySpinner.getSelectedItemPosition() == -1) ? 0 : citySpinner.getSelectedItemPosition();
//            city = (index == 0) ? City.MOSCOW : City.LONDON;
//            new WeatherAsyncTask(this).execute(WeatherAsyncTask.buildURL(city));
        }


        mItemsList = (RecyclerView) findViewById(R.id.weather_rv);
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);
        mItemsList.setLayoutManager(linearLayoutManager);
        mListAdapter = new ListAdapter(weather_data);
        mItemsList.setAdapter(mListAdapter);
        mItemsList.addOnItemTouchListener(new RecyclerTouchListener(this,
                mItemsList, new ClickListener() {
            @Override
            public void onClick(View view, final int position) {
                //Values are passing to activity & to fragment as well
                Toast.makeText(MainActivity.this, "Single Click on position :"+position,
                        Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onLongClick(View view, int position) {
                Toast.makeText(MainActivity.this, "Long press on position :"+position,
                        Toast.LENGTH_LONG).show();
            }
        }));


        mItemsList.setHasFixedSize(true);
//        mListAdapter.notifyDataSetChanged();
    }
}
