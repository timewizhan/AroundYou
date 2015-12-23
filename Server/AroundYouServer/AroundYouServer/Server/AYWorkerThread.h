#ifndef _AY_WORKER_THREAD_
#define _AY_WORKER_THREAD_

#include "..\Common\Common.h"
#include "..\DataBase\DataBase.h"
#include "AYServerStruct.h"
#include "AYProtocol.h"
#include "AYWorkerThreadStruct.h"

class CAYWorkerThread
{
	ST_WORKER_THREAD	m_stWorkerThread;
	ST_DB_LOGIN_TOKEN	m_stDBLoginToken;

	ST_RECV_HEADER_DATA m_stRecvHeaderData;
	ST_RECV_DATA		m_stRecvData;

	/*
		The following methods are related to common methods,
		which are used for normal operation
	*/
	VOID ConvertReceivedDataToString(char *pReceiveBuf, DWORD dwByteTransferred, std::string &refstrReceivedData);
	DWORD ParseSimpleReceivedDataByJSON(DWORD dwSentResProtocolNumber, std::string &refstrRecvData);
	DWORD ParseReceivedDataByJSON(const std::string &refstrReceivedData, ST_RECV_HEADER_DATA &refstRecvHeaderData);

	DWORD CheckFirstRequestNumber(ST_RECV_HEADER_DATA &refstRecvHeaderData);
	DWORD BranchByRequestNumber(ST_RECV_HEADER_DATA &refstRecvHeaderData);
	DWORD ParseDataByJSON(VOID *pProto, ST_RECV_DATA &refstRecvData);
	DWORD RequestToDataBase(VOID *pProtoRoot, ST_DB_RESULT &refstDBResult);

	/*
		The following methods are related to communication with client
		methods to communicate with client are read/write operation
	*/

	DWORD SendSimpleResByProtocolNumber(DWORD dwWantedProtocolNumber);
	DWORD SendResDataToClientByProtocolNumber(DWORD dwProtocolNumberToSend, ST_SEND_DATA &refstSendData);

	// Common Send/Receive Method
	DWORD SendDataToClient(std::string &refstrSendData);
	DWORD ReceiveDataFromClient(char *pRecvedBuf, DWORD dwNextSizeOfData);

	/*
		The following methods are related to protocol.
		each number is protocol number
	*/
	// 101 Protocol (User)
	DWORD PROTO_101_ABOUT_USER(ST_RECV_HEADER_DATA &refstRecvHeaderData);
	DWORD PROTO_101_OPERATION_ABOUT_USER(char *pRecvedBuf);

	// 201 Protocol (Recommended Store)
	DWORD PROTO_201_RECOMMENTED_STORE(ST_RECV_HEADER_DATA &refstRecvHeaderData);
	DWORD PROTO_201_OPERATION_RECOMMENTED_STORE(char *pRecvedBuf, ST_SEND_DATA &refstSendData);

	// 202 Protocol (Recommended Menu)
	DWORD PROTO_201_RECOMMENTED_MENU(ST_RECV_HEADER_DATA &refstRecvHeaderData);

	DWORD MakeSendPacket(DWORD dwNumberOfRequest, ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData);
	DWORD MakeSendPacket_201_Data(ST_DB_RESULT &refstDBResult, ST_SEND_DATA &refstSendData);

public:
	CAYWorkerThread(SOCKET ClientSocket);
	~CAYWorkerThread();

	DWORD StartWorkerThread(char *pReceiveBuf, DWORD dwByteTransferred);
};

#endif