package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.media.Image;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

public class LoginActivity extends Activity {
    private PreProcess m_PreProcess;
    private PhoneInfo m_phoneInfo;
    CTouchXY m_CTouchXY;
    CImageMover m_CImageMover;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login_page);

        m_PreProcess = new PreProcess();
        m_phoneInfo = new PhoneInfo();
        m_CTouchXY = new CTouchXY();
        m_CImageMover = new CImageMover();
    }

    private int m_iTouchX;
    public boolean onTouchEvent(MotionEvent event) {
        super.onTouchEvent(event);

        m_CTouchXY.setTouchX((int) event.getX());
        m_CTouchXY.setTouchY((int) event.getY());

        if(event.getAction()== MotionEvent.ACTION_DOWN) {
            Log.i("Touch Event", "Touch Down");
        }
        else if(event.getAction() == MotionEvent.ACTION_MOVE) {
            Log.i("Touch Event", "Touch Move");
            m_CImageMover.moveImageByTouchXY(m_CTouchXY);
        }
        else if(event.getAction() == MotionEvent.ACTION_UP) {
            Log.i("Touch Event", "Touch Up");
        }
        return true;
    }

    class CImageMover {
        private ImageView m_imageBarView, m_imageBarMoveView;

        public CImageMover() {
            m_imageBarView = (ImageView) findViewById(R.id.imageLoginBar);
            m_imageBarMoveView = (ImageView) findViewById(R.id.imageLoginBarMove);
            setLimitedValue();
        }

        boolean bSet = false;
        int m_ninitialValue, m_nBoundaryValue;

        public void moveImageByTouchXY(CTouchXY cTouchXY) {
            if (!bSet) {
                setLimitedValue();
                bSet = true;
            }

            if (!checkInvalidXY(cTouchXY))
                return;

            int nTouchX = cTouchXY.getTouchX();
            if (nTouchX < m_ninitialValue) {
                Log.i("Image Move", "Invalid value : less than inital value");
            }
            else if (nTouchX > nTouchX) {
                Log.i("Image Move", "Invalid value : more than bounday value");
            }
            else {
                m_imageBarMoveView.setX(nTouchX);
                checkNextActivity(nTouchX);
            }
        }

        private void setLimitedValue() {
            int[] arrayBarLocation = new int[2];
            m_imageBarView.getLocationOnScreen(arrayBarLocation);

            m_ninitialValue = (int)m_imageBarView.getX() - (m_imageBarMoveView.getWidth() / 2);
            m_nBoundaryValue = m_ninitialValue + m_imageBarView.getWidth();

            String strLimited = "XValue " +  m_ninitialValue + " Boundary " + m_nBoundaryValue;
            Log.i("Limited", strLimited);
        }

        private final int m_paddingValue = 5;
        private boolean checkInvalidXY(CTouchXY cTouchXY) {
            int[] arrayImageMoveBarlocation = new int[2];
            m_imageBarMoveView.getLocationOnScreen(arrayImageMoveBarlocation);

            int nCurrentImageX = arrayImageMoveBarlocation[0] - m_paddingValue;
            int nCurrentImageY = arrayImageMoveBarlocation[1] - m_paddingValue;
            int nCurrentXMaxRange = nCurrentImageX + m_imageBarMoveView.getWidth() + m_paddingValue;
            int nCurrentYMaxRange = nCurrentImageY + m_imageBarMoveView.getHeight() + m_paddingValue;

            String strMsgX = "Image X : " + nCurrentImageX + " Image Max X : " + nCurrentXMaxRange + " Touch X" + cTouchXY.getTouchX();
            String strMsgY = "Image Y : " + nCurrentImageY + " Image Max Y : " + nCurrentYMaxRange + " Touch Y" + cTouchXY.getTouchY();

            Log.i("Login Touch XY", strMsgX);
            Log.i("Login Touch XY", strMsgY);

            if (nCurrentImageX <= cTouchXY.getTouchX() && cTouchXY.getTouchX() <= nCurrentXMaxRange) {
                if (nCurrentImageY <= cTouchXY.getTouchY() && cTouchXY.getTouchY() <= nCurrentYMaxRange) {
                    Log.i("Touch XY", "valid XY value");
                    return true;
                }
            }
            return false;
        }

        private  void checkNextActivity(int nTouchX) {
            int nAllowedPos = (m_nBoundaryValue / 3) * 2;
            if (nTouchX > nAllowedPos) {
                Log.i("LoginActivity", "Go to next activity");
                m_PreProcess.execute();
            }
        }
    }

    class CTouchXY {
        private int m_iTouchX;
        private int m_iTouchY;

        public CTouchXY() {
            m_iTouchX = m_iTouchY = 0;
        }

        public void setTouchX(int X) {
            m_iTouchX = X;
        }

        public void setTouchY(int Y) {
            m_iTouchY = Y;
        }

        public int getTouchX() {
            return m_iTouchX;
        }

        public int getTouchY() {
            return m_iTouchY;
        }
    }

    class PhoneInfo {
        TelephonyManager m_TelephonyManager;

        public PhoneInfo() {
            m_TelephonyManager = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
        }

        public String getPhoneNumber() {
            return m_TelephonyManager.getLine1Number();
        }
    }

    private class PreProcess extends AsyncTask<Void, Void, Void> {
        ProgressDialog asyncDialog = new ProgressDialog(LoginActivity.this);

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
                Communication communication = new Communication();

                PROTOCOL_ROOT protoReq100 = getProtoLoginReq();
                String strOutData;
                strOutData = communication.communicateWithServer(E_PROTOCOL_TYPE.E_PROTO_LOGIN_101, protoReq100);
                if (strOutData.length() < 1) {
                    throw new Exception();
                }
            } catch (Exception e) {
                Log.e("LoginActivity", "Fail to operate works in background");
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            asyncDialog.dismiss();
            super.onPostExecute(result);

            Log.i("LoginActivity", "Succeed to log in");
            goToNextAcitivty();
        }

        private PROTOCOL_ROOT getProtoLoginReq() {
            PROTOCOL_REQ_101_ABOUT_USER protoReq101 = new PROTOCOL_REQ_101_ABOUT_USER();
            protoReq101.strCallID = m_phoneInfo.getPhoneNumber();
            return (PROTOCOL_ROOT) protoReq101;
        }

        private void goToNextAcitivty() {
            Intent intent = new Intent(LoginActivity.this, MainActivity.class);

            DataLoginToMain dataLoginToMain = new DataLoginToMain();
            dataLoginToMain.setPhoneNumber(m_phoneInfo.getPhoneNumber());

            intent.putExtra("DataLoginToMain", dataLoginToMain);
            startActivity(intent);
        }
    }

}

