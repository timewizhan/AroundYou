#include "AYWorkerThread.h"
#include "AYProtocol.h"
#include "AYServerError.h"

#include "..\Common\Log.h"
#include "json\reader.h"
#include "json\json.h"

#include "SignIn.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
CAYWorkerThread::CAYWorkerThread(SOCKET ClientSocket)
{
	::memset(&m_stWorkerThread, 0x00, sizeof(ST_WORKER_THREAD));
	m_stWorkerThread.hClientSocket = ClientSocket;

	m_stDBLoginToken.strDatabaseName	= "postgres";
	m_stDBLoginToken.strDatabaseIP		= "165.132.120.155";
	m_stDBLoginToken.strUserName		= "postgres";
	m_stDBLoginToken.strPassword		= "cclabiu";
	m_stDBLoginToken.strPort			= "5432";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
CAYWorkerThread::~CAYWorkerThread()
{
	closesocket(m_stWorkerThread.hClientSocket);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ReceiveDataFromClient(char *pReceiveBuf, DWORD dwByteTransferred, ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	refstRecvHeaderData.strRecvData = pReceiveBuf;
	refstRecvHeaderData.strRecvData.resize(dwByteTransferred);
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ParseReceivedHeaderByJSON(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	Json::Value JsonRoot;
	Json::Reader reader;
	bool bParsingRet = reader.parse(refstRecvHeaderData.strRecvData, JsonRoot);
	if (!bParsingRet) {
		ErrorLog("Fail to parse a received data [%s]", reader.getFormatedErrorMessages());
		std::cout << reader.getFormatedErrorMessages() << std::endl;
		return E_RET_FAIL;
	}

	std::string strRequest = JsonRoot.get("request", 0).asString();
	DWORD		dwSizeOfData = ::atoi(JsonRoot.get("size", 0).asString().c_str());
	refstRecvHeaderData.dwNumberOfRequest = ::atoi(strRequest.c_str());
	refstRecvHeaderData.dwSizeOfData = dwSizeOfData;
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::SendHeaderToClient(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNumberOfRequest = refstRecvHeaderData.dwNumberOfRequest;
	DWORD dwRet = E_RET_SUCCESS;
	Json::Value JsonRoot;
	if (dwNumberOfRequest == E_PROTO_REQ_HEADER_SIGNIN || 
		dwNumberOfRequest == E_PROTO_REQ_HEADER_SIGNUP ||
		dwNumberOfRequest == E_PROTO_REQ_HEADER_STORES ||
		dwNumberOfRequest == E_PROTO_REQ_HEADER_MENUS || 
		dwNumberOfRequest == E_PROTO_REQ_HEADER_COMMENTS ||
		dwNumberOfRequest == E_PROTO_REQ_HEADER_COMMENTS_INSERT) {
		JsonRoot["response"] = E_PROTO_RES_SUCCESS;

		Json::StyledWriter JsonWriter;
		std::string strSendData = JsonWriter.write(JsonRoot);
		
		dwRet = SendDataToClient(strSendData);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to send data about request");
		}
		DebugLog("Success to send data about request");
	}
	else {
		/*
			etc..
		*/
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ReceiveDataFromClient(ST_RECV_HEADER_DATA &refstRecvHeaderData, ST_RECV_DATA &refstRecvData)
{
	char *pRecvBuffer = NULL;
	pRecvBuffer = new char[refstRecvHeaderData.dwSizeOfData];
	::memset(pRecvBuffer, 0x00, refstRecvHeaderData.dwSizeOfData);

	int nRecv;
	nRecv = ::recv(m_stWorkerThread.hClientSocket, pRecvBuffer, refstRecvHeaderData.dwSizeOfData, 0);
	if (nRecv == SOCKET_ERROR) {
		ErrorLog("Fail to recv data from client");
		delete pRecvBuffer;
		return E_RET_FAIL;
	}
	refstRecvData.strRecvData = pRecvBuffer;
	refstRecvData.strRecvData.resize(nRecv);
	delete pRecvBuffer;
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ParseDataByJSON(DWORD &refdwNumberOfRequest, ST_RECV_DATA &refstRecvData)
{
	Json::Value JsonRoot;
	Json::Reader reader;
	bool bParsingRet = reader.parse(refstRecvData.strRecvData, JsonRoot);
	if (!bParsingRet) {
		ErrorLog("Fail to parse a received data [%s]", reader.getFormatedErrorMessages());
		std::cout << reader.getFormatedErrorMessages() << std::endl;
		return E_RET_FAIL;
	}

	std::string strRequest = JsonRoot.get("request", 0).asString();
	DWORD dwRequest = ::atoi(strRequest.c_str());
	refdwNumberOfRequest = dwRequest;
	/*
		you have to cast pointer to use pointer flexibly
	*/

	///////////// Sign In //////////////
	if (dwRequest == E_PROTO_REQ_DATA_SIGNIN) {
		m_pstProtoSignIn = new ST_PROTOCOL_SIGNIN;
		::memset(m_pstProtoSignIn, 0x00, sizeof(ST_PROTOCOL_SIGNIN));

		m_pstProtoSignIn->dwRequest			= dwRequest;
		m_pstProtoSignIn->strNickName		= JsonRoot.get("nickname", 0).asString();
		m_pstProtoSignIn->strEmail			= JsonRoot.get("email", 0).asString();
		m_pstProtoSignIn->strCallID			= JsonRoot.get("callid", 0).asString();
	}
	///////////// Sign Up //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_SIGNUP) {
		m_pstProtoSignUp = new ST_PROTOCOL_SIGNUP;
		::memset(m_pstProtoSignUp, 0x00, sizeof(ST_PROTOCOL_SIGNUP));

		m_pstProtoSignUp->dwRequest			= dwRequest;
		m_pstProtoSignUp->strNickName		= JsonRoot.get("nickname", 0).asString();
		m_pstProtoSignUp->strEmail			= JsonRoot.get("email", 0).asString();
		m_pstProtoSignUp->strCallID			= JsonRoot.get("callid", 0).asString();
	}
	///////////// Stores //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_STORES){		
		/*
			E_PROTO_REQ_DATA_MENUS is only message to get menu information
		*/
		return E_RET_SUCCESS;
	}
	///////////// Menus //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_MENUS) {
		m_pstProtoMenu = new ST_PROTOCOL_MENU;
		::memset(m_pstProtoMenu, 0x00, sizeof(ST_PROTOCOL_MENU));

		m_pstProtoMenu->strStoreIndex = JsonRoot.get("storeindex", 0).asString();
		return E_RET_SUCCESS;
	}
	///////////// Comments //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_COMMENTS) {
		m_pstProtoComment = new ST_PROTOCOL_COMMENT;
		::memset(m_pstProtoComment, 0x00, sizeof(ST_PROTOCOL_COMMENT));

		m_pstProtoComment->strStoreIndex = JsonRoot.get("storeindex", 0).asString();
		m_pstProtoComment->strMenuIndex = JsonRoot.get("menuindex", 0).asString();
		return E_RET_SUCCESS;
	}
	else if (dwRequest == E_PROTO_REQ_DATA_COMMENTS_INSERT) {
		m_pstProtoComment = new ST_PROTOCOL_COMMENT;
		::memset(m_pstProtoComment, 0x00, sizeof(ST_PROTOCOL_COMMENT));

		m_pstProtoComment->dwRequest			= dwRequest;
		m_pstProtoComment->strStoreIndex		= JsonRoot.get("storeindex", 0).asString();
		m_pstProtoComment->strMenuIndex			= JsonRoot.get("menuindex", 0).asString();
		m_pstProtoComment->strWriter			= JsonRoot.get("writer", 0).asString();
		m_pstProtoComment->strReputation		= JsonRoot.get("reputation", 0).asString();
		m_pstProtoComment->strText				= JsonRoot.get("text", 0).asString();
		m_pstProtoComment->strWriteTime			= JsonRoot.get("writetime", 0).asString();

		return E_RET_SUCCESS;
	}
	else if (dwRequest == E_PROTO_REQ_MENU_INSERT){
		m_pstProtoMenu = new ST_PROTOCOL_MENU;
		::memset(m_pstProtoMenu, 0x00, sizeof(ST_PROTOCOL_MENU));

		m_pstProtoMenu->dwRequest				= dwRequest;
		m_pstProtoMenu->dwMenuNumber			= JsonRoot["data"].get("menunumber", 0).asUInt();
		m_pstProtoMenu->strMenuName				= JsonRoot["data"].get("menuname", 0).asString();
		m_pstProtoMenu->dwMenuPrice				= JsonRoot["data"].get("menuprice", 0).asUInt();
		m_pstProtoMenu->strStoreName			= JsonRoot["data"].get("storename", 0).asString();

	}
	///////////// Client //////////////
	else if (dwRequest == E_PROTO_REQ_CLIENT_QUERY){
		/*
			Query inner data is nothing
		*/
		return E_RET_SUCCESS;
	}
	else if (dwRequest == E_PROTO_REQ_CLIENT_INSERT){
		m_pstProtoClient = new ST_PROTOCOL_CLIENT;
		::memset(m_pstProtoClient, 0x00, sizeof(ST_PROTOCOL_CLIENT));

		m_pstProtoClient->dwRequest				= dwRequest;
		m_pstProtoClient->strClientID			= JsonRoot["data"].get("clientid", 0).asUInt();
		m_pstProtoClient->strClientMail			= JsonRoot["data"].get("clientmail", 0).asUInt();
		m_pstProtoClient->strClientPhoneNumber	= JsonRoot["data"].get("clientphonenumber", 0).asString();
		m_pstProtoClient->dwClientNumber		= JsonRoot["data"].get("clientnumber", 0).asUInt();
	}
	///////////// Shop //////////////
	else if (dwRequest == E_PROTO_REQ_SHOP_QUERY){
		/*
			Query inner data is nothing
		*/
		return E_RET_SUCCESS;
	}
	else if (dwRequest == E_PROTO_REQ_SHOP_INSERT){
		m_pstProtoShop = new ST_PROTOCOL_SHOP;
		::memset(m_pstProtoShop, 0x00, sizeof(ST_PROTOCOL_SHOP));

		m_pstProtoShop->dwRequest			= dwRequest;
		m_pstProtoShop->strShopAlcohol		= JsonRoot["data"].get("shopalcohol", 0).asString();
		m_pstProtoShop->strShopCategory		= JsonRoot["data"].get("shopcategory", 0).asString();
		m_pstProtoShop->strShopETC			= JsonRoot["data"].get("shopetc", 0).asString();
		m_pstProtoShop->strShopFacility		= JsonRoot["data"].get("shopfacility", 0).asString();
		m_pstProtoShop->strShopHomepage		= JsonRoot["data"].get("shophomepage", 0).asString();
		m_pstProtoShop->strShopIntroduction = JsonRoot["data"].get("shopintroduction", 0).asString();
		m_pstProtoShop->strShopName			= JsonRoot["data"].get("shopname", 0).asString();
		m_pstProtoShop->dwShopNumber		= JsonRoot["data"].get("shopnumber", 0).asUInt();
		m_pstProtoShop->strShopParking		= JsonRoot["data"].get("shopparking", 0).asString();
		m_pstProtoShop->strShopPhoneNumber	= JsonRoot["data"].get("shopphonenumber", 0).asString();
		m_pstProtoShop->strShopRunningTime	= JsonRoot["data"].get("shoprunningtime", 0).asString();
		m_pstProtoShop->bShopSmoking		= JsonRoot["data"].get("shopsmoking", 0).asBool();
		m_pstProtoShop->strShopAddress		= JsonRoot["data"].get("shopaddress", 0).asString();
		m_pstProtoShop->strShopHoliday		= JsonRoot["data"].get("shopholiday", 0).asString();
	}
	else {
		ErrorLog("Invalid Request Code");
		return E_RET_FAIL;
	}

	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::RequestToDataBase(DWORD &refdwNumberOfRequest, ST_DB_RESULT &refstDBResult)
{
	HANDLE hDataBase = NULL;
	hDataBase = CreateDBInstance(E_DB_POSTGRES);
	if (hDataBase == NULL) {
		ErrorLog("Fail to create DB instance");
		return E_RET_FAIL;
	}

	DWORD dwRet;
	dwRet = ConnectToDB(hDataBase, m_stDBLoginToken);
	if (dwRet != E_RET_SUCCESS) {
		ErrorLog("Fail to connet to DB");
		return dwRet;
	}

	ST_DB_SQL stDBSQLQuery, stDBSQLInsert, stDBSQLUpdate;
	ST_DB_RESULT stDBResult;
	DWORD dwRequest = refdwNumberOfRequest;
	///////////// Sign In //////////////
	if (dwRequest == E_PROTO_REQ_DATA_SIGNIN) {
		stDBSQLQuery.strSQL = "SELECT clientmail, clientphonenumber, \"clientNickName\" FROM \"client\" WHERE clientmail = \'" + m_pstProtoSignIn->strEmail + "\'";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	///////////// Sign Up //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_SIGNUP) {
		stDBSQLQuery.strSQL = "SELECT clientmail, clientphonenumber, \"clientNickName\" FROM \"client\" WHERE clientmail = \'" + m_pstProtoSignUp->strEmail + "\'";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		
		if (stDBResult.vecstDBResultLines.size() < 1) {
			std::string strInputValue = "\'" + m_pstProtoSignUp->strNickName + "\', \'" + m_pstProtoSignUp->strEmail + "\', \'" + std::to_string(1) + "\', " + m_pstProtoSignUp->strCallID;
			stDBSQLInsert.strSQL = "INSERT INTO \"client\"(clientid, clientmail, clientphonenumber, \"clientNumber\") VALUES (" + strInputValue + ")";
			dwRet = InsertToDB(hDataBase, stDBSQLInsert);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to query data from DataBase");
				return E_RET_FAIL;
			}
		}
		refstDBResult = stDBResult;
	}
	///////////// Stores //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_STORES) {
		stDBSQLQuery.strSQL = "SELECT storename, reputation, index FROM \"stores\"";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	///////////// Menus //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_MENUS) {
		stDBSQLQuery.strSQL = "SELECT menuname, reputation, price, index FROM \"store_" + m_pstProtoMenu->strStoreIndex + "\"";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	///////////// Comments //////////////
	else if (dwRequest == E_PROTO_REQ_DATA_COMMENTS) {
		stDBSQLQuery.strSQL = "SELECT writer, reputation, text, writetime FROM \"store_" + m_pstProtoComment->strStoreIndex + "_" + m_pstProtoComment->strMenuIndex + "\"";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_DATA_COMMENTS_INSERT) {
		std::string strInputValue = "\'" + m_pstProtoComment->strWriter + "\', \'" + m_pstProtoComment->strReputation + "\', \'" + m_pstProtoComment->strText + "\', \'" + m_pstProtoComment->strWriteTime + "\'";
		stDBSQLInsert.strSQL = "INSERT INTO \"store_" + m_pstProtoComment->strStoreIndex + "_" + m_pstProtoComment->strMenuIndex + "\"(writer, reputation, text, writetime) VALUES (" + strInputValue + ")";
		dwRet = InsertToDB(hDataBase, stDBSQLInsert);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
	}
	//////////////////////////
	else if (dwRequest == E_PROTO_REQ_MENU_INSERT) {
		stDBSQLQuery.strSQL = "SELECT menuname FROM \"menu\" WHERE storename = \'" + m_pstProtoMenu->strMenuName + "\'";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		/*
			if line is 1, you have to update data in database
			otherwise, data to get data from client is new value
		*/
		if (stDBResult.vecstDBResultLines.size() < 1) {
			std::string strInputValue = std::to_string(m_pstProtoMenu->dwMenuNumber) + ", \'" + m_pstProtoMenu->strMenuName + "\', " + std::to_string(m_pstProtoMenu->dwMenuPrice) + ", \'" + m_pstProtoMenu->strStoreName + "\'";
			stDBSQLInsert.strSQL = "INSERT INTO \"menu\"(menunumber, menuname, menuprice, storename) VALUES (" + strInputValue + ")";
			dwRet = InsertToDB(hDataBase, stDBSQLInsert);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to query data from DataBase");
				return E_RET_FAIL;
			}
		}
		else {
			stDBSQLUpdate.strSQL = "";
			dwRet = UpdateToDB(hDataBase, stDBSQLInsert);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to query data from DataBase");
				return E_RET_FAIL;
			}
		}

		refstDBResult = stDBResult;
	}
	///////////// Client //////////////
	else if (dwRequest == E_PROTO_REQ_CLIENT_QUERY){
		stDBSQLQuery.strSQL = "SELECT * FROM \"client\"";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_CLIENT_INSERT){
		stDBSQLQuery.strSQL = "SELECT clientid FROM \"client\" WHERE storename = \'" + m_pstProtoClient->strClientID + "\'";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		/*
			if line is 1, you have to update data in database
			otherwise, data to get data from client is new value
		*/
		if (stDBResult.vecstDBResultLines.size() < 1) {
			std::string strInputValue = "\'" + m_pstProtoClient->strClientID + "\', \'" + m_pstProtoClient->strClientMail + "\', \'" + std::to_string(m_pstProtoClient->dwClientNumber) + "\', " + std::to_string(m_pstProtoClient->dwClientNumber);
			stDBSQLInsert.strSQL = "INSERT INTO \"client\"(clientid, clientmail, clientphonenumber, \"clientNumber\") VALUES (" + strInputValue + ")";
			dwRet = InsertToDB(hDataBase, stDBSQLInsert);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to query data from DataBase");
				return E_RET_FAIL;
			}
		}
		else {
			stDBSQLUpdate.strSQL = "";
			dwRet = UpdateToDB(hDataBase, stDBSQLInsert);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to query data from DataBase");
				return E_RET_FAIL;
			}
		}

		refstDBResult = stDBResult;
	}
	///////////// Shop //////////////
	else if (dwRequest == E_PROTO_REQ_SHOP_QUERY){
		stDBSQLQuery.strSQL = "SELECT * FROM \"shop\"";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_SHOP_INSERT){
		stDBSQLQuery.strSQL = "SELECT shopname FROM \"shop\" WHERE shopname = \'" + m_pstProtoShop->strShopName + "\'";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		/*
			if line is 1, you have to update data in database
			otherwise, data to get data from client is new value
		*/
		if (stDBResult.vecstDBResultLines.size() < 1) {
			std::string strInputValue = std::to_string(m_pstProtoShop->dwShopNumber) + ", \'" + m_pstProtoShop->strShopName + "\', \'" + m_pstProtoShop->strShopAddress + "\', \'" + m_pstProtoShop->strShopPhoneNumber + "\', \'" + m_pstProtoShop->strShopCategory + "\', \'" + m_pstProtoShop->strShopHomepage + "\', \'" + m_pstProtoShop->strShopETC + "\', " + std::to_string(m_pstProtoShop->bShopSmoking) + ", \'" + m_pstProtoShop->strShopParking + "\', \'" + m_pstProtoShop->strShopAlcohol + "\', \'" + m_pstProtoShop->strShopFacility + "\', \'" + m_pstProtoShop->strShopIntroduction + "\'";
			stDBSQLInsert.strSQL = "INSERT INTO \"shop\"(shopnumber, shopname, shopaddress, shopphonenumber, shopcategory, shophomepage, shoprunningtime, shopholiday, shopetc, shopsmoking, shopparking, shopalcohol, shopfacility, shopintroduction) VALUES (" + strInputValue + ")";
			dwRet = InsertToDB(hDataBase, stDBSQLInsert);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to query data from DataBase");
				return E_RET_FAIL;
			}
		}
		else {
			stDBSQLUpdate.strSQL = "";
			dwRet = UpdateToDB(hDataBase, stDBSQLInsert);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to query data from DataBase");
				return E_RET_FAIL;
			}
		}

		refstDBResult = stDBResult;
	}
	else {
		ErrorLog("Invalid Request Code");
		return E_RET_FAIL;
	}
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket(DWORD &refdwNumberOfRequest, ST_DB_RESULT &refstDBResult, DWORD &refdwResponse, std::string &refstrSendData)
{
	Json::Value JsonRoot;
	std::string strSendData;
	/*if (refstDBResult.vecstDBResultLines.size() < 1) {
		DebugLog("DB return data is nothing to store");

		JsonRoot["response"] = E_PROTO_RES_NOTHING;
		JsonRoot["size"] = 0;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
		return E_RET_SUCCESS;
	}*/
	
	///////////// Sign In //////////////
	if (refdwNumberOfRequest == E_PROTO_REQ_DATA_SIGNIN) {
		CSignIn *pSignIn = NULL;
		pSignIn = new CSignIn();
		if (pSignIn == NULL) {
			return E_RET_FAIL;
		}
		
		DWORD dwRet;
		dwRet = pSignIn->CheckSignInData(m_pstProtoSignIn, refstDBResult);
		if (dwRet == E_RET_SUCCESS) {
			refdwResponse = E_PROTO_RES_SUCCESS;
		}
		else if (dwRet == E_RET_FAIL) {
			refdwResponse = E_PROTO_RES_DATA_NOT_EXIST;
		}
		else {
			refdwResponse = (int)dwRet;
		}
		refstrSendData = "";
	}
	///////////// Sign Up //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_DATA_SIGNUP) {
		if (refstDBResult.vecstDBResultLines.size() > 0) {
			refdwResponse = E_PROTO_RES_DATA_ACCOUNT_EXIST;
		}
		else {
			refdwResponse = E_PROTO_RES_SUCCESS;
		}
		refstrSendData = "";
	}
	///////////// Stores //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_DATA_STORES) {
		refstrSendData = "";
		if (refstDBResult.vecstDBResultLines.size() < 1) {
			refdwResponse = E_PROTO_RES_DATA_STORES_NOT_EXIST;
			return E_RET_SUCCESS;
		}
		/*
			Response data is following..
			1. std::string	strStoreName;
			2. std::string	strReputation;
			3. std::string  strIndex
		*/
		DWORD dwLineCount;
		Json::Value JsonData;
		for (dwLineCount = 0; dwLineCount < refstDBResult.vecstDBResultLines.size(); dwLineCount++) {
			ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[dwLineCount];
			if (stDBResultLine.vecstrResult.size() != 3) {
				/*
					DB Result Query have 3 argument
				*/
				refdwResponse = E_PROTO_RES_QUERY_FAIL;
				break;
			}

			DWORD dwParamCount;
			for (dwParamCount = 0; dwParamCount < stDBResultLine.vecstrResult.size(); dwParamCount++) {
				switch (dwParamCount)
				{
				case 0:
					JsonData["storename"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 1:
					JsonData["reputation"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 2:
					JsonData["index"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				default:
					break;
				}
			}
			JsonRoot["data"].append(JsonData);
		}

		JsonRoot["count"] = (int)dwLineCount;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
		refdwResponse = E_PROTO_RES_SUCCESS;
		return E_RET_SUCCESS;
	}
	///////////// Menus //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_DATA_MENUS) {
		refstrSendData = "";
		if (refstDBResult.vecstDBResultLines.size() < 1) {
			refdwResponse = E_PROTO_RES_DATA_MENUS_NOT_EXIST;
			return E_RET_SUCCESS;
		}
		/*
			Response data is following..
			1. std::string	strMenuName;
			2. std::string	strReputation;
			3. std::string	strPrice;
			4. std::string	strIndex;
		*/
		DWORD dwLineCount;
		Json::Value JsonData;
		for (dwLineCount = 0; dwLineCount < refstDBResult.vecstDBResultLines.size(); dwLineCount++) {
			ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[dwLineCount];
			if (stDBResultLine.vecstrResult.size() != 4) {
				/*
					DB Result Query have 4 argument
				*/
				refdwResponse = E_PROTO_RES_QUERY_FAIL;
				break;
			}

			DWORD dwParamCount;
			for (dwParamCount = 0; dwParamCount < stDBResultLine.vecstrResult.size(); dwParamCount++) {
				switch (dwParamCount)
				{
				case 0:
					JsonData["menuname"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 1:
					JsonData["reputation"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 2:
					JsonData["price"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 3:
					JsonData["index"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				default:
					break;
				}
			}
			JsonRoot["data"].append(JsonData);
		}

		JsonRoot["count"] = (int)dwLineCount;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
		refdwResponse = E_PROTO_RES_SUCCESS;
		return E_RET_SUCCESS;
	}
	///////////// Comments //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_DATA_COMMENTS) {
		refstrSendData = "";
		if (refstDBResult.vecstDBResultLines.size() < 1) {
			refdwResponse = E_PROTO_RES_DATA_MENUS_NOT_EXIST;
			return E_RET_SUCCESS;
		}
		/*
		Response data is following..
		1. std::string	strWriter;
		2. std::string	strReputation;
		3. std::string  strText
		4. std::string  strWritetime
		*/
		DWORD dwLineCount;
		Json::Value JsonData;
		for (dwLineCount = 0; dwLineCount < refstDBResult.vecstDBResultLines.size(); dwLineCount++) {
			ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[dwLineCount];
			if (stDBResultLine.vecstrResult.size() != 4) {
				/*
					DB Result Query have 4 argument
				*/
				refdwResponse = E_PROTO_RES_QUERY_FAIL;
				break;
			}

			DWORD dwParamCount;
			for (dwParamCount = 0; dwParamCount < stDBResultLine.vecstrResult.size(); dwParamCount++) {
				switch (dwParamCount)
				{
				case 0:
					JsonData["writer"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 1:
					JsonData["reputation"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 2:
					JsonData["text"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 3:
					JsonData["writetime"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				default:
					break;
				}
			}
			JsonRoot["data"].append(JsonData);
		}

		JsonRoot["count"] = (int)dwLineCount;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
		refdwResponse = E_PROTO_RES_SUCCESS;
		return E_RET_SUCCESS;
	}
	else if (refdwNumberOfRequest == E_PROTO_REQ_DATA_COMMENTS_INSERT) {
		/*
			Insert request have to return 'ok' or 'no'
		*/
		refdwResponse = E_PROTO_RES_SUCCESS;
		return E_RET_SUCCESS;
	}
	///////////// Menu //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_MENU_QUERY) {
		/*
			Return data is following..
			1. DWORD		dwMenuNumber;
			2. std::string	strMenuName;
			3. DWORD		dwMenuPrice;
			4. std::string	strStoreName;
		*/
		JsonRoot["response"]	= E_PROTO_RES_QUERY_SUCESS;
		JsonRoot["size"]		= 0;

		DWORD dwLineCount;
		Json::Value JsonData;
		for (dwLineCount = 0; dwLineCount < refstDBResult.vecstDBResultLines.size(); dwLineCount++) {
			ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[dwLineCount];
			if (stDBResultLine.vecstrResult.size() != 4) {
				/*
					DB Result Query have 4 argument
				*/
				JsonRoot["response"] = E_PROTO_RES_QUERY_FAIL;
				break;
			}
			DWORD dwParamCount;
			for (dwParamCount = 0; dwParamCount < stDBResultLine.vecstrResult.size(); dwParamCount++) {
				switch (dwParamCount)
				{
				case 0:
					JsonData["menunumber"] = ::atoi(stDBResultLine.vecstrResult[dwParamCount].c_str());
					break;
				case 1:
					JsonData["menuname"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 2:
					JsonData["menuprice"] = ::atoi(stDBResultLine.vecstrResult[dwParamCount].c_str());
					break;
				case 3:
					JsonData["storename"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				default:
					break;
				}
			}
			JsonRoot["data"].append(JsonData);
		}

		JsonRoot["size"] = (int)dwLineCount;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	else if (refdwNumberOfRequest == E_PROTO_REQ_MENU_INSERT) {
		JsonRoot["response"] = E_PROTO_RES_INSERT_SUCCESS;
		JsonRoot["size"] = 0;

		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	///////////// Client //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_CLIENT_QUERY) {
		/*
			Return data is following..
			1. std::string		strClientID;
			2. std::string		strClientMail;
			3. std::string		strClientPhoneNumber;
			4. DWORD			dwClientNumber;
		*/
		JsonRoot["response"]	= E_PROTO_RES_QUERY_SUCESS;
		JsonRoot["size"]		= 0;

		DWORD dwLineCount;
		Json::Value JsonData;
		for (dwLineCount = 0; dwLineCount < refstDBResult.vecstDBResultLines.size(); dwLineCount++) {
			ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[dwLineCount];
			if (stDBResultLine.vecstrResult.size() != 4) {
				/*
					DB Result Query have 4 argument
				*/
				JsonRoot["response"] = E_PROTO_RES_QUERY_FAIL;
				break;
			}
			DWORD dwParamCount;
			for (dwParamCount = 0; dwParamCount < stDBResultLine.vecstrResult.size(); dwParamCount++) {
				switch (dwParamCount)
				{
				case 0:
					JsonData["clientid"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 1:
					JsonData["clientmail"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 2:
					JsonData["clientphonenumber"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 3:
					JsonData["clientNumber"] = ::atoi(stDBResultLine.vecstrResult[dwParamCount].c_str());
					break;
				default:
					break;
				}
			}
			JsonRoot["data"].append(JsonData);
		}

		JsonRoot["size"] = (int)dwLineCount;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	else if (refdwNumberOfRequest == E_PROTO_REQ_CLIENT_INSERT) {
		JsonRoot["response"] = E_PROTO_RES_INSERT_SUCCESS;
		JsonRoot["size"] = 0;

		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	///////////// Comment //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_COMMNET_QUERY) {
		/*
			Return data is following..
			1. DWORD			dwCommentNumber
			2. std::string		strClientId
			3. std::string		strCommentTime
			4. std::string		strCommentContent
			5. DWORD			dwStoreNumber
		*/
		JsonRoot["response"]	= E_PROTO_RES_QUERY_SUCESS;
		JsonRoot["size"]		= 0;

		DWORD dwLineCount;
		Json::Value JsonData;
		for (dwLineCount = 0; dwLineCount < refstDBResult.vecstDBResultLines.size(); dwLineCount++) {
			ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[dwLineCount];
			if (stDBResultLine.vecstrResult.size() != 4) {
				/*
					DB Result Query have 5 argument
				*/
				JsonRoot["response"] = E_PROTO_RES_QUERY_FAIL;
				break;
			}
			DWORD dwParamCount;
			for (dwParamCount = 0; dwParamCount < stDBResultLine.vecstrResult.size(); dwParamCount++) {
				switch (dwParamCount)
				{
				case 0:
					JsonData["commentnumber"] = ::atoi(stDBResultLine.vecstrResult[dwParamCount].c_str());
					break;
				case 1:
					JsonData["clientid"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 2:
					JsonData["commenttime"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 3:
					JsonData["commentcontent"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 4:
					JsonData["storenumber"] = ::atoi(stDBResultLine.vecstrResult[dwParamCount].c_str());
					break;
				default:
					break;
				}
			}
			JsonRoot["data"].append(JsonData);
		}

		JsonRoot["size"] = (int)dwLineCount;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	else if (refdwNumberOfRequest == E_PROTO_REQ_COMMNET_INSERT) {
		JsonRoot["response"] = E_PROTO_RES_INSERT_SUCCESS;
		JsonRoot["size"] = 0;

		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	///////////// Shop //////////////
	else if (refdwNumberOfRequest == E_PROTO_REQ_SHOP_QUERY) {
		/*
			Return data is following..
			1. DWORD			dwShopNumber;
			2. std::string		strShopName;
			3. std::string		strShopAddress;
			4. std::string		strShopPhoneNumber;
			5. std::string		strShopCategory;
			6. std::string		strShopHomepage;
			7. std::string		strShopRunningTime;
			8. std::string		strShopHoliday;
			9. std::string		strShopETC;
			10. BOOL			bShopSmoking;
			11. std::string		strShopParking;
			12. std::string		strShopAlcohol;
			13. std::string		strShopFacility;
			14. std::string		strShopIntroduction;
		*/
		JsonRoot["response"] = E_PROTO_RES_QUERY_SUCESS;
		JsonRoot["size"] = 0;

		DWORD dwLineCount;
		Json::Value JsonData;
		for (dwLineCount = 0; dwLineCount < refstDBResult.vecstDBResultLines.size(); dwLineCount++) {
			ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[dwLineCount];
			if (stDBResultLine.vecstrResult.size() != 14) {
				/*
				DB Result Query have 5 argument
				*/
				JsonRoot["response"] = E_PROTO_RES_QUERY_FAIL;
				break;
			}
			DWORD dwParamCount;
			for (dwParamCount = 0; dwParamCount < stDBResultLine.vecstrResult.size(); dwParamCount++) {
				switch (dwParamCount)
				{
				case 0:
					JsonData["shopnumber"] = ::atoi(stDBResultLine.vecstrResult[dwParamCount].c_str());
					break;
				case 1:
					JsonData["shopname"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 2:
					JsonData["shopaddress"] = stDBResultLine.vecstrResult[dwParamCount];
					break;
				case 3:
					JsonData["shopphonenumber"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 4:
					JsonData["shopcategory"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 5:
					JsonData["shophomepage"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 6:
					JsonData["shoprunningtime"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 7:
					JsonData["shopholiday"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 8:
					JsonData["shopetc"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 9:
					JsonData["shopsmoking"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 10:
					JsonData["shopparking"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 11:
					JsonData["shopalcohol"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 12:
					JsonData["shopfacility"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 13:
					JsonData["shopintroduction"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				case 14:
					JsonData["storenumber"] = stDBResultLine.vecstrResult[dwParamCount].c_str();
					break;
				default:
					break;
				}
			}
			JsonRoot["data"].append(JsonData);
		}

		JsonRoot["size"] = (int)dwLineCount;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	else if (refdwNumberOfRequest == E_PROTO_REQ_SHOP_INSERT) {
		JsonRoot["response"] = E_PROTO_RES_INSERT_SUCCESS;
		JsonRoot["size"] = 0;

		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
	}
	else {
		return E_RET_FAIL;
	}
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::SendDataToClient(std::string &refstrSendData)
{
	int nSizeOfData = refstrSendData.size();
	int nSent = 0, nRet;
	BOOL bContinue = TRUE;
	while (bContinue) {
		nRet = send(m_stWorkerThread.hClientSocket, refstrSendData.c_str() + nSent, nSizeOfData - nSent, 0);
		if (nSent == SOCKET_ERROR) {
			int nRet = WSAGetLastError();
			ShowErrorSend(nRet);
			bContinue = FALSE;
			continue;
		}
		else if (nRet == nSizeOfData) {
			DebugLog("Success to send data to client [%s]", refstrSendData.c_str());
			bContinue = FALSE;
			continue;
		}
		
		/*
			if all data is not sent to client yet, continue to send to rest of data
		*/
		nSent = nRet;
	}
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::SendResponseToClient(DWORD &refdwNumberOfRequest, DWORD &refdwResponse, std::string &refstrSendData)
{
	DWORD dwRet;
	DWORD dwSizeOfData = refstrSendData.size();

	/*
		Send Header Msg To Client
	*/
	dwRet = SendResponseHeaderToClient(refdwNumberOfRequest, refdwResponse, dwSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	/*
		Send Data Msg To Client
	*/
	if (dwSizeOfData > 0)  {
		// Waiting for 1 second
		::Sleep(1000);
		dwRet = SendResponseDataToClient(refdwNumberOfRequest, refstrSendData);
		if (dwRet != E_RET_SUCCESS) {
			return dwRet;
		}
	}
	
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::SendResponseHeaderToClient(DWORD &refdwNumberOfRequest, DWORD &refdwResponse, DWORD &refstSizeOfData)
{
	Json::Value JsonRoot;
	std::string strSendData;

	JsonRoot["response"] = (int)refdwResponse;
	JsonRoot["size"] = (int)refstSizeOfData;

	Json::StyledWriter JsonWriter;
	strSendData = JsonWriter.write(JsonRoot);

	DWORD dwRet;
	dwRet = SendDataToClient(strSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::SendResponseDataToClient(DWORD &refdwNumberOfRequest, std::string &refstrSendData)
{
	DWORD dwRet;
	dwRet = SendDataToClient(refstrSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::StartWorkerThread(char *pReceiveBuf, DWORD dwByteTransferred)
{
	DWORD dwRet;
	try
	{
		/*
			Mobile <----> Server
			1. Mobile ---> Server (Mobile send Header msg)
			2. Mobile <--- Server (Server send response msg about Header)
			3. Mobile ---> Server (Mobile send Data Msg)
			4. Mobile <--- Server (Server send response header msg)
			5. Mobile <--- Server (Server send response data msg)		
		*/

		/*
			Step 1.
		*/
		// Receive Header from client
		ST_RECV_HEADER_DATA stRecvHeaderData;
		dwRet = ReceiveDataFromClient(pReceiveBuf, dwByteTransferred, stRecvHeaderData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate ReceiveDataFromClient");
		}

		// Parse Header Data By JSON
		dwRet = ParseReceivedHeaderByJSON(stRecvHeaderData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate ParseReceivedHeaderByJSON");
		}

		/*
			Step 2.
		*/
		dwRet = SendHeaderToClient(stRecvHeaderData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate SendHeaderToClient");
		}

		/*
			Step 3.
			In sequence of step 3, server have to communicate with database.
			so, server take some second to get data that client want.
			After finishin operation of server, it send data twice (response header, response data)
			- response header is included with size of response data
		*/
		ST_RECV_DATA stRecvData;
		dwRet = ReceiveDataFromClient(stRecvHeaderData, stRecvData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate ReceiveDataFromClient");
		}

		DWORD dwNumberOfRequest;
		dwRet = ParseDataByJSON(dwNumberOfRequest, stRecvData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate ParseDataByJSON");
		}

		ST_DB_RESULT stDBResult;
		dwRet = RequestToDataBase(dwNumberOfRequest, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate RequestToDataBase");
		}
	
		std::string strSendData;
		DWORD dwResponse;
		dwRet = MakeSendPacket(dwNumberOfRequest, stDBResult, dwResponse, strSendData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate MakeSendPacket");
		}
		/*
			Step 4, 5
		*/
		dwRet = SendResponseToClient(dwNumberOfRequest, dwResponse, strSendData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate SendResponseToClient");
		}
	}
	catch (std::exception &e)
	{
		ErrorLog(e.what());
		if (m_pstProtoSignIn)
			delete m_pstProtoSignIn;
		if (m_pstProtoSignUp)
			delete m_pstProtoSignUp;
		if (m_pstProtoMenu) 
			delete m_pstProtoMenu;
		if (m_pstProtoClient)
			delete m_pstProtoClient;
		if (m_pstProtoComment)
			delete m_pstProtoComment;
		if (m_pstProtoShop)
			delete m_pstProtoShop;
		return E_RET_FAIL;
	}

	if (m_pstProtoSignIn)
		delete m_pstProtoSignIn;
	if (m_pstProtoSignUp)
		delete m_pstProtoSignUp;
	if (m_pstProtoMenu)
		delete m_pstProtoMenu;
	if (m_pstProtoClient)
		delete m_pstProtoClient;
	if (m_pstProtoComment)
		delete m_pstProtoComment;
	if (m_pstProtoShop)
		delete m_pstProtoShop;
	return E_RET_SUCCESS;
} 