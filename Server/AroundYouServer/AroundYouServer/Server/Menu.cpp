#include "Menu.h"

bool CompareMenuEvaluation(ST_DB_RESULT_LINE &refstFirstDBResultLine, ST_DB_RESULT_LINE &refstSecondDBResultLine)
{
	DOUBLE dFirstValue = ::atof(refstFirstDBResultLine.vecstrResult[0].c_str());
	DOUBLE dSecondValue = ::atof(refstSecondDBResultLine.vecstrResult[0].c_str());
	return dFirstValue > dSecondValue;
}

DWORD CMenu::SortStoreEvaluationByAscendingOrder(ST_DB_RESULT &refstDBResult)
{
	ST_DB_RESULT stDBResult = refstDBResult;

	// use custom method that c++ provide
	std::sort(stDBResult.vecstDBResultLines.begin(), stDBResult.vecstDBResultLines.end(), CompareMenuEvaluation);
	refstDBResult = stDBResult;

	return E_RET_SUCCESS;
}

VOID CMenu::PruningOrderedDataByRequestedNumber(ST_DB_RESULT &refstDBResult, DWORD dwRequestedNumber)
{
	if (refstDBResult.vecstDBResultLines.size() < dwRequestedNumber) {
		return;
	}

	refstDBResult.vecstDBResultLines.resize(dwRequestedNumber);
}
