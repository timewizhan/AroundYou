package com.timewiz.cclab.aroundyou;

import java.util.ArrayList;

/**
 * Created by jmhan on 2016-03-17.
 */
public class Protocol {
    // nothing
}

enum E_PROTOCOL_TYPE
{
    E_PROTO_LOGIN_101,

    E_PROTO_MAIN_201,
    E_PROTO_MAIN_202,

    E_PROTO_SELECT_301,
    E_PROTO_SELECT_302,

    E_PROTO_DETAIL_503,
}

enum E_PROTOCOL_REQUEST_TYPE
{
    E_PROTO_REQ_CONNECTION_START,
    E_PROTO_REQ_READY_FOR_DATA,

    // Protocol 101
    E_PROTO_REQ_USER_LOG_IN,
    E_PROTO_REQ_USER_SIGN_IN,
    E_PROTO_REQ_USER_SIGN_UP,

    // Protocol 201
    E_PROTO_REQ_RECOMMENDED_STORE,
    // Protocol 202
    E_PROTO_REQ_RECOMMENDED_MENU,

    // Protocol 301
    E_PROTO_REQ_SELECTED_STORE,
    // Protocol 302
    E_PROTO_REQ_SELECTED_MENU,

    // Protocol 503
    E_PROTO_REQ_DETAIL_MENU,

    E_PROTO_REQ_INPUT_EVALUATION,

    E_PROTO_REQ_INPUT_COMMENT,
    E_PROTO_REQ_SHOW_COMMENT_LIST,
    E_PROTO_REQ_NEXT_SHOW_COMMENT_LIST,

    E_PROTO_UNKNOWN
};

enum E_PROTOCOL_RESPONSE_TYPE
{
    E_PROTO_RES_READY_FOR_DATA,
    E_PROTO_RES_NEXT_STEP_EXIST,
    E_PROTO_RES_FINISH,
    E_PROTO_RES_FAIL_OPERATION,
    E_PROTO_RES_INVALID_NUMBER,


	/*
		Common Response
	*/
    E_PROTO_RES_SUCCESS,
    E_PROTO_RES_FAIL,

    E_PROTO_RES_UNKNOWN
};

enum E_LOCATION_TYPE
{
    E_LOCATION_SHINCHON_EHWA,

    E_LOCATION_UNKNOWN
};

interface PROTOCOL_ROOT {}

class PROTOCOL_REQ_ROOT implements PROTOCOL_ROOT{
    int nRequest;
}

class PROTOCOL_RES_ROOT implements PROTOCOL_ROOT{
    int nResponse;
}

/*
    Request
 */

// Default Req
class PROTOCOL_REQ_DEFAULT extends PROTOCOL_REQ_ROOT {}
class PROTOCOL_REQ_DEFAULT_ARG extends PROTOCOL_REQ_ROOT {
    int nNextReq;
    int nNextSize;
}

class PROTOCOL_REQ_101_ABOUT_USER extends PROTOCOL_REQ_ROOT {
    String strCallID;
}

class PROTOCOL_REQ_201_RECOMMENDED_STORE extends PROTOCOL_REQ_ROOT {
    int nLocation;
    int nNumberOfRequsetedMaximum;
}

class PROTOCOL_REQ_202_RECOMMENDED_MENU extends PROTOCOL_REQ_ROOT {
    int nLocation;
    int nNumberOfRequsetedMaximum;
}

class PROTOCOL_REQ_301_SELECTED_STORE extends PROTOCOL_REQ_ROOT {
    public int nLocation;
    public int nStoreID;
    public int nCallID;
}

class PROTOCOL_REQ_302_SELECTED_MENU extends PROTOCOL_REQ_ROOT {
    int nLocation;
    int nStoreID;
    int nCallID;
};

class PROTOCOL_REQ_503_DETAIL_MENU extends PROTOCOL_REQ_ROOT {
    int nLocation;
    int nStoreID;
};

/*
    Response
 */

// Default Res
class PROTOCOL_RES_DEFAULT extends PROTOCOL_RES_ROOT {}
class PROTOCOL_RESQ_DEFAULT_ARG extends PROTOCOL_RES_ROOT {
    int nNextSize;
}

class PROTOCOL_RES_201_RECOMMENDED_STORE extends PROTOCOL_RES_ROOT {
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
};

class PROTOCOL_RES_202_RECOMMENDED_MENU extends PROTOCOL_RES_ROOT {
    int nStoreID;
    int nStoreLocation;
    int nMenuID;
    int nMenuEvaluation;
    String strMenuName;
};

class PROTOCOL_RES_301_SELECT_STORE_MENUS {
    int nMenuID;
    int nMenuPrice;
    int nMenuEvaluation;
    String strMenuName;
}

class PROTOCOL_RES_301_SELECT_STORE extends PROTOCOL_RES_ROOT {
    String strStoreAddress;
    String strStoreTel;
    String strStoreTime;
    int nStoreLike;
    int nMenuTotalCount;
    ArrayList<PROTOCOL_RES_301_SELECT_STORE_MENUS> arrayMenus;
};

class PROTOCOL_RES_302_SELECT_MENU extends PROTOCOL_RES_ROOT {
    int nMenuLike;
    int nMenuTotalCount;
};

class PROTOCOL_RES_503_DETAIL_MENU extends PROTOCOL_RES_ROOT {
    int nMenuTotalCount;
    ArrayList<PROTOCOL_RES_301_SELECT_STORE_MENUS> arrayMenus;
};