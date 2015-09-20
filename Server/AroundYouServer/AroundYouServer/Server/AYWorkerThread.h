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

	ST_PROTOCOL_CLIENT	*m_pstProtoClient;
	ST_PROTOCOL_MENU	*m_pstProtoMenu;
	ST_PROTOCOL_COMMENT *m_pstProtoComment;
	ST_PROTOCOL_SHOP	*m_pstProtoShop;

	DWORD ReceiveDataFromClient(char *pReceiveBuf);
	DWORD ParseDataByJSON(DWORD &refdwNumberOfRequestvoid);
	DWORD RequestToDataBase(DWORD &refdwNumberOfRequest, ST_DB_RESULT &refstDBResult);
	DWORD MakeSendPacket(DWORD &refdwNumberOfRequest, ST_DB_RESULT &refstDBResult, std::string &refstrSendData);
	DWORD SendDataToClient(std::string &refstrSendData);
public:
	CAYWorkerThread(SOCKET ClientSocket);
	~CAYWorkerThread();

	DWORD StartWorkerThread(char *pReceiveBuf);
};

#endif