/**
 * Created by nastasia on 02.10.16.
 */
package ru.bmstu.labs.hadoop.lab2;
import org.apache.hadoop.io.WritableComparable;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

//ТРЕБОВАНИЯ:
//использовать package
//Давать классам более осознанные имена, типа FirstKeyPartPartitioner
//константы private static final int ИМЯ_БОЛЬШИМИ_БУКВАМИ
//имена полей в кэмел-кейс
//все, что идея предлагает делать private - делать private
//скобки расставлять так, как идея предлагает (открывающая скобка - на той же строке, что и функция/оператор if
//использовать if-else if, даже если в первом if был ретурн ради сохранения логики
//cancelled - булеан
//магическая константа -1 - как-то обозначить
//если в функции строка повторяется несолько раз и была скопипащена - надо оформлять в отдельную функцию, чтобы
//легко можно было исправить в случае изменений
public class FlightKey implements WritableComparable{

    int cancelled; //0, 1
    int dest_airport_id;
    float arr_delay_new;
    float air_time;

   private static final int idx_cancelled = 19;
    static int idx_airport = 14;
    static int idx_delay = 18;
    static int idx_air_time = 21;

    public FlightKey()
    {

    }

//    static void initColumns(String s) {
//        String[] arr = s.split(",");
//        for(int i = 0; i < arr.length; ++i) {
//            if (arr[i].equals("\"ARR_DELAY_NEW\"")) {
//                idx_delay = i;
//            }
//            else if (arr[i].equals("\"DEST_AIRPORT_ID\"")){
//                idx_airport = i;
//            }
//            else if (arr[i].equals("\"CANCELLED\"")) {
//                idx_cancelled = i;
//            }
//            else if (arr[i].equals("\"AIR_TIME\"")) {
//                idx_air_time = i;
//            }
//        }
//
//        System.out.println(idx_delay);
//        System.out.println(idx_airport);
//        System.out.println(idx_cancelled);
//        System.out.println(idx_air_time);
//    }

    public FlightKey(String src) {
        String[] arr = src.split(",");
        cancelled = Float.valueOf(arr[idx_cancelled]).intValue();
        arr_delay_new = arr[idx_delay].isEmpty() ? -1 : Float.valueOf(arr[idx_delay]);
        dest_airport_id = Integer.valueOf(arr[idx_airport]);
        air_time = arr[idx_air_time].isEmpty() ? -1 : Float.valueOf(arr[idx_air_time]);
//        for (int i = 0; i < arr.length; ++i) {
//            System.out.print("{"+arr[i]+"}");
//        }
    }

    @Override
    public String toString() {
        return "Cancelled: " + (cancelled > 0 ? "YES" : "NO" ) +  //String.valueOf(cancelled) +
                " Destination airport ID: " + String.valueOf(dest_airport_id) +
                " Delayed: " + (arr_delay_new > 0 ? String.valueOf(arr_delay_new) : "NO")  +
                " Air time: " + (air_time > 0 ?  String.valueOf(air_time) : "NO" );
    }

    public boolean arrivedInTime() {
        return cancelled == 0 && arr_delay_new == 0;
    }
//----------------------------------Comparable----------------------------------
    @Override
    public int compareTo(Object o) {
        FlightKey other = (FlightKey)o;
        if (cancelled != other.cancelled) {
            return -(cancelled - other.cancelled);
        } else  if (arr_delay_new != other.arr_delay_new) {
            return (arr_delay_new - other.arr_delay_new < 0 ) ? -1 : 1;
        }
        if (dest_airport_id != other.dest_airport_id)
        {
            return dest_airport_id - other.dest_airport_id;
        }
        float delta = air_time - other.air_time;
        return delta == 0 ? 0 : (delta < 0 ? -1 : 1);

    }

    @Override
    public boolean equals(Object o) {
        if (o == null || ! (o instanceof FlightKey)) return false;
        FlightKey other = (FlightKey)o;
        return (cancelled == other.cancelled)
                && (dest_airport_id == other.dest_airport_id)
                && (arr_delay_new ==  other.arr_delay_new);
    }

    public float delayTime()
    {
        return cancelled == 1 ? -1 : arr_delay_new;
    }

    @Override
    public int hashCode() {
        return toString().hashCode();
    }

    public int firstPartHashCode() {
        return String.valueOf(delayTime()).hashCode();
    }
//----------------------------------Writable----------------------------------

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeInt(cancelled); //0, 1
        out.writeInt(dest_airport_id);
        out.writeFloat(arr_delay_new);
        out.writeFloat(air_time);
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        cancelled = in.readInt();
        dest_airport_id = in.readInt();
        arr_delay_new = in.readFloat();
        air_time = in.readFloat();
    }
}
