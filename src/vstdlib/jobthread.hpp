//
// Created by ENDERZOMBI102 on 28/03/2024.
//
#pragma once

#include "vstdlib/jobthread.h"
#include "tier0/dbg.h"

#include "tier1/utlvector.h"
//-----------------------------------------------------------------------------

class CThreadPool : public IThreadPool {
public:
	CThreadPool();
	~CThreadPool() override;

	// Thread functions
	bool Start( const ThreadPoolStartParams_t& startParams = ThreadPoolStartParams_t() ) override;
	bool Stop( int timeout = TT_INFINITE ) override;

	// Functions for any thread
	unsigned GetJobCount() override;
	int NumThreads() override;
	int NumIdleThreads() override;

	// Pause/resume processing jobs
	int SuspendExecution() override;
	int ResumeExecution() override;

	// Offer the current thread to the pool
	int YieldWait( CThreadEvent * *pEvents, int nEvents, bool bWaitAll = true, unsigned timeout = TT_INFINITE ) override;
	int YieldWait( CJob**, int nJobs, bool bWaitAll = true, unsigned timeout = TT_INFINITE ) override;
	void Yield( unsigned timeout ) override;

	// Add a native job to the queue (master thread)
	void AddJob( CJob* ) override;

	// All threads execute pFunctor asap. Thread will either wake up
	//  and execute or execute pFunctor right after completing current job and
	//  before looking for another job.
	void ExecuteHighPriorityFunctor( CFunctor * pFunctor ) override;

	// Change the priority of an active job
	void ChangePriority( CJob * p, JobPriority_t priority ) override;

	// Bulk job manipulation (blocking)
	int ExecuteToPriority( JobPriority_t toPriority, JobFilter_t pfnFilter = nullptr ) override;
	int AbortAll() override;

	//-----------------------------------------------------
	void Reserved1() override { }

private:
	void AddFunctorInternal( CFunctor*, CJob** = nullptr, const char* pszDescription = nullptr, unsigned flags = 0 ) override;

	// Services for internal use by job instances
	friend class CJob;

	CJob* GetDummyJob() override;

public:
	void Distribute( bool bDistribute = true, int* pAffinityTable = nullptr ) override;

	bool Start( const ThreadPoolStartParams_t& startParams, const char* pszNameOverride ) override;

// og
private:
	static unsigned PoolThreadFunc( void* pParam );

private:
	CJob* m_pPendingJob;

	CThreadEvent m_JobAvailable;
	CThreadEvent m_JobAccepted;
	CThreadEvent m_Exit;
	CInterlockedInt m_IdleCount;

	CUtlVector<ThreadHandle_t> m_Threads;
	CUtlVector<CThreadManualEvent> m_IdleEvents;
};
