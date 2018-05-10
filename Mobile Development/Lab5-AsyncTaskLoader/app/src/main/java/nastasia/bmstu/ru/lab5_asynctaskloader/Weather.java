package nastasia.bmstu.ru.lab5_asynctaskloader;

import org.json.JSONException;
import org.json.JSONObject;


public class Weather {

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
    private double temp;
    private int pressure;
    private int humidity;
    private double temp_min;
    private double temp_max;

    public Weather() {
    }

    public Weather(JSONObject fullInfo) throws JSONException {
        JSONObject weatherObject = fullInfo.getJSONArray("weather").getJSONObject(0);
        this.main = weatherObject.getString("main");
        this.city = fullInfo.getString("name");
        JSONObject mainObject = fullInfo.optJSONObject("main");
        this.temp     = mainObject.getDouble("temp");
        this.pressure = mainObject.getInt("pressure");
        this.humidity = mainObject.getInt("humidity");
        this.temp_min = mainObject.getDouble("temp_min");
        this.temp_max = mainObject.getDouble("temp_max");

    }

    @Override
    public String toString() {
        return String.format(
                "Today the weater in the city: %s is: %s\n" +
                        "Temperature = %f (min = %f, max = %f)\n" +
                        "Pressure = %d\n" +
                        "Humidity = %d",
                city, main, temp, temp_min, temp_max, pressure, humidity
        );
    }
}
