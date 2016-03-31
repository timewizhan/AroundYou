package com.timewiz.cclab.aroundyou;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by jmhan on 2016-03-28.
 */
public class ItemsListAdapter extends BaseAdapter {
    private ArrayList<Item> m_arrayItem;
    private Context m_Context;

    public ItemsListAdapter(Context context, ArrayList<Item> arrayItem) {
        super();
        m_Context = context;
        m_arrayItem = arrayItem;
    }

    @Override
    public int getCount() {
        return m_arrayItem.size();
    }

    @Override
    public Object getItem(int position) {
        return m_arrayItem.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            LayoutInflater inflater = (LayoutInflater) m_Context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView= inflater.inflate(R.layout.list_one_item, null);
        }
        Item item = m_arrayItem.get(position);



        TextView m_textName;
        TextView m_textHashTag;
        TextView m_textPrice;
        TextView m_textReputation;
        m_textName = (TextView) convertView.findViewById(R.id.textOneItemName);
        m_textHashTag = (TextView) convertView.findViewById(R.id.textOneItemHashTag);
        m_textPrice = (TextView) convertView.findViewById(R.id.textOneItemPrice);
        m_textReputation = (TextView) convertView.findViewById(R.id.textOneItemReputation);

        m_textName.setText(item.getItemName());
        m_textHashTag.setText(item.getItemHashTag());
        m_textPrice.setText(item.getItemPrice());
        m_textReputation.setText(item.getItemReputation());

        return convertView;
    }
}
