package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageView;

import java.net.Socket;
import java.util.ArrayList;

/**
 * Created by jmhan on 2016-03-13.
 */
public class MainActivity extends Activity implements View.OnClickListener {
    private WebView m_webView;

    private PreProcess m_PreProcess;
    private ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE> arrayProtoRecommendedStore;
    private Communication m_Communication;
    private PROTOCOL_ROOT protoReq200;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_page);

        m_webView = (WebView) findViewById(R.id.webViewImageFromServer);
        m_webView.setWebViewClient(new WebViewClient());
        m_webView.loadUrl("http://www.google.com");

        setConstuctor();
        setListener();
    }

    private ImageView imageSlideBarMenu;
    private ImageView imageAppName;
    private ImageView imageRecommendStoreEnable;
    private ImageView imageRecommendMenuDisable;
    private ImageView imageLeftArrow;
    private ImageView imageRightArrow;

    private void setConstuctor() {
        m_PreProcess = new PreProcess();
        arrayProtoRecommendedStore = new ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE>();
        m_Communication = new Communication();
        protoReq200 = new PROTOCOL_REQ_201_RECOMMENDED_STORE();

        imageSlideBarMenu = (ImageView) findViewById(R.id.imageSlideBarMenu);
        imageAppName = (ImageView) findViewById(R.id.imageAppName);
        imageRecommendStoreEnable = (ImageView) findViewById(R.id.imageRecommendStoreEnable);
        imageRecommendMenuDisable = (ImageView) findViewById(R.id.imageRecommendMenuDisable);
        imageLeftArrow = (ImageView) findViewById(R.id.imageLeftArrow);
        imageRightArrow = (ImageView) findViewById(R.id.imageRightArrow);
    }

    private void setListener() {
        imageSlideBarMenu.setOnClickListener(this);
        imageAppName.setOnClickListener(this);
        imageRecommendStoreEnable.setOnClickListener(this);
        imageRecommendMenuDisable.setOnClickListener(this);
        imageLeftArrow.setOnClickListener(this);
        imageRightArrow.setOnClickListener(this);
    }

    @Override
    protected void onStart() {
        super.onStart();
        m_PreProcess.execute();
    }

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
                break;
            case R.id.imageRecommendMenuDisable:
                Log.i("MainActivity", "Click image (imageRecommendMenuDisable)");
                break;
            case R.id.imageLeftArrow:
                Log.i("MainActivity", "Click image (imageLeftArrow)");
                break;
            case R.id.imageRightArrow:
                Log.i("MainActivity", "Click image (imageRightArrow)");
                break;
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
                /*
                Errno errno;
                String strOutData = "";
                errno = m_Communication.communicateWithServer(E_PROTOCOL_TYPE.E_PROTO_MAIN_201, protoReq200, strOutData);
                if (errno != Errno.E_RET_SUCCESS) {
                    throw new Exception();
                }

                JSONFactory jsonFactory = JSONFactory.getJSONInstance();
                arrayProtoRecommendedStore = jsonFactory.buildResForProto201(strOutData);
                */
                Thread.sleep(1000);
            } catch (Exception e) {
                Log.e("MainActivity", "Fail to operate works in background");
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            asyncDialog.dismiss();
            super.onPostExecute(result);
        }
    }


}
