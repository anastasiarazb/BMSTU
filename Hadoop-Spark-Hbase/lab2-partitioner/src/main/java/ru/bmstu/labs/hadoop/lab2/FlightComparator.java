/**
 * Created by nastasia on 02.10.16.
 */
package ru.bmstu.labs.hadoop.lab2;
//import org.apache.hadoop.io.RawComparator;
import org.apache.hadoop.io.WritableComparator;
import org.apache.hadoop.io.WritableComparable;

/*в GroupingComparatorClass надо перегрузить конструктор по умолчанию, вызвав в нем конструктор предка с двумя
параметрами — классом ключа и true в качестве второго параметра. Второй параметр говорит hadoop десериализовывать
экземпляры ключей перед сравнением.*/
public class FlightComparator extends WritableComparator /*implements RawComparator*/ {

    protected FlightComparator() {

        super(FlightKey.class, true);

    }

    @Override
    public int compare(WritableComparable a1, WritableComparable b1) {

        FlightKey a = (FlightKey) a1;

        FlightKey b = (FlightKey) b1;

        float delta = a.delayTime()-b.delayTime();

        return delta == 0 ? 0 : (delta < 0 ? -1 : 1);

    }

}
