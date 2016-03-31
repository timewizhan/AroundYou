package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ListView;

import java.util.ArrayList;

/**
 * Created by jmhan on 2016-03-28.
 */
public class ItemsListActivity extends Activity{
    private ListView m_ListView;
    private ArrayList<Item> m_arrayItems;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.list_page);
        setConstuctor();
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    void setConstuctor() {
        m_ListView = (ListView) findViewById(R.id.listView);
        m_arrayItems = new ArrayList<Item>();
        m_arrayItems.add(new Item("Name 1", "Hash Tag 1", "1", "1.0"));
        m_arrayItems.add(new Item("Name 2", "Hash Tag 2", "2", "2.0"));
        m_arrayItems.add(new Item("Name 3", "Hash Tag 3", "3", "3.0"));
        m_arrayItems.add(new Item("Name 4", "Hash Tag 4", "4", "4.0"));
        m_arrayItems.add(new Item("Name 5", "Hash Tag 5", "5", "5.0"));

        ItemsListAdapter adapter = new ItemsListAdapter(this, m_arrayItems);
        m_ListView.setAdapter(adapter);
    }
}
