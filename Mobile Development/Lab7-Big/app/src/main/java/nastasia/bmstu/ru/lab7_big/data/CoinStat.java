package nastasia.bmstu.ru.lab7_big.data;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Iterator;

public class CoinStat{

    static class DetailedStat {
        String stat;

    }

    public final String date;
    public final String value;
    public final String currency;

    public CoinStat(String date, double value, String currency) {
        this.date  = date;
        this.value = Double.toString(value);
        this.currency = currency;
    }

    public CoinStat(String date, String value, String currency) {
        this.date  = date;
        this.value = value;
        this.currency = currency;
    }

    public static ArrayList<CoinStat> parse(JSONObject source, Currency currency) throws JSONException {
        JSONObject history = source.getJSONObject("history");
        ArrayList<CoinStat> list = new ArrayList<>();
        Iterator<String> date_keys = history.keys();
        while (date_keys.hasNext()) {
            String date  = date_keys.next();
            String cur   = currency.toString();
            double value = history
                    .getJSONObject(date)
                    .getJSONObject("price")
                    .getDouble(cur);
            list.add(new CoinStat(date, value, cur));
        }
        return list;
    }
}