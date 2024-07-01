//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once
#include "isystemclient.hpp"
#include "vpkedit/PackFile.h"

class CPackSystemClient : public ISystemClient {
public:
	// NOTE: This is PRIVATE
	CPackSystemClient( int id, const char* path, std::unique_ptr<vpkedit::PackFile>&& pack );
	static auto Open( int id, const std::string& absolute, const char* path ) -> std::shared_ptr<ISystemClient>;
	[[nodiscard]] auto GetNativePath() const -> const char* override;
	[[nodiscard]] auto GetNativeAbsolutePath() const -> const char* override;
	[[nodiscard]] auto GetIdentifier() const -> int override;
	auto Shutdown() -> void override;
public: // FS interaction
	auto Flush ( const FileDescriptor* handle ) -> bool override;
	auto Walk  ( uint16_t nwname, const char* wname ) -> void override;
	auto Open  ( const char* path, OpenMode mode ) -> FileDescriptor* override;
	auto Close ( const FileDescriptor* handle ) -> void override;
	auto Create( const char* path, dirmode_t perm, OpenMode mode ) -> FileDescriptor* override;
	auto Read  ( const FileDescriptor* handle, void* buffer, uint32_t count ) -> int32_t override;
	auto Write ( const FileDescriptor* handle, void const* buffer, uint32_t count ) -> int32_t override;
	auto Remove( const FileDescriptor* handle ) -> void override;
	auto Stat  ( const FileDescriptor* handle ) -> StatData override;
private:
	int m_iId;
	const char* m_szNativePath;
	std::unique_ptr<vpkedit::PackFile> m_PackFile;
};