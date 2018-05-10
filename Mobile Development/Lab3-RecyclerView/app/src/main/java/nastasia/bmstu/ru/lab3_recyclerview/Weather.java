package nastasia.bmstu.ru.lab3_recyclerview;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;


public class Weather {

    public static int KelvinToCelsius(double t) {
        return (int)(t - 273.15);
    }

    public static int HPaToMMHg(double p) {
        return (int)(p*1.33322368);
    }

    /*
    {"coord":{"lon":-0.13,"lat":51.51}
    "weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}]
    "base":"stations",
    "main":{"temp":289.7,"pressure":1018,"humidity":72,"temp_min":288.15,"temp_max":291.15},
    "visibility":10000,
    "wind":{"speed":5.1,"deg":220},
    "clouds":{"all":90},
    "dt":1505920800,
    "sys":{"type":1,"id":5091,"message":0.0034,"country":"GB","sunrise":1505886277,"sunset":1505930501},
    "id":2643743,"name":"London","cod":200
    }
    */
    private String city;
    private String main;
    private int temp;
    private int pressure;
    private int humidity;
    private int temp_min;
    private int temp_max;

    public Weather() {
    }

    public Weather(JSONObject fullInfo) throws JSONException {
        JSONObject weatherObject = fullInfo.getJSONArray("weather").getJSONObject(0);
        this.main = weatherObject.getString("main");
        this.city = fullInfo.getString("name");
        JSONObject mainObject = fullInfo.optJSONObject("main");
        this.temp     = KelvinToCelsius(mainObject.getDouble("temp"));
        this.pressure = HPaToMMHg(mainObject.getDouble("pressure"));
        this.humidity = mainObject.getInt("humidity");
        this.temp_min = KelvinToCelsius(mainObject.getDouble("temp_min"));
        this.temp_max = KelvinToCelsius(mainObject.getDouble("temp_max"));

    }

    @Override
    public String toString() {
        return String.format(
                "Today the weather in the city: %s is: %s\n" +
                        "Temperature = %d (min = %d, max = %d)\n" +
                        "Pressure = %d\n" +
                        "Humidity = %d",
                city, main, temp, temp_min, temp_max, pressure, humidity
        );
    }

    public List<LineItem> toList() {
        ArrayList<LineItem> result = new ArrayList<>(7);
        result.add(new LineItem("The city: " + city));
        result.add(new LineItem("The weather is: " + main));
        result.add(new LineItem(R.drawable.temperature_img, "Temperature: " + temp  + " °C"));
        result.add(new LineItem(R.drawable.t_min_img, "Min temp: " + temp_min + " °C"));
        result.add(new LineItem(R.drawable.t_max_img, "Max temp: " + temp_max + " °C"));
        result.add(new LineItem(R.drawable.pressure_img , "Pressure: " + pressure+ " mmHg"));
        result.add(new LineItem(R.drawable.humidity_img, "Humidity: " + humidity + "%"));
        return result;
    }
}
