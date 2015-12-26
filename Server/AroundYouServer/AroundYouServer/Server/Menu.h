#ifndef _MENU_
#define _MENU_

#include "..\Common\Common.h"
#include "..\DataBase\DataBaseType.h"
#include <algorithm>
#include <functional>

class CMenu
{
public:
	DWORD SortStoreEvaluationByAscendingOrder(ST_DB_RESULT &refstDBResult);
	VOID PruningOrderedDataByRequestedNumber(ST_DB_RESULT &refstDBResult, DWORD dwRequestedNumber);
};

#endif