//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "usermessages.h"

// This provides an alternative to HOOK_MESSAGE, where you can declare it globally
// instead of finding a place to run it.
// It registers a function called __MsgFunc_<msgName>
#define USER_MESSAGE_REGISTER( msgName ) \
	static CUserMessageRegister userMessageRegister_##msgName( #msgName, __MsgFunc_##msgName );


class CUserMessageRegister {
public:
	CUserMessageRegister( const char* pMessageName, pfnUserMsgHook pHookFn );

	// This is called at startup to register all the user messages.
	static void RegisterAll();


private:
	const char* m_pMessageName;
	pfnUserMsgHook m_pHookFn;

	// Linked list of all the CUserMessageRegisters.
	static CUserMessageRegister* s_pHead;
	CUserMessageRegister* m_pNext;
};
