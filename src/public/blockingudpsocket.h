//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "netadr.h"

class CBlockingUDPSocket {
public:
	explicit CBlockingUDPSocket();
	virtual ~CBlockingUDPSocket();

	bool WaitForMessage( float timeOutInSeconds );
	unsigned int ReceiveSocketMessage( struct sockaddr_in* packet_from, unsigned char* buf, size_t bufsize );
	bool SendSocketMessage( const struct sockaddr_in& rRecipient, const unsigned char* buf, size_t bufsize );

	bool IsValid() const { return m_Socket != 0; }

protected:
	bool CreateSocket();

	class CImpl;
	CImpl* m_pImpl;

	netadr_t m_cserIP;
	unsigned int m_Socket;
};
