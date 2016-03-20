package com.timewiz.cclab.aroundyou;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.content.Intent;

/**
 * Created by jmhan on 2016-03-03.
 */
public class IntroActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.intro_page);
        Handler introHandler = new Handler();
        introHandler.postDelayed(new IntroHandler() , 2000);
    }

    private class IntroHandler implements Runnable{
        public void run() {
            startActivity(new Intent(IntroActivity.this, LoginActivity.class));
            IntroActivity.this.finish();
        }
    }
}
