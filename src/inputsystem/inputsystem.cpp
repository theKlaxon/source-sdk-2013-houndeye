//
// Created by ENDERZOMBI102 on 06/09/2023.
//

#include <SDL.h>

#include "icommandline.h"
#include "inputsystem.hpp"
#include "strtools.h"

InitReturnVal_t CInputSystem::Init() {
	if ( CommandLine()->CheckParm( "-nojoy" ) );

	auto res = SDL_InitSubSystem( SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC );

	if ( res < 0 ) {

	}

	return BaseClass::Init();
}

void CInputSystem::Shutdown() {
	BaseClass::Shutdown();
}

// IInputSystem
void CInputSystem::AttachToWindow( void* hWnd ) {
	// WHY DOES ERROR CRASH
	if ( hWnd == nullptr )
		Error( "Called `CInputSystem::AttachToWindow` with a `nullptr`!" );

	this->m_pSdlWindow = SDL_CreateWindowFrom( hWnd );
	AssertMsg( this->m_pSdlWindow != nullptr, SDL_GetError() );
}

void CInputSystem::DetachFromWindow() {
	if ( this->m_pSdlWindow == nullptr ) {
		Warning( "Called `CInputSystem::DetachFromWindow` when not attached to a window!" );
		return;
	}

	SDL_DestroyWindow( this->m_pSdlWindow );
}

void CInputSystem::EnableInput( bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableInput` not implemented" );
}

void CInputSystem::EnableMessagePump( bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableMessagePump` not implemented" );
}

void CInputSystem::PollInputState() {
	AssertMsg( false, "TODO: `CInputSystem::PollInputState` not implemented" );
}

int CInputSystem::GetPollTick() const {
	AssertMsg( false, "TODO: `CInputSystem::GetPollTick` not implemented" );
	return 0;
}

bool CInputSystem::IsButtonDown( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::IsButtonDown` not implemented" );
	return false;
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
	AssertMsg( false, "TODO: `CInputSystem::GetJoystickCount` not implemented" );
	return 0;
}

void CInputSystem::EnableJoystickInput( int nJoystick, bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableJoystickInput` not implemented" );
}

void CInputSystem::EnableJoystickDiagonalPOV( int nJoystick, bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableJoystickDiagonalPOV` not implemented" );
}

void CInputSystem::SampleDevices() {
	SDL_
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
	SDL_WarpMouseGlobal( x, y );
}

void* CInputSystem::GetHapticsInterfaceAddress() const {
	AssertMsg( false, "TODO: `CInputSystem::GetHapticsInterfaceAddress` not implemented" );
	return nullptr;
}

void CInputSystem::SetNovintPure( bool bPure ) {
	AssertMsg( false, "TODO: `CInputSystem::SetNovintPure` not implemented" );
}

bool CInputSystem::GetRawMouseAccumulators( int& accumX, int& accumY ) {
	AssertMsg( false, "TODO: `CInputSystem::GetRawMouseAccumulators` not implemented" );
	return false;
}

void CInputSystem::SetConsoleTextMode( bool bConsoleTextMode ) {
	this->m_bConsoleTextMode = bConsoleTextMode;
}


CInputSystem gInputSystem{};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CInputSystem, IInputSystem, INPUTSYSTEM_INTERFACE_VERSION, gInputSystem )

