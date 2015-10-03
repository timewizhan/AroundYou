#ifndef _SIGN_IN_
#define _SIGN_IN_

#include "..\Common\Common.h"
#include "..\DataBase\DataBaseType.h"
#include "AYProtocol.h"
#include "AYServerStruct.h"

class CSignIn
{
	DWORD CheckNickName(std::string &refstrReqNickName, std::string &refstrDBNickName);
	DWORD CheckEmail(std::string &refstrReqEmail, std::string &refstrDBEmail);
	DWORD CheckCallId(std::string &refstrReqCallId, std::string &refstrDBCallId);
public:
	CSignIn();
	~CSignIn();

	DWORD CheckSignInData(ST_PROTOCOL_SIGNIN *pstProtoSignIn, ST_DB_RESULT &refstDBResult);
};

#endif