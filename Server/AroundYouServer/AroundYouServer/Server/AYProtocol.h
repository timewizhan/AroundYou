#ifndef _AY_PROTOCOL_
#define _AY_PROTOCOL_

/*
	Protocol is divided with request, response
*/

enum E_PROTOCOL_REQUEST_TYPE
{
	/*
		Sign In
	*/
	E_PROTO_REQ_HEADER_SIGNIN = 0,
	E_PROTO_REQ_DATA_SIGNIN,

	/*
		Sign Up
	*/
	E_PROTO_REQ_HEADER_SIGNUP,
	E_PROTO_REQ_DATA_SIGNUP,

	E_PROTO_REQ_MENU_QUERY,
	E_PROTO_REQ_MENU_INSERT,

	E_PROTO_REQ_CLIENT_QUERY,
	E_PROTO_REQ_CLIENT_INSERT,

	E_PROTO_REQ_COMMNET_QUERY,
	E_PROTO_REQ_COMMNET_INSERT,

	E_PROTO_REQ_SHOP_QUERY,
	E_PROTO_REQ_SHOP_INSERT,

	E_PROTO_UNKNOWN
};

enum E_PROTOCOL_RESPONSE_TYPE
{
	/*
		Common Response 
	*/
	E_PROTO_RES_SUCCESS = 0,
	E_PROTO_RES_FAIL,
	
	/*
		Sign In
	*/
	E_PROTO_RES_DATA_NOT_EXIST,
	E_PROTO_RES_DATA_NOT_FOUND_EMAIL,
	E_PROTO_RES_DATA_NOT_FOUND_NICKNAME,
	E_PROTO_RES_DATA_NOT_FOUND_CALLID,

	/*
		Sign Up
	*/
	E_PROTO_RES_DATA_ACCOUNT_EXIST,


	E_PROTO_RES_NOTHING,

	E_PROTO_RES_QUERY_SUCESS,
	E_PROTO_RES_QUERY_FAIL,
	E_PROTO_RES_INSERT_SUCCESS,
	E_PROTO_RES_INSERT_FAIL,

	E_PROTO_RES_UNKNOWN
};

struct ST_PROTOCOL_ROOT
{
	DWORD dwRequest;
	//DWORD dwSizeOfData;
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
	DWORD		dwCommentNumber;
	DWORD		dwStoreNumber;
	std::string	strClientID;
	std::string	strCommentTime;
	std::string	strCommentContent;
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

