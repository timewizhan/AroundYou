#ifndef _AY_SERVER_
#define _AY_SERVER_

#include "..\Common\Common.h"
#include "..\HelpTool\HelpTool.h"
#include "AYServerStruct.h"

class CAYServer
{
	BOOL	m_bStartServer;

	ST_SERVER_INIT			m_stServerInit;
	ST_SERVER_IOCP_DATA		m_stServerIOCPData;
	ST_SERVER_WORKER_THREAD m_stServerWorkerThreads;
	ST_SERVER_STATUS		m_stServerStatus;

	CRITICAL_SECTION		m_CriticalSection;
	std::vector<ST_SERVER_CONNECTION>	m_vecstServerConnection;

	/*
		HelpTool is about system method
		ex) Getsystem, GetAddress etc..
	*/
	CHelpTool	*m_pHelpTool;

	DWORD InitServerSock(DWORD dwPort);
	DWORD InitIOCompletionPort(DWORD dwNumberOfConcurrentThreads);
	DWORD InitWorkerThread();

	/*
		InitServerValue method have internel method
		neccesary method (bind, listen)
	*/
	DWORD InitServerValue(DWORD dwPort, DWORD dwBackLog);
	DWORD AcceptServer(ST_CLIENT_SOCKET &refstClientSocket);
	DWORD CompleteReadFromClient(ST_CLIENT_SOCKET &refstClientSocket);

public:
	CAYServer();
	~CAYServer();

	DWORD StartServer(DWORD dwPort, DWORD dwBackLog);
	DWORD StopServer();
};

unsigned int WINAPI WorkerCompletionThread(void *pIOCPData);

#endif