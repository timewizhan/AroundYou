#ifndef _DUMMY_CLIENT_STRUCT_
#define _DUMMY_CLIENT_STRUCT_

#include "..\Common\Common.h"

struct ST_CLIENT_SOCK
{
	SOCKET ConnectToServerSock;
	SOCKADDR_IN stConnectServer;
};

#endif