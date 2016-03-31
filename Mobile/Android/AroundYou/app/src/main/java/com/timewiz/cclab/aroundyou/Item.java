package com.timewiz.cclab.aroundyou;

/**
 * Created by jmhan on 2016-03-28.
 */
public class Item {

    private String m_strItemName;
    private String m_strItemHashTag;
    private String m_strItemPrice;
    private String m_strItemReputation;

    public Item() {
        m_strItemName = "";
        m_strItemHashTag = "";
        m_strItemPrice = "0";
        m_strItemReputation = "0.0";
    }

    public Item(String strItemName, String strItemHashTag, String strItemPrice, String strItemReputation) {
        m_strItemName = strItemName;
        m_strItemHashTag = strItemHashTag;
        m_strItemPrice = strItemPrice;
        m_strItemReputation = strItemReputation;
    }

    public void setItemName(String itemName) {
        m_strItemName = itemName;
    }

    public void setItemHashTag(String itemHashTag) {
        m_strItemHashTag = itemHashTag;
    }

    public void setItemPrice(String itemPrice) {
        m_strItemPrice = itemPrice;
    }

    public void setItemReputation(String itemReputation) {
        m_strItemReputation = itemReputation;
    }

    public String getItemName() {
        return m_strItemName;
    }

    public String getItemHashTag() {
        return m_strItemHashTag;
    }

    public String getItemPrice() {
        return m_strItemPrice;
    }

    public String getItemReputation() {
        return  m_strItemReputation;
    }

}
