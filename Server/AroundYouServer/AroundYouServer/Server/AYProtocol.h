#ifndef _AY_PROTOCOL_
#define _AY_PROTOCOL_

/*
	Protocol is divided with request, response
*/

enum E_PROTOCOL_REQUEST_TYPE
{
	E_PROTO_REQ_CONNECTION_START = 0,
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
	E_PROTO_RES_READY_FOR_DATA = 0,
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
	E_LOCATION_SHINCHON_EHWA = 0,

	E_LOCATION_UNKNOWN
};


struct ST_RECV_HEADER_DATA
{
	DWORD dwReqNumber;
	DWORD dwNextReqNumber;
	DWORD dwNextSizeOfData;
};

struct ST_PROTOCOL_ROOT
{
	DWORD dwRequest;
	//DWORD dwSizeOfData;
};

/*
	Request
*/

struct ST_PROTOCOL_REQ_101_ABOUT_USER : public ST_PROTOCOL_ROOT
{
	std::string strCallID;
};

struct ST_PROTOCOL_REQ_201_RECOMMENDED_STORE : public ST_PROTOCOL_ROOT
{
	DWORD dwLocation;
	DWORD dwNumberOfRequestedMaximun;
};


struct ST_PROTOCOL_REQ_202_RECOMMENDED_MENU : public ST_PROTOCOL_ROOT
{
	DWORD dwLocation;
	DWORD dwNumberOfRequestedMaximun;
};

struct ST_PROTOCOL_REQ_301_SELECTED_STORE : public ST_PROTOCOL_ROOT
{
	DWORD dwLocation;
	DWORD dwStoreID;
	std::string strCallID;
};

struct ST_PROTOCOL_REQ_302_SELECTED_MENU : public ST_PROTOCOL_ROOT
{
	DWORD dwLocation;
	DWORD dwStoreID;
	std::string strCallID;
};

struct ST_PROTOCOL_REQ_401_INPUT_EVALUATION : public ST_PROTOCOL_ROOT
{
	DWORD dwStoreID;
	DWORD dwMenuID;
	DWORD dwTaste;
	DWORD dwKind;
	DWORD dwMood;
	DWORD dwCallID;
};

struct ST_PROTOCOL_REQ_501_INPUT_COMMENT : public ST_PROTOCOL_ROOT
{
	DWORD dwStoreID;
	DWORD dwLocation;
	DWORD dwCallID;
	std::string strCommentWriting;
	std::string strCommentAuthor;
	std::string strCommentTime;
};

struct ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST : public ST_PROTOCOL_ROOT
{
	DWORD dwStoreID;
	DWORD dwLocation;
	DWORD dwCommentReqCount;
	DWORD dwNumberOfRequestedMaximun;
};


/*
	Response
*/

struct ST_PROTOCOL_RES_201_RECOMMENDED_STORE : public ST_PROTOCOL_ROOT
{
	DWORD dwStoreID;
	DWORD dwStoreLocation;
	DWORD dwLocation;
	DWORD dwStoreEvaluationTaste;
	DWORD dwStoreEvaluationKind;
	DWORD dwStoreEvaluationMood;
	std::string strStoreName;
	std::string strStoreInfoETC;
	std::string strStoreShortIntro;
	std::string strStoreHashTag;
	std::string strStoreAddress;
	std::string strStoreTel;
	std::string strStoreOpenTime;
	std::string strStoreCloseTime;
};

struct ST_PROTOCOL_RES_202_RECOMMENDED_MENU : public ST_PROTOCOL_ROOT
{
	DWORD dwStoreID;
	DWORD dwMenuID;
	DWORD dwMenuPrice;
	DWORD dwMenuEvaluation;
	std::string strMenuName;
};

struct ST_PROTOCOL_RES_301_SELECT_STORE : public ST_PROTOCOL_ROOT
{
	DWORD dwMenuID;
	DWORD dwMenuPrice;
	DWORD dwMenuEvaluation;
	std::string strMenuName;
};

struct ST_PROTOCOL_RES_302_SELECT_MENU : public ST_PROTOCOL_ROOT
{
	DWORD dwMenuID;
	DWORD dwMenuEvaluation;
	std::string strMenuName;
};

struct ST_PROTOCOL_RES_502_SHOW_COMMENT_LIST : public ST_PROTOCOL_ROOT
{
	std::string strCommentWriting;
	std::string strCommentAuthor;
	std::string strCommentTime;
};

struct ST_PROTOCOL_SIGNIN : public ST_PROTOCOL_ROOT
{
	std::string	strNickName;
	std::string strEmail;
	std::string strCallID;
};

struct ST_PROTOCOL_SIGNUP : public ST_PROTOCOL_ROOT
{
	std::string	strNickName;
	std::string strEmail;
	std::string strCallID;
};

struct ST_PROTOCOL_MENU : public ST_PROTOCOL_ROOT
{
	DWORD		dwMenuNumber;
	DWORD		dwMenuPrice;
	std::string	strMenuName;
	std::string	strStoreName;
	std::string	strStoreIndex;
};

struct ST_PROTOCOL_CLIENT : public ST_PROTOCOL_ROOT
{
	std::string strClientID;
	std::string strClientMail;
	std::string strClientPhoneNumber;
	DWORD		dwClientNumber;
};

struct ST_PROTOCOL_COMMENT : public ST_PROTOCOL_ROOT
{
	std::string	strStoreIndex;
	std::string	strMenuIndex;
	std::string strWriter;
	std::string strReputation;
	std::string	strText;
	std::string	strWriteTime;
};

struct ST_PROTOCOL_SHOP : public ST_PROTOCOL_ROOT
{
	DWORD dwShopNumber;
	BOOL bShopSmoking;
	std::string strShopName;
	std::string strShopAddress;
	std::string strShopPhoneNumber;
	std::string strShopCategory;
	std::string strShopHomepage;
	std::string strShopRunningTime;
	std::string strShopHoliday;
	std::string strShopETC;
	std::string strShopParking;
	std::string strShopAlcohol;
	std::string strShopFacility;
	std::string strShopIntroduction;
};

#endif

