package ru.bmstu.nastasia.lab9_contentprovider.adapter;

import android.content.Context;
import android.graphics.Color;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckedTextView;

import java.util.List;

import ru.bmstu.nastasia.lab9_contentprovider.R;
import ru.bmstu.nastasia.lab9_contentprovider.data.CheckedString;


public class ListAdapter extends RecyclerView.Adapter<ListAdapter.InfoString> {

    class InfoString extends RecyclerView.ViewHolder {

        CheckedTextView contentTV;
//
        public InfoString(View itemView) {
            super(itemView);
            contentTV = itemView.findViewById(R.id.checkedTextView);
        }
//
        public void setData(CheckedString line) {
            contentTV.setText(line.getContent());
            contentTV.setChecked(line.isChecked());
            int color = line.isChecked()
                    ? Color.parseColor("#888888")
                    : Color.BLACK;
            contentTV.setTextColor(color);
        }
    }

    private List<CheckedString> data;

    public ListAdapter(List<CheckedString> data) {
        setData(data);
    }

    public void setData(List<CheckedString> data) {
        this.data = data;
    }

    public List<CheckedString> getData() {
        return data;
    }

    @Override
    public InfoString onCreateViewHolder(ViewGroup parent, int viewType) {
        Context context = parent.getContext();
        int layoutIdForListItem = R.layout.list_item;
        LayoutInflater inflater = LayoutInflater.from(context);
        boolean shouldAttachToParentImmediately = false;

        View view = inflater.inflate(layoutIdForListItem, parent, shouldAttachToParentImmediately);
        InfoString viewHolder = new InfoString(view);

        return viewHolder;
    }

    @Override
    public void onBindViewHolder(InfoString holder, int position) {
        holder.setData(data.get(position));
        Log.d("onBindViewHolder___", position + " " + data.get(position).getContent() + " "  + getItemCount());
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

