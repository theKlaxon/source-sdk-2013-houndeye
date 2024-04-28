//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once
#include "tier0/platform.h"
#include "filesystem.h"

#include <functional>
#include <memory>



enum dirmode_t : uint32_t {
	// High byte is the same as qidtype_t
	X9P_DM_DIR    = 0x80000000,	// Directory
	X9P_DM_APPEND = 0x40000000,	// Append only
	X9P_DM_EXCL   = 0x20000000,	// Exclusive use
	X9P_DM_MOUNT  = 0x10000000,	// Mounted channel -- Unused?
	X9P_DM_AUTH   = 0x08000000,	// Authentication file
	X9P_DM_TMP    = 0x04000000,	// Temporary/not-backed-up file

	// A user that is the file's owner checks "other", "group", and "other"
	X9P_DM_OWNER_READ   = 0b100000000,	// Can the owner read?
	X9P_DM_OWNER_WRITE  = 0b010000000,	// Can the owner write?
	X9P_DM_OWNER_EXEC   = 0b001000000,	// Can the owner execute?

	// A user from the file's group checks both "other" and "group"
	X9P_DM_GROUP_READ   = 0b000100000,	// Can the group read?
	X9P_DM_GROUP_WRITE  = 0b000010000,	// Can the group write?
	X9P_DM_GROUP_EXEC   = 0b000001000,	// Can the group execute?

	// A user unrelated to the file checks only "other"
	X9P_DM_OTHER_READ   = 0b000000100,	// Can read?
	X9P_DM_OTHER_WRITE  = 0b000000010,	// Can write?
	X9P_DM_OTHER_EXEC   = 0b000000001,	// Can execute?

	// Aggregated permissions
	X9P_DM_READ  = X9P_DM_OWNER_READ  | X9P_DM_GROUP_READ  | X9P_DM_OTHER_READ,
	X9P_DM_WRITE = X9P_DM_OWNER_WRITE | X9P_DM_GROUP_WRITE | X9P_DM_OTHER_WRITE,
	X9P_DM_EXEC  = X9P_DM_OWNER_EXEC  | X9P_DM_GROUP_EXEC  | X9P_DM_OTHER_EXEC,

	// Masks
	X9P_DM_MASK_OWNER = X9P_DM_OWNER_READ | X9P_DM_OWNER_WRITE | X9P_DM_OWNER_EXEC,
	X9P_DM_MASK_GROUP = X9P_DM_GROUP_READ | X9P_DM_GROUP_WRITE | X9P_DM_GROUP_EXEC,
	X9P_DM_MASK_OTHER = X9P_DM_OTHER_READ | X9P_DM_OTHER_WRITE | X9P_DM_OTHER_EXEC,

	X9P_DM_PERM_MASK = X9P_DM_MASK_OWNER | X9P_DM_MASK_GROUP | X9P_DM_MASK_OTHER,
	X9P_DM_PROTOCOL_MASK = X9P_DM_PERM_MASK | X9P_DM_DIR | X9P_DM_APPEND | X9P_DM_EXCL | X9P_DM_MOUNT | X9P_DM_AUTH | X9P_DM_TMP
};

struct stat_t {
	uint16_t size;    // Total bytes used by following data

	dirmode_t mode;   // Permissions and flags
	uint64_t atime;   // Access time (unix time)
	uint64_t mtime;   // Modification time (unix time)
	uint64_t length;  // File Length in bytes

	// File name
	std::string name() { return {}; }
};


enum class openmode_t : uint8_t {
	Read,
	Write,
	ReadWrite,
	Exec,
	Trunc = 0x10,
	Close = 0x40,
};

abstract_class ISystemClient {
public: // metadata
	[[nodiscard]] virtual auto GetNativePath() const -> const char* = 0;
	[[nodiscard]] virtual auto GetNativeAbsolutePath() const -> const char* = 0;
	[[nodiscard]] virtual auto GetIdentifier() const -> int = 0;
	virtual auto Shutdown() -> void = 0;
	auto operator==( ISystemClient& other ) const -> bool { return this == &other || this->GetIdentifier() == other.GetIdentifier(); }
	auto operator==( const ISystemClient& other ) const -> bool { return this == &other || this->GetIdentifier() == other.GetIdentifier(); }
public: // fs interactions
	virtual auto Flush ( FileHandle_t file ) -> bool = 0;
	virtual auto Walk  ( uint16_t nwname, const char* wname ) -> void = 0;
	virtual auto Open  ( const char* path, const char* mode ) -> FileHandle_t = 0;
	virtual auto Create( const char* name, dirmode_t perm, openmode_t mode ) -> FileHandle_t = 0;
	virtual auto Read  ( FileHandle_t file, void* buffer, uint32_t count ) -> uint32_t = 0;
	virtual auto Write ( FileHandle_t file, const void* buffer, uint32_t count ) -> uint32_t = 0;
	virtual auto Remove( FileHandle_t file ) -> void = 0;
	virtual auto Stat  ( FileHandle_t file ) -> void = 0;
};
