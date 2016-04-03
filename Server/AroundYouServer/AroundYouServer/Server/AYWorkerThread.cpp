#include "AYWorkerThread.h"
#include "AYProtocol.h"
#include "AYServerError.h"

#include "..\Common\Log.h"
#include "json\reader.h"
#include "json\json.h"

#include "Store.h"
#include "Menu.h"
#include "Comment.h"

#define DEFAULT_RECV_DATA 32

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
VOID CAYWorkerThread::ConvertReceivedDataToString(char *pReceiveBuf, DWORD dwByteTransferred, std::string &refstrReceivedData)
{
	refstrReceivedData = pReceiveBuf;
	refstrReceivedData.resize(dwByteTransferred);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ParseSimpleReceivedDataByJSON(DWORD dwSentResProtocolNumber, std::string &refstrRecvData)
{
	Json::Value JsonRoot;
	Json::Reader reader;
	bool bParsingRet = reader.parse(refstrRecvData, JsonRoot);
	if (!bParsingRet) {
		ErrorLog("Fail to parse a received data [%s]", reader.getFormatedErrorMessages());
		std::cout << reader.getFormatedErrorMessages() << std::endl;
		return E_RET_FAIL;
	}

	DWORD dwRet = E_RET_SUCCESS;
	DWORD dwProtocolNumber = JsonRoot.get("req", 0).asInt();
	if (dwProtocolNumber != E_PROTO_REQ_READY_FOR_DATA) {
		dwRet = E_RET_FAIL;
	}
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ParseReceivedDataByJSON(const std::string &refstrReceivedData, ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	Json::Value JsonRoot;
	Json::Reader reader;
	bool bParsingRet = reader.parse(refstrReceivedData, JsonRoot);
	if (!bParsingRet) {
		ErrorLog("Fail to parse a received data [%s]", reader.getFormatedErrorMessages());
		std::cout << reader.getFormatedErrorMessages() << std::endl;
		return E_RET_FAIL;
	}

	refstRecvHeaderData.dwReqNumber			= JsonRoot.get("req", 0).asInt();
	refstRecvHeaderData.dwNextReqNumber		= JsonRoot.get("next_req", 0).asInt();
	refstRecvHeaderData.dwNextSizeOfData	= JsonRoot.get("next_size", 0).asInt();
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::CheckFirstRequestNumber(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	if (refstRecvHeaderData.dwReqNumber != E_PROTO_REQ_CONNECTION_START) {
		return E_RET_FAIL;
	}

	return E_RET_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::BranchByRequestNumber(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwRet = E_RET_FAIL;
	DWORD dwNextReqNumber = refstRecvHeaderData.dwNextReqNumber;
	switch (dwNextReqNumber)
	{
	case E_PROTO_REQ_USER_LOG_IN:
	case E_PROTO_REQ_USER_SIGN_IN:
	case E_PROTO_REQ_USER_SIGN_UP:
		dwRet = PROTO_101_ABOUT_USER(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_RECOMMENDED_STORE:
		dwRet = PROTO_201_RECOMMENTED_STORE(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_RECOMMENDED_MENU:
		dwRet = PROTO_202_RECOMMENTED_MENU(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_SELECTED_STORE:
		dwRet = PROTO_301_SELECTED_STORE(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_SELECTED_MENU:
		dwRet = PROTO_302_SELECTED_MENU(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_INPUT_EVALUATION:
		dwRet = PROTO_401_INPUT_EVALUATION(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_INPUT_COMMENT:
		dwRet = PROTO_501_INPUT_COMMENT(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_SHOW_COMMENT_LIST:
		dwRet = PROTO_502_SHOW_COMMENT_LIST(refstRecvHeaderData);
		break;
	default:
		break;
	}
	return dwRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::ParseDataByJSON(VOID *pProto, ST_RECV_DATA &refstRecvData)
{
	Json::Value JsonRoot;
	Json::Reader reader;
	bool bParsingRet = reader.parse(refstRecvData.strReceivedData, JsonRoot);
	if (!bParsingRet) {
		ErrorLog("Fail to parse a received data [%s]", reader.getFormatedErrorMessages());
		std::cout << reader.getFormatedErrorMessages() << std::endl;
		return E_RET_FAIL;
	}

	DWORD dwRequest = JsonRoot.get("req", 0).asInt();

	///////////// User //////////////
	if (dwRequest == E_PROTO_REQ_USER_LOG_IN || dwRequest == E_PROTO_REQ_USER_SIGN_IN || dwRequest == E_PROTO_REQ_USER_SIGN_UP) {
		((ST_PROTOCOL_REQ_101_ABOUT_USER *)pProto)->dwRequest = dwRequest;
		((ST_PROTOCOL_REQ_101_ABOUT_USER *)pProto)->strCallID = JsonRoot["data"].get("call_id", 0).asString();
	}
	else if (dwRequest == E_PROTO_REQ_RECOMMENDED_STORE) {
		((ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *)pProto)->dwRequest					= dwRequest;
		((ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *)pProto)->dwLocation					= JsonRoot["data"].get("location", 0).asInt();
		((ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *)pProto)->dwNumberOfRequestedMaximun	= JsonRoot["data"].get("res_number", 0).asInt();
	}
	else if (dwRequest == E_PROTO_REQ_RECOMMENDED_MENU) {
		((ST_PROTOCOL_REQ_202_RECOMMENDED_MENU *)pProto)->dwRequest						= dwRequest;
		((ST_PROTOCOL_REQ_202_RECOMMENDED_MENU *)pProto)->dwLocation					= JsonRoot["data"].get("location", 0).asInt();
		((ST_PROTOCOL_REQ_202_RECOMMENDED_MENU *)pProto)->dwNumberOfRequestedMaximun	= JsonRoot["data"].get("res_number", 0).asInt();
	}
	else if (dwRequest == E_PROTO_REQ_SELECTED_STORE) {
		((ST_PROTOCOL_REQ_301_SELECTED_STORE *)pProto)->dwRequest						= dwRequest;
		((ST_PROTOCOL_REQ_301_SELECTED_STORE *)pProto)->dwStoreID						= JsonRoot["data"].get("store_id", 0).asInt();
		((ST_PROTOCOL_REQ_301_SELECTED_STORE *)pProto)->dwLocation						= JsonRoot["data"].get("location", 0).asInt();
		((ST_PROTOCOL_REQ_301_SELECTED_STORE *)pProto)->strCallID						= JsonRoot["data"].get("call_id", 0).asString();
	}
	else if (dwRequest == E_PROTO_REQ_SELECTED_MENU) {
		((ST_PROTOCOL_REQ_302_SELECTED_MENU *)pProto)->dwRequest						= dwRequest;
		((ST_PROTOCOL_REQ_302_SELECTED_MENU *)pProto)->dwStoreID						= JsonRoot["data"].get("store_id", 0).asInt();
		((ST_PROTOCOL_REQ_302_SELECTED_MENU *)pProto)->dwLocation						= JsonRoot["data"].get("location", 0).asInt();
		((ST_PROTOCOL_REQ_302_SELECTED_MENU *)pProto)->strCallID						= JsonRoot["data"].get("call_id", 0).asString();
	}
	else if (dwRequest == E_PROTO_REQ_INPUT_EVALUATION) {
		((ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProto)->dwRequest						= dwRequest;
		((ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProto)->dwStoreID						= ::atoi(JsonRoot["data"].get("store_id", 0).asString().c_str());
		((ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProto)->dwMenuID						= ::atoi(JsonRoot["data"].get("menu_id", 0).asString().c_str());
		((ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProto)->dwTaste						= ::atoi(JsonRoot["data"]["evaluation"].get("taste", 0).asString().c_str());
		((ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProto)->dwKind						= ::atoi(JsonRoot["data"]["evaluation"].get("kind", 0).asString().c_str());
		((ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProto)->dwMood						= ::atoi(JsonRoot["data"]["evaluation"].get("mood", 0).asString().c_str());
		((ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProto)->dwCallID						= ::atoi(JsonRoot["data"]["user"].get("call_id", 0).asString().c_str());
	}
	else if (dwRequest == E_PROTO_REQ_INPUT_COMMENT) {
		((ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProto)->dwRequest						= dwRequest;
		((ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProto)->strCommentWriting				= JsonRoot["data"]["comment"].get("comment_writing", 0).asString().c_str();
		((ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProto)->strCommentAuthor					= JsonRoot["data"]["comment"].get("comment_author", 0).asString().c_str();
		((ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProto)->strCommentTime					= JsonRoot["data"]["comment"].get("comment_time", 0).asString().c_str();
		((ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProto)->dwStoreID						= ::atoi(JsonRoot["data"].get("store_id", 0).asString().c_str());
		((ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProto)->dwLocation						= ::atoi(JsonRoot["data"].get("location", 0).asString().c_str());
		((ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProto)->dwCallID							= ::atoi(JsonRoot["data"]["user"].get("call_id", 0).asString().c_str());
	}
	else if (dwRequest == E_PROTO_REQ_SHOW_COMMENT_LIST) {
		((ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST *)pProto)->dwRequest					= dwRequest;
		((ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST *)pProto)->dwStoreID					= ::atoi(JsonRoot["data"].get("store_id", 0).asString().c_str());
		((ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST *)pProto)->dwLocation					= ::atoi(JsonRoot["data"].get("location", 0).asString().c_str());
		((ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST *)pProto)->dwCommentReqCount			= ::atoi(JsonRoot["data"].get("comment_req_count", 0).asString().c_str());
	}
	else {
		ErrorLog("Invalid Request Code");
		return E_RET_FAIL;
	}

	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::RequestToDataBase(VOID *pProtoRoot, ST_DB_RESULT &refstDBResult)
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

	ST_PROTOCOL_ROOT *pstProtoRoot = (ST_PROTOCOL_ROOT *)pProtoRoot;
	DWORD dwRequest = pstProtoRoot->dwRequest;

	if (dwRequest == E_PROTO_REQ_USER_LOG_IN || dwRequest == E_PROTO_REQ_USER_SIGN_IN || dwRequest == E_PROTO_REQ_USER_SIGN_UP) {
		ST_PROTOCOL_REQ_101_ABOUT_USER *pstProto101 = (ST_PROTOCOL_REQ_101_ABOUT_USER *)pProtoRoot;
		stDBSQLQuery.strSQL = "SELECT \'call_id\' FROM user WHERE \'call_id\' = \'" + pstProto101->strCallID + "\'";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		refstDBResult = stDBResult;
		if (stDBResult.vecstDBResultLines.size() >= 1) {
			return E_RET_SUCCESS;
		}

		stDBSQLInsert.strSQL = "INSERT INTO \"user\" (call_id) VALUES (\'" + pstProto101->strCallID + "\')";
		dwRet = InsertToDB(hDataBase, stDBSQLInsert);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
	}
	else if (dwRequest == E_PROTO_REQ_RECOMMENDED_STORE) {
		ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *pstProto201 = (ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *)pProtoRoot;

		/*
			Collecting Sequence
			1. store_evaluation_taste
			2. store_evaluation_kind
			3. store_evaluation_mood
			4. store_id
			5. store_name
			6. store_location
			7. store_info_etc
			8. location
			9. store_short_intro
			10. store_hashtag
			11. store_address
			12. store_tel
			13. store_open_time
			14. store_close_time
		*/
		stDBSQLQuery.strSQL = "SELECT " 
								"store_evaluation_taste, "
								"store_evaluation_kind, "
								"store_evaluation_mood, "
								"store_id, "
								"store_name, "
								"store_location, "
								"store_info_etc, "
								"location, "
								"store_short_intro, "
								"store_hash_tag, "
								"store_tel, "
								"store_open_time, "
								"store_close_time "
								"FROM store";

		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		CStore Store;
		dwRet = Store.SortStoreEvaluationByAscendingOrder(stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			return E_RET_FAIL;
		}
		Store.PruningOrderedDataByRequestedNumber(stDBResult, pstProto201->dwNumberOfRequestedMaximun);
		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_RECOMMENDED_MENU) {
		ST_PROTOCOL_REQ_202_RECOMMENDED_MENU *pstProto202 = (ST_PROTOCOL_REQ_202_RECOMMENDED_MENU *)pProtoRoot;
		/*
			Collecting Sequence
			1. menu_evaluation
			2. store_id
			3. menu_id
			4. menu_name
			5. menu_price
		*/

		stDBSQLQuery.strSQL = "SELECT "
			"menu_evaluation, "
			"store_id, "
			"menu_id, "
			"menu_name "
			"FROM menu";

		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		CMenu Menu;
		dwRet = Menu.SortStoreEvaluationByAscendingOrder(stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			return E_RET_FAIL;
		}

		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_SELECTED_STORE) {
		ST_PROTOCOL_REQ_301_SELECTED_STORE *pstProto301 = (ST_PROTOCOL_REQ_301_SELECTED_STORE *)pProtoRoot;
		/*
			Collecting Sequence
			1. menu_evaluation
			2. menu_name
			3. menu_id
			4. menu_price
		*/
		
		stDBSQLQuery.strSQL = "SELECT "
			"menu_evaluation, "
			"menu_name, "
			"menu_id, "
			"menu_price "
			"FROM "
			"menu "
			"WHERE store_id = \'" + std::to_string(pstProto301->dwStoreID) + "\'"
			;

		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		CMenu Menu;
		dwRet = Menu.SortStoreEvaluationByAscendingOrder(stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			return E_RET_FAIL;
		}

		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_SELECTED_MENU) {
		ST_PROTOCOL_REQ_302_SELECTED_MENU *pstProto302 = (ST_PROTOCOL_REQ_302_SELECTED_MENU *)pProtoRoot;
		/*
			Collecting Sequence
			1. menu_evaluation
			2. menu_name
			3. menu_id
		*/

		stDBSQLQuery.strSQL = "SELECT "
			"menu_id "
			"FROM "
			"menu ";
			"WHERE store_id = \'" + std::to_string(pstProto302->dwStoreID) + "\'"
			;

		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
		refstDBResult = stDBResult;
	}
	else if (dwRequest == E_PROTO_REQ_INPUT_EVALUATION) {
		ST_PROTOCOL_REQ_401_INPUT_EVALUATION *pstProto401 = (ST_PROTOCOL_REQ_401_INPUT_EVALUATION *)pProtoRoot;
		stDBSQLInsert.strSQL = "INSERT INTO user(call_id, evaluation_menu_id, comment_store_id) VALUES (" 
			+ std::to_string(pstProto401->dwCallID) + ","
			+ std::to_string(pstProto401->dwMenuID) + ","
			+ std::to_string(pstProto401->dwStoreID) + ")"
			;

		dwRet = InsertToDB(hDataBase, stDBSQLInsert);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
	}
	else if (dwRequest == E_PROTO_REQ_INPUT_COMMENT) {
		ST_PROTOCOL_REQ_501_INPUT_COMMENT *pstProto501 = (ST_PROTOCOL_REQ_501_INPUT_COMMENT *)pProtoRoot;
		stDBSQLInsert.strSQL = "INSERT INTO comment(store_id, comment_author, comment_time, comment_writing) VALUES ("
			+ std::to_string(pstProto501->dwStoreID) + ","
			+ pstProto501->strCommentAuthor + ","
			+ pstProto501->strCommentTime + ","
			+ pstProto501->strCommentWriting + ")"
			;

		dwRet = InsertToDB(hDataBase, stDBSQLInsert);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}
	}
	else if (dwRequest == E_PROTO_REQ_SHOW_COMMENT_LIST) {
		ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST *pstProto502 = (ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST *)pProtoRoot;
		/*
			Collecting Sequence
			1. comment_writing
			2. comment_author
			3. comment_time
		*/

		stDBSQLQuery.strSQL = "SELECT "
			"comment_writing, "
			"comment_author, "
			"comment_time, "
			"FROM "
			"WHERE store_id=" + std::to_string(pstProto502->dwStoreID) +
			"comment";

		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		CComment Comment;
		Comment.PruningOrderedDataByRequestedNumber(stDBResult, pstProto502->dwNumberOfRequestedMaximun);
		refstDBResult = stDBResult;
	}
	else {
		ErrorLog("Invalid Request Code");
		return E_RET_FAIL;
	}
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::SendSimpleResByProtocolNumber(DWORD dwWantedProtocolNumber)
{
	Json::Value JsonRoot;
	std::string strSendData;

	JsonRoot["res"] = static_cast<int>(dwWantedProtocolNumber);

	Json::StyledWriter JsonWriter;
	strSendData = JsonWriter.write(JsonRoot);

	DWORD dwRet;
	dwRet = SendDataToClient(strSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}


DWORD CAYWorkerThread::SendResDataToClientByProtocolNumber(DWORD dwProtocolNumberToSend, ST_SEND_DATA &refstSendData)
{
	if (dwProtocolNumberToSend == E_PROTO_RES_NEXT_STEP_EXIST) {
		Json::Value JsonRoot;
		std::string strSendData;

		JsonRoot["res"]			= static_cast<int>(dwProtocolNumberToSend);
		JsonRoot["next_size"]	= static_cast<int>(refstSendData.strSendData.size());

		Json::StyledWriter JsonWriter;
		strSendData = JsonWriter.write(JsonRoot);

		DWORD dwRet;
		dwRet = SendDataToClient(strSendData);
		if (dwRet != E_RET_SUCCESS) {
			return dwRet;
		}
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
			DebugLog("Success to send data to client");
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
DWORD CAYWorkerThread::ReceiveDataFromClient(char *pRecvedBuf, DWORD refdwNextSizeOfData)
{
	int nRecv;
	nRecv = ::recv(m_stWorkerThread.hClientSocket, pRecvedBuf, refdwNextSizeOfData, 0);
	if (nRecv == SOCKET_ERROR) {
		ErrorLog("Fail to recv data from client");
		return E_RET_FAIL;
	}
	return E_RET_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_101_ABOUT_USER(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	dwRet = PROTO_101_OPERATION_ABOUT_USER(pRecvedBuf);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
	}
	else {
		dwProtocolNumberToSend = E_PROTO_RES_FINISH;
	}

	dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_101_OPERATION_ABOUT_USER(char *pRecvedBuf)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_101_ABOUT_USER *m_pstProto101_AboutUser = NULL;
	m_pstProto101_AboutUser = new (std::nothrow) ST_PROTOCOL_REQ_101_ABOUT_USER;
	if (m_pstProto101_AboutUser == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto101_AboutUser, 0x00, sizeof(ST_PROTOCOL_REQ_101_ABOUT_USER));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto101_AboutUser, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto101_AboutUser, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_201_RECOMMENTED_STORE(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{

	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	ST_SEND_DATA stSendData;
	dwRet = PROTO_201_OPERATION_RECOMMENTED_STORE(pRecvedBuf, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
		dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
		if (dwRet != E_RET_SUCCESS) {
			return dwRet;
		}
		return dwRet;
	}

	dwProtocolNumberToSend = E_PROTO_RES_NEXT_STEP_EXIST;
	dwRet = SendResDataToClientByProtocolNumber(dwProtocolNumberToSend, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	pRecvedBuf = new (std::nothrow) char[DEFAULT_RECV_DATA];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, DEFAULT_RECV_DATA);
	dwRet = ReceiveDataFromClient(pRecvedBuf, DEFAULT_RECV_DATA);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}
	std::string strRecvData = pRecvedBuf;
	delete pRecvedBuf;

	dwRet = ParseSimpleReceivedDataByJSON(dwProtocolNumberToSend, strRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = SendDataToClient(stSendData.strSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_201_OPERATION_RECOMMENTED_STORE(char *pRecvedBuf, ST_SEND_DATA &refstSendData)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *m_pstProto201_Recommended_store = NULL;
	m_pstProto201_Recommended_store = new (std::nothrow) ST_PROTOCOL_REQ_201_RECOMMENDED_STORE;
	if (m_pstProto201_Recommended_store == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto201_Recommended_store, 0x00, sizeof(ST_PROTOCOL_REQ_201_RECOMMENDED_STORE));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto201_Recommended_store, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto201_Recommended_store, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	DWORD dwProtocolNumber = E_PROTO_REQ_RECOMMENDED_STORE;
	dwRet = MakeSendPacket(dwProtocolNumber, stDBResult, refstSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_202_RECOMMENTED_MENU(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	ST_SEND_DATA stSendData;
	dwRet = PROTO_202_OPERATION_RECOMMENTED_MENU(pRecvedBuf, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
		dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
		if (dwRet != E_RET_SUCCESS) {
			return dwRet;
		}
		return dwRet;
	}

	dwProtocolNumberToSend = E_PROTO_RES_NEXT_STEP_EXIST;
	dwRet = SendResDataToClientByProtocolNumber(dwProtocolNumberToSend, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	pRecvedBuf = new (std::nothrow) char[DEFAULT_RECV_DATA];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, DEFAULT_RECV_DATA);
	dwRet = ReceiveDataFromClient(pRecvedBuf, DEFAULT_RECV_DATA);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}
	std::string strRecvData = pRecvedBuf;
	delete pRecvedBuf;

	dwRet = ParseSimpleReceivedDataByJSON(dwProtocolNumberToSend, strRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = SendDataToClient(stSendData.strSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

DWORD CAYWorkerThread::PROTO_202_OPERATION_RECOMMENTED_MENU(char *pRecvedBuf, ST_SEND_DATA &refstSendData)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_202_RECOMMENDED_MENU *m_pstProto202_Recommended_menu = NULL;
	m_pstProto202_Recommended_menu = new (std::nothrow) ST_PROTOCOL_REQ_202_RECOMMENDED_MENU;
	if (m_pstProto202_Recommended_menu == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto202_Recommended_menu, 0x00, sizeof(ST_PROTOCOL_REQ_202_RECOMMENDED_MENU));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto202_Recommended_menu, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto202_Recommended_menu, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	DWORD dwProtocolNumber = E_PROTO_REQ_RECOMMENDED_MENU;
	dwRet = MakeSendPacket(dwProtocolNumber, stDBResult, refstSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

DWORD CAYWorkerThread::PROTO_301_SELECTED_STORE(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	ST_SEND_DATA stSendData;
	dwRet = PROTO_301_OPERATION_SELECTED_STORE(pRecvedBuf, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
		dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
		if (dwRet != E_RET_SUCCESS) {
			return dwRet;
		}
		return dwRet;
	}

	dwProtocolNumberToSend = E_PROTO_RES_NEXT_STEP_EXIST;
	dwRet = SendResDataToClientByProtocolNumber(dwProtocolNumberToSend, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	pRecvedBuf = new (std::nothrow) char[DEFAULT_RECV_DATA];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, DEFAULT_RECV_DATA);
	dwRet = ReceiveDataFromClient(pRecvedBuf, DEFAULT_RECV_DATA);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}
	std::string strRecvData = pRecvedBuf;
	delete pRecvedBuf;

	dwRet = ParseSimpleReceivedDataByJSON(dwProtocolNumberToSend, strRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = SendDataToClient(stSendData.strSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

DWORD CAYWorkerThread::PROTO_301_OPERATION_SELECTED_STORE(char *pRecvedBuf, ST_SEND_DATA &refstSendData)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_301_SELECTED_STORE *m_pstProto301_Selected_store = NULL;
	m_pstProto301_Selected_store = new (std::nothrow) ST_PROTOCOL_REQ_301_SELECTED_STORE;
	if (m_pstProto301_Selected_store == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto301_Selected_store, 0x00, sizeof(ST_PROTOCOL_REQ_301_SELECTED_STORE));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto301_Selected_store, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto301_Selected_store, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	DWORD dwProtocolNumber = E_PROTO_REQ_SELECTED_STORE;
	dwRet = MakeSendPacket(dwProtocolNumber, stDBResult, refstSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_302_SELECTED_MENU(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	ST_SEND_DATA stSendData;
	dwRet = PROTO_302_OPERATION_SELECTED_MENU(pRecvedBuf, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
		dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
		if (dwRet != E_RET_SUCCESS) {
			return dwRet;
		}
		return dwRet;
	}

	dwProtocolNumberToSend = E_PROTO_RES_NEXT_STEP_EXIST;
	dwRet = SendResDataToClientByProtocolNumber(dwProtocolNumberToSend, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	pRecvedBuf = new (std::nothrow) char[DEFAULT_RECV_DATA];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, DEFAULT_RECV_DATA);
	dwRet = ReceiveDataFromClient(pRecvedBuf, DEFAULT_RECV_DATA);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}
	std::string strRecvData = pRecvedBuf;
	delete pRecvedBuf;

	dwRet = ParseSimpleReceivedDataByJSON(dwProtocolNumberToSend, strRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = SendDataToClient(stSendData.strSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_302_OPERATION_SELECTED_MENU(char *pRecvedBuf, ST_SEND_DATA &refstSendData)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_302_SELECTED_MENU *m_pstProto302_Selected_menu = NULL;
	m_pstProto302_Selected_menu = new (std::nothrow) ST_PROTOCOL_REQ_302_SELECTED_MENU;
	if (m_pstProto302_Selected_menu == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto302_Selected_menu, 0x00, sizeof(ST_PROTOCOL_REQ_302_SELECTED_MENU));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto302_Selected_menu, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto302_Selected_menu, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	DWORD dwProtocolNumber = E_PROTO_REQ_SELECTED_MENU;
	dwRet = MakeSendPacket(dwProtocolNumber, stDBResult, refstSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::PROTO_401_INPUT_EVALUATION(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	dwRet = PROTO_401_OPERATION_ABOUT_INPUT_EVALUATION(pRecvedBuf);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
	}
	else {
		dwProtocolNumberToSend = E_PROTO_RES_FINISH;
	}

	dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}

DWORD CAYWorkerThread::PROTO_401_OPERATION_ABOUT_INPUT_EVALUATION(char *pRecvedBuf)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_401_INPUT_EVALUATION *m_pstProto401_Input_Evaluation = NULL;
	m_pstProto401_Input_Evaluation = new (std::nothrow) ST_PROTOCOL_REQ_401_INPUT_EVALUATION;
	if (m_pstProto401_Input_Evaluation == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto401_Input_Evaluation, 0x00, sizeof(ST_PROTOCOL_REQ_401_INPUT_EVALUATION));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto401_Input_Evaluation, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto401_Input_Evaluation, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}

DWORD CAYWorkerThread::PROTO_501_INPUT_COMMENT(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	dwRet = PROTO_501_OPERATION_ABOUT_COMMENT(pRecvedBuf);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
	}
	else {
		dwProtocolNumberToSend = E_PROTO_RES_FINISH;
	}

	dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}

DWORD CAYWorkerThread::PROTO_501_OPERATION_ABOUT_COMMENT(char *pRecvedBuf)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_501_INPUT_COMMENT *m_pstProto501_Input_Comment = NULL;
	m_pstProto501_Input_Comment = new (std::nothrow) ST_PROTOCOL_REQ_501_INPUT_COMMENT;
	if (m_pstProto501_Input_Comment == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto501_Input_Comment, 0x00, sizeof(ST_PROTOCOL_REQ_501_INPUT_COMMENT));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto501_Input_Comment, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto501_Input_Comment, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}

DWORD CAYWorkerThread::PROTO_502_SHOW_COMMENT_LIST(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	DWORD dwNextSizeOfData = refstRecvHeaderData.dwNextSizeOfData;
	char *pRecvedBuf = new (std::nothrow) char[dwNextSizeOfData];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, dwNextSizeOfData);

	DWORD dwRet;
	dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_READY_FOR_DATA);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = ReceiveDataFromClient(pRecvedBuf, dwNextSizeOfData);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}

	DWORD dwProtocolNumberToSend;
	ST_SEND_DATA stSendData;
	dwRet = PROTO_502_OPERATION_SHOW_COMMENT_LIST(pRecvedBuf, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		dwProtocolNumberToSend = E_PROTO_RES_FAIL_OPERATION;
		dwRet = SendSimpleResByProtocolNumber(dwProtocolNumberToSend);
		if (dwRet != E_RET_SUCCESS) {
			return dwRet;
		}
		return dwRet;
	}

	dwProtocolNumberToSend = E_PROTO_RES_NEXT_STEP_EXIST;
	dwRet = SendResDataToClientByProtocolNumber(dwProtocolNumberToSend, stSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	pRecvedBuf = new (std::nothrow) char[DEFAULT_RECV_DATA];
	if (pRecvedBuf == NULL) {
		return E_RET_FAIL;
	}
	memset(pRecvedBuf, 0x00, DEFAULT_RECV_DATA);
	dwRet = ReceiveDataFromClient(pRecvedBuf, DEFAULT_RECV_DATA);
	if (dwRet != E_RET_SUCCESS) {
		delete pRecvedBuf;
		return dwRet;
	}
	std::string strRecvData = pRecvedBuf;
	delete pRecvedBuf;

	dwRet = ParseSimpleReceivedDataByJSON(dwProtocolNumberToSend, strRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = SendDataToClient(stSendData.strSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

DWORD CAYWorkerThread::PROTO_502_OPERATION_SHOW_COMMENT_LIST(char *pRecvedBuf, ST_SEND_DATA &refstSendData)
{
	ST_RECV_DATA stRecvData;
	stRecvData.strReceivedData = pRecvedBuf;
	delete pRecvedBuf;

	ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST *m_pstProto502_Show_Comment_Menu = NULL;
	m_pstProto502_Show_Comment_Menu = new (std::nothrow) ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST;
	if (m_pstProto502_Show_Comment_Menu == NULL) {
		return E_RET_FAIL;
	}

	::memset(m_pstProto502_Show_Comment_Menu, 0x00, sizeof(ST_PROTOCOL_REQ_502_SHOW_COMMENT_LIST));

	DWORD dwRet;
	dwRet = ParseDataByJSON((VOID *)m_pstProto502_Show_Comment_Menu, stRecvData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	ST_DB_RESULT stDBResult;
	dwRet = RequestToDataBase((VOID *)m_pstProto502_Show_Comment_Menu, stDBResult);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	DWORD dwProtocolNumber = E_PROTO_REQ_SHOW_COMMENT_LIST;
	dwRet = MakeSendPacket(dwProtocolNumber, stDBResult, refstSendData);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket(DWORD dwNumberOfRequest, ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	DWORD dwRet;
	if (dwNumberOfRequest == E_PROTO_REQ_RECOMMENDED_STORE) {
		dwRet = MakeSendPacket_201_Data(refstDBResult, refstSendData);
	}
	else if (dwNumberOfRequest == E_PROTO_REQ_RECOMMENDED_MENU) {
		dwRet = MakeSendPacket_202_Data(refstDBResult, refstSendData);
	}
	else if (dwNumberOfRequest == E_PROTO_REQ_SELECTED_STORE) {
		dwRet = MakeSendPacket_301_Data(refstDBResult, refstSendData);
	}
	else if (dwNumberOfRequest == E_PROTO_REQ_SELECTED_MENU) {
		dwRet = MakeSendPacket_302_Data(refstDBResult, refstSendData);
	}
	else if (dwNumberOfRequest == E_PROTO_REQ_SHOW_COMMENT_LIST) {
		dwRet = MakeSendPacket_502_Data(refstDBResult, refstSendData);
	}
	else {
		dwRet = E_RET_FAIL;
	}
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket_201_Data(ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	Json::Value JsonResData;
	Json::Value JsonStores;

	std::vector<ST_DB_RESULT_LINE>::iterator iterstDBResultLine;
	for (iterstDBResultLine = refstDBResult.vecstDBResultLines.begin(); iterstDBResultLine != refstDBResult.vecstDBResultLines.end(); iterstDBResultLine++) {
		/*
			DB Parameter
			1. store_evaluation_taste
			2. store_evaluation_kind
			3. store_evaluation_mood
			4. store_id
			5. store_name
			6. store_location
			7. store_info_etc
			8. location
			9. store_short_intro
			10. store_hashtag
			11. store_address
			12. store_tel
			13. store_open_time
			14. store_close_time
		*/

		ST_DB_RESULT_LINE stDBResultLine = (*iterstDBResultLine);
		if (stDBResultLine.vecstrResult.size() != 13) {
			break;
		}

		ST_PROTOCOL_RES_201_RECOMMENDED_STORE stProtoRes201RecommendedStore;
		stProtoRes201RecommendedStore.dwStoreEvaluationTaste		= ::atoi(stDBResultLine.vecstrResult[0].c_str());
		stProtoRes201RecommendedStore.dwStoreEvaluationKind			= ::atoi(stDBResultLine.vecstrResult[1].c_str());
		stProtoRes201RecommendedStore.dwStoreEvaluationMood			= ::atoi(stDBResultLine.vecstrResult[2].c_str());
		stProtoRes201RecommendedStore.dwStoreID						= ::atoi(stDBResultLine.vecstrResult[3].c_str());
		stProtoRes201RecommendedStore.strStoreName					= stDBResultLine.vecstrResult[4];
		stProtoRes201RecommendedStore.dwStoreLocation				= ::atoi(stDBResultLine.vecstrResult[5].c_str());
		stProtoRes201RecommendedStore.strStoreInfoETC				= stDBResultLine.vecstrResult[6];
		stProtoRes201RecommendedStore.dwLocation					= ::atoi(stDBResultLine.vecstrResult[7].c_str());
		stProtoRes201RecommendedStore.strStoreShortIntro			= stDBResultLine.vecstrResult[8];
		stProtoRes201RecommendedStore.strStoreHashTag				= stDBResultLine.vecstrResult[9];
		stProtoRes201RecommendedStore.strStoreTel					= stDBResultLine.vecstrResult[10];
		stProtoRes201RecommendedStore.strStoreOpenTime				= stDBResultLine.vecstrResult[11];
		stProtoRes201RecommendedStore.strStoreCloseTime				= stDBResultLine.vecstrResult[12];

		Json::Value JsonStoreEvaluation;
		Json::Value JsonStorePriceRange;
		Json::Value JsonStoreShortInfo;

		JsonStoreEvaluation["taste"]	= static_cast<int>(stProtoRes201RecommendedStore.dwStoreEvaluationTaste);
		JsonStoreEvaluation["kind"]		= static_cast<int>(stProtoRes201RecommendedStore.dwStoreEvaluationKind);
		JsonStoreEvaluation["mood"]		= static_cast<int>(stProtoRes201RecommendedStore.dwStoreEvaluationMood);

		// To do ...
		JsonStorePriceRange["min_price"] = "";
		JsonStorePriceRange["max_price"] = "";

		// To do ...
		JsonStoreShortInfo["number_of_like"] = "";
		JsonStoreShortInfo["number_of_evaluation"] = "";
		JsonStoreShortInfo["number_of_comment"] = "";

		Json::Value JsonStore;
		JsonStore["store_id"]			= static_cast<int>(stProtoRes201RecommendedStore.dwStoreID);
		JsonStore["store_name"]			= stProtoRes201RecommendedStore.strStoreName;
		JsonStore["store_location"]		= static_cast<int>(stProtoRes201RecommendedStore.dwStoreLocation);
		JsonStore["store_short_intro"]	= stProtoRes201RecommendedStore.strStoreShortIntro;
		JsonStore["store_hashtag"]		= stProtoRes201RecommendedStore.strStoreHashTag;
		JsonStore["store_evaluation"]	= JsonStoreEvaluation;
		JsonStore["store_price_range"]	= JsonStorePriceRange;
		JsonStore["store_short_info"]	= JsonStoreShortInfo;

		JsonStores.append(JsonStore);
	}
	DWORD dwRetNumber = refstDBResult.vecstDBResultLines.size();
	Json::Value JsonRetNumber;
	Json::Value JsonStoresArray;

	JsonRetNumber["ret_number"]			= static_cast<int>(dwRetNumber);
	JsonStoresArray["stores"]			= JsonStores;

	JsonResData["res"]					= E_PROTO_RES_FINISH;
	JsonResData["data"]					= JsonRetNumber;
	JsonResData["data"]					= JsonStoresArray;

	Json::StyledWriter JsonWriter;
	std::string strSendData;
	strSendData = JsonWriter.write(JsonResData);

	refstSendData.strSendData = strSendData;

	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket_202_Data(ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	Json::Value JsonRoot;
	Json::Value JsonResData;
	Json::Value JsonMenus;

	std::vector<ST_DB_RESULT_LINE>::iterator iterstDBResultLine;
	for (iterstDBResultLine = refstDBResult.vecstDBResultLines.begin(); iterstDBResultLine != refstDBResult.vecstDBResultLines.end(); iterstDBResultLine++) {
		/*
			Collecting Sequence
			1. menu_evaluation
			2. store_id
			3. menu_id
			4. menu_name
		*/

		ST_DB_RESULT_LINE stDBResultLine = (*iterstDBResultLine);
		if (stDBResultLine.vecstrResult.size() != 4) {
			break;
		}

		ST_PROTOCOL_RES_202_RECOMMENDED_MENU stProtoRes202RecommendedMenu;
		stProtoRes202RecommendedMenu.dwMenuEvaluation	= ::atoi(stDBResultLine.vecstrResult[0].c_str());
		stProtoRes202RecommendedMenu.dwStoreID			= ::atoi(stDBResultLine.vecstrResult[1].c_str());
		stProtoRes202RecommendedMenu.dwMenuID			= ::atoi(stDBResultLine.vecstrResult[2].c_str());
		stProtoRes202RecommendedMenu.strMenuName		= stDBResultLine.vecstrResult[3];

		Json::Value JsonMenuStore;
		JsonMenuStore["store_id"]		= static_cast<int>(stProtoRes202RecommendedMenu.dwStoreID);
		
		// to do ...
		JsonMenuStore["store_location"] = static_cast<int>(1);

		Json::Value JsonMenu;
		JsonMenu["menu_id"]				= static_cast<int>(stProtoRes202RecommendedMenu.dwStoreID);
		JsonMenu["menu_name"]			= stProtoRes202RecommendedMenu.strMenuName;
		JsonMenu["menu_evaluation"]		= static_cast<int>(stProtoRes202RecommendedMenu.dwMenuEvaluation);
		JsonMenu["store"]				= JsonMenuStore;

		JsonMenus.append(JsonMenu);
	}

	DWORD dwRetNumber = refstDBResult.vecstDBResultLines.size();
	Json::Value JsonRetNumber;
	Json::Value JsonMenusArray;

	JsonRetNumber["ret_number"] = static_cast<int>(dwRetNumber);
	JsonMenusArray["menus"] = JsonMenus;

	JsonResData["res"] = static_cast<int>(E_PROTO_RES_FINISH);
	JsonResData["data"] = JsonRetNumber;
	JsonResData["data"] = JsonMenusArray;

	Json::StyledWriter JsonWriter;
	std::string strSendData;
	strSendData = JsonWriter.write(JsonResData);

	refstSendData.strSendData = strSendData;
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket_301_Data(ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	Json::Value JsonResData;
	Json::Value JsonMenuArray;

	std::vector<ST_DB_RESULT_LINE>::iterator iterstDBResultLine;
	DWORD dwMenuCount = 0;
	for (iterstDBResultLine = refstDBResult.vecstDBResultLines.begin(); iterstDBResultLine != refstDBResult.vecstDBResultLines.end(); iterstDBResultLine++) {
		if (dwMenuCount >= 3) {
			// only add menu count
			dwMenuCount++;
			continue;
		}
		
		ST_DB_RESULT_LINE stDBResultLine = (*iterstDBResultLine);

		ST_PROTOCOL_RES_301_SELECT_STORE stProtoRes301SelectedStore;
		stProtoRes301SelectedStore.dwMenuEvaluation = ::atoi(stDBResultLine.vecstrResult[0].c_str());
		stProtoRes301SelectedStore.strMenuName		= stDBResultLine.vecstrResult[1];
		stProtoRes301SelectedStore.dwMenuID			= ::atoi(stDBResultLine.vecstrResult[2].c_str());
		stProtoRes301SelectedStore.dwMenuPrice		= ::atoi(stDBResultLine.vecstrResult[3].c_str());
		

		Json::Value JsonMenu;
		JsonMenu["menu_id"]			= static_cast<int>(stProtoRes301SelectedStore.dwMenuID);
		JsonMenu["menu_name"]		= stProtoRes301SelectedStore.strMenuName;
		JsonMenu["menu_evaluation"] = static_cast<int>(stProtoRes301SelectedStore.dwMenuEvaluation);
		JsonMenu["menu_price"]		= static_cast<int>(stProtoRes301SelectedStore.dwMenuPrice);

		JsonMenuArray.append(JsonMenu);
		dwMenuCount++;
	}

	Json::Value JsonRetNumber;
	Json::Value JsonMenu;
	Json::Value JsonMenus;

	JsonMenu["menu"] = JsonMenuArray;
	JsonMenu["menu_total_count"] = static_cast<int>(dwMenuCount);

	JsonMenus["menus"] = JsonMenu;

	JsonResData["res"] = static_cast<int>(E_PROTO_RES_FINISH);
	JsonResData["data"] = JsonMenus;

	Json::StyledWriter JsonWriter;
	std::string strSendData;
	strSendData = JsonWriter.write(JsonResData);

	refstSendData.strSendData = strSendData;
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket_302_Data(ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	Json::Value JsonResData;
	Json::Value JsonMenu;

	DWORD dwMenuCount = refstDBResult.vecstDBResultLines.size();
	JsonMenu["menu"] = static_cast<int>(dwMenuCount);

	JsonResData["res"] = static_cast<int>(E_PROTO_RES_FINISH);
	JsonResData["data"] = JsonMenu;

	Json::StyledWriter JsonWriter;
	std::string strSendData;
	strSendData = JsonWriter.write(JsonResData);

	refstSendData.strSendData = strSendData;
	return E_RET_SUCCESS;
}

DWORD CAYWorkerThread::MakeSendPacket_502_Data(ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	Json::Value JsonRoot;
	Json::Value JsonResData;
	Json::Value JsonComments;

	std::vector<ST_DB_RESULT_LINE>::iterator iterstDBResultLine;
	for (iterstDBResultLine = refstDBResult.vecstDBResultLines.begin(); iterstDBResultLine != refstDBResult.vecstDBResultLines.end(); iterstDBResultLine) {
		/*
			Collecting Sequence
			1. comment_writing
			2. comment_author
			3. comment_time
		*/

		ST_DB_RESULT_LINE stDBResultLine = (*iterstDBResultLine);
		if (stDBResultLine.vecstrResult.size() != 3) {
			break;
		}

		ST_PROTOCOL_RES_502_SHOW_COMMENT_LIST stProtoRes502_ShowCommentList;
		stProtoRes502_ShowCommentList.strCommentWriting		= stDBResultLine.vecstrResult[0];
		stProtoRes502_ShowCommentList.strCommentAuthor		= stDBResultLine.vecstrResult[1];
		stProtoRes502_ShowCommentList.strCommentTime		= stDBResultLine.vecstrResult[2];

		Json::Value JsonComment;
		JsonComment["comment_writing"]	= stProtoRes502_ShowCommentList.strCommentWriting;
		JsonComment["comment_author"]	= stProtoRes502_ShowCommentList.strCommentAuthor;
		JsonComment["comment_time"]		= stProtoRes502_ShowCommentList.strCommentTime;

		JsonComments.append(JsonComment);
	}

	Json::Value JsonRetNumber;
	Json::Value JsonCommentsArray;

	DWORD dwCommentCount = refstDBResult.vecstDBResultLines.size();
	JsonRetNumber["ret_number"] = std::to_string(dwCommentCount);
	JsonCommentsArray["menus"] = JsonComments;

	JsonResData["res"] = std::to_string(E_PROTO_RES_FINISH);
	JsonResData["data"] = JsonRetNumber;
	JsonResData["data"] = JsonCommentsArray;

	Json::StyledWriter JsonWriter;
	std::string strSendData;
	strSendData = JsonWriter.write(JsonRoot);

	refstSendData.strSendData = strSendData;
	return E_RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::StartWorkerThread(char *pReceiveBuf, DWORD dwByteTransferred)
{
	DWORD dwRet;
	try
	{
		/*
			Protocol documentation is written in excel file.
			To see protocol in detail, it's good for me to open and check the file
		*/
		std::string strReceivedData;
		strReceivedData.clear();
		ConvertReceivedDataToString(pReceiveBuf, dwByteTransferred, strReceivedData);
		if (strReceivedData.length() == 0) {
			throw std::exception("Fail to convert buf data to string");
		}

		ST_RECV_HEADER_DATA stRecvHeaderData;
		dwRet = ParseReceivedDataByJSON(strReceivedData, stRecvHeaderData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to analyze received data by json");
		}

		dwRet = BranchByRequestNumber(stRecvHeaderData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Invalid next protocol");
		}
	}
	catch (std::exception &e)
	{
		ErrorLog(e.what());
		dwRet = SendSimpleResByProtocolNumber(E_PROTO_RES_FAIL_OPERATION);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to send Error Msg to client");
		}
		return E_RET_FAIL;
	}
	DebugLog("Success to communicate with client");
	return E_RET_SUCCESS;
} 