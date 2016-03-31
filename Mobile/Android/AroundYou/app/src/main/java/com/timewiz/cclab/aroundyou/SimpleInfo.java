package com.timewiz.cclab.aroundyou;

import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import java.text.DecimalFormat;

/**
 * Created by jmhan on 2016-03-31.
 */
public class SimpleInfo {
    private TextInfo m_TextInfo;
    private RankInfo m_RankInfo;
    private UserInfo m_UserInfo;

    public SimpleInfo(View view) {
        m_TextInfo = new TextInfo(view);
        m_RankInfo = new RankInfo(view);
        //m_UserInfo = new UserInfo(view);
    }

    public void setSimpleDetailInfo(SimpleInfoData simpleInfoData) {
        m_TextInfo.setTextInfo(simpleInfoData);
        m_RankInfo.setRankInfo(simpleInfoData);
    }

    private class TextInfo {
        private TextView m_TextRecommendedName;
        private TextView m_TextHashTag;
        private TextView m_TextIntroduction;

        public TextInfo(View view) {
            m_TextRecommendedName = (TextView) view.findViewById(R.id.textRecommendName);
            m_TextHashTag = (TextView) view.findViewById(R.id.textHashTag);
            m_TextIntroduction = (TextView) view.findViewById(R.id.textIntroduction);
        }

        public void setTextInfo(SimpleInfoData simpleInfoData) {
            m_TextRecommendedName.setText(simpleInfoData.strStoreName);
            m_TextHashTag.setText(simpleInfoData.strStoreHashTag);
            m_TextIntroduction.setText(simpleInfoData.strStoreShortIntro);
        }
    }

    private class RankInfo {
        private ImageView[] arrImageDelicious;
        private ImageView[] arrImageAttitude;
        private ImageView[] arrImageMood;
        private TextView textReputation;

        public RankInfo(View view) {
            arrImageDelicious = new ImageView[5];
            arrImageDelicious[0] =  (ImageView) view.findViewById(R.id.imageRatingDelicious_1);
            arrImageDelicious[1] =  (ImageView) view.findViewById(R.id.imageRatingDelicious_2);
            arrImageDelicious[2] =  (ImageView) view.findViewById(R.id.imageRatingDelicious_3);
            arrImageDelicious[3] =  (ImageView) view.findViewById(R.id.imageRatingDelicious_4);
            arrImageDelicious[4] =  (ImageView) view.findViewById(R.id.imageRatingDelicious_5);

            arrImageAttitude = new ImageView[5];
            arrImageAttitude[0] =  (ImageView) view.findViewById(R.id.imageRatingAttitude_1);
            arrImageAttitude[1] =  (ImageView) view.findViewById(R.id.imageRatingAttitude_2);
            arrImageAttitude[2] =  (ImageView) view.findViewById(R.id.imageRatingAttitude_3);
            arrImageAttitude[3] =  (ImageView) view.findViewById(R.id.imageRatingAttitude_4);
            arrImageAttitude[4] =  (ImageView) view.findViewById(R.id.imageRatingAttitude_5);

            arrImageMood = new ImageView[5];
            arrImageMood[0] =  (ImageView) view.findViewById(R.id.imageRatingMood_1);
            arrImageMood[1] =  (ImageView) view.findViewById(R.id.imageRatingMood_2);
            arrImageMood[2] =  (ImageView) view.findViewById(R.id.imageRatingMood_3);
            arrImageMood[3] =  (ImageView) view.findViewById(R.id.imageRatingMood_4);
            arrImageMood[4] =  (ImageView) view.findViewById(R.id.imageRatingMood_5);

            textReputation = (TextView) view.findViewById(R.id.textReputation);
        }

        public void setRankInfo(SimpleInfoData simpleInfoData) {
            int nTaste = simpleInfoData.dwStoreEvaluationTaste;
            int nKind = simpleInfoData.dwStoreEvaluationKind;
            int nMood = simpleInfoData.dwStoreEvaluationMood;

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
}

class SimpleInfoData {
    public int dwStoreEvaluationTaste;
    public int dwStoreEvaluationKind;
    public int dwStoreEvaluationMood;
    public String strStoreName;
    public String strStoreHashTag;
    public String strStoreShortIntro;

    public SimpleInfoData() {
        dwStoreEvaluationTaste = dwStoreEvaluationKind = dwStoreEvaluationMood = 0;
        strStoreName = strStoreHashTag = strStoreShortIntro = "";
    }
}