//
// Created by ENDERZOMBI102 on 28/03/2024.
//   Heavily modified version of the SDK 2006 version of `jobthread.cpp`
//
#include "platform.h"
#if IsWindows()
	#include <synchapi.h>
#endif
#include "jobthread.hpp"

CThreadPool::CThreadPool()
	: m_pPendingJob( nullptr ), m_Exit( true ) { }
CThreadPool::~CThreadPool() = default;

bool CThreadPool::Start( const ThreadPoolStartParams_t& startParams ) {
	for ( auto i{0}; i < startParams.nThreads; i += 1 ) {
		int iThread = this->m_Threads.AddToTail();
		this->m_IdleEvents.AddToTail();
		this->m_Threads[ iThread ] = CreateSimpleThread( PoolThreadFunc, this, startParams.nStackSize );
		this->m_IdleEvents[ iThread ].Wait();
	}

	if ( startParams.fDistribute == ThreeState_t::TRS_TRUE ) {
		// TODO
	}

	return true;
}
bool CThreadPool::Stop( int timeout ) {
	this->m_Exit.Set();
	#if IsWindows()
		return WaitForMultipleObjects( m_Threads.Count(), (HANDLE*) m_Threads.Base(), true, timeout );
	#else
		auto flag{ true };
		// FIXME: Timeout is not accumulated!
		for ( const auto& thread : this->m_Threads ) {
//			flag = ThreadJoin( thread, timeout ) && flag;
		}
		return flag;
	#endif
}

unsigned int CThreadPool::GetJobCount() {
	AssertUnreachable();
	return {};
}
int CThreadPool::NumThreads() {
	return this->m_Threads.Count();
}
int CThreadPool::NumIdleThreads() {
	return this->m_IdleCount;
}

int CThreadPool::SuspendExecution() {
	/*if ( !ThreadInMainThread() ) {
		Assert( 0 );
		return 0;
	}

	// If not already suspended
	if ( m_nSuspend == 0 ) {
		// Make sure state is correct
		int curCount = Suspend();
		Resume();
		Assert( curCount == 0 );

		if ( curCount == 0 ) {
			CallWorker( AF_SUSPEND );

			// Because worker must signal before suspending, we could reach
			// here with the thread not actually suspended
			while ( Suspend() == 0 ) {
				Resume();
				ThreadSleep();
			}
			Resume();
		}

	#if IsDebug()
		curCount = Suspend();
		Resume();
		Assert( curCount > 0 );
	#endif
	}

	return m_nSuspend++;*/
	AssertUnreachable();
	return {};
}
int CThreadPool::ResumeExecution() {
	/*if ( !ThreadInMainThread() ) {
		Assert( 0 );
		return 0;
	}

	AssertMsg( m_nSuspend >= 1, "Attempted resume when not suspended" );
	int result = m_nSuspend--;
	if ( m_nSuspend == 0 ) {
		Resume();
	}
	return result;*/
	AssertUnreachable();
	return {};
}

int CThreadPool::YieldWait( CThreadEvent * *pEvents, int nEvents, bool bWaitAll, unsigned timeout ) { AssertUnreachable(); return {}; }
int CThreadPool::YieldWait( CJob**, int nJobs, bool bWaitAll, unsigned timeout ) { AssertUnreachable(); return {}; }
void CThreadPool::Yield( unsigned timeout ) { AssertUnreachable(); }

void CThreadPool::AddJob( CJob* pJob ) {
	if (! pJob ) {
		return;
	}

	/*BoostPriority();

	// If queue is full, wait for worker to get something from our queue
	WaitPut();

	m_mutex.Lock();

	pJob->AddRef();

	unsigned i = m_queue.Tail();
	int priority = pJob->GetPriority();

	while ( i != m_queue.InvalidIndex() && priority > m_queue[ i ]->GetPriority() ) {
		i = m_queue.Previous( i );
	}

	if ( i != m_queue.InvalidIndex() ) {
		pJob->m_queueID = m_queue.InsertAfter( i, pJob );
	} else {
		pJob->m_queueID = m_queue.AddToHead( pJob );
	}

	pJob->m_pFulfiller = this;
	pJob->m_status = JOB_STATUS_PENDING;

	if ( m_queue.Count() == 1 ) {
		// Release worker to remove an object from our queue
		m_JobSignal.Set();
	}

	m_mutex.Unlock();*/
	AssertUnreachable();
}

void CThreadPool::ExecuteHighPriorityFunctor( CFunctor* pFunctor ) {
	AssertUnreachable();
}

void CThreadPool::ChangePriority( CJob* pJob, JobPriority_t priority ) {
	AssertUnreachable();
}

int CThreadPool::ExecuteToPriority( JobPriority_t toPriority, JobFilter_t pfnFilter ) {
	AssertUnreachable();
	return {};
}
int CThreadPool::AbortAll() {
	/*int nExecuted = 0;
	if ( CThread::GetCurrentCThread() != this ) {
		SuspendExecution();
	}

	if ( m_queue.Count() ) {
		CJob* pJob = NULL;

		while ( ( pJob = GetJob() ) != NULL ) {
			pJob->Abort();
			pJob->Release();
		}
	}

	if ( CThread::GetCurrentCThread() != this ) {
		ResumeExecution();
	}

	return nExecuted;*/
	AssertUnreachable();
	return {};
}

void CThreadPool::AddFunctorInternal( CFunctor* pFunctor, CJob** ppJob, const char* pszDescription, unsigned flags ) {
	AssertUnreachable();
}

CJob* CThreadPool::GetDummyJob() {
	AssertUnreachable();
	return {};
}

void CThreadPool::Distribute( bool bDistribute, int* pAffinityTable ) {
	AssertUnreachable();
}

bool CThreadPool::Start( const ThreadPoolStartParams_t& startParams, const char* pszNameOverride ) {
	AssertUnreachable();
	return {};
}

unsigned CThreadPool::PoolThreadFunc( void* pParam ) {
	/*CThreadPool* pOwner = (CThreadPool*) pParam;
	int iThread = pOwner->m_Threads.Count() - 1;
	pOwner->m_IdleEvents[ iThread ].Set();

	HANDLE waitHandles[] =
		{
			pOwner->m_JobAvailable,
			pOwner->m_Exit };

	DWORD waitResult;

	while ( ( waitResult = WaitForMultipleObjects( ARRAYSIZE( waitHandles ), waitHandles, false, INFINITE ) ) != WAIT_FAILED ) {
		switch ( waitResult - WAIT_OBJECT_0 ) {
			case 0: {
				pOwner->m_IdleEvents[ iThread ].Reset();
				CJob* pJob = pOwner->m_pPendingJob;
				pOwner->m_pPendingJob = NULL;
				pOwner->m_JobAccepted.Set();

				pJob->TryExecute();
				pJob->Release();

				pOwner->m_IdleEvents[ iThread ].Set();
				break;
			}

			case 1: {
				return 0;
			}
		}
	}*/

	AssertUnreachable();
	return 1;
}


/*
inline void CThreadPool::WaitPut() {
	if ( m_bUseSemaphore )
		m_PutSemaphore.Wait();
}

inline void CThreadPool::ReleasePut() {
#if IsWindows()
	if ( m_bUseSemaphore )
		m_PutSemaphore.Release();
#endif
}


bool CThreadPool::RemoveJob( CJob* pJob ) {
	if ( !pJob ) {
		return false;
	}

	AUTO_LOCK( m_mutex );

	if ( !m_queue.IsValidIndex( pJob->m_queueID ) ) {
		return false;
	}

	// Take the job out
	m_queue.Remove( pJob->m_queueID );
	pJob->m_queueID = m_queue.InvalidIndex();
	pJob->m_pFulfiller = NULL;
	pJob->Release();

	// Release master to put more in
	ReleasePut();

	// If we're transitioning to empty...
	if ( m_queue.Count() == 0 ) {
		// Block the worker until there's something to do...
		m_JobSignal.Reset();
	}

	return true;
}

int CThreadPool::ExecuteToPriority( JobPriority_t iToPriority ) {
	int nExecuted = 0;

	if ( CThread::GetCurrentCThread() != this ) {
		SuspendExecution();
	}

	if ( m_queue.Count() ) {
		CJob* pJob = NULL;

		while ( ( pJob = GetJob() ) != NULL ) {
			if ( pJob->GetPriority() >= iToPriority ) {
				pJob->Execute();
				pJob->Release();
				pJob = NULL;
				nExecuted++;
			} else {
				break;
			}
		}

		// Extracted one of lower priority, so reinsert it...
		if ( pJob ) {
			AddJob( pJob );
			pJob->Release();
		}
	}

	if ( CThread::GetCurrentCThread() != this ) {
		ResumeExecution();
	}
	return nExecuted;
}


CJob* CThreadPool::GetJob() {
	CJob* pReturn = NULL;
	m_mutex.Lock();
	unsigned i = m_queue.Head();

	if ( i != m_queue.InvalidIndex() ) {
		pReturn = m_queue[ i ];
		pReturn->AddRef();
		RemoveJob( pReturn );
	}

	m_mutex.Unlock();
	return pReturn;
}

bool CThreadPool::Start( unsigned nBytesStack ) {
	if ( CWorkerThread::Start( nBytesStack ) ) {
		BoostPriority();
		return true;
	}
	return false;
}

int CThreadPool::Run() {
	enum FulfillerEvent_t {
		CALL_FROM_MASTER,
		JOB_REQUEST
	};

	g_TestThreadPool.Init( 4, 0, 2, false );

	// Wait for either a call from the master thread, or an item in the queue...
	DWORD waitResult;
	bool bExit = false;
	HANDLE waitHandles[ 2 ];

	waitHandles[ CALL_FROM_MASTER ] = GetCallHandle();
	waitHandles[ JOB_REQUEST ] = GetJobSignalHandle();

	while ( !bExit &&
			( waitResult = WaitForMultipleObjects( 2, waitHandles, false, INFINITE ) ) != WAIT_FAILED ) {
		switch ( waitResult - WAIT_OBJECT_0 ) {
				// It's a call from the master thread...
			case CALL_FROM_MASTER: {
				switch ( GetCallParam() ) {
					case AF_EXIT:
						Reply( true );
						bExit = true;
						break;

					case AF_SUSPEND:
						g_TestThreadPool.WaitForIdle();
						Reply( true );
						Suspend();
						break;

					default:
						AssertMsg( 0, "Unknown call async fulfiller" );
						Reply( false );
						break;
				}
				break;
			}

				// Otherwise, if there's a read request to service...
			case JOB_REQUEST: {
				// Get the request
				CJob* pJob;

				while ( ( pJob = GetJob() ) != NULL ) {
					// Job can be NULL if the main thread may have preempted and fulfilled
					// the job on its own.
					g_TestThreadPool.Execute( pJob );
					pJob->Release();
				}

				break;
			}

			default:
				AssertMsg( 0, "There was nothing to do!" );
		}
	}

	g_TestThreadPool.Term();
	return 0;
}

//-----------------------------------------------------------------------------
// CJob
//-----------------------------------------------------------------------------
JobStatus_t CJob::Execute() {
	AUTO_LOCK( m_mutex );
	AddRef();

	JobStatus_t result;

	switch ( m_status ) {
		case JOB_STATUS_UNSERVICED:
		case JOB_STATUS_PENDING: {
			if ( m_pFulfiller )// Jobs can exist on thier own
			{
				CAutoLock autoLock( m_pFulfiller->GetQueueLock() );
				if ( m_pFulfiller ) {
					m_pFulfiller->RemoveJob( this );
				}
			}

			// Service it
			m_status = JOB_STATUS_INPROGRESS;
			ThreadSleep( 0 );
			result = m_status = DoExecute();
			DoCleanup();
			ThreadSleep( 0 );
			break;
		}

		case JOB_STATUS_INPROGRESS:
			AssertMsg( 0, "Mutex Should have protected use while processing" );
			// fall through...

		case JOB_OK:
		case JOB_STATUS_ABORTED:
			result = m_status;
			break;

		default:
			AssertMsg( m_status < JOB_OK, "Unknown async job state" );
			result = m_status;
	}

	Release();
	return result;
}

JobStatus_t CJob::TryExecute() {
	// TryLock() would only fail if another thread has entered
	// Execute() or Abort()
	if ( TryLock() ) {
		// ...service the request
		Execute();
		Unlock();
	}
	return m_status;
}

JobStatus_t CJob::Abort( bool bDiscard ) {
	AUTO_LOCK( m_mutex );
	AddRef();

	JobStatus_t result;

	switch ( m_status ) {
		case JOB_STATUS_UNSERVICED:
		case JOB_STATUS_PENDING: {
			if ( m_pFulfiller )// Jobs can exist on thier own
			{
				CAutoLock autoLock( m_pFulfiller->GetQueueLock() );
				if ( m_pFulfiller ) {
					m_pFulfiller->RemoveJob( this );
				}
			}

			result = m_status = DoAbort( bDiscard );
			if ( bDiscard )
				DoCleanup();
		} break;

		case JOB_STATUS_ABORTED:
		case JOB_STATUS_INPROGRESS:
		case JOB_OK:
			result = m_status;
			break;

		default:
			AssertMsg( m_status < JOB_OK, "Unknown async job state" );
			result = m_status;
	}

	Release();
	return result;
}
*/