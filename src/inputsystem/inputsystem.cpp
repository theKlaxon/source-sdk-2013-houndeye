//
// Created by ENDERZOMBI102 on 06/09/2023.
//

#include <SDL.h>

#include "icommandline.h"
#include "inputsystem.hpp"

InitReturnVal_t CInputSystem::Init() {
	if ( CommandLine()->CheckParm( "-nojoy" ) );

	auto res = SDL_InitSubSystem( SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC );

	if ( res < 0 ) {
		// TODO: Finish this
	}

	return BaseClass::Init();
}

void CInputSystem::Shutdown() {
	// TODO: Finish this
	BaseClass::Shutdown();
}

// IInputSystem
void CInputSystem::AttachToWindow( void* hWnd ) {
	// WHY DOES ERROR CRASH
	if ( hWnd == nullptr )
		Error( "Called `CInputSystem::AttachToWindow` with a `nullptr`!" );

	if ( this->m_pSdlWindow )
		Error( "`CInputSystem::AttachToWindow`: Cannot attach to two windows at once!" );

	this->m_pSdlWindow = SDL_CreateWindowFrom( hWnd );
	AssertMsg( this->m_pSdlWindow != nullptr, SDL_GetError() );
}

void CInputSystem::DetachFromWindow() {
	if ( this->m_pSdlWindow == nullptr ) {
		DevWarning( "Called `CInputSystem::DetachFromWindow` when not attached to a window!" );
		return;
	}

	SDL_DestroyWindow( this->m_pSdlWindow );
}

void CInputSystem::EnableInput( bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableInput` not implemented" );
}

void CInputSystem::EnableMessagePump( bool bEnable ) {
	// asked to disable
	if ( bEnable ) {
		// if are we already disabled skip
		if (! this->m_bRunning )
			return;

		// disable the pump
		this->m_bRunning = false;
		this->m_pEventPump.Join();
	} else {
		// if are we already enabled skip
		if ( this->m_bRunning )
			return;

		// enable the pump
		this->m_bRunning = true;
		this->m_pEventPump.Start();
	}
}

void CInputSystem::PollInputState() {
	AssertMsg( false, "TODO: `CInputSystem::PollInputState` not implemented" );
}

int CInputSystem::GetPollTick() const {
	AssertMsg( false, "TODO: `CInputSystem::GetPollTick` not implemented" );
	SDL_GetTicks();
	return 0;
}

bool CInputSystem::IsButtonDown( ButtonCode_t code ) const {
	if ( code >= ButtonCode_t::BUTTON_CODE_LAST ) {
		AssertMsg( false, "Given ButtonCode_t is too high! (%d > %d)", code, ButtonCode_t::BUTTON_CODE_LAST );
		return false;
	}

	return this->m_cState.m_bButtons[code];
}

int CInputSystem::GetButtonPressedTick( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetButtonPressedTick` not implemented" );
	return 0;
}

int CInputSystem::GetButtonReleasedTick( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetButtonReleasedTick` not implemented" );
	return 0;
}

int CInputSystem::GetAnalogValue( AnalogCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetAnalogValue` not implemented" );
	return 0;
}

int CInputSystem::GetAnalogDelta( AnalogCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetAnalogDelta` not implemented" );
	return 0;
}

int CInputSystem::GetEventCount() const {
	AssertMsg( false, "TODO: `CInputSystem::GetEventCount` not implemented" );
	return 0;
}

const InputEvent_t* CInputSystem::GetEventData() const {
	AssertMsg( false, "TODO: `CInputSystem:: CInputSystem` not implemented" );
	return nullptr;
}

void CInputSystem::PostUserEvent( const InputEvent_t& event ) {
	AssertMsg( false, "TODO: `CInputSystem::PostUserEvent` not implemented" );
}

int CInputSystem::GetJoystickCount() const {
	auto count = SDL_NumJoysticks();

	if ( count < 0 ) {
		DevWarning( "[AuroraSource | InputSystem] Failed to enumerate joysticks: %s", SDL_GetError() );
		return 0;
	}

	return count;
}

void CInputSystem::EnableJoystickInput( int nJoystick, bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableJoystickInput` not implemented" );
}

void CInputSystem::EnableJoystickDiagonalPOV( int nJoystick, bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableJoystickDiagonalPOV` not implemented" );
}

void CInputSystem::SampleDevices() {
	AssertMsg( false, "TODO: `CInputSystem::SampleDevices` not implemented" );
}

void CInputSystem::SetRumble( float fLeftMotor, float fRightMotor, int userId ) {
	AssertMsg( false, "TODO: `CInputSystem::SetRumble` not implemented" );
}

void CInputSystem::StopRumble() {
	AssertMsg( false, "TODO: `CInputSystem::StopRumble` not implemented" );
}

void CInputSystem::ResetInputState() {
	AssertMsg( false, "TODO: `CInputSystem::ResetInputState` not implemented" );
	// reset button states
	V_memset( &this->m_cState.m_bButtons, 0, ButtonCode_t::BUTTON_CODE_COUNT );
	// reset mouse motion accumulators
	this->m_cState.m_nMouseAccX = 0;
	this->m_cState.m_nMouseAccY = 0;
}

void CInputSystem::SetPrimaryUserId( int userId ) {
	AssertMsg( false, "TODO: `CInputSystem::SetPrimaryUserId` not implemented" );
}

const char* CInputSystem::ButtonCodeToString( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::ButtonCodeToString` not implemented" );
	return nullptr;
}

const char* CInputSystem::AnalogCodeToString( AnalogCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::AnalogCodeToString` not implemented" );
	return nullptr;
}

ButtonCode_t CInputSystem::StringToButtonCode( const char* pString ) const {
	AssertMsg( false, "TODO: `CInputSystem::StringToButtonCode` not implemented" );
	return KEY_CAPSLOCK;
}

AnalogCode_t CInputSystem::StringToAnalogCode( const char* pString ) const {
	AssertMsg( false, "TODO: `CInputSystem::StringToAnalogCode` not implemented" );
	return ANALOG_CODE_INVALID;
}

void CInputSystem::SleepUntilInput( int nMaxSleepTimeMS ) {
	AssertMsg( false, "TODO: `CInputSystem::SleepUntilInput` not implemented" );
}

ButtonCode_t CInputSystem::VirtualKeyToButtonCode( int nVirtualKey ) const {
	AssertMsg( false, "TODO: `CInputSystem::VirtualKeyToButtonCode` not implemented" );
	return KEY_CAPSLOCK;
}

int CInputSystem::ButtonCodeToVirtualKey( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::ButtonCodeToVirtualKey` not implemented" );
	return 0;
}

ButtonCode_t CInputSystem::ScanCodeToButtonCode( int lParam ) const {
	AssertMsg( false, "TODO: `CInputSystem::ScanCodeToButtonCode` not implemented" );
	return KEY_CAPSLOCK;
}

int CInputSystem::GetPollCount() const {
	AssertMsg( false, "TODO: `CInputSystem::GetPollCount` not implemented" );
	return 0;
}

void CInputSystem::SetCursorPosition( int x, int y ) {
	if ( SDL_WarpMouseGlobal( x, y ) < 0 )
		DevWarning( "[AuroraSource | InputSystem] Failed to warp mouse pointer: %s", SDL_GetError() );
}

void* CInputSystem::GetHapticsInterfaceAddress() const {
	AssertMsg( false, "TODO: `CInputSystem::GetHapticsInterfaceAddress` not implemented" );
	return nullptr;
}

void CInputSystem::SetNovintPure( bool bPure ) {
	AssertMsg( false, "TODO: `CInputSystem::SetNovintPure` not implemented" );
}

bool CInputSystem::GetRawMouseAccumulators( int& accumX, int& accumY ) {
	accumX = this->m_cState.m_nMouseAccX;
	this->m_cState.m_nMouseAccX = 0;

	accumY = this->m_cState.m_nMouseAccY;
	this->m_cState.m_nMouseAccY = 0;

	return true;
}

void CInputSystem::SetConsoleTextMode( bool bConsoleTextMode ) {
	this->m_bConsoleTextMode = bConsoleTextMode;
}


CInputSystem gInputSystem{};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CInputSystem, IInputSystem, INPUTSYSTEM_INTERFACE_VERSION, gInputSystem )

int CInputSystem::CMessagePumpThread::Run() {
	SDL_Event sdlEvent;
	while ( gInputSystem.m_bRunning ) {

		while( SDL_PollEvent( &sdlEvent ) ) {
			InputEvent_t inputEvent{ };
			switch ( sdlEvent.type ) {
				case SDL_EventType::SDL_QUIT:
					inputEvent.m_nType = InputEventType_t::IE_Quit;
					break;
			}

			gInputSystem.m_cEventQueue.QueueMessage( inputEvent );
		}
	}
	return 0;
}
