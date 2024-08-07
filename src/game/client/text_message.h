//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once

abstract_class IHudTextMessage {
public:
	virtual char* LocaliseTextString( const char* msg, char* dst_buffer, int buffer_size ) = 0;
	virtual char* BufferedLocaliseTextString( const char* msg ) = 0;
	virtual const char* LookupString( const char* msg_name, int* msg_dest = NULL ) = 0;
};

extern IHudTextMessage* hudtextmessage;