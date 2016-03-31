package com.timewiz.cclab.aroundyou;

import java.io.Serializable;

/**
 * Created by jmhan on 2016-03-29.
 */
public class DataIPC {
    // nothing
}

// LoginActivity -> MainActivity
class DataLoginToMain implements Serializable {
    private String m_strPhoneNumber;

    public void setPhoneNumber(String strPhoneNumber) {
        this.m_strPhoneNumber = strPhoneNumber;
    }

    public String getNumber() {
        return this.m_strPhoneNumber;
    }
}

// MainActivity -> StoreActivity
class DataMainToStore implements Serializable  {
    int dwStoreID;
    int dwStoreLocation;
    int dwLocation;
    int dwStoreEvaluationTaste;
    int dwStoreEvaluationKind;
    int dwStoreEvaluationMood;
    String strStoreName;
    String strStoreInfoETC;
    String strStoreShortIntro;
    String strStoreHashTag;
    String strStoreAddress;
    String strStoreTel;
    String strStoreOpenTime;
    String strStoreCloseTime;
}

// MainActivity -> MenuActivity
class DataMainToMenu implements Serializable  {
    int dwStoreID;
    int dwLocation;
    int dwMenuID;
    int dwMenuPrice;
    int dwMenuEvaluation;
    String strMenuName;
}

// MainActivity -> ItemsListActivity
class DataMainToListItems implements Serializable  {
    int dwLocation;
    int dwStoreOrMenu;
}
