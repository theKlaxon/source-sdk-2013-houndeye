//
// Created by ENDERZOMBI102 on 22/02/2024.
//
#include "filesystem.hpp"
#include "interface.h"

static CFileSystemStdio g_FullFileSystem{};

// ---------------
// AppSystem
// ---------------
bool CFileSystemStdio::Connect( CreateInterfaceFn factory ) {
//	AssertUnreachable();
	return true;
}
void CFileSystemStdio::Disconnect() {
	// AssertUnreachable();
}
void* CFileSystemStdio::QueryInterface( const char* pInterfaceName ) {
	// AssertUnreachable();
	if ( strcmp( pInterfaceName, FILESYSTEM_INTERFACE_VERSION ) == 0 )
		return &g_FullFileSystem;

	return nullptr;
}
InitReturnVal_t CFileSystemStdio::Init() {
//	AssertUnreachable();
	return InitReturnVal_t::INIT_OK;
}
void CFileSystemStdio::Shutdown() {
//	AssertUnreachable();
}


// ---------------
// IBaseFilesystem
// ---------------
int CFileSystemStdio::Read( void* pOutput, int size, FileHandle_t file ) { AssertUnreachable(); return {}; }
int CFileSystemStdio::Write( void const* pInput, int size, FileHandle_t file ) { AssertUnreachable(); return {}; }

FileHandle_t CFileSystemStdio::Open( const char* pFileName, const char* pOptions, const char* pathID ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::Close( FileHandle_t file ) { AssertUnreachable(); }

void CFileSystemStdio::Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType ) { AssertUnreachable(); }
unsigned int CFileSystemStdio::Tell( FileHandle_t file ) { AssertUnreachable(); return {}; }
unsigned int CFileSystemStdio::Size( FileHandle_t file ) { AssertUnreachable(); return {}; }
unsigned int CFileSystemStdio::Size( const char* pFileName, const char* pPathID ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::Flush( FileHandle_t file ) { AssertUnreachable(); }
bool CFileSystemStdio::Precache( const char* pFileName, const char* pPathID ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::FileExists( const char* pFileName, const char* pPathID ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::IsFileWritable( char const* pFileName, const char* pPathID ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::SetFileWritable( char const* pFileName, bool writable, const char* pPathID ) { AssertUnreachable(); return {}; }

long CFileSystemStdio::GetFileTime( const char* pFileName, const char* pPathID ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::ReadFile( const char* pFileName, const char* pPath, CUtlBuffer& buf, int nMaxBytes, int nStartingByte, FSAllocFunc_t pfnAlloc ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::WriteFile( const char* pFileName, const char* pPath, CUtlBuffer& buf ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::UnzipFile( const char* pFileName, const char* pPath, const char* pDestination ) { AssertUnreachable(); return {}; }


// ---------------
// IFileSystem
// ---------------
// ---- Steam operations ----
bool CFileSystemStdio::IsSteam() const {
	return false;
}

FilesystemMountRetval_t CFileSystemStdio::MountSteamContent( int nExtraAppId ) {
	return FilesystemMountRetval_t::FILESYSTEM_MOUNT_FAILED;
}


// ---- Search path manipulation ----
void CFileSystemStdio::AddSearchPath( const char* pPath, const char* pathID, SearchPathAdd_t addType ) {

}
bool CFileSystemStdio::RemoveSearchPath( const char* pPath, const char* pathID ) {
	auto index{ this->m_SearchPaths.Find( pathID ) };
	if ( index == SearchPathMap::InvalidIndex() )
		return false;

	auto& searchPath{ this->m_SearchPaths[index] };
	for ( int i{0}; i < searchPath.Count(); i += 1 ) {
		if ( V_strcmp( searchPath[i]->GetNativePath(), pPath ) == 0 ) {
			searchPath[i]->Shutdown();
			searchPath.Remove( i );
			return true;
		}
	}
	return false;
}

void CFileSystemStdio::RemoveAllSearchPaths() {
	for ( auto& [pathId, searchPath] : this->m_SearchPaths ) {
		for ( auto& system : searchPath )
			system->Shutdown();
		searchPath.RemoveAll();
	}
	this->m_SearchPaths.RemoveAll();
}

void CFileSystemStdio::RemoveSearchPaths( const char* szPathID ) {
	for ( auto& [pathId, searchPath] : this->m_SearchPaths ) {
		if ( strcmp( pathId, szPathID ) == 0 ) {
			// first shutdown and remove all systems
			for ( auto& system: searchPath )
				system->Shutdown();
			searchPath.RemoveAll();
			// then remove the path itself
			this->m_SearchPaths.Remove( szPathID );
			break;
		}
	}
}

void CFileSystemStdio::MarkPathIDByRequestOnly( const char* pPathID, bool bRequestOnly ) { AssertUnreachable(); }

const char* CFileSystemStdio::RelativePathToFullPath( const char* pFileName, const char* pPathID, char* pDest, int maxLenInChars, PathTypeFilter_t pathFilter, PathTypeQuery_t* pPathType ) { AssertUnreachable(); return {}; }

int CFileSystemStdio::GetSearchPath( const char* pathID, bool bGetPackFiles, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::AddPackFile( const char* fullpath, const char* pathID ) { AssertUnreachable(); return {}; }

// ---- File manipulation operations ----
void CFileSystemStdio::RemoveFile( char const* pRelativePath, const char* pathID ) { AssertUnreachable(); }

bool CFileSystemStdio::RenameFile( char const* pOldPath, char const* pNewPath, const char* pathID ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::CreateDirHierarchy( const char* path, const char* pathID ) { AssertUnreachable(); }

bool CFileSystemStdio::IsDirectory( const char* pFileName, const char* pathID ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::FileTimeToString( char* pStrip, int maxCharsIncludingTerminator, long fileTime ) { AssertUnreachable(); }

// ---- Open file operations ----
void CFileSystemStdio::SetBufferSize( FileHandle_t file, unsigned nBytes ) { AssertUnreachable(); }

bool CFileSystemStdio::IsOk( FileHandle_t file ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::EndOfFile( FileHandle_t file ) { AssertUnreachable(); return {}; }

char* CFileSystemStdio::ReadLine( char* pOutput, int maxChars, FileHandle_t file ) { AssertUnreachable(); return {}; }
int CFileSystemStdio::FPrintf( FileHandle_t file, PRINTF_FORMAT_STRING const char* pFormat, ... ) { AssertUnreachable(); return {}; }

// ---- Dynamic library operations ----
CSysModule* CFileSystemStdio::LoadModule( const char* pFileName, const char* pPathID, bool bValidatedDllOnly ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::UnloadModule( CSysModule * pModule ) { AssertUnreachable(); }

// ---- File searching operations -----
const char* CFileSystemStdio::FindFirst( const char* pWildCard, FileFindHandle_t* pHandle ) { AssertUnreachable(); return {}; }
const char* CFileSystemStdio::FindNext( FileFindHandle_t handle ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::FindIsDirectory( FileFindHandle_t handle ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::FindClose( FileFindHandle_t handle ) { AssertUnreachable(); }

const char* CFileSystemStdio::FindFirstEx( const char* pWildCard, const char* pPathID, FileFindHandle_t* pHandle ) { AssertUnreachable(); return {}; }

// ---- File name and directory operations ----
const char* CFileSystemStdio::GetLocalPath( const char* pFileName, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::FullPathToRelativePath( const char* pFullpath, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::GetCurrentDirectory( char* pDirectory, int maxlen ) { AssertUnreachable(); return {}; }

// ---- Filename dictionary operations ----
FileNameHandle_t CFileSystemStdio::FindOrAddFileName( char const* pFileName ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::String( const FileNameHandle_t& handle, char* buf, int buflen ) { AssertUnreachable(); return {}; }

// ---- Global Asynchronous file operations ----
FSAsyncStatus_t CFileSystemStdio::AsyncReadMultiple( const FileAsyncRequest_t* pRequests, int nRequests, FSAsyncControl_t* phControls ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncAppend( const char* pFileName, const void* pSrc, int nSrcBytes, bool bFreeMemory, FSAsyncControl_t* pControl ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncAppendFile( const char* pAppendToFileName, const char* pAppendFromFileName, FSAsyncControl_t* pControl ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::AsyncFinishAll( int iToPriority ) { AssertUnreachable(); }
void CFileSystemStdio::AsyncFinishAllWrites() { AssertUnreachable(); }
FSAsyncStatus_t CFileSystemStdio::AsyncFlush() { AssertUnreachable(); return {}; }
bool CFileSystemStdio::AsyncSuspend() { AssertUnreachable(); return {}; }
bool CFileSystemStdio::AsyncResume() { AssertUnreachable(); return {}; }

void CFileSystemStdio::AsyncAddFetcher( IAsyncFileFetch * pFetcher ) { AssertUnreachable(); }
void CFileSystemStdio::AsyncRemoveFetcher( IAsyncFileFetch * pFetcher ) { AssertUnreachable(); }

FSAsyncStatus_t CFileSystemStdio::AsyncBeginRead( const char* pszFile, FSAsyncFile_t* phFile ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncEndRead( FSAsyncFile_t hFile ) { AssertUnreachable(); return {}; }

// ---- Asynchronous Request management ----
FSAsyncStatus_t CFileSystemStdio::AsyncFinish( FSAsyncControl_t hControl, bool wait ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncGetResult( FSAsyncControl_t hControl, void** ppData, int* pSize ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncAbort( FSAsyncControl_t hControl ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncStatus( FSAsyncControl_t hControl ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncSetPriority( FSAsyncControl_t hControl, int newPriority ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::AsyncAddRef( FSAsyncControl_t hControl ) { AssertUnreachable(); }
void CFileSystemStdio::AsyncRelease( FSAsyncControl_t hControl ) { AssertUnreachable(); }

// ---- Remote resource management ----
WaitForResourcesHandle_t CFileSystemStdio::WaitForResources( const char* resourcelist ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float* progress, bool* complete ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::CancelWaitForResources( WaitForResourcesHandle_t handle ) { AssertUnreachable(); }

int CFileSystemStdio::HintResourceNeed( const char* hintlist, int forgetEverything ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::IsFileImmediatelyAvailable( const char* pFileName ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::GetLocalCopy( const char* pFileName ) { AssertUnreachable(); }

// ---- Debugging operations ----
void CFileSystemStdio::PrintOpenedFiles() { AssertUnreachable(); }
void CFileSystemStdio::PrintSearchPaths() {
	Log( "---- Search Path table ----\n" );
	for ( const auto& [searchPathId, searchPath] : this->m_SearchPaths ) {
		Log( "%s:\n", searchPathId );
		for ( const auto& path: searchPath ) {
			Log( "  - %s\n", path->GetNativePath() );
		}
	}
}

void CFileSystemStdio::SetWarningFunc( void ( *pfnWarning )( PRINTF_FORMAT_STRING const char* fmt, ... ) ) { AssertUnreachable(); }
void CFileSystemStdio::SetWarningLevel( FileWarningLevel_t level ) { AssertUnreachable(); }
void CFileSystemStdio::AddLoggingFunc( void ( *pfnLogFunc )( const char* fileName, const char* accessType ) ) { AssertUnreachable(); }
void CFileSystemStdio::RemoveLoggingFunc( FileSystemLoggingFunc_t logFunc ) { AssertUnreachable(); }

const FileSystemStatistics* CFileSystemStdio::GetFilesystemStatistics() { AssertUnreachable(); return {}; }

// ---- Start of new functions after Lost Coast release (7/05) ----
FileHandle_t CFileSystemStdio::OpenEx( const char* pFileName, const char* pOptions, unsigned flags, const char* pathID, char** ppszResolvedFilename ) { AssertUnreachable(); return {}; }

int CFileSystemStdio::ReadEx( void* pOutput, int sizeDest, int size, FileHandle_t file ) { AssertUnreachable(); return {}; }
int CFileSystemStdio::ReadFileEx( const char* pFileName, const char* pPath, void** ppBuf, bool bNullTerminate, bool bOptimalAlloc, int nMaxBytes, int nStartingByte, FSAllocFunc_t pfnAlloc ) { AssertUnreachable(); return {}; }

FileNameHandle_t CFileSystemStdio::FindFileName( char const* pFileName ) { AssertUnreachable(); return {}; }

#if defined( TRACK_BLOCKING_IO )
	void CFileSystemStdio::EnableBlockingFileAccessTracking( bool state ) { AssertUnreachable(); }
	bool CFileSystemStdio::IsBlockingFileAccessEnabled() const { AssertUnreachable(); return {}; }

	IBlockingFileItemList* CFileSystemStdio::RetrieveBlockingFileAccessInfo() { AssertUnreachable(); return {}; }
#endif

void CFileSystemStdio::SetupPreloadData() { AssertUnreachable(); }
void CFileSystemStdio::DiscardPreloadData() { AssertUnreachable(); }

void CFileSystemStdio::LoadCompiledKeyValues( KeyValuesPreloadType_t type, char const* archiveFile ) { AssertUnreachable(); }

KeyValues* CFileSystemStdio::LoadKeyValues( KeyValuesPreloadType_t type, char const* filename, char const* pPathID ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::LoadKeyValues( KeyValues & head, KeyValuesPreloadType_t type, char const* filename, char const* pPathID ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::ExtractRootKeyName( KeyValuesPreloadType_t type, char* outbuf, size_t bufsize, char const* filename, char const* pPathID ) { AssertUnreachable(); return {}; }

FSAsyncStatus_t CFileSystemStdio::AsyncWrite( const char* pFileName, const void* pSrc, int nSrcBytes, bool bFreeMemory, bool bAppend, FSAsyncControl_t* pControl ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncWriteFile( const char* pFileName, const CUtlBuffer* pSrc, int nSrcBytes, bool bFreeMemory, bool bAppend, FSAsyncControl_t* pControl ) { AssertUnreachable(); return {}; }
FSAsyncStatus_t CFileSystemStdio::AsyncReadMultipleCreditAlloc( const FileAsyncRequest_t* pRequests, int nRequests, const char* pszFile, int line, FSAsyncControl_t* phControls ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::GetFileTypeForFullPath( char const* pFullPath, wchar_t* buf, size_t bufSizeInBytes ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::ReadToBuffer( FileHandle_t hFile, CUtlBuffer& buf, int nMaxBytes, FSAllocFunc_t pfnAlloc ) { AssertUnreachable(); return {}; }

// ---- Optimal IO operations ----
bool CFileSystemStdio::GetOptimalIOConstraints( FileHandle_t hFile, unsigned* pOffsetAlign, unsigned* pSizeAlign, unsigned* pBufferAlign ) { AssertUnreachable(); return {}; }
inline unsigned CFileSystemStdio::GetOptimalReadSize( FileHandle_t hFile, unsigned nLogicalSize ) { AssertUnreachable(); return {}; }
void* CFileSystemStdio::AllocOptimalReadBuffer( FileHandle_t hFile, unsigned nSize, unsigned nOffset ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::FreeOptimalReadBuffer( void* pBuffer ) { AssertUnreachable(); }

void CFileSystemStdio::BeginMapAccess() { AssertUnreachable(); }
void CFileSystemStdio::EndMapAccess() { AssertUnreachable(); }

bool CFileSystemStdio::FullPathToRelativePathEx( const char* pFullpath, const char* pPathId, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }

int CFileSystemStdio::GetPathIndex( const FileNameHandle_t& handle ) { AssertUnreachable(); return {}; }
long CFileSystemStdio::GetPathTime( const char* pPath, const char* pPathID ) { AssertUnreachable(); return {}; }

DVDMode_t CFileSystemStdio::GetDVDMode() { AssertUnreachable(); return {}; }

// ---- Whitelisting for pure servers. ----
void CFileSystemStdio::EnableWhitelistFileTracking( bool bEnable, bool bCacheAllVPKHashes, bool bRecalculateAndCheckHashes ) { AssertUnreachable(); }

void CFileSystemStdio::RegisterFileWhitelist( IPureServerWhitelist * pWhiteList, IFileList * *pFilesToReload ) { AssertUnreachable(); }

void CFileSystemStdio::MarkAllCRCsUnverified() { AssertUnreachable(); }

void CFileSystemStdio::CacheFileCRCs( const char* pPathname, ECacheCRCType eType, IFileList* pFilter ) { AssertUnreachable(); }
EFileCRCStatus CFileSystemStdio::CheckCachedFileHash( const char* pPathID, const char* pRelativeFilename, int nFileFraction, FileHash_t* pFileHash ) { AssertUnreachable(); return {}; }

int CFileSystemStdio::GetUnverifiedFileHashes( CUnverifiedFileHash * pFiles, int nMaxFiles ) { AssertUnreachable(); return {}; }

int CFileSystemStdio::GetWhitelistSpewFlags() { AssertUnreachable(); return {}; }
void CFileSystemStdio::SetWhitelistSpewFlags( int flags ) { AssertUnreachable(); }

void CFileSystemStdio::InstallDirtyDiskReportFunc( FSDirtyDiskReportFunc_t func ) { AssertUnreachable(); }

// ---- Low-level file caching ----
FileCacheHandle_t CFileSystemStdio::CreateFileCache() { AssertUnreachable(); return {}; }
void CFileSystemStdio::AddFilesToFileCache( FileCacheHandle_t cacheId, const char** ppFileNames, int nFileNames, const char* pPathID ) { AssertUnreachable(); }
bool CFileSystemStdio::IsFileCacheFileLoaded( FileCacheHandle_t cacheId, const char* pFileName ) { AssertUnreachable(); return {}; }
bool CFileSystemStdio::IsFileCacheLoaded( FileCacheHandle_t cacheId ) { AssertUnreachable(); return {}; }
void CFileSystemStdio::DestroyFileCache( FileCacheHandle_t cacheId ) { AssertUnreachable(); }

bool CFileSystemStdio::RegisterMemoryFile( CMemoryFileBacking * pFile, CMemoryFileBacking * *ppExistingFileWithRef ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::UnregisterMemoryFile( CMemoryFileBacking * pFile ) { AssertUnreachable(); }

void CFileSystemStdio::CacheAllVPKFileHashes( bool bCacheAllVPKHashes, bool bRecalculateAndCheckHashes ) { AssertUnreachable(); }
bool CFileSystemStdio::CheckVPKFileHash( int PackFileID, int nPackFileNumber, int nFileFraction, MD5Value_t& md5Value ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::NotifyFileUnloaded( const char* pszFilename, const char* pPathId ) { AssertUnreachable(); }

bool CFileSystemStdio::GetCaseCorrectFullPath_Ptr( const char* pFullPath, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }


EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CFileSystemStdio, IFileSystem, FILESYSTEM_INTERFACE_VERSION, g_FullFileSystem );
