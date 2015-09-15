#ifndef _DUMMY_CLIENT_
#define _DUMMY_CLIENT_

#include "..\Common\Common.h"
#include "DummyClientStruct.h"

class CDummyClient
{
	ST_CLIENT_SOCK m_stClientSock;
public:
	CDummyClient();
	~CDummyClient();

	DWORD ConnectToServer(std::string &refstrAddrss, DWORD &refdwPort);
	DWORD SendMsg(std::string &refstrMsg);
	DWORD RecvMsg(std::string &refstrMsg);
};

#endif