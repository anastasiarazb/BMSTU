package nastasia.bmstu.ru.lab3_recyclerview;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.text.Layout;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;

class LineItem {

    public static final int no_image = android.R.color.transparent;
    private int img_id;
    private String message;

    public LineItem(int img_id, String message) {
        this.img_id = img_id;
        this.message = message;
    }

    public LineItem(String message) {
        this.img_id = no_image;
        this.message = message;
    }


    public String getText(){
        return message;
    }

    public int getImage() {
        return img_id;
    }
}

public class ListAdapter extends RecyclerView.Adapter<ListAdapter.InfoString> {

    List<LineItem> data;

    private static int countItems;

    public ListAdapter(List<LineItem> data) {
        countItems = 0;
        setData(data);
    }

    public void setData(List<LineItem> data) {
        this.data = data;
    }

    public List<LineItem> getData() {
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

//        viewHolder.setData("ViewHolder index: " + countItems);
        countItems++;

        return viewHolder;
    }

    @Override
    public void onBindViewHolder(InfoString holder, int position) {
        holder.dataView.setText(data.get(position).getText());
        holder.imgView.setImageResource(data.get(position).getImage());
    }

    @Override
    public int getItemCount() {
        return data.size();
    }

    class InfoString extends RecyclerView.ViewHolder {

        //    ImageView sign;
        TextView dataView;
        ImageView imgView;

        public InfoString(View itemView) {
            super(itemView);
            dataView = (TextView)  itemView.findViewById(R.id.weather_item);
            imgView  = (ImageView) itemView.findViewById(R.id.item_img);
        }

        public void setData(String info) {
            dataView.setText(info);
        }
    }

    @Override
    public void onAttachedToRecyclerView(RecyclerView recyclerView) {
        super.onAttachedToRecyclerView(recyclerView);
    }


}

