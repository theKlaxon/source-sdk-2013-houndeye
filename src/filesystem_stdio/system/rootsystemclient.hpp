//
// Created by ENDERZOMBI102 on 10/08/2024.
//
#pragma once
#include "isystemclient.hpp"


class CRootSystemClient : public ISystemClient {
public:
	[[nodiscard]]
	auto GetNativePath() const -> const char* override;
	[[nodiscard]]
	auto GetNativeAbsolutePath() const -> const char* override;
	[[nodiscard]]
	auto GetIdentifier() const -> int override;
	[[nodiscard]]
	auto GetType() const -> const char* override;
	auto Shutdown() -> void override;
public: // FS interaction
	// file ops
	auto Open  ( const char* pPath, OpenMode pMode ) -> FileDescriptor* override;
	auto Read  ( const FileDescriptor* pDesc, void* pBuffer, uint32_t pCount ) -> int32_t override;
	auto Write ( const FileDescriptor* pDesc, const void* pBuffer, uint32_t pCount ) -> int32_t override;
	auto Flush ( const FileDescriptor* pDesc ) -> bool override;
	auto Close ( const FileDescriptor* pDesc ) -> void override;
	// generic ops
	auto Walk  ( const FileDescriptor* pDesc, const WalkEntry*& pEntry ) -> void override;
	auto Create( const char* pPath, FileType pType, OpenMode pMode ) -> FileDescriptor* override;
	auto Remove( const FileDescriptor* pDesc ) -> void override;
	auto Stat  ( const FileDescriptor* pDesc ) -> std::optional<StatData> override;
};
