#include "AYWorkerThread.h"
#include "AYProtocol.h"
#include "AYServerError.h"

#include "..\Common\Log.h"
#include "json\reader.h"
#include "json\json.h"

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
DWORD CAYWorkerThread::ReceiveDataFromClient(char *pReceiveBuf)
{
	m_stWorkerThread.strReceivedData = pReceiveBuf;
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ParseDataByJSON(DWORD &refdwNumberOfRequestvoid)
{
	Json::Value JsonRoot;
	Json::Reader reader;
	bool bParsingRet = reader.parse(m_stWorkerThread.strReceivedData, JsonRoot);
	if (!bParsingRet) {
		ErrorLog("Fail to parse a received data");
		return E_RET_FAIL;
	}

	DWORD dwRequest = JsonRoot.get("request", 0).asUInt();
	DWORD dwSizeOfReceivedData = JsonRoot.get("size", 0).asUInt();
	refdwNumberOfRequestvoid = dwRequest;

	/*
		you have to cast pointer to use pointer flexibly
	*/
	///////////// Menu //////////////
	if (dwRequest == E_PROTO_REQ_MENU_QUERY){		
		/*
			Query inner data is nothing
		*/
		return E_RET_SUCCESS;
	}
	if (dwRequest == E_PROTO_REQ_MENU_INSERT){
		m_pstProtoMenu = new ST_PROTOCOL_MENU;
		::memset(m_pstProtoMenu, 0x00, sizeof(ST_PROTOCOL_MENU));

		m_pstProtoMenu->dwRequest				= dwRequest;
		m_pstProtoMenu->dwSizeOfData			= dwSizeOfReceivedData;
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
		m_pstProtoClient->dwSizeOfData			= dwSizeOfReceivedData;
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
		m_pstProtoShop->dwSizeOfData		= dwSizeOfReceivedData;
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
	///////////// Comment //////////////
	else if (dwRequest == E_PROTO_REQ_COMMNET_QUERY){
		/*
			Query inner data is nothing
		*/
		return E_RET_SUCCESS;
	}
	else if (dwRequest == E_PROTO_REQ_COMMNET_INSERT){
		m_pstProtoComment= new ST_PROTOCOL_COMMENT;
		::memset(m_pstProtoComment, 0x00, sizeof(ST_PROTOCOL_COMMENT));

		m_pstProtoComment->dwRequest			= dwRequest;
		m_pstProtoComment->dwSizeOfData			= dwSizeOfReceivedData;
		m_pstProtoComment->dwCommentNumber		= JsonRoot["data"].get("commentnumber", 0).asUInt();
		m_pstProtoComment->strClientID			= JsonRoot["data"].get("clientid", 0).asUInt();
		m_pstProtoComment->strCommentTime		= JsonRoot["data"].get("commenttime", 0).asString();
		m_pstProtoComment->strCommentContent	= JsonRoot["data"].get("commentcontent", 0).asString();
		m_pstProtoComment->dwStoreNumber		= JsonRoot["data"].get("storenumber", 0).asUInt();
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
	///////////// Menu //////////////
	if (dwRequest == E_PROTO_REQ_MENU_QUERY) {
		stDBSQLQuery.strSQL = "SELECT * FROM \"menu\"";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
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
	///////////// Comment //////////////
	else if (dwRequest == E_PROTO_REQ_COMMNET_QUERY){
		stDBSQLQuery.strSQL = "SELECT * FROM \"comment\"";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_COMMNET_INSERT){
		stDBSQLQuery.strSQL = "SELECT clientid FROM \"comment\" WHERE storename = \'" + m_pstProtoComment->strClientID + "\'";
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
			std::string strInputValue = std::to_string(m_pstProtoComment->dwCommentNumber) + ", \'" + m_pstProtoComment->strClientID + "\', \'" + m_pstProtoComment->strCommentTime + "\', \'" + m_pstProtoComment->strCommentContent + "\', " + std::to_string(m_pstProtoComment->dwStoreNumber);
			stDBSQLInsert.strSQL = "INSERT INTO \"comment\"(commentnumber, clientid, commenttime, commentcontent, storenumber) VALUES (" + strInputValue + ")";
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
DWORD CAYWorkerThread::MakeSendPacket(DWORD &refdwNumberOfRequest, ST_DB_RESULT &refstDBResult, std::string &refstrSendData)
{
	Json::Value JsonRoot;
	std::string strSendData;
	if (refstDBResult.vecstDBResultLines.size() < 1) {
		DebugLog("DB return data is nothing to store");

		JsonRoot["response"] = E_PROTO_RES_NOTHING;
		JsonRoot["size"] = 0;
		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);
		refstrSendData = strSendData;
		return E_RET_SUCCESS;
	}
	
	///////////// Menu //////////////
	if (refdwNumberOfRequest == E_PROTO_REQ_MENU_QUERY) {
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
DWORD CAYWorkerThread::StartWorkerThread(char *pReceiveBuf)
{
	DWORD dwRet;
	try
	{
		dwRet = ReceiveDataFromClient(pReceiveBuf);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate ReceiveDataFromClient");
		}

		DWORD dwNumberOfRequest;
		dwRet = ParseDataByJSON(dwNumberOfRequest);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate ParseDataByJSON");
		}

		ST_DB_RESULT stDBResult;
		dwRet = RequestToDataBase(dwNumberOfRequest, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate RequestToDataBase");
		}
	
		std::string strSendData;
		dwRet = MakeSendPacket(dwNumberOfRequest, stDBResult, strSendData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate MakeSendPacket");
		}

		dwRet = SendDataToClient(strSendData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to operate SendDataToClient");
		}
	}
	catch (std::exception &e)
	{
		ErrorLog(e.what());
		if (m_pstProtoMenu) {
			delete m_pstProtoMenu;
		}
		if (m_pstProtoClient) {
			delete m_pstProtoClient;
		}
		if (m_pstProtoComment) {
			delete m_pstProtoComment;
		}
		if (m_pstProtoShop) {
			delete m_pstProtoShop;
		}
		return E_RET_FAIL;
	}

	if (m_pstProtoMenu) {
		delete m_pstProtoMenu;
	}
	if (m_pstProtoClient) {
		delete m_pstProtoClient;
	}
	if (m_pstProtoComment) {
		delete m_pstProtoComment;
	}
	if (m_pstProtoShop) {
		delete m_pstProtoShop;
	}
	return E_RET_SUCCESS;
} 