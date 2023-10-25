//
// Created by ENDERZOMBI102 on 06/09/2023.
//

#include <SDL.h>
#include <array>

#include "icommandline.h"
#include "inputsystem.hpp"

InitReturnVal_t CInputSystem::Init() {
	if ( CommandLine()->CheckParm( "-nojoy" ) ) ;

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
	AssertMsg( false, "TODO: `CInputSystem::EnableInput( %d )` not implemented", bEnable );
}

void CInputSystem::EnableMessagePump( bool bEnable ) {
	// asked to disable
	if ( bEnable ) {
		// if are we already disabled skip
		if ( !this->m_bRunning )
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
	AssertMsg( false, "TODO: `CInputSystem::PollInputState()` not implemented" );
}

int CInputSystem::GetPollTick() const {
	AssertMsg( false, "TODO: `CInputSystem::GetPollTick()` not implemented" );
	SDL_GetTicks();
	return 0;
}

bool CInputSystem::IsButtonDown( ButtonCode_t code ) const {
	if ( code >= ButtonCode_t::BUTTON_CODE_LAST ) {
		AssertMsg( false, "Given ButtonCode_t is too high! (%d > %d)", code, ButtonCode_t::BUTTON_CODE_LAST );
		return false;
	}

	return this->m_cState.m_bButtons[ code ];
}

int CInputSystem::GetButtonPressedTick( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetButtonPressedTick( %d )` not implemented", code );
	return 0;
}

int CInputSystem::GetButtonReleasedTick( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetButtonReleasedTick( %d )` not implemented", code );
	return 0;
}

int CInputSystem::GetAnalogValue( AnalogCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetAnalogValue( %d )` not implemented", code );
	return 0;
}

int CInputSystem::GetAnalogDelta( AnalogCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::GetAnalogDelta( %d )` not implemented", code );
	return 0;
}

int CInputSystem::GetEventCount() const {
	AssertMsg( false, "TODO: `CInputSystem::GetEventCount()` not implemented" );
	return 0;
}

const InputEvent_t* CInputSystem::GetEventData() const {
	AssertMsg( false, "TODO: `CInputSystem::GetEventData()` not implemented" );
	return nullptr;
}

void CInputSystem::PostUserEvent( const InputEvent_t& event ) {
	AssertMsg( false, "TODO: `CInputSystem::PostUserEvent( { m_nType: %d, m_nTick: %d, m_nData: %d, m_nData2: %d, m_nData3: %d } )` not implemented", event.m_nType, event.m_nTick, event.m_nData, event.m_nData2, event.m_nData3 );
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
	AssertMsg( false, "TODO: `CInputSystem::EnableJoystickInput( %d, %d )` not implemented", nJoystick, bEnable );
}

void CInputSystem::EnableJoystickDiagonalPOV( int nJoystick, bool bEnable ) {
	AssertMsg( false, "TODO: `CInputSystem::EnableJoystickDiagonalPOV( %d, %d )` not implemented", nJoystick, bEnable );
}

void CInputSystem::SampleDevices() {
	AssertMsg( false, "TODO: `CInputSystem::SampleDevices()` not implemented" );
}

void CInputSystem::SetRumble( float fLeftMotor, float fRightMotor, int userId ) {
	AssertMsg( false, "TODO: `CInputSystem::SetRumble( %f, %f, %d )` not implemented", fLeftMotor, fRightMotor, userId );
}

void CInputSystem::StopRumble() {
	AssertMsg( false, "TODO: `CInputSystem::StopRumble()` not implemented" );
}

void CInputSystem::ResetInputState() {
	AssertMsg( false, "TODO: `CInputSystem::ResetInputState()` not implemented" );
	// reset button states
	V_memset( &this->m_cState.m_bButtons, 0, ButtonCode_t::BUTTON_CODE_COUNT );
	// reset mouse motion accumulators
	this->m_cState.m_nMouseAccX = 0;
	this->m_cState.m_nMouseAccY = 0;
}

void CInputSystem::SetPrimaryUserId( int userId ) {
	AssertMsg( false, "TODO: `CInputSystem::SetPrimaryUserId( %d )` not implemented", userId );
}

const char* CInputSystem::ButtonCodeToString( ButtonCode_t code ) const {
	for ( auto entry : CInputSystem::BUTTON_MAP )
		if ( entry.code == code )
			return entry.name;
	return "";
}

const char* CInputSystem::AnalogCodeToString( AnalogCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::AnalogCodeToString( %d )` not implemented", code );
	return nullptr;
}

ButtonCode_t CInputSystem::StringToButtonCode( const char* pString ) const {
	for ( auto& entry : CInputSystem::BUTTON_MAP )
		if ( entry.name && Q_strcmp( entry.name, pString ) == 0 )
			return entry.code;
	return BUTTON_CODE_INVALID;
}

AnalogCode_t CInputSystem::StringToAnalogCode( const char* pString ) const {
	AssertMsg( false, "TODO: `CInputSystem::StringToAnalogCode` not implemented" );
	return ANALOG_CODE_INVALID;
}

void CInputSystem::SleepUntilInput( int nMaxSleepTimeMS ) {
	AssertMsg( false, "TODO: `CInputSystem::SleepUntilInput( %d )` not implemented", nMaxSleepTimeMS );
}

ButtonCode_t CInputSystem::VirtualKeyToButtonCode( int nVirtualKey ) const {
	AssertMsg( false, "TODO: `CInputSystem::VirtualKeyToButtonCode( %d )` not implemented", nVirtualKey );
	return KEY_CAPSLOCK;
}

int CInputSystem::ButtonCodeToVirtualKey( ButtonCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::ButtonCodeToVirtualKey( %d )` not implemented", code );
	return 0;
}

ButtonCode_t CInputSystem::ScanCodeToButtonCode( int lParam ) const {
	AssertMsg( false, "TODO: `CInputSystem::ScanCodeToButtonCode( %d )` not implemented", lParam );
	return KEY_CAPSLOCK;
}

int CInputSystem::GetPollCount() const {
	AssertMsg( false, "TODO: `CInputSystem::GetPollCount()` not implemented" );
	return 0;
}

void CInputSystem::SetCursorPosition( int x, int y ) {
	if ( SDL_WarpMouseGlobal( x, y ) < 0 )
		DevWarning( "[AuroraSource | InputSystem] Failed to warp mouse pointer: %s", SDL_GetError() );
}

void* CInputSystem::GetHapticsInterfaceAddress() const {
	AssertMsg( false, "TODO: `CInputSystem::GetHapticsInterfaceAddress()` not implemented" );
	return nullptr;
}

void CInputSystem::SetNovintPure( bool bPure ) {
	AssertMsg( false, "TODO: `CInputSystem::SetNovintPure( %d )` not implemented", bPure );
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

		while ( SDL_PollEvent( &sdlEvent ) ) {
			InputEvent_t inputEvent{};
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

std::array<CInputSystem::ButtonEntry_t, 107> CInputSystem::BUTTON_MAP {{
	{ "0", ButtonCode_t::KEY_0 },
	{ "1", ButtonCode_t::KEY_1 },
	{ "2", ButtonCode_t::KEY_2 },
	{ "3", ButtonCode_t::KEY_3 },
	{ "4", ButtonCode_t::KEY_4 },
	{ "5", ButtonCode_t::KEY_5 },
	{ "6", ButtonCode_t::KEY_6 },
	{ "7", ButtonCode_t::KEY_7 },
	{ "8", ButtonCode_t::KEY_8 },
	{ "9", ButtonCode_t::KEY_9 },
	{ "A", ButtonCode_t::KEY_A },
	{ "B", ButtonCode_t::KEY_B },
	{ "C", ButtonCode_t::KEY_C },
	{ "D", ButtonCode_t::KEY_D },
	{ "E", ButtonCode_t::KEY_E },
	{ "F", ButtonCode_t::KEY_F },
	{ "G", ButtonCode_t::KEY_G },
	{ "H", ButtonCode_t::KEY_H },
	{ "I", ButtonCode_t::KEY_I },
	{ "J", ButtonCode_t::KEY_J },
	{ "K", ButtonCode_t::KEY_K },
	{ "L", ButtonCode_t::KEY_L },
	{ "M", ButtonCode_t::KEY_M },
	{ "N", ButtonCode_t::KEY_N },
	{ "O", ButtonCode_t::KEY_O },
	{ "P", ButtonCode_t::KEY_P },
	{ "Q", ButtonCode_t::KEY_Q },
	{ "R", ButtonCode_t::KEY_R },
	{ "S", ButtonCode_t::KEY_S },
	{ "T", ButtonCode_t::KEY_T },
	{ "U", ButtonCode_t::KEY_U },
	{ "V", ButtonCode_t::KEY_V },
	{ "W", ButtonCode_t::KEY_W },
	{ "X", ButtonCode_t::KEY_X },
	{ "Y", ButtonCode_t::KEY_Y },
	{ "Z", ButtonCode_t::KEY_Z },
	{ "PAD_0", ButtonCode_t::KEY_PAD_0 },
	{ "PAD_1", ButtonCode_t::KEY_PAD_1 },
	{ "PAD_2", ButtonCode_t::KEY_PAD_2 },
	{ "PAD_3", ButtonCode_t::KEY_PAD_3 },
	{ "PAD_4", ButtonCode_t::KEY_PAD_4 },
	{ "PAD_5", ButtonCode_t::KEY_PAD_5 },
	{ "PAD_6", ButtonCode_t::KEY_PAD_6 },
	{ "PAD_7", ButtonCode_t::KEY_PAD_7 },
	{ "PAD_8", ButtonCode_t::KEY_PAD_8 },
	{ "PAD_9", ButtonCode_t::KEY_PAD_9 },
	{ "PAD_DIVIDE", ButtonCode_t::KEY_PAD_DIVIDE },
	{ "PAD_MULTIPLY", ButtonCode_t::KEY_PAD_MULTIPLY },
	{ "PAD_MINUS", ButtonCode_t::KEY_PAD_MINUS },
	{ "PAD_PLUS", ButtonCode_t::KEY_PAD_PLUS },
	{ "PAD_ENTER", ButtonCode_t::KEY_PAD_ENTER },
	{ "PAD_DECIMAL", ButtonCode_t::KEY_PAD_DECIMAL },
	{ "LBRACKET", ButtonCode_t::KEY_LBRACKET },
	{ "RBRACKET", ButtonCode_t::KEY_RBRACKET },
	{ "SEMICOLON", ButtonCode_t::KEY_SEMICOLON },
	{ "APOSTROPHE", ButtonCode_t::KEY_APOSTROPHE },
	{ "BACKQUOTE", ButtonCode_t::KEY_BACKQUOTE },
	{ "COMMA", ButtonCode_t::KEY_COMMA },
	{ "PERIOD", ButtonCode_t::KEY_PERIOD },
	{ "SLASH", ButtonCode_t::KEY_SLASH },
	{ "BACKSLASH", ButtonCode_t::KEY_BACKSLASH },
	{ "MINUS", ButtonCode_t::KEY_MINUS },
	{ "EQUAL", ButtonCode_t::KEY_EQUAL },
	{ "ENTER", ButtonCode_t::KEY_ENTER },
	{ "SPACE", ButtonCode_t::KEY_SPACE },
	{ "BACKSPACE", ButtonCode_t::KEY_BACKSPACE },
	{ "TAB", ButtonCode_t::KEY_TAB },
	{ "CAPSLOCK", ButtonCode_t::KEY_CAPSLOCK },
	{ "NUMLOCK", ButtonCode_t::KEY_NUMLOCK },
	{ "ESCAPE", ButtonCode_t::KEY_ESCAPE },
	{ "SCROLLLOCK", ButtonCode_t::KEY_SCROLLLOCK },
	{ "INSERT", ButtonCode_t::KEY_INSERT },
	{ "DELETE", ButtonCode_t::KEY_DELETE },
	{ "HOME", ButtonCode_t::KEY_HOME },
	{ "END", ButtonCode_t::KEY_END },
	{ "PAGEUP", ButtonCode_t::KEY_PAGEUP },
	{ "PAGEDOWN", ButtonCode_t::KEY_PAGEDOWN },
	{ "BREAK", ButtonCode_t::KEY_BREAK },
	{ "LSHIFT", ButtonCode_t::KEY_LSHIFT },
	{ "RSHIFT", ButtonCode_t::KEY_RSHIFT },
	{ "LALT", ButtonCode_t::KEY_LALT },
	{ "RALT", ButtonCode_t::KEY_RALT },
	{ "LCONTROL", ButtonCode_t::KEY_LCONTROL },
	{ "RCONTROL", ButtonCode_t::KEY_RCONTROL },
	{ "LWIN", ButtonCode_t::KEY_LWIN },
	{ "RWIN", ButtonCode_t::KEY_RWIN },
	{ "APP", ButtonCode_t::KEY_APP },
	{ "UP", ButtonCode_t::KEY_UP },
	{ "LEFT", ButtonCode_t::KEY_LEFT },
	{ "DOWN", ButtonCode_t::KEY_DOWN },
	{ "RIGHT", ButtonCode_t::KEY_RIGHT },
	{ "F1", ButtonCode_t::KEY_F1 },
	{ "F2", ButtonCode_t::KEY_F2 },
	{ "F3", ButtonCode_t::KEY_F3 },
	{ "F4", ButtonCode_t::KEY_F4 },
	{ "F5", ButtonCode_t::KEY_F5 },
	{ "F6", ButtonCode_t::KEY_F6 },
	{ "F7", ButtonCode_t::KEY_F7 },
	{ "F8", ButtonCode_t::KEY_F8 },
	{ "F9", ButtonCode_t::KEY_F9 },
	{ "F10", ButtonCode_t::KEY_F10 },
	{ "F11", ButtonCode_t::KEY_F11 },
	{ "F12", ButtonCode_t::KEY_F12 },
	{ "CAPSLOCKTOGGLE", ButtonCode_t::KEY_CAPSLOCKTOGGLE },
	{ "NUMLOCKTOGGLE", ButtonCode_t::KEY_NUMLOCKTOGGLE },
	{ "SCROLLLOCKTOGGLE", ButtonCode_t::KEY_SCROLLLOCKTOGGLE }
}};
