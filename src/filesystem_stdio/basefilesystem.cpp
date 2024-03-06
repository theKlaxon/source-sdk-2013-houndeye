//
// Created by ENDERZOMBI102 on 13/02/2024.
//
#include "basefilesystem.hpp"

#include <cstdio>
#if IsPosix()
	#include <sys/stat.h>
#endif

int CBaseFileSystemStdio::Read( void* pOutput, int size, FileHandle_t file ) {
	return static_cast<int>( fread( pOutput, 1, size, static_cast<FILE*>( file ) ) );
}
int CBaseFileSystemStdio::Write( void const* pOutput, int size, FileHandle_t file ) {
	return static_cast<int>( fwrite( pOutput, 1, size, static_cast<FILE*>( file ) ) );
}

FileHandle_t CBaseFileSystemStdio::Open( const char* pFileName, const char* pOptions, const char* pathID ) {
	return static_cast<FileHandle_t>( fopen( pFileName, pOptions ) );
}
void CBaseFileSystemStdio::Close( FileHandle_t file ) {
	fclose( static_cast<FILE*>( file ) );
}

void CBaseFileSystemStdio::Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType ) {
	fseek( static_cast<FILE*>( file ), pos, seekType );
}
unsigned int CBaseFileSystemStdio::Tell( FileHandle_t file ) {
	return ftell( static_cast<FILE*>( file ) );
}
unsigned int CBaseFileSystemStdio::Size( FileHandle_t file ) {
	auto handle{ static_cast<FILE*>( file ) };
	// save the current position
	fpos_t pos;
	fgetpos( handle, &pos );
	// get size
	fseek( handle, 0, SEEK_END );
	auto size{ ftell( handle ) };
	// restore position
	fsetpos( handle, &pos );
	return size;
}
unsigned int CBaseFileSystemStdio::Size( const char* pFileName, const char* pPathID ) { AssertUnreachable(); }

void CBaseFileSystemStdio::Flush( FileHandle_t file ) {
	fflush( static_cast<FILE*>( file ) );
}
bool CBaseFileSystemStdio::Precache( const char* pFileName, const char* pPathID ) { AssertUnreachable(); }

bool CBaseFileSystemStdio::FileExists( const char* pFileName, const char* pPathID ) { AssertUnreachable(); }
bool CBaseFileSystemStdio::IsFileWritable( char const* pFileName, const char* pPathID ) { AssertUnreachable(); }
bool CBaseFileSystemStdio::SetFileWritable( char const* pFileName, bool writable, const char* pPathID ) { AssertUnreachable(); }

long CBaseFileSystemStdio::GetFileTime( const char* pFileName, const char* pPathID ) {
	struct stat stats{};
	stat( pFileName, &stats );
    #if IsWindows()
        return static_cast<long>( stats.st_mtime );
    #elif IsLinux()
    	return stats.st_mtim.tv_sec; // TODO: Verify accuracy of return
    #endif
}

//--------------------------------------------------------
// Reads/writes files to utlbuffers. Use this for optimal read performance when doing open/read/close
//--------------------------------------------------------
bool CBaseFileSystemStdio::ReadFile( const char* pFileName, const char* pPath, CUtlBuffer& buf, int nMaxBytes, int nStartingByte, FSAllocFunc_t pfnAlloc ) { AssertUnreachable(); }
bool CBaseFileSystemStdio::WriteFile( const char* pFileName, const char* pPath, CUtlBuffer& buf ) { AssertUnreachable(); }
bool CBaseFileSystemStdio::UnzipFile( const char* pFileName, const char* pPath, const char* pDestination ) { AssertUnreachable(); }

static CBaseFileSystemStdio g_BaseFileSystemStdio{};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBaseFileSystemStdio, IBaseFileSystem, BASEFILESYSTEM_INTERFACE_VERSION, g_BaseFileSystemStdio )
