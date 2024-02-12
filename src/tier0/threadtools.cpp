//
// Created by ENDERZOMBI102 on 10/02/2024.
//
#include "threadtools.h"
#if IsWindows()
	#include <synchapi.h>
	#include <processthreadsapi.h>
#elif IsPosix()
	#include <sys/time.h>
#endif


static ThreadedLoadLibraryFunc_t g_pThrLoadLibFunc;

// ----- SimpleThread_t -----
//
ThreadHandle_t CreateSimpleThread( ThreadFunc_t pHandle, void* pParam, ThreadId_t* pID, unsigned stackSize );
ThreadHandle_t CreateSimpleThread( ThreadFunc_t pHandle, void* pParam, unsigned stackSize );
bool ReleaseThreadHandle( ThreadHandle_t pHandle );

void ThreadSleep( unsigned pDurationMs ) {
	#if IsWindows()
		Sleep( pDuration );
	#elif IsLinux()
		usleep( pDurationMs );
	#elif IsPosix()
		timespec spec{ .tv_sec = 0, .tv_nsec = static_cast<long>( pDurationMs * 1000 ) };
		nanosleep( &spec, nullptr );
	#else
		#error "ThreadSleep: Missing implementation!"
	#endif
}
uint ThreadGetCurrentId() {
	#if IsWindows()
		return static_cast<uint>( GetCurrentThreadId() );
	#elif IsPosix()
		return pthread_self();
	#else
		#error "ThreadSleep: Missing implementation!"
	#endif
}
ThreadHandle_t ThreadGetCurrentHandle();
int ThreadGetPriority( ThreadHandle_t hThread );
bool ThreadSetPriority( ThreadHandle_t hThread, int priority );
bool ThreadInMainThread();
void DeclareCurrentThreadIsMainThread();

void SetThreadedLoadLibraryFunc( ThreadedLoadLibraryFunc_t func ) {
	g_pThrLoadLibFunc = func;
}
ThreadedLoadLibraryFunc_t GetThreadedLoadLibraryFunc() {
	return g_pThrLoadLibFunc;
}

bool ThreadJoin( ThreadHandle_t, unsigned timeout );
void ThreadDetach( ThreadHandle_t );

void ThreadSetDebugName( ThreadId_t id, const char* pszName );

void ThreadSetAffinity( ThreadHandle_t hThread, int nAffinityMask );

#if IsWindows()
	int ThreadWaitForObjects( int nEvents, const HANDLE* pHandles, bool bWaitAll, unsigned timeout );
#endif

// ----- ThreadInterlocked* -----
//
#if !defined( USE_INTRINSIC_INTERLOCKED )
	long ThreadInterlockedIncrement( long volatile* pIt ) { // NOLINT(*-non-const-parameter)
		// type __atomic_add_fetch(type *ptr, type val, int memorder)
		return __atomic_add_fetch( pIt, 1, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedDecrement( long volatile* pIt ) { // NOLINT(*-non-const-parameter)
		// type __atomic_sub_fetch(type *ptr, type val, int memorder)
		return __atomic_sub_fetch( pIt, 1, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedExchange( long volatile* pIt, long pValue ) { // NOLINT(*-non-const-parameter)
		// type __atomic_exchange_n(type *ptr, type val, int memorder)
		return __atomic_exchange_n( pIt, pValue, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedExchangeAdd( long volatile* pIt, long pValue ) { // NOLINT(*-non-const-parameter)
		// type __atomic_fetch_add(type *ptr, type val, int memorder)
		return __atomic_fetch_add( pIt, pValue, __ATOMIC_ACQ_REL );
	}
	long ThreadInterlockedCompareExchange( long volatile* pIt, long pValue, long comperand ) { // NOLINT(*-non-const-parameter)
		long last;
		long expected = comperand;
		do {
			// void __atomic_load (type *ptr, type *ret, int memorder)
			__atomic_load( pIt, &last, __ATOMIC_RELAXED );
			// bool __atomic_compare_exchange_n(type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)
		} while (! __atomic_compare_exchange_n( pIt, &expected, pValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED ) );
		return last;
	}
	bool ThreadInterlockedAssignIf( long volatile* pIt, long value, long comperand ) { // NOLINT(*-non-const-parameter)
		// bool __atomic_compare_exchange_n(type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)
		return __atomic_compare_exchange_n( pIt, &comperand, value, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED );
	}
#endif
int64 ThreadInterlockedIncrement64( int64 volatile* pIt ) {// NOLINT(*-non-const-parameter)
	return __atomic_add_fetch( pIt, 1, __ATOMIC_ACQ_REL );
}
int64 ThreadInterlockedDecrement64( int64 volatile* pIt ) {// NOLINT(*-non-const-parameter)
	return __atomic_sub_fetch( pIt, 1, __ATOMIC_ACQ_REL );
}
int64 ThreadInterlockedCompareExchange64( int64 volatile* pIt, int64 pValue, int64 comperand ) {// NOLINT(*-non-const-parameter)
	int64 last;
	int64 expected = comperand;
	do __atomic_load( pIt, &last, __ATOMIC_RELAXED );
	while (! __atomic_compare_exchange_n( pIt, &expected, pValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED ) );
	return last;
}
int64 ThreadInterlockedExchange64( int64 volatile* pIt, int64 pValue ) {                        // NOLINT(*-non-const-parameter)
	return __atomic_exchange_n( pIt, pValue, __ATOMIC_ACQ_REL );
}
int64 ThreadInterlockedExchangeAdd64( int64 volatile* pIt, int64 pValue ) {// NOLINT(*-non-const-parameter)
	return __atomic_fetch_add( pIt, pValue, __ATOMIC_ACQ_REL );
}
bool ThreadInterlockedAssignIf64( volatile int64* pDest, int64 pValue, int64 comperand ) { // NOLINT(*-non-const-parameter)
	return __atomic_compare_exchange_n( pDest, &comperand, pValue, false, __ATOMIC_SEQ_CST, __ATOMIC_RELAXED );
}

// ----- CThreadRWLock -----
//
void CThreadRWLock::LockForWrite() {
	this->m_mutex.Lock();
	if ( this->m_nActiveReaders ) {
		this->m_CanWrite.Wait();
	}
	this->m_nWriters += 1;
	this->m_mutex.Unlock();
}
void CThreadRWLock::UnlockWrite() {
	this->m_mutex.Lock();
	this->m_nWriters -= 1;
	if ( this->m_nWriters == 0 && this->m_nPendingReaders != 0 ) {
		this->m_CanRead.Set();
	}
	this->m_mutex.Unlock();
}
void CThreadRWLock::WaitForRead() {
	this->m_nPendingReaders += 1;
	this->m_CanRead.Wait();
}

// ----- CThreadSpinRWLock -----
//
void CThreadSpinRWLock::LockForRead() {
	this->m_lockInfo.m_nReaders += 1;
}
void CThreadSpinRWLock::SpinLockForWrite( const uint32 pThreadId ) {
	while ( this->m_lockInfo.m_nReaders ) ;
	while (! this->m_nWriters.AssignIf( 0, 1 ) ) ;
	this->m_lockInfo.m_writerId = pThreadId;
}
void CThreadSpinRWLock::UnlockRead() {
	this->m_lockInfo.m_nReaders -= 1;
}
void CThreadSpinRWLock::UnlockWrite() {
	this->m_nWriters.AssignIf( 1, 0 );
}

// ----- CThreadFastMutex -----
//
void CThreadFastMutex::Lock( const uint32 pThreadId, unsigned nSpinSleepTime ) volatile {
	while (! this->TryLockInline( pThreadId ) )
		ThreadSleep( nSpinSleepTime );
}

// ----- CThreadSyncObject -----
//
CThreadSyncObject::CThreadSyncObject() {
	pthread_mutex_init( &this->m_Mutex, nullptr );
	pthread_cond_init( &this->m_Condition, nullptr );
	this->m_bInitalized = true;
}
CThreadSyncObject::~CThreadSyncObject() {
	if ( this->m_bInitalized ) {
		pthread_cond_destroy( &this->m_Condition );
		pthread_mutex_destroy( &this->m_Mutex );
		this->m_bInitalized = false;
	}
}
bool CThreadSyncObject::operator!() const {
	return ! this->m_bInitalized;
}
bool CThreadSyncObject::Wait( uint32 dwTimeoutMs ) {
	pthread_mutex_lock( &this->m_Mutex );
	timeval val{};
	gettimeofday( &val, nullptr );
	timespec spec{
		.tv_sec = val.tv_sec,
		.tv_nsec = static_cast<long>( dwTimeoutMs * 1000 ) + val.tv_usec / 1000 // TODO: Verify this
	};
	pthread_cond_timedwait( &this->m_Condition, &this->m_Mutex, &spec );
	pthread_mutex_unlock( &this->m_Mutex );
}
void CThreadSyncObject::AssertUseable() {
	#if IsDebug()
		Assert( this->m_bInitalized ); // TODO: ????
	#endif
}
// ----- CThreadEvent -----
//
CThreadEvent::CThreadEvent( bool fManualReset ) {
	this->m_bManualReset = fManualReset;
}
bool CThreadEvent::Set() {
	pthread_mutex_lock( &this->m_Mutex );
	this->m_cSet = 1;
	// wake up waiting threads
	auto err{ pthread_cond_signal( &this->m_Condition ) };
	pthread_mutex_unlock( &this->m_Mutex );
	// true if no error occurred, false otherwise. TODO?: return the error code instead of just bool
	return err == 0;
}
bool CThreadEvent::Reset() {
	this->m_cSet = false;
}
bool CThreadEvent::Check() {
	pthread_mutex_lock( &this->m_Mutex );
	auto isSet{ this->m_cSet == 1 };
	pthread_mutex_unlock( &this->m_Mutex );

	return isSet;
}
bool CThreadEvent::Wait( uint32 dwTimeout ) {
	return CThreadSyncObject::Wait( dwTimeout );
}




