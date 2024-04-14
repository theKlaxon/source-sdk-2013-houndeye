//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once
#include "tier0/platform.h"
#include <functional>
#include <memory>

using xerr_t = uint8_t;

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

struct qid_t { };


using FlushCallback  = std::function<void( xerr_t err )>;
using WalkCallback   = std::function<void( xerr_t err, uint16_t nwqid, qid_t* wqid )>;
using OpenCallback   = std::function<void( xerr_t err, qid_t* qid, uint32_t iounit )>;
using CreateCallback = std::function<void( xerr_t err, qid_t* qid, uint32_t iounit )>;
using ReadCallback   = std::function<void( xerr_t err, uint32_t count, void* data )>;
using WriteCallback  = std::function<void( xerr_t err, uint32_t count )>;
using RemoveCallback = std::function<void( xerr_t err )>;
using StatCallback   = std::function<void( xerr_t err, stat_t* stat )>;



abstract_class ISystemClient {
public:
	virtual auto GetNativePath() -> const char* = 0;
	virtual auto Shutdown() -> const char* = 0;
public:
	using Handle = uint32_t;
	virtual auto Flush ( Handle file ) -> void = 0;
	virtual auto Walk  ( uint16_t nwname, const char* wname ) -> void = 0;
	virtual auto Open  ( const char* path, const char* mode,  ) -> Handle = 0;
	virtual auto Create( const char* name, dirmode_t perm, openmode_t mode,  ) -> Handle = 0;
	virtual auto Read  ( Handle file, uint64_t offset, uint32_t count ) -> void = 0;
	virtual auto Write ( Handle file, uint64_t offset, uint32_t count ) -> void = 0;
	virtual auto Remove( Handle file ) -> void = 0;
	virtual auto Stat  ( Handle file ) -> void = 0;
protected:
	using PHandle = struct { uint8_t m_uClientType; uint8_t m_uClientId; uint16_t m_uFileHandle; };
};
