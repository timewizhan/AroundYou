#include "SignIn.h"

CSignIn::CSignIn()
{
}

CSignIn::~CSignIn()
{
}

DWORD CSignIn::CheckNickName(std::string &refstrReqNickName, std::string &refstrDBNickName)
{
	if (refstrReqNickName.compare(refstrDBNickName) != 0) {
		return E_PROTO_RES_DATA_NOT_FOUND_NICKNAME;
	}
	return E_RET_SUCCESS;
}

DWORD CSignIn::CheckEmail(std::string &refstrReqEmail, std::string &refstrDBEmail)
{
	if (refstrReqEmail.compare(refstrDBEmail) != 0) {
		return E_PROTO_RES_DATA_NOT_FOUND_EMAIL;
	}
	return E_RET_SUCCESS;
}

DWORD CSignIn::CheckCallId(std::string &refstrReqCallId, std::string &refstrDBCallId)
{
	if (refstrReqCallId.compare(refstrDBCallId) != 0) {
		return E_PROTO_RES_DATA_NOT_FOUND_CALLID;
	}
	return E_RET_SUCCESS;
}

DWORD CSignIn::CheckSignInData(ST_PROTOCOL_SIGNIN *pstProtoSignIn, ST_DB_RESULT &refstDBResult)
{
	/*
		User id have only one
	*/
	if (refstDBResult.vecstDBResultLines.size() != 1) {
		return E_RET_FAIL;
	}

	ST_DB_RESULT_LINE stDBResultLine = refstDBResult.vecstDBResultLines[0];
	std::string strEmail = stDBResultLine.vecstrResult[0];
	std::string strCallId = stDBResultLine.vecstrResult[1];
	std::string strNickName = stDBResultLine.vecstrResult[2];

	DWORD dwRet;
	dwRet = CheckEmail(pstProtoSignIn->strEmail, strEmail);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = CheckNickName(pstProtoSignIn->strNickName, strNickName);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	dwRet = CheckCallId(pstProtoSignIn->strCallID, strCallId);
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}

	return E_RET_SUCCESS;
}