#include "DummyClient.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDummyClient::CDummyClient()
{
	::memset(&m_stClientSock, 0x00, sizeof(m_stClientSock));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDummyClient::~CDummyClient()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CDummyClient::ConnectToServer(std::string &refstrAddrss, DWORD &refdwPort)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorLog("Fail to set WSA Startup");
		return E_RET_FAIL;
	}

	m_stClientSock.ConnectToServerSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_stClientSock.ConnectToServerSock == INVALID_SOCKET) {
		ErrorLog("Fail to set socket");
		return E_RET_FAIL;
	}

	m_stClientSock.stConnectServer.sin_family = AF_INET;
	m_stClientSock.stConnectServer.sin_port = htons((unsigned short)refdwPort);
	::inet_pton(AF_INET, refstrAddrss.c_str(), &m_stClientSock.stConnectServer.sin_addr.s_addr);


	if (::connect(m_stClientSock.ConnectToServerSock, (SOCKADDR *)&m_stClientSock.stConnectServer, sizeof(m_stClientSock.stConnectServer)) == SOCKET_ERROR) {
		ErrorLog("Fail to connect socket");
		return E_RET_FAIL;
	}
	return E_RET_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CDummyClient::SendMsg(std::string &refstrMsg)
{
	DWORD dwSent;
	DWORD dwSizeOfMsg = refstrMsg.size();

	dwSent = ::send(m_stClientSock.ConnectToServerSock, refstrMsg.c_str(), dwSizeOfMsg, 0);
	if (dwSent == SOCKET_ERROR) {
		return E_RET_FAIL;
	}

	DebugLog("Success to send to server [%s : %d]", refstrMsg.c_str(), dwSent);
	return E_RET_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CDummyClient::RecvMsg(std::string &refstrMsg)
{
	DWORD dwRecv;
	char szBuf[1024];
	dwRecv = ::recv(m_stClientSock.ConnectToServerSock, szBuf, 1024, 0);
	if (dwRecv == SOCKET_ERROR) {
		return E_RET_FAIL;
	}

	refstrMsg = szBuf;
	refstrMsg.resize(dwRecv);
	DebugLog("Success to recevie to server [%s : %d]", refstrMsg.c_str(), dwRecv);
	return E_RET_SUCCESS;
}