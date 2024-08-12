//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once
#include "isystemclient.hpp"


class CPlainSystemClient : public ISystemClient {
public:
	CPlainSystemClient( int32 pId, std::string pAbsolute, const char* pPath );
	static auto Open( int32 pId, const std::string& pAbsolute, const char* pPath ) -> std::shared_ptr<ISystemClient>;
	[[nodiscard]]
	auto GetNativePath() const -> const char* override;
	[[nodiscard]]
	auto GetNativeAbsolutePath() const -> const char* override;
	[[nodiscard]]
	auto GetIdentifier() const -> int32 override;
	[[nodiscard]]
	auto GetType() const -> const char* override;
	auto Shutdown() -> void override;
public: // FS interaction
	// file ops
	auto Open  ( const char* pPath, OpenMode pMode ) -> FileDescriptor* override;
	auto Read  ( const FileDescriptor* pDesc, void* pBuffer, uint32 pCount ) -> int32 override;
	auto Write ( const FileDescriptor* pDesc, const void* pBuffer, uint32 pCount ) -> int32 override;
	auto Flush ( const FileDescriptor* pDesc ) -> bool override;
	auto Close ( const FileDescriptor* pDesc ) -> void override;
	// generic ops
	auto Walk  ( const FileDescriptor* pDesc, const WalkEntry*& pEntry ) -> void override;
	auto Create( const char* pPath, FileType pType, OpenMode pMode ) -> FileDescriptor* override;
	auto Remove( const FileDescriptor* pDesc ) -> void override;
	auto Stat  ( const FileDescriptor* pDesc ) -> std::optional<StatData> override;
private:
	const int m_iId;
	const char* m_szNativePath;
	const std::string m_szNativeAbsolutePath;
};