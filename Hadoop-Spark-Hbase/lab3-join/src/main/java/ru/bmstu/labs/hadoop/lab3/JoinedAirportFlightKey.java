package ru.bmstu.labs.hadoop.lab3;

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
//если в функции строка повторяется несолько раз и была скопипасчена - надо оформлять в отдельную функцию, чтобы
//легко можно было исправить в случае изменений

public class JoinedAirportFlightKey implements WritableComparable{

    // Изначально предполагался enum RecordType, но при сериализации получалась гадость из-за того, что для получения int'ового значения из enum
    // нужно if-ами перебирать все поля. Было решено, что enum'ы не нужны.
    public static final int AIRPORT = 0;
    public static final int FLIGHT = 1;

    String RecordTypeToString(int t) {
        return t == AIRPORT ? "AIRPORT" : "FLIGHT";
    }

    //----------------------------------

    private int airportID;
    private int type;

    JoinedAirportFlightKey ()
    {

    }

    public JoinedAirportFlightKey(int airportID, int recordType) {
        this.airportID = airportID;
        this.type = recordType;
    }

    @Override
    public String toString() {
        return "Airport ID: " + Integer.toString(airportID) +
                " Record type: " + RecordTypeToString(type);
    }

    int airportID() {
        return airportID;
    }
//----------------------------------Comparable----------------------------------
    @Override
    public int compareTo(Object o) {
        JoinedAirportFlightKey other = (JoinedAirportFlightKey)o;
        if (this.airportID != other.airportID) {
            return this.airportID - other.airportID;
        }
        else {
            return this.type - other.type;
        }
//        final int LESS = -1;
//        final int BIGGER = 1;
//        if (this.type != other.type) {
//            Debug.set(true);
//            Debug.out("[JoinedAirportFlightKey.compareTo]: A.type = " + RecordTypeToString(this.type) +
//                    "; B.type = "+ RecordTypeToString(other.type) + "Compare = " + String.valueOf((this.type == AIRPORT) ? LESS : BIGGER));
//            return (this.type == AIRPORT) ? LESS : BIGGER;
//        }
//        else {
//            return this.airportID < other.airportID ? LESS : BIGGER;
//        }
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || ! (o instanceof JoinedAirportFlightKey)) {
            return false;
        }
        JoinedAirportFlightKey other = (JoinedAirportFlightKey)o;
        return (this.type == other.type)
                && (this.airportID == other.airportID);
    }

    @Override
    public int hashCode() {
        return toString().hashCode();
    }

    int firstPartHashCode() {
        return String.valueOf(airportID).hashCode(); // return airportID;
    }
//----------------------------------Writable----------------------------------

    @Override
    public void write(DataOutput out) throws IOException {
        out.writeInt(airportID);
        out.writeInt(type);
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        airportID = in.readInt();
        type = in.readInt();
    }
}
