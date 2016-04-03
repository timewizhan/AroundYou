package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;

/**
 * Created by jmhan on 2016-03-29.
 */
public class MenuActivity extends Activity{
    private Communication m_Communication;
    private PROTOCOL_ROOT protoReq300;
    private PROTOCOL_RES_302_SELECT_MENU m_protoSelectMenu;
    private SimpleMenuDetail m_SimpleMenuDetail;
    private DataMainToMenu dataMainToMenu;
    private WebViewImage m_WebViewImage;
    private DetailViewHeader m_DetailViewHeader;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.detail_menu_page);

        setConstuctor();
        setDeliveredDataFromBefore();
        setSimpleInfo();
    }

    private void setConstuctor() {
        m_Communication = new Communication();
        m_protoSelectMenu = new PROTOCOL_RES_302_SELECT_MENU();
        m_SimpleMenuDetail = new SimpleMenuDetail();
        m_WebViewImage = new WebViewImage();

        View v = findViewById(android.R.id.content).getRootView();
        //m_DetailViewHeader = new DetailViewHeader(v);
    }

    private void setDeliveredDataFromBefore() {
        Intent intent = getIntent();
        dataMainToMenu = (DataMainToMenu) intent.getSerializableExtra("DataMainToMenu");

        PROTOCOL_REQ_302_SELECTED_MENU protoReq302SelectMenu = new PROTOCOL_REQ_302_SELECTED_MENU();
        protoReq302SelectMenu.nStoreID = dataMainToMenu.dwStoreID;
        protoReq302SelectMenu.nLocation = dataMainToMenu.dwLocation;
        protoReq302SelectMenu.strCallID = dataMainToMenu.strCallID;

        protoReq300 = (PROTOCOL_ROOT) protoReq302SelectMenu;
    }

    private void setSimpleInfo() {
        View v = findViewById(android.R.id.content).getRootView();
        SimpleInfo simpleInfo = new SimpleInfo(v);

        SimpleInfoData simpleInfoData = new SimpleInfoData();
        simpleInfoData.nStoreEvaluationTaste = dataMainToMenu.dwMenuEvaluation;
        simpleInfoData.nStoreEvaluationKind = dataMainToMenu.dwMenuEvaluation;
        simpleInfoData.nStoreEvaluationMood = dataMainToMenu.dwMenuEvaluation;
        simpleInfoData.strStoreName = dataMainToMenu.strMenuName;
        simpleInfoData.strStoreHashTag = "";
        simpleInfoData.strStoreShortIntro = "";

        simpleInfo.setSimpleDetailInfo(simpleInfoData);
    }

    @Override
    protected void onStart() {
        super.onStart();

        m_SimpleMenuDetail.setMenuDetail();
        m_WebViewImage.viewWebView();
    }

    private class WebViewImage{
        private WebView m_webView;
        public WebViewImage() {
            m_webView = (WebView) findViewById(R.id.webViewDetailMenu);
            m_webView.setWebViewClient(new WebViewClient());
            m_webView.setVerticalScrollBarEnabled(false);
            m_webView.setHorizontalScrollBarEnabled(false);
            m_webView.setInitialScale(100);
        }

        public void viewWebView() {
            String strURL = buildWebViewURL(dataMainToMenu.dwLocation, dataMainToMenu.dwStoreID);
            m_webView.loadUrl(strURL);
        }

        private final String strBasicWebViewURL = "http://165.132.120.155:4000";
        private String buildWebViewURL(int dwLocation, int dwStoreID) {
            String strStoreOrMenu = "menu";
            String strURL = strBasicWebViewURL + "/" + dwLocation + "/" + dwStoreID + "/" + strStoreOrMenu;

            return strURL;
        }
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
}
