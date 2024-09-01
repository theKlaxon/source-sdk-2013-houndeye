//
// Created by ENDERZOMBI102 on 31/08/2024.
//
#include "appframework/AppFramework.h"


namespace {
	void* s_AppInstance{ nullptr };
}


void* GetAppInstance() {
	return s_AppInstance;
}

void SetAppInstance( void* hInstance ) {
	s_AppInstance = hInstance;
}

// --- CSteamApplication ---
CSteamApplication::CSteamApplication( CSteamAppSystemGroup* pAppSystemGroup )
	: m_pChildAppSystemGroup{ pAppSystemGroup } {
}

int CSteamApplication::Startup() {
	return this->m_pChildAppSystemGroup->Startup();
}
void CSteamApplication::Shutdown() {
	this->m_pChildAppSystemGroup->Shutdown();
}

// CSteamApplication - IAppSystem
bool CSteamApplication::Create() {
	return this->m_pChildAppSystemGroup->Create();
}
bool CSteamApplication::PreInit() {
	return this->m_pChildAppSystemGroup->PreInit();
}
int CSteamApplication::Main() {
	return this->m_pChildAppSystemGroup->Main();
}
void CSteamApplication::PostShutdown() {
	this->m_pChildAppSystemGroup->PostShutdown();
}
void CSteamApplication::Destroy() {
	this->m_pChildAppSystemGroup->Destroy();
}
