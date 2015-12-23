#ifndef _STORE_
#define _STORE_

#include "..\Common\Common.h"
#include "..\DataBase\DataBaseType.h"
#include <algorithm>
#include <functional>

class CStore
{
public:
	DWORD SortStoreEvaluationByAscendingOrder(ST_DB_RESULT &refstDBResult);
	VOID PruningOrderedDataByRequestedNumber(ST_DB_RESULT &refstDBResult, DWORD dwRequestedNumber);
};

#endif
