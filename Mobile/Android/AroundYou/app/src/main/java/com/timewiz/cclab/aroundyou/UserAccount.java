package com.timewiz.cclab.aroundyou;

/**
 * Created by jmhan on 2016-03-30.
 */
public class UserAccount {
    private UserAccount() {
        // nothing
    }

    private static UserAccount m_UserAccount;
    public static synchronized UserAccount getInstance() {
        if (m_UserAccount == null) {
            m_UserAccount = new UserAccount();
        }
        return m_UserAccount;
    }

    private String m_strPhoneNumber;
    public void setPhoneNumber(String strPhoneNumber) {
        m_strPhoneNumber = strPhoneNumber;
    }

    public String getPhoneNumber() {
        return m_strPhoneNumber;
    }
}
