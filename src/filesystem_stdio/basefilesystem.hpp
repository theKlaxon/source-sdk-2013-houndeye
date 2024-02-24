//
// Created by ENDERZOMBI102 on 13/02/2024.
//
#pragma once

#include "filesystem.h"

class CBaseFileSystemStdio : public IBaseFileSystem {
public:
	int Read( void* pOutput, int size, FileHandle_t file ) override;
	int Write( void const* pInput, int size, FileHandle_t file ) override;

	// if pathID is nullptr, all paths will be searched for the file
	FileHandle_t Open( const char* pFileName, const char* pOptions, const char* pathID = 0 ) override;
	void Close( FileHandle_t file ) override;

	void Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType ) override;
	unsigned int Tell( FileHandle_t file ) override;
	unsigned int Size( FileHandle_t file ) override;
	unsigned int Size( const char* pFileName, const char* pPathID = 0 ) override;

	void Flush( FileHandle_t file ) override;
	bool Precache( const char* pFileName, const char* pPathID = 0 ) override;

	bool FileExists( const char* pFileName, const char* pPathID = 0 ) override;
	bool IsFileWritable( char const* pFileName, const char* pPathID = 0 ) override;
	bool SetFileWritable( char const* pFileName, bool writable, const char* pPathID = 0 ) override;

	long GetFileTime( const char* pFileName, const char* pPathID = 0 ) override;

	//--------------------------------------------------------
	// Reads/writes files to utlbuffers. Use this for optimal read performance when doing open/read/close
	//--------------------------------------------------------
	bool ReadFile( const char* pFileName, const char* pPath, CUtlBuffer& buf, int nMaxBytes = 0, int nStartingByte = 0, FSAllocFunc_t pfnAlloc = nullptr ) override;
	bool WriteFile( const char* pFileName, const char* pPath, CUtlBuffer& buf ) override;
	bool UnzipFile( const char* pFileName, const char* pPath, const char* pDestination ) override;
};


