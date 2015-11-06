#include "Common\Common.h"
#include "Server\AYServer.h"
#include "Common\Log.h"

int main(int argc, char **argv)
{
	DWORD dwRet;
	dwRet = InitLog(E_LOG_CONSOLE);
	//dwRet = InitLog(E_LOG_FILE);

	if (argc < 2) {
		DebugLog("[Usage] : Server Port, Server BackLog");
		return E_RET_FAIL;
	}

	DebugLog("****************************************");
	DebugLog("******* Around You Server Init *********");
	DebugLog("****************************************");

	/*
		you have to configure argument for server
		argv[1] : Port
		argv[2] : Backlog
	*/
	std::string strPort, strBackLog;
	DWORD dwPort, dwBackLog;
	strPort = argv[1];
	dwPort = ::atoi(strPort.c_str());

	strBackLog = argv[2];
	dwBackLog = ::atoi(strBackLog.c_str());

	CAYServer *AYServer = NULL;
	AYServer = new CAYServer();
	if (!AYServer) {
		ErrorLog("Fail to get memory of AYServer");
		return E_RET_FAIL;
	}

	try
	{
		if (dwRet != E_RET_SUCCESS) {
			std::exception("Fail to init log");
		}

		DebugLog("****************************************");
		DebugLog("****** Around You Server Start *********");
		DebugLog("****************************************");

		dwRet = AYServer->StartServer(dwPort, dwBackLog);
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to start server");
		}

		dwRet = AYServer->StopServer();
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("Fail to stop server");
		}

	}
	catch (std::exception &e)
	{
		ErrorLog("****************************************");
		ErrorLog("****** Around You Server Exit  *********");
		ErrorLog("****************************************");
		ErrorLog("%s", e.what());

		if (AYServer)
			delete AYServer;

		DeleteCriticalSection(&CriticalSection);
		return E_RET_FAIL;
	}

	DebugLog("****************************************");
	DebugLog("****** Around You Server Exit  *********");
	DebugLog("****************************************");

	if (AYServer)
		delete AYServer;

	DeleteCriticalSection(&CriticalSection);
	return E_RET_SUCCESS;
}