#include "Comment.h"

VOID CComment::PruningOrderedDataByRequestedNumber(ST_DB_RESULT &refstDBResult, DWORD dwRequestedNumber)
{
	if (refstDBResult.vecstDBResultLines.size() < dwRequestedNumber) {
		return;
	}

	refstDBResult.vecstDBResultLines.resize(dwRequestedNumber);
}
