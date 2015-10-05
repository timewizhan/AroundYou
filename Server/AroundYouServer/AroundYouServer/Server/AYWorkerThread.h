#ifndef _AY_WORKER_THREAD_
#define _AY_WORKER_THREAD_

#include "..\Common\Common.h"
#include "..\DataBase\DataBase.h"
#include "AYServerStruct.h"
#include "AYProtocol.h"

class CAYWorkerThread
{
	ST_WORKER_THREAD	m_stWorkerThread;
	ST_DB_LOGIN_TOKEN	m_stDBLoginToken;

	ST_RECV_HEADER_DATA m_stRecvHeaderData;
	ST_RECV_DATA		m_stRecvData;

	/*
		Protocol structure
	*/
	ST_PROTOCOL_SIGNIN	*m_pstProtoSignIn;
	ST_PROTOCOL_SIGNUP	*m_pstProtoSignUp;

	ST_PROTOCOL_CLIENT	*m_pstProtoClient;
	ST_PROTOCOL_MENU	*m_pstProtoMenu;
	ST_PROTOCOL_COMMENT *m_pstProtoComment;
	ST_PROTOCOL_SHOP	*m_pstProtoShop;

	DWORD ReceiveDataFromClient(char *pReceiveBuf, DWORD dwByteTransferred, ST_RECV_HEADER_DATA &refstRecvHeaderData);
	DWORD ParseReceivedHeaderByJSON(ST_RECV_HEADER_DATA &refstRecvHeaderData);
	DWORD SendHeaderToClient(ST_RECV_HEADER_DATA &refstRecvHeaderData);
	DWORD ReceiveDataFromClient(ST_RECV_HEADER_DATA &refstRecvHeaderData, ST_RECV_DATA &refstRecvData);

	DWORD ParseDataByJSON(DWORD &refdwNumberOfRequest, ST_RECV_DATA &refstRecvData);
	DWORD RequestToDataBase(DWORD &refdwNumberOfRequest, ST_DB_RESULT &refstDBResult);
	DWORD MakeSendPacket(DWORD &refdwNumberOfRequest, ST_DB_RESULT &refstDBResult, DWORD &refdwResponse, std::string &refstrSendData);
	DWORD SendResponseToClient(DWORD &refdwNumberOfRequest, DWORD &refdwResponse, std::string &refstrSendData);
	DWORD SendResponseHeaderToClient(DWORD &refdwNumberOfRequest, DWORD &refdwResponse, DWORD &refstSizeOfData);
	DWORD SendResponseDataToClient(DWORD &refdwNumberOfRequest, std::string &refstrSendData);

	// Common Send Method
	DWORD SendDataToClient(std::string &refstrSendData);
public:
	CAYWorkerThread(SOCKET ClientSocket);
	~CAYWorkerThread();

	DWORD StartWorkerThread(char *pReceiveBuf, DWORD dwByteTransferred);
};

#endif