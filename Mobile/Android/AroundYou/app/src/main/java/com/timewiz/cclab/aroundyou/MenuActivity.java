package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

/**
 * Created by jmhan on 2016-03-29.
 */
public class MenuActivity extends Activity{
    private PreProcess m_PreProcess;
    private Communication m_Communication;
    private PROTOCOL_ROOT protoReq300;
    private PROTOCOL_RES_302_SELECT_MENU m_protoSelectMenu;
    private SimpleMenuDetail m_SimpleMenuDetail;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.detail_menu_page);

        setConstuctor();
        setDeliveredDataFromBefore();
        setSimpleInfo();
    }

    private void setConstuctor() {
        m_PreProcess = new PreProcess();
        m_Communication = new Communication();
        m_protoSelectMenu = new PROTOCOL_RES_302_SELECT_MENU();
        m_SimpleMenuDetail = new SimpleMenuDetail();
    }

    private void setDeliveredDataFromBefore() {
        /*
        Intent intent = getIntent();
        DataMainToMenu dataMainToMenu = (DataMainToMenu) intent.getSerializableExtra("DataMainToMenu");

        PROTOCOL_REQ_302_SELECTED_MENU protoReq302SelectMenu = new PROTOCOL_REQ_302_SELECTED_MENU();
        protoReq302SelectMenu.nStoreID = dataMainToMenu.dwStoreID;
        protoReq302SelectMenu.nCallID = 0;
        protoReq302SelectMenu.nLocation = dataMainToMenu.dwLocation;

        protoReq300 = (PROTOCOL_ROOT) protoReq302SelectMenu;*/
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

    private class SimpleMenuDetail {
        private TextView m_textDetailMenu;

        public SimpleMenuDetail() {
            m_textDetailMenu = (TextView) findViewById(R.id.textMenuCount);
        }

        public void setMenuDetail() {
            m_textDetailMenu.setText("( " + m_protoSelectMenu.nMenuTotalCount + " )");
        }
    }

    private class PreProcess extends AsyncTask<Void, Void, Void> {
        ProgressDialog asyncDialog = new ProgressDialog(MenuActivity.this);

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
                strOutData = m_Communication.communicateWithServer(E_PROTOCOL_TYPE.E_PROTO_SELECT_302, protoReq300);
                if (strOutData.length() < 1) {
                    throw new Exception();
                }

                JSONFactory jsonFactory = JSONFactory.getJSONInstance();
                m_protoSelectMenu = jsonFactory.buildResForProto302(strOutData);
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
        }
    }
}
