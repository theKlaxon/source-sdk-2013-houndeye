//
// Created by ENDERZOMBI102 on 22/02/2024.
//
#include "filesystem.hpp"
#include "interface.h"

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
	for ( int i{0}; i < this->m_SearchPath.Count(); i += 1 ) {
		if ( V_strcmp( this->m_SearchPath[i]->GetNativePath(), pPath ) == 0 ) {
			this->m_SearchPath[i]->Shutdown();
			this->m_SearchPath.Remove( i );
			return true;
		}
	}
	return false;
}

void CFileSystemStdio::RemoveAllSearchPaths() {
	for ( auto& system : this->m_SearchPath )
		system->Shutdown();
	this->m_SearchPath.RemoveAll();
}

void CFileSystemStdio::RemoveSearchPaths( const char* szPathID ) { }

void CFileSystemStdio::MarkPathIDByRequestOnly( const char* pPathID, bool bRequestOnly ) { }

const char* CFileSystemStdio::RelativePathToFullPath( const char* pFileName, const char* pPathID, char* pDest, int maxLenInChars, PathTypeFilter_t pathFilter, PathTypeQuery_t* pPathType ) { }

int CFileSystemStdio::GetSearchPath( const char* pathID, bool bGetPackFiles, char* pDest, int maxLenInChars ) { }

bool CFileSystemStdio::AddPackFile( const char* fullpath, const char* pathID ) { }


// ---- File manipulation operations ----
void CFileSystemStdio::RemoveFile( char const* pRelativePath, const char* pathID ) { }

bool CFileSystemStdio::RenameFile( char const* pOldPath, char const* pNewPath, const char* pathID ) { }

void CFileSystemStdio::CreateDirHierarchy( const char* path, const char* pathID ) { }

bool CFileSystemStdio::IsDirectory( const char* pFileName, const char* pathID ) { }

void CFileSystemStdio::FileTimeToString( char* pStrip, int maxCharsIncludingTerminator, long fileTime ) { }





static CFileSystemStdio g_FullFileSystem{};
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CFileSystemStdio, IFileSystem, FILESYSTEM_INTERFACE_VERSION, g_FullFileSystem );
