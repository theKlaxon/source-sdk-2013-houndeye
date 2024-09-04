//
// Created by ENDERZOMBI102 on 22/02/2024.
//
#include "platform.h"
#if IsWindows()
	#include <shlwapi.h>
#endif
#include <utility>
#include "../tier0/commandline.hpp"
#include "filesystem.hpp"
#include "interface.h"
#include "system/packsystemclient.hpp"
#include "system/plainsystemclient.hpp"
#include "system/rootsystemclient.hpp"

namespace {
	CFileSystemStdio g_FullFileSystem{};
	auto g_RootSystemClient{ std::make_shared<CRootSystemClient>() };

	constexpr auto parseOpenMode( const char* pMode ) -> OpenMode {
		OpenMode mode{};
		while ( *pMode != '\0' ) {
			switch ( *pMode ) {
				case 'r':
					mode.read = true;
					break;
				case 'w':
					mode.write = true;
					break;
				case 'b':
					mode.binary = true;
					break;
				case 't':
					mode.truncate = true;
					break;
				case 'a':
					mode.append = true;
					break;
				case '+':
					mode.update = true;
					break;
				default:
					std::unreachable();
			}

			pMode += 1;
		}

		return mode;
	}
}

// ---------------
// AppSystem
// ---------------
auto CFileSystemStdio::Connect( CreateInterfaceFn factory ) -> bool {
	return true;
}
auto CFileSystemStdio::Disconnect() -> void { }
auto CFileSystemStdio::QueryInterface( const char* pInterfaceName ) -> void* {
	if ( strcmp( pInterfaceName, FILESYSTEM_INTERFACE_VERSION ) == 0 ) {
		return &g_FullFileSystem;
	}

	return nullptr;
}
auto CFileSystemStdio::Init() -> InitReturnVal_t {
	if ( this->m_bInitialized ) {
		return InitReturnVal_t::INIT_OK;
	}

	return InitReturnVal_t::INIT_OK;
}
auto CFileSystemStdio::Shutdown() -> void {
	FileDescriptor::CleanupArena();
}

// ---------------
// IBaseFilesystem
// ---------------
int CFileSystemStdio::Read( void* pOutput, int size, FileHandle_t file ) {
	if (! (file && pOutput) ) {
		return -1;
	}

	auto* desc{ static_cast<FileDescriptor*>( file ) };

	int32 count{ desc->m_System.lock()->Read( desc, pOutput, size ) };
	if ( count > 0 ) {
		desc->m_Offset += count;
	}

	// should return -1 on read failure,
	return count;
}
int CFileSystemStdio::Write( const void* pInput, int size, FileHandle_t file ) {
	// special case for `size=0`, as might happen (bsplib) with a 1NULL `pInput`
	if ( size == 0 ) {
		return 0;
	}
	if (! (file && pInput) ) {
		return -1;
	}

	auto* desc{ static_cast<FileDescriptor*>( file ) };

	int32 count{ desc->m_System.lock()->Write( desc, pInput, size ) };
	if ( count > 0 ) {
		desc->m_Offset += count;
	}

	// should return -1 on read failure,
	return count;
}

FileHandle_t CFileSystemStdio::Open( const char* pFileName, const char* pOptions, const char* pathID ) {
	// parse the options
	auto mode{ parseOpenMode( pOptions ) };

	// absolute paths get special treatment
	if ( V_IsAbsolutePath( pFileName ) ) {
		auto desc{ g_RootSystemClient->Open( pFileName, mode ) };
		// only add to vector if we actually got an open file
		if ( desc != nullptr ) {
			desc->m_System = { g_RootSystemClient };
			this->m_Descriptors.AddToTail( desc );
			return desc;
		}
		return nullptr;
	}

	// if we got a pathID, only look into that SearchPath
	if ( pathID != nullptr ) {
		AssertFatalMsg( this->m_SearchPaths.Find( pathID ) == CUtlDict<SearchPath>::InvalidIndex(), "Was given pathID not loaded" );

		for ( const auto& system : this->m_SearchPaths[pathID].m_Clients ) {
			auto desc{ system->Open( pFileName, mode ) };
			// only add to vector if we actually got an open file
			if ( desc != nullptr ) {
				desc->m_System = { system };
				this->m_Descriptors.AddToTail( desc );
				return desc;
			}
		}
	} else {
		// else, look into all clients
		for ( const auto& [_, searchPath] : this->m_SearchPaths ) {
			for ( const auto& system : searchPath.m_Clients ) {
				auto desc{ system->Open( pFileName, mode ) };
				// only add to vector if we actually got an open file
				if ( desc != nullptr ) {
					desc->m_System = { system };
					this->m_Descriptors.AddToTail( desc );
					return desc;
				}
			}
		}
	}

	AssertFatalMsg( false, "Open: %s, %s, %s", pFileName, pOptions, pathID ); return {};
}
void CFileSystemStdio::Close( FileHandle_t file ) {
	auto desc{ static_cast<FileDescriptor*>( file ) };
	desc->m_System.lock()->Close( desc );
	this->m_Descriptors.FindAndRemove( desc );
	FileDescriptor::Free( desc );
}

void CFileSystemStdio::Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType ) {
	auto desc{ const_cast<FileDescriptor*>( static_cast<const FileDescriptor*>( file ) ) };
	auto size{ desc->m_System.lock()->Stat( desc )->m_Length };

	switch ( seekType ) {
		case FILESYSTEM_SEEK_CURRENT:
			// FIXME: This is dumb
			desc->m_Offset = Clamp( desc->m_Offset + pos, static_cast<uint64>( 0 ), size );
			break;
		case FILESYSTEM_SEEK_HEAD:
			desc->m_Offset = Clamp( static_cast<uint64>( pos ), static_cast<uint64>( 0 ), size );
			break;
		case FILESYSTEM_SEEK_TAIL:
			desc->m_Offset = Clamp( size - pos, static_cast<uint64>( 0 ), size );
			break;
	}
}
unsigned int CFileSystemStdio::Tell( FileHandle_t file ) {
	auto desc{ static_cast<FileDescriptor*>( file ) };

	return static_cast<int32>( desc->m_Offset );
}
unsigned int CFileSystemStdio::Size( FileHandle_t file ) {
	// if we already know the size, just return it
	auto desc{ static_cast<FileDescriptor*>( file ) };
	if ( desc->m_Size != -1 ) {
		return desc->m_Size;
	}

	// stat the file, "handle" error
	auto statMaybe{ desc->m_System.lock()->Stat( desc ) };
	if (! statMaybe ) {
		return -1;
	}

	// save size and return it
	desc->m_Size = static_cast<int64>( statMaybe.value().m_Length );
	return desc->m_Size;
}
unsigned int CFileSystemStdio::Size( const char* pFileName, const char* pPathID ) {
	// open file
	auto desc{ static_cast<FileDescriptor*>( this->Open( pFileName, "r", pPathID ) ) };
	if (! desc ) {
		return -1;
	}

	// get size
	auto size{ Size( desc ) };

	// close file
	Close( desc );

	// return size
	return size;
}

void CFileSystemStdio::Flush( FileHandle_t file ) {
	auto desc{ static_cast<FileDescriptor*>( file ) };
	desc->m_System.lock()->Flush( desc );
}
bool CFileSystemStdio::Precache( const char* pFileName, const char* pPathID ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::FileExists( const char* pFileName, const char* pPathID ) {
	if ( V_IsAbsolutePath( pFileName ) ) {
		#if IsWindows()
			return PathFileExistsA( pFileName ) == TRUE;
		#elif IsPosix()
			return access( pFileName, F_OK ) == 0;
		#endif
	}

	// try to open the file
	auto handle{ this->Open( pFileName, "r", pPathID ) };
	if ( handle ) {
		this->Close( handle );
		return true;
	}

	return false;
}
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
	AssertFatalMsg( pPath, "Was given an empty path!!" );

	this->m_iLastId += 1;

	// calculate base dir (current `-game` dir)
	char absolute[1024];
	if (! V_IsAbsolutePath( pPath ) ) {
		AssertFatalMsg( _getcwd( absolute, sizeof( absolute ) ), "V_MakeAbsolutePath: _getcwd failed." );
		V_ComposeFileName( absolute, CommandLine()->ParmValue( "-game", "" ), absolute, sizeof( absolute ) );

		// make the absolute path of the thing
		V_MakeAbsolutePath( absolute, 1024, pPath, absolute );
	} else {
		// already absolute, just copy
		strncpy( absolute, pPath, 1024 );
	}

	// if the path is non-existent, do nothing
	if (! this->FileExists( absolute ) ) {
		return;
	}

	// try all possibilities
	auto system{ CPlainSystemClient::Open( this->m_iLastId, absolute, pPath ) };
	system = system ? system : CPackSystemClient::Open( this->m_iLastId, absolute, pPath );
	AssertFatalMsg( system, "Unsupported path entry: %s", absolute );

	// make a new alloc of the string
	// TODO: Use string interning if possible
	pathID = V_strlower( V_strdup( pathID ) );

	if ( this->m_SearchPaths.Find( pathID ) == CUtlDict<SearchPath>::InvalidIndex() ) {
		this->m_SearchPaths.Insert( pathID );
	}

	if ( addType == SearchPathAdd_t::PATH_ADD_TO_HEAD ) {
		this->m_SearchPaths[ pathID ].m_Clients.AddToHead( system );
	} else {
		this->m_SearchPaths[ pathID ].m_Clients.AddToTail( system );
	}
	this->m_SearchPaths[ pathID ].m_ClientIDs.AddToTail( this->m_iLastId );
	delete[] pathID;
}
bool CFileSystemStdio::RemoveSearchPath( const char* pPath, const char* pathID ) {
	if ( m_SearchPaths.Find( pathID ) == CUtlDict<SearchPath>::InvalidIndex() ) {
		return false;
	}

	auto& systems{ this->m_SearchPaths[pathID].m_Clients };
	for ( int i{0}; i < systems.Count(); i += 1 ) {
		if ( V_strcmp( systems[i]->GetNativePath(), pPath ) == 0 ) {
			systems[i]->Shutdown();
			systems.Remove( i );
			return true;
		}
	}
	return false;
}

void CFileSystemStdio::RemoveAllSearchPaths() {
	// close all descriptors
	for ( auto desc : this->m_Descriptors ) {
		desc->m_System.lock()->Close( desc );
	}
	// clear descriptor cache
	this->m_Descriptors.Purge();
	// free the memory arena
	FileDescriptor::CleanupArena();

	// close all systems
	for ( auto& [pathId, searchPath] : this->m_SearchPaths ) {
		for ( auto& system : searchPath.m_Clients ) {
			system->Shutdown();
		}
		searchPath.m_Clients.Purge();
	}
	this->m_SearchPaths.Purge();
}

void CFileSystemStdio::RemoveSearchPaths( const char* szPathID ) {
	// is it a real search path?
	if ( m_SearchPaths.Find( szPathID ) == CUtlDict<SearchPath>::InvalidIndex() ) {
		return;
	}

	auto& search{ this->m_SearchPaths[szPathID] };

	// close all open descriptors the path's clients own
	for ( auto i{this->m_Descriptors.Count()}; i > 0; i -= 1 ) {
		auto client{ this->m_Descriptors[i]->m_System.lock() };
		if ( search.m_ClientIDs.Find( client->GetIdentifier() ) != CUtlVector<int>::InvalidIndex() ) {
			// close descriptor
			client->Close( this->m_Descriptors[ i ] );
			// remove from cache
			this->m_Descriptors.FindAndRemove( this->m_Descriptors[i] );
			// free it
			FileDescriptor::Free( this->m_Descriptors[i] );
		}
	}

	// remove the clients
	for ( auto& system : search.m_Clients ) {
		system->Shutdown();
	}
	search.m_Clients.Purge();
	search.m_ClientIDs.Purge();

	// delete search path
	this->m_SearchPaths.Remove( szPathID );
}

void CFileSystemStdio::MarkPathIDByRequestOnly( const char* pPathID, bool bRequestOnly ) {
	// make a new alloc of the string
	// TODO: Use string interning if possible
	auto pathID{ V_strlower( V_strdup( pPathID ) ) };

	if ( this->m_SearchPaths.Find( pathID ) == CUtlDict<SearchPath>::InvalidIndex() ) {
		this->m_SearchPaths.Insert( pathID );
	}

	this->m_SearchPaths[ pathID ].m_bRequestOnly = bRequestOnly;
	delete[] pathID;
}

const char* CFileSystemStdio::RelativePathToFullPath( const char* pFileName, const char* pPathID, char* pDest, int maxLenInChars, PathTypeFilter_t pathFilter, PathTypeQuery_t* pPathType ) { AssertUnreachable(); return {}; }

int CFileSystemStdio::GetSearchPath( const char* pathID, bool bGetPackFiles, char* pDest, int maxLenInChars ) {
	if ( m_SearchPaths.Find( pathID ) == CUtlDict<SearchPath>::InvalidIndex() ) {
		return 0;
	}

	int length{0};
	for ( const auto& client : m_SearchPaths[pathID].m_Clients ) {
		if ( V_strcmp( client->GetType(), "pack" ) == 0 && !bGetPackFiles ) {
			// pack files disabled...
			continue;
		}

		auto len{ static_cast<int32>( strlen( client->GetNativePath() ) ) };

		if ( length + len + 1 >= maxLenInChars ) {
			// can't do another one
			break;
		}

		memcpy( pDest + length, client->GetNativePath(), len );
		*(pDest + length) = ';';
		length += len + 1;
	}
	return length;
}

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
const char* CFileSystemStdio::FindFirst( const char* pWildCard, FileFindHandle_t* pHandle ) {
	// TODO: For now, just support absolute paths
	AssertMsg( V_IsAbsolutePath( pWildCard ), "FindFirst(not absolute!!): %s, %p", pWildCard, pHandle );




	return {};
}
const char* CFileSystemStdio::FindNext( FileFindHandle_t handle ) {
	AssertUnreachable();
	return {};
}
bool CFileSystemStdio::FindIsDirectory( FileFindHandle_t handle ) {
	AssertUnreachable();
	return {};
}
void CFileSystemStdio::FindClose( FileFindHandle_t handle ) {
	AssertUnreachable();
}

const char* CFileSystemStdio::FindFirstEx( const char* pWildCard, const char* pPathID, FileFindHandle_t* pHandle ) { AssertUnreachable(); return {}; }

// ---- File name and directory operations ----
const char* CFileSystemStdio::GetLocalPath( const char* pFileName, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::FullPathToRelativePath( const char* pFullpath, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }

bool CFileSystemStdio::GetCurrentDirectory( char* pDirectory, int maxlen ) {
    #if IsWindows()
        return _getcwd( pDirectory, maxlen ) != nullptr;
    #else
    	return getcwd( pDirectory, maxlen ) != nullptr;
    #endif
}

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
		for ( const auto& path : searchPath.m_Clients ) {
			if ( path->GetNativePath() && strcmp( path->GetNativePath(), "" ) != 0 ) {
				Log( "  - %s\n", path->GetNativePath() );
			}
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

int CFileSystemStdio::ReadEx( void* pOutput, int sizeDest, int size, FileHandle_t file ) {
	// TODO: DO the `Ex` part :P
	if (! (file && pOutput) ) {
		return -1;
	}

	auto desc{ static_cast<FileDescriptor*>( file ) };
	return desc->m_System.lock()->Read( desc, pOutput, size );
}
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
bool CFileSystemStdio::GetOptimalIOConstraints( FileHandle_t hFile, unsigned* pOffsetAlign, unsigned* pSizeAlign, unsigned* pBufferAlign ) {
	if (! hFile ) {
		return false;
	}
	const uint32 value{ strcmp( static_cast<FileDescriptor*>( hFile )->m_System.lock()->GetType(), "pack" ) != 0 };

	if ( pOffsetAlign ) {
		*pOffsetAlign = value;
	}
	if ( pSizeAlign ) {
		*pSizeAlign = value;
	}
	if ( pBufferAlign ) {
		*pBufferAlign = value;
	}

	return false;
}
inline unsigned CFileSystemStdio::GetOptimalReadSize( FileHandle_t hFile, unsigned nLogicalSize ) { AssertUnreachable(); return {}; }
void* CFileSystemStdio::AllocOptimalReadBuffer( FileHandle_t hFile, unsigned nSize, unsigned nOffset ) {
	// FIXME: Actually do the thing
	return new char[nSize];
}
void CFileSystemStdio::FreeOptimalReadBuffer( void* pBuffer ) {
	// FIXME: Actually do the thing
	delete[] static_cast<char*>( pBuffer );
}

void CFileSystemStdio::BeginMapAccess() { AssertUnreachable(); }
void CFileSystemStdio::EndMapAccess() { AssertUnreachable(); }

bool CFileSystemStdio::FullPathToRelativePathEx( const char* pFullpath, const char* pPathId, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }

int CFileSystemStdio::GetPathIndex( const FileNameHandle_t& handle ) { AssertUnreachable(); return {}; }
long CFileSystemStdio::GetPathTime( const char* pPath, const char* pPathID ) { AssertUnreachable(); return {}; }

DVDMode_t CFileSystemStdio::GetDVDMode() {
	return DVDMode_t::DVDMODE_OFF;
}

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

bool CFileSystemStdio::RegisterMemoryFile( CMemoryFileBacking* pFile, CMemoryFileBacking** ppExistingFileWithRef ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::UnregisterMemoryFile( CMemoryFileBacking* pFile ) { AssertUnreachable(); }

void CFileSystemStdio::CacheAllVPKFileHashes( bool bCacheAllVPKHashes, bool bRecalculateAndCheckHashes ) { AssertUnreachable(); }
bool CFileSystemStdio::CheckVPKFileHash( int PackFileID, int nPackFileNumber, int nFileFraction, MD5Value_t& md5Value ) { AssertUnreachable(); return {}; }

void CFileSystemStdio::NotifyFileUnloaded( const char* pszFilename, const char* pPathId ) { AssertUnreachable(); }

bool CFileSystemStdio::GetCaseCorrectFullPath_Ptr( const char* pFullPath, char* pDest, int maxLenInChars ) { AssertUnreachable(); return {}; }


EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CFileSystemStdio, IFileSystem, FILESYSTEM_INTERFACE_VERSION, g_FullFileSystem );
