package nastasia.bmstu.ru.lab7_big.adapter;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.List;
import java.util.Locale;

import nastasia.bmstu.ru.lab7_big.R;
import nastasia.bmstu.ru.lab7_big.data.CoinStat;


public class ListAdapter extends RecyclerView.Adapter<ListAdapter.InfoString> {

    class InfoString extends RecyclerView.ViewHolder {

        //    ImageView sign;
        TextView keyView;
        TextView valueView;
        TextView separatorView;
        TextView separatorView2;
        TextView additionalInfoView;

        public InfoString(View itemView) {
            super(itemView);
            keyView    = itemView.findViewById(R.id.item_key);
            valueView  = itemView.findViewById(R.id.item_value);
            separatorView = itemView.findViewById(R.id.item_separator);
            separatorView2 = itemView.findViewById(R.id.item_separator2);
            additionalInfoView = itemView.findViewById(R.id.item_additional_info);
        }

        public void setData(CoinStat line) {
            keyView.setText(line.date);
            valueView.setText(line.value);
            if (line.value != null && !line.value.equals("")) {
                separatorView.setText(":    ");
            }
            if (line.currency != null && !line.value.equals("")) {
                separatorView2.setText(" ");
                additionalInfoView.setText(line.currency);

                String val = full_precision
                        ? line.value
                        : String.format(Locale.ENGLISH, "%.2f", Double.parseDouble(line.value));
                valueView.setText(val);
            }
        }
    }

    private List<CoinStat> data;

    private boolean full_precision;

    public ListAdapter(List<CoinStat> data, boolean full_precision) {
        setData(data, full_precision);
    }

//    public void setData(List<CoinStat> data) {
//        this.data = data;
//    }

    public void setData(List<CoinStat> data, boolean full_precision) {
        this.full_precision = full_precision;
        this.data = data;
    }

    public List<CoinStat> getData() {
        return data;
    }

    @Override
    public InfoString onCreateViewHolder(ViewGroup parent, int viewType) {
        Context context = parent.getContext();
        int layoutIdForListItem = R.layout.list_item;
        LayoutInflater inflater = LayoutInflater.from(context);
        boolean shouldAttachToParentImmediately = false;

        View view = inflater.inflate(layoutIdForListItem, parent, shouldAttachToParentImmediately);
        ListAdapter.InfoString viewHolder = new ListAdapter.InfoString(view);

        return viewHolder;
    }

    @Override
    public void onBindViewHolder(InfoString holder, int position) {
        holder.setData(data.get(position));
    }

    @Override
    public int getItemCount() {
        return data.size();
    }


    @Override
    public void onAttachedToRecyclerView(RecyclerView recyclerView) {
        super.onAttachedToRecyclerView(recyclerView);
    }


}

