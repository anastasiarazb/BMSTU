package ru.bmstu.hadoop.hbase;

import ru.bmstu.hadoop.hbase.model.*;
import org.apache.hadoop.hbase.Cell;
import org.apache.hadoop.hbase.exceptions.DeserializationException;
import org.apache.hadoop.hbase.filter.Filter;
import org.apache.hadoop.hbase.filter.FilterBase;
import org.apache.hadoop.hbase.util.Bytes;

import java.io.IOException;

public class CancelDelayFlightFilter extends FilterBase {

    private float   minDelay;
    private boolean shouldBePassed;

    public CancelDelayFlightFilter() {
        super();
        shouldBePassed = false;
    }

    public CancelDelayFlightFilter(float minDelayTime) {
        minDelay = minDelayTime;
        shouldBePassed = false;
    }

    @Override
    public void reset() throws IOException {
        shouldBePassed = false;
    }

    //вызывается для каждого KeyValue, можем вернуть команду что делать дальше(пропуск строки, добавить KeyValue и т.д.)
    @Override
    public ReturnCode filterKeyValue(Cell cell) throws IOException {
        //Получить имя столбца и хранящееся значение из массива байт
        String columnName = new String(cell.getQualifierArray(), cell.getQualifierOffset(), cell.getQualifierLength());
        String value      = new String(cell.getValueArray(),     cell.getValueOffset(),     cell.getValueLength());

        //Если ячейка содержит опоздание, больше заданного, либо отменy - она должна быть пропущена фильтром
        shouldBePassed |=  columnName.equals(FlightTable.ARR_DELAY_NEW) && !value.isEmpty()
                           && Float.parseFloat(value) > minDelay;
        shouldBePassed |= columnName.equals(FlightTable.CANCELLED)
                           && Float.parseFloat(value) == 1.00f;
        //Включить все ячейки (Include the Cell)
        return ReturnCode.INCLUDE; //вернуть команду что делать дальше(пропуск строки, добавить KeyValue и т.д.)

    }

    @Override
    public boolean filterRow() throws IOException {
        return !shouldBePassed;
    }

    @Override
    public byte[] toByteArray() throws IOException {
        return Bytes.toBytes(minDelay);
    }

    public static Filter parseFrom(byte[] pbBytes) throws DeserializationException {
        return new CancelDelayFlightFilter(Bytes.toFloat(pbBytes));
    }

}
