#include "Common\Common.h"
#include "DummyClient\DummyClient.h"

int main()
{
	DWORD dwPort = 5432;
	std::string strServerAddress = "165.132.120.160";

	CDummyClient DummyClient;
	DWORD dwRet;

	try
	{
		dwRet = DummyClient.ConnectToServer(strServerAddress, dwPort);
		if (dwRet != E_RET_SUCCESS) {
			std::exception("Fail to connect to server");
		}

		std::string strMsg = "Test : Hello World";
		dwRet = DummyClient.SendMsg(strMsg);
		if (dwRet != E_RET_SUCCESS) {
			std::exception("Fail to send msg to server");
		}

		std::string strRecvMsg;
		dwRet = DummyClient.RecvMsg(strRecvMsg);
		if (dwRet != E_RET_SUCCESS) {
			std::exception("Fail to receive msg to server");
		}
	}
	catch (std::exception &e)
	{
		ErrorLog("%s", e.what());
		return E_RET_FAIL;
	}

	return E_RET_SUCCESS;
}