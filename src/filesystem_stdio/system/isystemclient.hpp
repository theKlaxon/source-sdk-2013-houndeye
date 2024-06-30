//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once
#include "filesystem.h"
#include "mempool.h"
#include "tier0/platform.h"

#include <functional>
#include <memory>

class ISystemClient;


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


namespace openmode {
	enum Type : uint8_t {
		Read,
		Write,
		ReadWrite,
		Exec,
		Trunc = 0x10,
		Close = 0x40,

		INVALID = 0xFF
	};
}

/**
 * Internal representation of an open file.
 * Uses a memory arena to avoid sparse allocations.
 */
struct FileDescriptor {
	std::weak_ptr<ISystemClient> m_System;
	uintptr_t m_Handle;
	size_t m_Offset{0};

	/**
	 * Allocates a new instance of a descriptor.
	 * @return
	 */
	static auto Make() -> FileDescriptor*;
	/**
	 * Frees an existing descriptor.
	 * @return
	 */
	static auto Free( FileDescriptor* ) -> void;
	/**
	 * Cleans the arena, removing all descriptors.
	 * @return
	 */
	static auto CleanupArena() -> void;
};
using ClientFileHandle = const FileDescriptor*;


abstract_class ISystemClient {
public: // metadata
	[[nodiscard]]
	virtual auto GetNativePath() const -> const char* = 0;
	[[nodiscard]]
	virtual auto GetNativeAbsolutePath() const -> const char* = 0;
	[[nodiscard]]
	virtual auto GetIdentifier() const -> int = 0;
	virtual auto Shutdown() -> void = 0;
	auto operator==( ISystemClient& other ) const -> bool { return this == &other || this->GetIdentifier() == other.GetIdentifier(); }
	auto operator==( const ISystemClient& other ) const -> bool { return this == &other || this->GetIdentifier() == other.GetIdentifier(); }
public: // fs interactions
	// file ops
	virtual auto Open  ( const char* path, openmode::Type mode ) -> FileDescriptor* = 0;
	virtual auto Read  ( const FileDescriptor* desc, uint64_t offset, void* buffer, uint32_t count ) -> uint32_t = 0;
	virtual auto Write ( const FileDescriptor* desc, uint64_t offset, const void* buffer, uint32_t count ) -> uint32_t = 0;
	virtual auto Flush ( const FileDescriptor* desc ) -> bool = 0;
	virtual auto Close ( const FileDescriptor* desc ) -> void = 0;
	// generic ops
	virtual auto Walk  ( uint16_t nwname, const char* wname ) -> void = 0;
	virtual auto Create( const char* path, dirmode_t perm, openmode::Type mode ) -> FileDescriptor* = 0;
	virtual auto Remove( const FileDescriptor* desc ) -> void = 0;
	virtual auto Stat  ( const FileDescriptor* desc ) -> void = 0;
};
