package nastasia.bmstu.ru.lab7_big.adapter;

import android.view.View;

public interface ClickListener{
    void onClick(View view,int position);
    void onLongClick(View view,int position);
}
