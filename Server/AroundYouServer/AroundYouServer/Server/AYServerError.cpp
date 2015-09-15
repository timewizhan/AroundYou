#include "AYServerError.h"
#include "..\Common\Log.h"

VOID ShowErrorWSAStartup(DWORD dwErr)
{
	switch (dwErr)
	{
	case WSASYSNOTREADY:
		ErrorLog("The underlying network subsystem is not ready for network communication.");
		break;
	case WSAVERNOTSUPPORTED:
		ErrorLog("The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
		break;
	case WSAEINPROGRESS:
		ErrorLog("A blocking Windows Sockets 1.1 operation is in progress.");
		break;
	case WSAEPROCLIM:
		ErrorLog("A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
		break;
	case WSAEFAULT:
		ErrorLog("The lpWSAData parameter is not a valid pointer.");
		break;
	default:
		ErrorLog("Cannot find Error Type");
		break;
	}
}

VOID ShowErrorWSASocket(DWORD dwErr)
{
	switch (dwErr)
	{
	case WSANOTINITIALISED:
	case WSAENETDOWN:
	case WSAEAFNOSUPPORT:
	case WSAEFAULT:
	case WSAEINPROGRESS:
	case WSAEINVAL:
	case WSAEINVALIDPROVIDER:
	case WSAEINVALIDPROCTABLE:
	case WSAEMFILE:
	case WSAENOBUFS:
	case WSAEPROTONOSUPPORT:
	case WSAEPROTOTYPE:
	case WSAEPROVIDERFAILEDINIT:
	case WSAESOCKTNOSUPPORT:
		break;
	default:
		ErrorLog("Cannot find Error Type");
		break;
	}
}

VOID ShowErrorSetSockOpt(DWORD dwErr)
{
	switch (dwErr)
	{
	case WSANOTINITIALISED:
		ErrorLog("A successful WSAStartup call must occur before using this function.");
		break;
	case WSAENETDOWN:
		ErrorLog("The network subsystem has failed.");
		break;
	case WSAEFAULT:
		ErrorLog("The buffer pointed to by the optval parameter is not in a valid part of the process address space or the optlen parameter is too small.");
		break;
	case WSAEINPROGRESS:
		ErrorLog("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
		break;
	case WSAEINVAL:
		ErrorLog("The level parameter is not valid, or the information in the buffer pointed to by the optval parameter is not valid.");
		break;
	case WSAENETRESET:
		ErrorLog("The connection has timed out when SO_KEEPALIVE is set.");
		break;
	case WSAENOPROTOOPT:
		ErrorLog("The option is unknown or unsupported for the specified provider or socket (see SO_GROUP_PRIORITY limitations).");
		break;
	case WSAENOTCONN:
		ErrorLog("The connection has been reset when SO_KEEPALIVE is set.");
		break;
	case WSAENOTSOCK:
		ErrorLog("The descriptor is not a socket.");
		break;
	default:
		ErrorLog("Cannot find Error Type");
		break;
	}
}