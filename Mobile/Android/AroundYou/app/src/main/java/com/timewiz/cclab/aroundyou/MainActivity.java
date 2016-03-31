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
    private DetailInfo m_DetailInfo;
    private ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE> m_arrayProtoRecommendedStore;
    private Communication m_Communication;
    private PROTOCOL_ROOT protoReq200;

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
    private LinearLayout m_layoutMainView;

    private void setConstuctor() {
        m_PreProcess = new PreProcess();
        m_WebViewImage = new WebViewImage();
        m_DetailInfo = new DetailInfo();
        m_arrayProtoRecommendedStore = new ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE>();
        m_Communication = new Communication();
        protoReq200 = new PROTOCOL_REQ_201_RECOMMENDED_STORE();

        m_imageSlideBarMenu = (ImageView) findViewById(R.id.imageSlideBarMenu);
        m_imageAppName = (ImageView) findViewById(R.id.imageAppName);
        m_imageRecommendStore = (ImageView) findViewById(R.id.imageRecommendStoreEnable);
        m_imageRecommendMenu = (ImageView) findViewById(R.id.imageRecommendMenuDisable);
        m_imageLeftArrow = (ImageView) findViewById(R.id.imageLeftArrow);
        m_imageLeftArrow.setVisibility(View.INVISIBLE);

        m_imageRightArrow = (ImageView) findViewById(R.id.imageRightArrow);
        m_layoutMainView = (LinearLayout) findViewById(R.id.layoutMainView);
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
                break;
            case R.id.imageRecommendMenuDisable:
                Log.i("MainActivity", "Click image (imageRecommendMenuDisable)");
                m_imageRecommendStore.setImageResource(R.drawable.main_recommend_store_disable);
                m_imageRecommendMenu.setImageResource(R.drawable.main_recommend_menu_enable);
                m_nStoreOrMenu = 1;
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

        m_WebViewImage.clickNextWebView(m_nCurrentPos);
        m_DetailInfo.clickNextDetailInfo(m_nCurrentPos);
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
                // Todo
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
                    }
                    else {
                        intent = new Intent(MainActivity.this, MenuActivity.class);
                    }
                    startActivity(intent);
                    break;
            }
            return false;
        }
    }

    private class DetailInfo {
        private TextInfo m_TextInfo;
        private RankInfo m_RankInfo;
        private UserInfo m_UserInfo;

        public DetailInfo() {
            m_TextInfo = new TextInfo();
            m_RankInfo = new RankInfo();
            m_UserInfo = new UserInfo();
        }

        public void clickNextDetailInfo(int nPos) {
            m_TextInfo.setTextInfo(nPos);
            m_RankInfo.setRankInfo(nPos);
        }
    }

    private class TextInfo {
        private TextView m_TextRecommendedName;
        private TextView m_TextHashTag;
        private TextView m_TextIntroduction;

        public TextInfo() {
            m_TextRecommendedName = (TextView) findViewById(R.id.textRecommendName);
            m_TextHashTag = (TextView) findViewById(R.id.textHashTag);
            m_TextIntroduction = (TextView) findViewById(R.id.textIntroduction);
        }

        public void setTextInfo(int nPos) {
            PROTOCOL_RES_201_RECOMMENDED_STORE protoRes201RecommendedStore;
            protoRes201RecommendedStore = m_arrayProtoRecommendedStore.get(nPos);

            m_TextRecommendedName.setText(protoRes201RecommendedStore.strStoreName);
            m_TextHashTag.setText(protoRes201RecommendedStore.strStoreHashTag);
            m_TextIntroduction.setText(protoRes201RecommendedStore.strStoreShortIntro);
        }
    }

    private class RankInfo {
        private ImageView[] arrImageDelicious;
        private ImageView[] arrImageAttitude;
        private ImageView[] arrImageMood;
        private TextView textReputation;

        public RankInfo() {
            arrImageDelicious = new ImageView[5];
            arrImageDelicious[0] =  (ImageView) findViewById(R.id.imageRatingDelicious_1);
            arrImageDelicious[1] =  (ImageView) findViewById(R.id.imageRatingDelicious_2);
            arrImageDelicious[2] =  (ImageView) findViewById(R.id.imageRatingDelicious_3);
            arrImageDelicious[3] =  (ImageView) findViewById(R.id.imageRatingDelicious_4);
            arrImageDelicious[4] =  (ImageView) findViewById(R.id.imageRatingDelicious_5);

            arrImageAttitude = new ImageView[5];
            arrImageAttitude[0] =  (ImageView) findViewById(R.id.imageRatingAttitude_1);
            arrImageAttitude[1] =  (ImageView) findViewById(R.id.imageRatingAttitude_2);
            arrImageAttitude[2] =  (ImageView) findViewById(R.id.imageRatingAttitude_3);
            arrImageAttitude[3] =  (ImageView) findViewById(R.id.imageRatingAttitude_4);
            arrImageAttitude[4] =  (ImageView) findViewById(R.id.imageRatingAttitude_5);

            arrImageMood = new ImageView[5];
            arrImageMood[0] =  (ImageView) findViewById(R.id.imageRatingMood_1);
            arrImageMood[1] =  (ImageView) findViewById(R.id.imageRatingMood_2);
            arrImageMood[2] =  (ImageView) findViewById(R.id.imageRatingMood_3);
            arrImageMood[3] =  (ImageView) findViewById(R.id.imageRatingMood_4);
            arrImageMood[4] =  (ImageView) findViewById(R.id.imageRatingMood_5);

            textReputation = (TextView) findViewById(R.id.textReputation);
        }

        public void setRankInfo(int nPos) {
            PROTOCOL_RES_201_RECOMMENDED_STORE protoRes201RecommendedStore;
            protoRes201RecommendedStore = m_arrayProtoRecommendedStore.get(m_nCurrentPos);

            int nTaste = protoRes201RecommendedStore.dwStoreEvaluationTaste;
            int nKind = protoRes201RecommendedStore.dwStoreEvaluationKind;
            int nMood = protoRes201RecommendedStore.dwStoreEvaluationMood;

            setDeliciousRank(nTaste);
            setAttitudeRank(nKind);
            setMoodRank(nMood);
            setReputation(nTaste, nKind, nMood);
        }

        private final int nMax = 5;
        private void setDeliciousRank(int nValue) {
            for (int i = 0; i < nValue; i++) {
                arrImageDelicious[i].setImageResource(R.drawable.rating_enable);
            }
            for (int i = nValue; i < nMax; i++) {
                arrImageDelicious[i].setImageResource(R.drawable.rating_disable);
            }
        }

        private void setAttitudeRank(int nValue) {
            for (int i = 0; i < nValue; i++) {
                arrImageAttitude[i].setImageResource(R.drawable.rating_enable);
            }
            for (int i = nValue; i < nMax; i++) {
                arrImageAttitude[i].setImageResource(R.drawable.rating_disable);
            }
        }

        private void setMoodRank(int nValue) {
            for (int i = 0; i < nValue; i++) {
                arrImageMood[i].setImageResource(R.drawable.rating_enable);
            }
            for (int i = nValue; i < nMax; i++) {
                arrImageMood[i].setImageResource(R.drawable.rating_disable);
            }
        }

        private final int MAX_NUM = 3;
        private void setReputation(int nTaste, int nKind, int nMood) {
            int nSum = nTaste + nKind + nMood;
            float fReputation = nSum / MAX_NUM;
            DecimalFormat format = new DecimalFormat(".#");
            String strReputation = format.format(fReputation);

            textReputation.setText(strReputation);
        }

    }

    private class UserInfo {
        // Todo
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
                strOutData = m_Communication.communicateWithServer(E_PROTOCOL_TYPE.E_PROTO_MAIN_201, protoReq200);
                if (strOutData.length() < 1) {
                    throw new Exception();
                }

                JSONFactory jsonFactory = JSONFactory.getJSONInstance();
                m_arrayProtoRecommendedStore = jsonFactory.buildResForProto201(strOutData);
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

