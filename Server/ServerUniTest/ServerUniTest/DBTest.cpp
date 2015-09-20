#include "Common.h"

#include "..\..\AroundYouServer\AroundYouServer\DataBase\DataBase.h"

TEST(DATABASE, SELECT_SQL)
{
	ST_DB_LOGIN_TOKEN stDBLoginToken;
	stDBLoginToken.strDatabaseName = "postgres";
	stDBLoginToken.strDatabaseIP = "165.132.120.155";
	stDBLoginToken.strUserName = "postgres";
	stDBLoginToken.strPassword = "cclabiu";
	stDBLoginToken.strPort = "5432";

	HANDLE hDataBase = NULL;
	hDataBase = CreateDBInstance(E_DB_POSTGRES);
	ASSERT_TRUE(hDataBase != NULL);

	DWORD dwRet;
	dwRet = ConnectToDB(hDataBase, stDBLoginToken);
	ASSERT_TRUE(dwRet == E_RET_SUCCESS);

	/*
		Client Table
	*/
	ST_DB_SQL stDBQuerySQLFromClient;
	stDBQuerySQLFromClient.strSQL = "SELECT clientid, clientmail, clientphonenumber, \"clientNumber\" FROM \"client\" WHERE clientid = \'test\'";
	ST_DB_RESULT stDBResult;
	dwRet = QueryFromDB(hDataBase, stDBQuerySQLFromClient, stDBResult);
	ASSERT_TRUE(dwRet == E_RET_SUCCESS);

	if (stDBResult.vecstrResult.size() < 1) {
		return;
	}

	/*
		Comment Table
	*/
	ST_DB_SQL stDBQuerySQLFromComment;
	stDBQuerySQLFromComment.strSQL = "SELECT commentnumber, clientid, commenttime, commentcotent, storenumber FROM \"comment\" WHERE clientid = \'test\'";
	stDBResult.vecstrResult.clear();
	dwRet = QueryFromDB(hDataBase, stDBQuerySQLFromComment, stDBResult);
	ASSERT_TRUE(dwRet == E_RET_SUCCESS);

	if (stDBResult.vecstrResult.size() < 1) {
		return;
	}

	/*
		Menu Table
	*/
	ST_DB_SQL stDBQuerySQLFromMenu;
	stDBQuerySQLFromMenu.strSQL = "SELECT menunumber, menuname, menuprice, storename FROM \"menu\" WHERE menuname = \'test\'";
	stDBResult.vecstrResult.clear();
	dwRet = QueryFromDB(hDataBase, stDBQuerySQLFromMenu, stDBResult);
	ASSERT_TRUE(dwRet == E_RET_SUCCESS);

	/*
		Shop Table
	*/
	ST_DB_SQL stDBQuerySQLFromShop;
	stDBQuerySQLFromShop.strSQL = "SELECT shopnumber, shopname, shopaddress, shopphonenumber, shopcategory, \
		shophompage, shoprunningtime, shopholiday, shopetc, shopsmoking,\
		shopparking, shopalcohol, shopfacility, shopintroduction FROM \"shop\" WHERE shopname = \'test\'";
	stDBResult.vecstrResult.clear();
	dwRet = QueryFromDB(hDataBase, stDBQuerySQLFromShop, stDBResult);
	ASSERT_TRUE(dwRet == E_RET_SUCCESS);

	dwRet = QuitDB(hDataBase);
	ASSERT_TRUE(dwRet == E_RET_SUCCESS);
}