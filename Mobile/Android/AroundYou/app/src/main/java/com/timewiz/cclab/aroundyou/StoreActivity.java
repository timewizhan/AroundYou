package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.media.Image;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.ArrayList;

/**
 * Created by jmhan on 2016-03-29.
 */
public class StoreActivity extends Activity implements View.OnClickListener{
    private PreProcess m_PreProcess;
    private Communication m_Communication;
    private PROTOCOL_ROOT protoReq300;
    private PROTOCOL_RES_301_SELECT_STORE m_protoSelectStore;
    private SimpleStoreDetail m_SimpleStoreDetail;
    private ImageView m_imageMenuMore;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.detail_store_page);

        setConstuctor();
        setListener();
        //setDeliveredDataFromBefore();
        setSimpleInfo();
    }

    private void setConstuctor() {
        m_PreProcess = new PreProcess();
        m_Communication = new Communication();
        m_protoSelectStore = new PROTOCOL_RES_301_SELECT_STORE();
        m_SimpleStoreDetail = new SimpleStoreDetail();
        m_imageMenuMore = (ImageView) findViewById(R.id.imageMenuMore);
    }

    private void setListener() {
        m_imageMenuMore.setOnClickListener(this);
    }

    private void setDeliveredDataFromBefore() {

        Intent intent = getIntent();
        DataMainToStore dataLoginToMain = (DataMainToStore) intent.getSerializableExtra("DataMainToStore");

        PROTOCOL_REQ_301_SELECTED_STORE protoReq301SelectStore = new PROTOCOL_REQ_301_SELECTED_STORE();
        protoReq301SelectStore.nStoreID = dataLoginToMain.dwStoreID;
        protoReq301SelectStore.nCallID = 0;
        protoReq301SelectStore.nLocation = dataLoginToMain.dwLocation;

        protoReq300 = (PROTOCOL_ROOT) protoReq301SelectStore;
    }

    private void setSimpleInfo() {
        View v = findViewById(android.R.id.content).getRootView();
        SimpleInfo simpleInfo = new SimpleInfo(v);

        SimpleInfoData simpleInfoData = new SimpleInfoData();

        // Test
        simpleInfoData.dwStoreEvaluationKind = 1;
        simpleInfoData.dwStoreEvaluationMood = 1;
        simpleInfoData.dwStoreEvaluationTaste = 1;
        simpleInfo.setSimpleDetailInfo(simpleInfoData);
    }

    @Override
    protected void onStart() {
        super.onStart();

        //m_PreProcess.execute();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.imageMenuMore:
                Log.i("StoreActivity", "Click image (imageMenuMore)");
                startActivity(new Intent(StoreActivity.this, MenuListActivity.class));
                break;
        }
    }

    private class SimpleStoreDetail {
        private TextView m_textMenuCount;
        private TextView m_textDetailAdd;
        private TextView m_textDetailTel;
        private TextView textDetailTime;

        public SimpleStoreDetail() {
            m_textMenuCount = (TextView) findViewById(R.id.textMenuCount);
            m_textDetailAdd = (TextView) findViewById(R.id.textDetailAdd);
            m_textDetailTel = (TextView) findViewById(R.id.textDetailTel);
            textDetailTime = (TextView) findViewById(R.id.textDetailTime);
        }

        public void setStoreDetail() {
            m_textMenuCount.setText("( " + m_protoSelectStore.nMenuTotalCount + " )");
            m_textDetailAdd.setText(m_protoSelectStore.strStoreAddress);
            m_textDetailTel.setText(m_protoSelectStore.strStoreTel);
            textDetailTime.setText(m_protoSelectStore.strStoreTime);
        }
    }

    private class PreProcess extends AsyncTask<Void, Void, Void> {
        ProgressDialog asyncDialog = new ProgressDialog(StoreActivity.this);

        @Override
        protected void onPreExecute() {
            asyncDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            asyncDialog.setMessage("Loading...");

            // show dialog
            asyncDialog.show();
            super.onPreExecute();
        }

        @Override
        protected Void doInBackground(Void... arg0) {
            try {
                Errno errno;
                String strOutData;
                strOutData = m_Communication.communicateWithServer(E_PROTOCOL_TYPE.E_PROTO_SELECT_301, protoReq300);
                if (strOutData.length() < 1) {
                    throw new Exception();
                }

                JSONFactory jsonFactory = JSONFactory.getJSONInstance();
                m_protoSelectStore = jsonFactory.buildResForProto301(strOutData);
            } catch (Exception e) {
                Log.e("StoreActivity", "Fail to operate works in background");
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            asyncDialog.dismiss();
            super.onPostExecute(result);

            Log.i("StoreActivity", "Complete receiving data from server");
            m_SimpleStoreDetail.setStoreDetail();
        }
    }
}
