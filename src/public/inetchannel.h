//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "inetchannelinfo.h"
#include "tier0/platform.h"
#include "tier1/bitbuf.h"


class IDemoRecorder;
class INetMessage;
class INetChannelHandler;
class INetChannelInfo;
typedef struct netpacket_s netpacket_t;
typedef struct netadr_s netadr_t;

abstract_class INetChannel : public INetChannelInfo {
public:
	virtual ~INetChannel() = default;

	virtual void SetDataRate( float rate ) = 0;
	virtual bool RegisterMessage( INetMessage * msg ) = 0;
	virtual bool StartStreaming( unsigned int challengeNr ) = 0;
	virtual void ResetStreaming() = 0;
	virtual void SetTimeout( float seconds ) = 0;
	virtual void SetDemoRecorder( IDemoRecorder * recorder ) = 0;
	virtual void SetChallengeNr( unsigned int chnr ) = 0;

	virtual void Reset() = 0;
	virtual void Clear() = 0;
	virtual void Shutdown( const char* reason ) = 0;

	virtual void ProcessPlayback() = 0;
	virtual bool ProcessStream() = 0;
	virtual void ProcessPacket( struct netpacket_s * packet, bool bHasHeader ) = 0;

	virtual bool SendNetMsg( INetMessage & msg, bool bForceReliable = false, bool bVoice = false ) = 0;
	#if IsPosix()
		ALWAYS_INLINE bool SendNetMsg( INetMessage const& msg, bool bForceReliable = false, bool bVoice = false ) { return SendNetMsg( *( (INetMessage*) &msg ), bForceReliable, bVoice ); }
	#endif
	virtual bool SendData( bf_write & msg, bool bReliable = true ) = 0;
	virtual bool SendFile( const char* filename, unsigned int transferID ) = 0;
	virtual void DenyFile( const char* filename, unsigned int transferID ) = 0;
	virtual void RequestFile_OLD( const char* filename, unsigned int transferID ) = 0;// get rid of this function when we version the
	virtual void SetChoked() = 0;
	virtual int SendDatagram( bf_write * data ) = 0;
	virtual bool Transmit( bool onlyReliable = false ) = 0;

	virtual const netadr_t& GetRemoteAddress() const = 0;
	virtual INetChannelHandler* GetMsgHandler() const = 0;
	virtual int GetDropNumber() const = 0;
	virtual int GetSocket() const = 0;
	virtual unsigned int GetChallengeNr() const = 0;
	virtual void GetSequenceData( int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck ) = 0;
	virtual void SetSequenceData( int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck ) = 0;

	virtual void UpdateMessageStats( int msggroup, int bits ) = 0;
	virtual bool CanPacket() const = 0;
	virtual bool IsOverflowed() const = 0;
	virtual bool IsTimedOut() const = 0;
	virtual bool HasPendingReliableData() = 0;

	virtual void SetFileTransmissionMode( bool bBackgroundMode ) = 0;
	virtual void SetCompressionMode( bool bUseCompression ) = 0;
	virtual unsigned int RequestFile( const char* filename ) = 0;
	virtual float GetTimeSinceLastReceived() const = 0;// get time since last received packet in seconds

	virtual void SetMaxBufferSize( bool bReliable, int nBytes, bool bVoice = false ) = 0;

	virtual bool IsNull() const = 0;
	virtual int GetNumBitsWritten( bool bReliable ) = 0;
	virtual void SetInterpolationAmount( float flInterpolationAmount ) = 0;
	virtual void SetRemoteFramerate( float flFrameTime, float flFrameTimeStdDeviation ) = 0;

	// Max # of payload bytes before we must split/fragment the packet
	virtual void SetMaxRoutablePayloadSize( int nSplitSize ) = 0;
	virtual int GetMaxRoutablePayloadSize() = 0;

	virtual int GetProtocolVersion() = 0;
};
