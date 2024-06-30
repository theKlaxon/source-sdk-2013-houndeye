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
	auto Flush ( FileHandle_t file ) -> bool override;
	auto Walk  ( uint16_t nwname, const char* wname ) -> void override;
	auto Open  ( const char* path, openmode::Type mode ) -> FileHandle_t override;
	auto Close ( FileHandle_t file ) -> void override;
	auto Create( const char* path, dirmode_t perm, openmode::Type mode ) -> FileHandle_t override;
	auto Read  ( FileHandle_t file, uint64_t offset, void* buffer, uint32_t count ) -> uint32_t override;
	auto Write ( FileHandle_t file, uint64_t offset, void const* buffer, uint32_t count ) -> uint32_t override;
	auto Remove( FileHandle_t file ) -> void override;
	auto Stat  ( FileHandle_t file ) -> void override;
private:
	int m_iId;
	const char* m_szNativePath;
	std::unique_ptr<vpkedit::PackFile> m_PackFile;
};