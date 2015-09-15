#ifndef _AY_WORKER_THREAD_
#define _AY_WORKER_THREAD_

#include "..\Common\Common.h"

class CAYWorkerThread
{
public:
	CAYWorkerThread();
	~CAYWorkerThread();

	DWORD StartWorkerThread(HANDLE hCompletionPort);
};

#endif