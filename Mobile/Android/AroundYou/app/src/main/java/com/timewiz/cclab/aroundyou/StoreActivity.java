package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.media.Image;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
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
    private WebViewImage m_WebViewImage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.detail_store_page);

        setConstuctor();
        setListener();
        setDeliveredDataFromBefore();
        setSimpleInfo();
    }

    private void setConstuctor() {
        m_PreProcess = new PreProcess();
        m_Communication = new Communication();
        m_protoSelectStore = new PROTOCOL_RES_301_SELECT_STORE();
        m_SimpleStoreDetail = new SimpleStoreDetail();
        m_WebViewImage = new WebViewImage();
        m_imageMenuMore = (ImageView) findViewById(R.id.imageMenuMore);
    }

    private void setListener() {
        m_imageMenuMore.setOnClickListener(this);
    }

    private DataMainToStore dataLoginToMain;
    private void setDeliveredDataFromBefore() {
        Intent intent = getIntent();
        dataLoginToMain = (DataMainToStore) intent.getSerializableExtra("DataMainToStore");

        PROTOCOL_REQ_301_SELECTED_STORE protoReq301SelectStore = new PROTOCOL_REQ_301_SELECTED_STORE();
        protoReq301SelectStore.nStoreID = dataLoginToMain.nStoreID;
        protoReq301SelectStore.nLocation = dataLoginToMain.nLocation;
        protoReq301SelectStore.strCallID = dataLoginToMain.strCallID;

        protoReq300 = (PROTOCOL_ROOT) protoReq301SelectStore;
    }

    private void setSimpleInfo() {
        View v = findViewById(android.R.id.content).getRootView();
        SimpleInfo simpleInfo = new SimpleInfo(v);

        SimpleInfoData simpleInfoData = new SimpleInfoData();
        simpleInfoData.nStoreEvaluationTaste = dataLoginToMain.nStoreEvaluationTaste;
        simpleInfoData.nStoreEvaluationKind = dataLoginToMain.nStoreEvaluationKind;
        simpleInfoData.nStoreEvaluationMood = dataLoginToMain.nStoreEvaluationMood;
        simpleInfoData.strStoreName = dataLoginToMain.strStoreName;
        simpleInfoData.strStoreHashTag = dataLoginToMain.strStoreHashTag;
        simpleInfoData.strStoreShortIntro = dataLoginToMain.strStoreShortIntro;

        simpleInfo.setSimpleDetailInfo(simpleInfoData);
    }

    @Override
    protected void onStart() {
        super.onStart();

        m_PreProcess.execute();
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

    private class WebViewImage{
        private WebView m_webView;
        public WebViewImage() {
            m_webView = (WebView) findViewById(R.id.webViewSelectStore);
            m_webView.setWebViewClient(new WebViewClient());
            m_webView.setVerticalScrollBarEnabled(false);
            m_webView.setHorizontalScrollBarEnabled(false);
            m_webView.setInitialScale(100);
        }

        public void viewWebView() {
            String strURL = buildWebViewURL(dataLoginToMain.nStoreLocation, dataLoginToMain.nStoreID);
            m_webView.loadUrl(strURL);
        }

        private final String strBasicWebViewURL = "http://165.132.120.155:4000";
        private String buildWebViewURL(int dwLocation, int dwStoreID) {
            String strStoreOrMenu = "store";
            String strURL = strBasicWebViewURL + "/" + dwLocation + "/" + dwStoreID + "/" + strStoreOrMenu;

            return strURL;
        }
    }

    private class SimpleStoreDetail {
        private TextView m_textMenuCount;
        private TextView m_textDetailAdd;
        private TextView m_textDetailTel;
        private TextView m_textDetailTime;
        private TextView m_arrtextStoreMenu[];

        public SimpleStoreDetail() {
            m_textMenuCount = (TextView) findViewById(R.id.textMenuCount);
            m_textDetailAdd = (TextView) findViewById(R.id.textDetailAdd);
            m_textDetailTel = (TextView) findViewById(R.id.textDetailTel);
            m_textDetailTime = (TextView) findViewById(R.id.textDetailTime);

            m_arrtextStoreMenu = new TextView[3];
            m_arrtextStoreMenu[0] = (TextView) findViewById(R.id.textStoreMenu1);
            m_arrtextStoreMenu[1] = (TextView) findViewById(R.id.textStoreMenu2);
            m_arrtextStoreMenu[2] = (TextView) findViewById(R.id.textStoreMenu3);
        }

        public void setStoreDetail() {
            m_textMenuCount.setText("( " + m_protoSelectStore.nMenuTotalCount + " )");
            m_textDetailAdd.setText(dataLoginToMain.strStoreAddress);
            m_textDetailTel.setText(dataLoginToMain.strStoreTel);

            String strStoreTime = dataLoginToMain.strStoreOpenTime + " ~ " + dataLoginToMain.strStoreCloseTime;
            m_textDetailTime.setText(strStoreTime);

            int nArraySize = m_protoSelectStore.arrayMenus.size();
            final int TOTAL_VIEW_MENU = 3;

            int nNotViewMenu = TOTAL_VIEW_MENU - nArraySize;

            PROTOCOL_RES_301_SELECT_STORE_MENUS protoSelectStore;
            for (int i = 0; i < nArraySize; i++) {
                protoSelectStore = m_protoSelectStore.arrayMenus.get(i);
                if (protoSelectStore.strMenuName.length() > 0) {
                    m_arrtextStoreMenu[i].setText(protoSelectStore.strMenuName);
                    continue;
                }
                m_arrtextStoreMenu[i].setText("No info");
            }

            for (int i = nArraySize; i < TOTAL_VIEW_MENU; i++) {
                m_arrtextStoreMenu[i].setText("No info");
            }
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
            m_WebViewImage.viewWebView();
        }
    }
}
