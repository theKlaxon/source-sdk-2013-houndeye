//
// Created by ENDERZOMBI102 on 23/02/2024.
//
#pragma once
#include "tier0/platform.h"
#include <functional>
#include <memory>

using xerr_t = uint8_t;
using qid_t = int;
using stat_t = int;
using xstr_t = int;

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

enum openmode_t : uint8_t {
	X9P_OPEN_READ,
	X9P_OPEN_WRITE,
	X9P_OPEN_READWRITE,
	X9P_OPEN_EXEC,
	X9P_OPEN_TRUNC = 0x10,
	X9P_OPEN_CLOSE = 0x40,

	X9P_OPEN_STATE_MASK = X9P_OPEN_READ | X9P_OPEN_WRITE | X9P_OPEN_READWRITE | X9P_OPEN_EXEC,
	X9P_OPEN_PROTOCOL_MASK = X9P_OPEN_STATE_MASK | X9P_OPEN_TRUNC | X9P_OPEN_CLOSE,

	// Bits not used by by 9P
	X9P_OPEN_EX_INVALID = 128,
};




typedef std::function<void( xerr_t err )> FlushCallback;
typedef std::function<void( xerr_t err, uint16_t nwqid, qid_t* wqid )> WalkCallback;
typedef std::function<void( xerr_t err, qid_t* qid, uint32_t iounit )> OpenCallback;
typedef std::function<void( xerr_t err, qid_t* qid, uint32_t iounit )> CreateCallback;
typedef std::function<void( xerr_t err, uint32_t count, void* data )> ReadCallback;
typedef std::function<void( xerr_t err, uint32_t count )> WriteCallback;
typedef std::function<void( xerr_t err )> RemoveCallback;
typedef std::function<void( xerr_t err, stat_t* stat )> StatCallback;

abstract_class ISystemClient {
public:
	virtual auto Flush( FlushCallback callback ) -> void = 0;
	virtual auto Walk( uint16_t nwname, xstr_t wname, WalkCallback callback ) -> void = 0;
	virtual auto Open( openmode_t mode, OpenCallback callback ) -> void = 0;
	virtual auto Create( xstr_t name, dirmode_t perm, openmode_t mode, CreateCallback callback ) -> void = 0;
	virtual auto Read( uint64_t offset, uint32_t count, ReadCallback callback ) -> void = 0;
	virtual auto Write( uint64_t offset, uint32_t count, void* data, WriteCallback callback ) -> void = 0;
	virtual auto Remove( RemoveCallback callback ) -> void = 0;
	virtual auto Stat( StatCallback callback ) -> void = 0;
	virtual auto GetNativePath() -> const char* = 0;
	virtual auto Shutdown() -> const char* = 0;

	static auto CreateFor( const char* string ) -> std::shared_ptr<ISystemClient>;
};
