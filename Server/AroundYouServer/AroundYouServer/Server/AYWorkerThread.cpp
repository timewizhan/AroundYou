#include "AYWorkerThread.h"
#include "AYProtocol.h"
#include "AYServerError.h"

#include "..\Common\Log.h"
#include "json\reader.h"
#include "json\json.h"

#include "SignIn.h"
#include "Store.h"

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
	DWORD dwProtocolNumber = ::atoi(JsonRoot.get("req", 0).asString().c_str());
	if (dwSentResProtocolNumber == E_PROTO_REQ_NEXT_RECOMMENDED_STORE) {
		if (dwProtocolNumber != E_PROTO_REQ_READY_FOR_DATA) {
			dwRet = E_RET_FAIL;
		}
	}
	else {
		// nothing
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

	refstRecvHeaderData.dwReqNumber			= ::atoi(JsonRoot.get("req", 0).asString().c_str());
	refstRecvHeaderData.dwNextReqNumber		= ::atoi(JsonRoot.get("next_req", 0).asString().c_str());
	refstRecvHeaderData.dwNextSizeOfData	= ::atoi(JsonRoot.get("next_size", 0).asString().c_str());
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
	case E_PROTO_REQ_USER_SIGN_IN:
	case E_PROTO_REQ_USER_SIGN_UP:
		dwRet = PROTO_101_ABOUT_USER(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_RECOMMENDED_STORE:
		dwRet = PROTO_201_RECOMMENTED_STORE(refstRecvHeaderData);
		break;
	case E_PROTO_REQ_RECOMMENDED_MENU:
		dwRet = PROTO_201_RECOMMENTED_MENU(refstRecvHeaderData);
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

	std::string strRequest = JsonRoot.get("req", 0).asString();
	DWORD dwRequest = ::atoi(strRequest.c_str());

	///////////// User //////////////
	if (dwRequest == E_PROTO_REQ_USER_SIGN_IN || dwRequest == E_PROTO_REQ_USER_SIGN_UP) {
		((ST_PROTOCOL_REQ_101_ABOUT_USER *)pProto)->dwRequest = dwRequest;
		((ST_PROTOCOL_REQ_101_ABOUT_USER *)pProto)->strCallID = JsonRoot.get("call_id", 0).asString();
	}
	else if (dwRequest == E_PROTO_REQ_RECOMMENDED_STORE) {
		((ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *)pProto)->dwRequest					= dwRequest;
		((ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *)pProto)->dwLocation					= ::atoi(JsonRoot["data"].get("location", 0).asString().c_str());
		((ST_PROTOCOL_REQ_201_RECOMMENDED_STORE *)pProto)->dwNumberOfRequestedMaximun	= ::atoi(JsonRoot["data"].get("res_number", 0).asString().c_str());
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

	if (dwRequest == E_PROTO_REQ_USER_SIGN_IN || dwRequest == E_PROTO_REQ_USER_SIGN_UP) {
		ST_PROTOCOL_REQ_101_ABOUT_USER *pstProto101 = (ST_PROTOCOL_REQ_101_ABOUT_USER *)pProtoRoot;
		stDBSQLQuery.strSQL = "SELECT call_id FROM user WHERE call_id = \'" + pstProto101->strCallID + "\'";
		dwRet = QueryFromDB(hDataBase, stDBSQLQuery, stDBResult);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Fail to query data from DataBase");
			return E_RET_FAIL;
		}

		refstDBResult = stDBResult;
		if (stDBResult.vecstDBResultLines.size() >= 1) {
			return E_RET_SUCCESS;
		}

		stDBSQLInsert.strSQL = "INSERT INTO user(call_id) VALUES (" + pstProto101->strCallID + ")";
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
								"store_hashtag, "
								"store_address, "
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

	JsonRoot["res"] = std::to_string(dwWantedProtocolNumber);

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

		JsonRoot["res"] = std::to_string(dwProtocolNumberToSend);
		JsonRoot["next_size"] = std::to_string(refstSendData.strSendData.size());

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
DWORD CAYWorkerThread::PROTO_201_RECOMMENTED_MENU(ST_RECV_HEADER_DATA &refstRecvHeaderData)
{
	return E_RET_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket(DWORD dwNumberOfRequest, ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	DWORD dwRet;
	if (dwNumberOfRequest == E_PROTO_REQ_RECOMMENDED_STORE) {
		dwRet = MakeSendPacket_201_Data(refstDBResult, refstSendData);
	}
	else {
		dwRet = E_RET_FAIL;
	}
	return dwRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CAYWorkerThread::MakeSendPacket_201_Data(ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData)
{
	Json::Value JsonRoot;
	Json::Value JsonResData;
	Json::Value JsonStores;

	std::vector<ST_DB_RESULT_LINE>::iterator iterstDBResultLine;
	for (iterstDBResultLine = refstDBResult.vecstDBResultLines.begin(); iterstDBResultLine != refstDBResult.vecstDBResultLines.end(); iterstDBResultLine) {
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
		if (stDBResultLine.vecstrResult.size() != 14) {
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
		stProtoRes201RecommendedStore.strStoreAddress				= stDBResultLine.vecstrResult[10];
		stProtoRes201RecommendedStore.strStoreTel					= stDBResultLine.vecstrResult[11];
		stProtoRes201RecommendedStore.strStoreOpenTime				= stDBResultLine.vecstrResult[12];
		stProtoRes201RecommendedStore.strStoreCloseTime				= stDBResultLine.vecstrResult[13];

		Json::Value JsonStoreEvaluation;
		Json::Value JsonStorePriceRange;
		Json::Value JsonStoreShortInfo;

		JsonStoreEvaluation["taste"] = std::to_string(stProtoRes201RecommendedStore.dwStoreEvaluationTaste);
		JsonStoreEvaluation["kind"] = std::to_string(stProtoRes201RecommendedStore.dwStoreEvaluationKind);
		JsonStoreEvaluation["mood"] = std::to_string(stProtoRes201RecommendedStore.dwStoreEvaluationMood);

		// To do ...
		JsonStorePriceRange["min_price"] = "";
		JsonStorePriceRange["max_price"] = "";

		// To do ...
		JsonStoreShortInfo["number_of_like"] = "";
		JsonStoreShortInfo["number_of_evaluation"] = "";
		JsonStoreShortInfo["number_of_comment"] = "";

		Json::Value JsonStore;
		JsonStore["store_id"] = std::to_string(stProtoRes201RecommendedStore.dwStoreID);
		JsonStore["store_name"] = stProtoRes201RecommendedStore.strStoreName;
		JsonStore["store_location"] = std::to_string(stProtoRes201RecommendedStore.dwStoreLocation);
		JsonStore["store_short_intro"] = stProtoRes201RecommendedStore.strStoreShortIntro;
		JsonStore["store_hashtag"] = stProtoRes201RecommendedStore.strStoreHashTag;
		JsonStore["store_evaluation"] = JsonStoreEvaluation;
		JsonStore["store_price_range"] = JsonStorePriceRange;
		JsonStore["store_short_info"] = JsonStoreShortInfo;

		JsonStores.append(JsonStore);
	}
	DWORD dwRetNumber = refstDBResult.vecstDBResultLines.size();
	Json::Value JsonRetNumber;
	Json::Value JsonStoresArray;

	JsonRetNumber["ret_number"] = std::to_string(dwRetNumber);
	JsonStoresArray["stores"] = JsonStores;

	JsonResData["res"] = std::to_string(E_PROTO_RES_FINISH);
	JsonResData["data"] = JsonRetNumber;
	JsonResData["data"] = JsonStoresArray;

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
			Protocol documentation is wrote in excel file.
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

		dwRet = CheckFirstRequestNumber(stRecvHeaderData);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Invalid first connection protocol");
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