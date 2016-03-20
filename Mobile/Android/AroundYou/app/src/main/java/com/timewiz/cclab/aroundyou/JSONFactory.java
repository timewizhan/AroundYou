package com.timewiz.cclab.aroundyou;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.ArrayList;

/**
 * Created by jmhan on 2016-03-17.
 */
public class JSONFactory {
    private static JSONFactory m_JSONFactory;

    private JSONFactory() {
        // For singleton
    }

    public static JSONFactory getJSONInstance() {
        if (m_JSONFactory == null) {
            m_JSONFactory = new JSONFactory();
        }
        return m_JSONFactory;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public String buildReqDefault(E_PROTOCOL_REQUEST_TYPE eReqToSend) {
        JSONObject jsonRoot = new JSONObject();
        String strResult = null;
        try {
            jsonRoot.put("req", eReqToSend.ordinal());
            strResult = jsonRoot.toString();
        }
        catch (JSONException e) {
            Log.e("JSONFactory", "Fail to build json string (DEFAULT REQUEST)");
        }
        return strResult;
    }

    public String buildReqDefaultArg(E_PROTOCOL_REQUEST_TYPE eNextReqToSend, int nSizeOfNextData) {
        JSONObject jsonRoot = new JSONObject();
        String strResult = null;
        try {
            jsonRoot.put("req", E_PROTOCOL_REQUEST_TYPE.E_PROTO_REQ_CONNECTION_START.ordinal());
            jsonRoot.put("next_req", eNextReqToSend.ordinal());
            jsonRoot.put("next_size", nSizeOfNextData);
            strResult = jsonRoot.toString();
        }
        catch (JSONException e) {
            Log.e("JSONFactory", "Fail to build json string (E_PROTO_REQ_CONNECTION_START)");
        }
        return strResult;
    }

    //PROTOCOL_REQ_201_RECOMMENDED_STORE
    public String buildReqForProto201(PROTOCOL_REQ_201_RECOMMENDED_STORE proto) {
        JSONObject jsonRoot = new JSONObject();
        String strResult = null;
        try {
            jsonRoot.put("req", E_PROTOCOL_REQUEST_TYPE.E_PROTO_REQ_RECOMMENDED_STORE.ordinal());

            JSONObject jsonData = new JSONObject();
            jsonData.put("location", proto.nLocation);
            jsonData.put("res_number", proto.nNumberOfRequsetedMaximum);

            jsonRoot.put("data", jsonData);

            strResult = jsonRoot.toString();
        }
        catch (JSONException e) {
            Log.e("JSONFactory", "Fail to build json string (PROTOCOL_REQ_201_RECOMMENDED_STORE)");
        }
        return strResult;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public PROTOCOL_RES_DEFAULT buildResDefault(String strInput) {
        JSONParser jsonParser = new JSONParser();

        PROTOCOL_RES_DEFAULT protoResDefault = new PROTOCOL_RES_DEFAULT();
        try {
            JSONObject jsonRoot = (JSONObject) jsonParser.parse(strInput);
            int nRes = (int) jsonRoot.get("res");

            protoResDefault.nResponse = nRes;
        }
        catch (ParseException e) {
            Log.e("JSONFactory", "Fail to parse json string (PROTOCOL_RES_DEFAULT)");
        }
        catch (JSONException e) {

        }
        return protoResDefault;
    }

    public PROTOCOL_RESQ_DEFAULT_ARG buildResDefaultArg(String strInput) {
        JSONParser jsonParser = new JSONParser();

        PROTOCOL_RESQ_DEFAULT_ARG protoResDefaultArg = new PROTOCOL_RESQ_DEFAULT_ARG();
        try {
            JSONObject jsonRoot = (JSONObject) jsonParser.parse(strInput);
            int nRes = (int) jsonRoot.get("res");
            int nNextSize = (int) jsonRoot.get("next_size");

            protoResDefaultArg.nResponse = nRes;
            protoResDefaultArg.nNextSize = nNextSize;
        }
        catch (ParseException e) {
            Log.e("JSONFactory", "Fail to parse json string (PROTOCOL_RESQ_DEFAULT_ARG)");
        }
        catch (JSONException e) {

        }
        return protoResDefaultArg;
    }

    // PROTOCOL_RES_201_RECOMMENDED_STORE
    public ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE> buildResForProto201(String strInput) {
        JSONParser jsonParser = new JSONParser();

        ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE> arrayRes201 = new ArrayList<PROTOCOL_RES_201_RECOMMENDED_STORE>();
        try {
            JSONObject jsonRoot = (JSONObject) jsonParser.parse(strInput);

            int nRes = (int) jsonRoot.get("res");
            JSONObject jsonData = (JSONObject) jsonRoot.get("data");

            int nRetNumber = (int) jsonData.get("ret_number");
            JSONArray jsonStores = (JSONArray) jsonData.get("stores");

            for (int i = 0; i < jsonStores.length(); i++) {
                JSONObject jsonStore = (JSONObject) jsonStores.get(i);
                PROTOCOL_RES_201_RECOMMENDED_STORE protoRes201RecommendedStore = new PROTOCOL_RES_201_RECOMMENDED_STORE();

                protoRes201RecommendedStore.dwStoreID = (int) jsonStore.get("store_id");
                protoRes201RecommendedStore.strStoreName = (String) jsonStore.get("store_name");
                protoRes201RecommendedStore.dwStoreLocation = (int) jsonStore.get("store_location");

                JSONObject jsonStoreEvaluation = (JSONObject) jsonStore.get("store_evaluation");
                protoRes201RecommendedStore.dwStoreEvaluationTaste = (int) jsonStoreEvaluation.get("taste");
                protoRes201RecommendedStore.dwStoreEvaluationKind = (int) jsonStoreEvaluation.get("kind");
                protoRes201RecommendedStore.dwStoreEvaluationMood = (int) jsonStoreEvaluation.get("mood");

                protoRes201RecommendedStore.strStoreShortIntro = (String) jsonStore.get("store_short_intro");
                protoRes201RecommendedStore.strStoreHashTag = (String) jsonStore.get("store_hashtag");

                // Todo
                /*
                JSONObject jsonStorePriceRange = (JSONObject)  jsonStore.get("store_price_range");
                int nMinPrice = (int) jsonStorePriceRange.get("min_price");
                int nMaxPrice = (int) jsonStorePriceRange.get("max_price");

                JSONObject jsonStoreShortInfo = (JSONObject)  jsonStore.get("store_short_info");
                int nNumberOfLike = (int) jsonStoreShortInfo.get("number_of_like");
                int nNumberOfEvaluation = (int) jsonStoreShortInfo.get("number_of_evaluation");
                int nNumberOfComment = (int) jsonStoreShortInfo.get("number_of_comment");
                */
            }


        }
        catch (ParseException e) {
            Log.e("JSONFactory", "Fail to parse json string (PROTOCOL_RES_201_RECOMMENDED_STORE)");
        }
        catch (JSONException e) {

        }
        return arrayRes201;
    }
}
