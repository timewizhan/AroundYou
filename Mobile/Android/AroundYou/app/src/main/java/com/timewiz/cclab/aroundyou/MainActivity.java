package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.net.Socket;
import java.text.DecimalFormat;
import java.util.ArrayList;

/**
 * Created by jmhan on 2016-03-13.
 */
public class MainActivity extends Activity implements View.OnClickListener{
    private PreProcess m_PreProcess;
    private WebViewImage m_WebViewImage;
    private Communication m_Communication;
    private SimpleInfo m_SimpleInfo;

    private PROTOCOL_ROOT protoReq201;
    private PROTOCOL_ROOT protoReq202;
    private ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE> m_arrayProtoRecommendedStore;
    private ArrayList<PROTOCOL_RES_202_RECOMMENDED_MENU> m_arrayProtoRecommendedMenu;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_page);

        setConstuctor();
        setListener();
        setDeliveredDataFromBefore();
    }

    private ImageView m_imageSlideBarMenu;
    private ImageView m_imageAppName;
    private ImageView m_imageRecommendStore;
    private ImageView m_imageRecommendMenu;
    private ImageView m_imageLeftArrow;
    private ImageView m_imageRightArrow;
    private ImageView m_imageBestRank;
    private LinearLayout m_layoutMainView;

    private void setConstuctor() {
        m_PreProcess = new PreProcess();
        m_WebViewImage = new WebViewImage();
        m_arrayProtoRecommendedStore = new ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE>();
        m_Communication = new Communication();

        protoReq201 = new PROTOCOL_REQ_201_RECOMMENDED_STORE();
        protoReq202 = new PROTOCOL_REQ_202_RECOMMENDED_MENU();

        m_imageSlideBarMenu = (ImageView) findViewById(R.id.imageSlideBarMenu);
        m_imageAppName = (ImageView) findViewById(R.id.imageAppName);
        m_imageRecommendStore = (ImageView) findViewById(R.id.imageRecommendStoreEnable);
        m_imageRecommendMenu = (ImageView) findViewById(R.id.imageRecommendMenuDisable);
        m_imageBestRank = (ImageView) findViewById(R.id.imageBestRank);
        m_imageLeftArrow = (ImageView) findViewById(R.id.imageLeftArrow);
        m_imageLeftArrow.setVisibility(View.INVISIBLE);

        m_imageRightArrow = (ImageView) findViewById(R.id.imageRightArrow);
        m_layoutMainView = (LinearLayout) findViewById(R.id.layoutMainView);

        View v = findViewById(android.R.id.content).getRootView();
        m_SimpleInfo = new SimpleInfo(v);
    }

    private void setListener() {
        m_imageSlideBarMenu.setOnClickListener(this);
        m_imageAppName.setOnClickListener(this);
        m_imageRecommendStore.setOnClickListener(this);
        m_imageRecommendMenu.setOnClickListener(this);
        m_imageLeftArrow.setOnClickListener(this);
        m_imageRightArrow.setOnClickListener(this);
        m_layoutMainView.setOnClickListener(this);
    }

    private void setDeliveredDataFromBefore() {
        Intent intent = getIntent();
        DataLoginToMain dataLoginToMain = (DataLoginToMain) intent.getSerializableExtra("DataLoginToMain");

        String strPhoneNumber = dataLoginToMain.getNumber();
        UserAccount.getInstance().setPhoneNumber(strPhoneNumber);
    }

    @Override
    protected void onStart() {
        super.onStart();

        m_PreProcess.execute();
    }

    private int m_nCurrentPos = 0;

    // 0 is Store
    // 1 is Menu
    private int m_nStoreOrMenu = 0;

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.imageSlideBarMenu:
                Log.i("MainActivity", "Click image (imageSlideBarMenu)");
                break;
            case R.id.imageAppName:
                Log.i("MainActivity", "Click image (imageAppName)");
                break;
            case R.id.imageRecommendStoreEnable:
                Log.i("MainActivity", "Click image (imageRecommendStoreEnable)");
                m_imageRecommendStore.setImageResource(R.drawable.main_recommend_store_enable);
                m_imageRecommendMenu.setImageResource(R.drawable.main_recommend_menu_disable);
                m_nStoreOrMenu = 0;

                checkPosIndex();
                break;
            case R.id.imageRecommendMenuDisable:
                Log.i("MainActivity", "Click image (imageRecommendMenuDisable)");
                m_imageRecommendStore.setImageResource(R.drawable.main_recommend_store_disable);
                m_imageRecommendMenu.setImageResource(R.drawable.main_recommend_menu_enable);
                m_nStoreOrMenu = 1;

                checkPosIndex();
                break;
            case R.id.imageLeftArrow:
                m_nCurrentPos--;
                Log.i("MainActivity", "Click image (imageLeftArrow)" + m_nCurrentPos);

                checkPosIndex();
                break;
            case R.id.imageRightArrow:
                m_nCurrentPos++;
                Log.i("MainActivity", "Click image (imageRightArrow)" + m_nCurrentPos);

                checkPosIndex();
                break;
            case R.id.layoutMainView:
                //InputMethodManager inputMethodManager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                //inputMethodManager.hideSoftInputFromWindow();
                break;
        }
    }

    private void checkPosIndex() {
        final int nMinPos = 0;
        final int nMaxPos = 9;

        if (m_nCurrentPos <= nMinPos) {
            m_imageLeftArrow.setVisibility(View.INVISIBLE);
            m_imageRightArrow.setVisibility(View.VISIBLE);
        }
        else if (nMaxPos <= m_nCurrentPos) {
            m_imageLeftArrow.setVisibility(View.VISIBLE);
            m_imageRightArrow.setVisibility(View.INVISIBLE);
        }
        else {
            m_imageLeftArrow.setVisibility(View.VISIBLE);
            m_imageRightArrow.setVisibility(View.VISIBLE);
        }

        if (m_nCurrentPos == 0) {
            m_imageBestRank.setVisibility(View.VISIBLE);
            m_imageBestRank.setImageResource(R.drawable.best_rank_1);
        }
        else if (m_nCurrentPos == 1) {
            m_imageBestRank.setVisibility(View.VISIBLE);
            m_imageBestRank.setImageResource(R.drawable.best_rank_2);
        }
        else if (m_nCurrentPos == 2) {
            m_imageBestRank.setVisibility(View.VISIBLE);
            m_imageBestRank.setImageResource(R.drawable.best_rank_3);
        }
        else {
            m_imageBestRank.setVisibility(View.INVISIBLE);
        }

        clickCurrentDetailInfo(m_nCurrentPos);
    }

    private void clickCurrentDetailInfo(int nCurrentPos) {
        SimpleInfoData simpleInfoData = new SimpleInfoData();

        if (m_nStoreOrMenu == 0) {
            PROTOCOL_RES_201_RECOMMENDED_STORE protoRes201RecommendedStore;
            if (m_arrayProtoRecommendedStore.size() < nCurrentPos + 1) {
                simpleInfoData.strStoreName = "No info";
                simpleInfoData.strStoreHashTag = "No info";
                simpleInfoData.strStoreShortIntro = "No info";

                simpleInfoData.nStoreEvaluationKind = 0;
                simpleInfoData.nStoreEvaluationMood = 0;
                simpleInfoData.nStoreEvaluationTaste = 0;
            }
            else {
                protoRes201RecommendedStore = m_arrayProtoRecommendedStore.get(nCurrentPos);

                simpleInfoData.strStoreName = protoRes201RecommendedStore.strStoreName;
                simpleInfoData.strStoreHashTag = protoRes201RecommendedStore.strStoreHashTag;
                simpleInfoData.strStoreShortIntro = protoRes201RecommendedStore.strStoreShortIntro;

                simpleInfoData.nStoreEvaluationKind = protoRes201RecommendedStore.dwStoreEvaluationKind;
                simpleInfoData.nStoreEvaluationMood = protoRes201RecommendedStore.dwStoreEvaluationMood;
                simpleInfoData.nStoreEvaluationTaste = protoRes201RecommendedStore.dwStoreEvaluationTaste;

                m_WebViewImage.clickNextWebView(m_nCurrentPos);
            }
        }
        else {
            PROTOCOL_RES_202_RECOMMENDED_MENU protoRes202RecommendedMenu;
            if (m_arrayProtoRecommendedMenu.size() < nCurrentPos + 1) {
                simpleInfoData.strStoreName = "No info";
                simpleInfoData.strStoreHashTag = "No info";
                simpleInfoData.strStoreShortIntro = "No info";

                simpleInfoData.nStoreEvaluationKind = 0;
                simpleInfoData.nStoreEvaluationMood = 0;
                simpleInfoData.nStoreEvaluationTaste = 0;
            }
            else {
                protoRes202RecommendedMenu = m_arrayProtoRecommendedMenu.get(nCurrentPos);

                simpleInfoData.strStoreName = protoRes202RecommendedMenu.strMenuName;
                simpleInfoData.strStoreHashTag = "";
                simpleInfoData.strStoreShortIntro = "";

                simpleInfoData.nStoreEvaluationKind = protoRes202RecommendedMenu.nMenuEvaluation;
                simpleInfoData.nStoreEvaluationMood = protoRes202RecommendedMenu.nMenuEvaluation;
                simpleInfoData.nStoreEvaluationTaste = protoRes202RecommendedMenu.nMenuEvaluation;

                m_WebViewImage.clickNextWebView(m_nCurrentPos);
            }
        }

        m_SimpleInfo.setSimpleDetailInfo(simpleInfoData);
    }

    private void makeIntentForStoreActivity(Intent intent) {
        DataMainToStore dataMainToStore = new DataMainToStore();
        PROTOCOL_RES_201_RECOMMENDED_STORE storeData = m_arrayProtoRecommendedStore.get(m_nCurrentPos);

        dataMainToStore.nStoreID = storeData.dwStoreID;
        dataMainToStore.nStoreLocation = storeData.dwStoreLocation;
        dataMainToStore.nLocation = storeData.dwLocation;
        dataMainToStore.nStoreEvaluationTaste = storeData.dwStoreEvaluationTaste;
        dataMainToStore.nStoreEvaluationKind = storeData.dwStoreEvaluationKind;
        dataMainToStore.nStoreEvaluationMood = storeData.dwStoreEvaluationMood;
        dataMainToStore.strCallID = UserAccount.getInstance().getPhoneNumber();
        dataMainToStore.strStoreName = storeData.strStoreName;
        dataMainToStore.strStoreInfoETC = storeData.strStoreInfoETC;
        dataMainToStore.strStoreShortIntro = storeData.strStoreShortIntro;
        dataMainToStore.strStoreHashTag = storeData.strStoreHashTag;
        dataMainToStore.strStoreAddress = storeData.strStoreAddress;
        dataMainToStore.strStoreTel = storeData.strStoreTel;
        dataMainToStore.strStoreOpenTime = storeData.strStoreOpenTime;
        dataMainToStore.strStoreCloseTime = storeData.strStoreCloseTime;

        intent.putExtra("DataMainToStore", dataMainToStore);
    }

    private void makeIntentForMenuActivity(Intent intent) {
        DataMainToMenu dataMainToMenu = new DataMainToMenu();
        PROTOCOL_RES_202_RECOMMENDED_MENU menuData = m_arrayProtoRecommendedMenu.get(m_nCurrentPos);

        dataMainToMenu.dwStoreID = menuData.nStoreID;
        dataMainToMenu.dwLocation = menuData.nStoreLocation;
        dataMainToMenu.dwMenuID = menuData.nMenuID;
        dataMainToMenu.dwMenuPrice = 0;
        dataMainToMenu.dwMenuEvaluation = menuData.nMenuEvaluation;
        dataMainToMenu.strMenuName = menuData.strMenuName;
        dataMainToMenu.strCallID = UserAccount.getInstance().getPhoneNumber();

        intent.putExtra("DataMainToMenu", dataMainToMenu);
    }

    private void makeIntentForItemsListActivity(Intent intent) {

    }

    private class WebViewImage implements View.OnTouchListener{
        private WebView m_webView;
        public WebViewImage() {
            m_webView = (WebView) findViewById(R.id.webViewImageFromServer);
            m_webView.setWebViewClient(new WebViewClient());
            m_webView.setVerticalScrollBarEnabled(false);
            m_webView.setHorizontalScrollBarEnabled(false);
            m_webView.setInitialScale(100);
            m_webView.setOnTouchListener(this);
        }

        public void clickNextWebView(int nPos) {
            if (m_nStoreOrMenu == 0) {
                PROTOCOL_RES_201_RECOMMENDED_STORE protoRes201RecommendedStore;
                protoRes201RecommendedStore = m_arrayProtoRecommendedStore.get(m_nCurrentPos);

                String strURL = buildWebViewURL(protoRes201RecommendedStore.dwStoreLocation, protoRes201RecommendedStore.dwStoreID);
                m_webView.loadUrl(strURL);
            }
            else {
                PROTOCOL_RES_202_RECOMMENDED_MENU protoRes202RecommendedMenu;
                protoRes202RecommendedMenu = m_arrayProtoRecommendedMenu.get(m_nCurrentPos);

                String strURL = buildWebViewURL(protoRes202RecommendedMenu.nStoreLocation, protoRes202RecommendedMenu.nStoreID);
                m_webView.loadUrl(strURL);
            }
        }

        private final String strBasicWebViewURL = "http://165.132.120.155:4000";
        private String buildWebViewURL(int dwLocation, int dwStoreID) {
            String strStoreOrMenu = m_nStoreOrMenu == 0 ? "store" : "menu";
            String strURL = strBasicWebViewURL + "/" + dwLocation + "/" + dwStoreID + "/" + strStoreOrMenu;

            return strURL;
        }

        @Override
        public boolean onTouch(View v, MotionEvent event) {
            Intent intent = null;
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    if (m_nStoreOrMenu == 0) {
                        intent = new Intent(MainActivity.this, StoreActivity.class);
                        makeIntentForStoreActivity(intent);
                    }
                    else {
                        intent = new Intent(MainActivity.this, MenuActivity.class);
                        makeIntentForMenuActivity(intent);
                    }
                    startActivity(intent);
                    break;
            }
            return false;
        }
    }

    private class PreProcess extends AsyncTask<Void, Void, Void> {
        ProgressDialog asyncDialog = new ProgressDialog(MainActivity.this);

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
                strOutData = m_Communication.communicateWithServer(E_PROTOCOL_TYPE.E_PROTO_MAIN_201, protoReq201);
                if (strOutData.length() < 1) {
                    throw new Exception();
                }

                JSONFactory jsonFactory = JSONFactory.getJSONInstance();
                m_arrayProtoRecommendedStore = jsonFactory.buildResForProto201(strOutData);

                strOutData = "";
                strOutData = m_Communication.communicateWithServer(E_PROTOCOL_TYPE.E_PROTO_MAIN_202, protoReq202);
                if (strOutData.length() < 1) {
                    throw new Exception();
                }
                m_arrayProtoRecommendedMenu = jsonFactory.buildResForProto202(strOutData);

            } catch (Exception e) {
                Log.e("MainActivity", "Fail to operate works in background");
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            asyncDialog.dismiss();
            super.onPostExecute(result);

            Log.i("MainActivity", "Complete receiving data from server");
            checkPosIndex();
        }
    }


}

