#ifndef _COMMENT_
#define _COMMENT_

#include "..\Common\Common.h"
#include "..\DataBase\DataBaseType.h"
#include <algorithm>
#include <functional>

class CComment
{
public:
	VOID PruningOrderedDataByRequestedNumber(ST_DB_RESULT &refstDBResult, DWORD dwRequestedNumber);
};

#endif
