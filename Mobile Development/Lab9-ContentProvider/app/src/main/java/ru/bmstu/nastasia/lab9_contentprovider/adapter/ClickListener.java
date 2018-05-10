package ru.bmstu.nastasia.lab9_contentprovider.adapter;

import android.view.View;

public interface ClickListener{
    void onClick(View view, int position);
    void onLongClick(View view, int position);
}
