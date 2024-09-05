//
// Created by ENDERZOMBI102 on 06/09/2023.
//
#include "SDL3/SDL.h"
#include "ButtonEntry.hpp"
#include "icommandline.h"
#include "inputsystem.hpp"

InitReturnVal_t CInputSystem::Init() {
	int res{ SDL_InitSubSystem( SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC ) };

	if ( res != 0 ) {
		Error( "[AuroraSource|InputSystem] Failed to initialize SDL (%s)", SDL_GetError() );
		return InitReturnVal_t::INIT_FAILED;
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
	if ( hWnd == nullptr ) {
		Error( "Called `CInputSystem::AttachToWindow` with a `nullptr`!" );
	}

	if ( m_SdlWindow ) {
		Error( "`CInputSystem::AttachToWindow`: Cannot attach to two windows at once!" );
	}

	auto props { SDL_CreateProperties() };
	#if IsWindows()
		SDL_SetNumberProperty( props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, reinterpret_cast<int>( hWnd ) );
	#elif IsLinux()
		// TODO: When we move to wayland, this should change!
		SDL_SetNumberProperty( props, SDL_PROPERTY_WINDOW_X11_WINDOW_NUMBER, reinterpret_cast<int>( hWnd ) );
	#endif

		m_SdlWindow = SDL_CreateWindowWithProperties( props );
	AssertMsg( m_SdlWindow != nullptr, "%s", SDL_GetError() );
}

void CInputSystem::DetachFromWindow() {
	if ( m_SdlWindow == nullptr ) {
		DevWarning( "Called `CInputSystem::DetachFromWindow` when not attached to a window!" );
		return;
	}

	SDL_DestroyWindow( m_SdlWindow );
	m_SdlWindow = nullptr;
}

void CInputSystem::EnableInput( bool bEnable ) {
	m_Enabled = bEnable;
}

void CInputSystem::EnableMessagePump( bool bEnable ) {
	// asked to disable
	if ( bEnable ) {
		// if are we already disabled skip
		if (! m_Running ) {
			return;
		}
		// disable the pump
		m_Running = false;
		m_EventPump.Join();
	} else {
		// if are we already enabled skip
		if ( m_Running ) {
			return;
		}
		// enable the pump
		m_Running = true;
		m_EventPump.Start();
	}
}

void CInputSystem::PollInputState() {
	if (! m_Enabled ) {
		return;
	}
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

	return m_State.m_Buttons[ code ];
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
	int count{0};
	SDL_GetJoysticks( &count );

	if ( count < 0 ) {
		DevWarning( "[AuroraSource|InputSystem] Failed to enumerate joysticks: %s", SDL_GetError() );
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
	V_memset( &m_State.m_Buttons, 0, ButtonCode_t::BUTTON_CODE_COUNT );
	// reset mouse motion accumulators
	m_State.m_MouseAccX = 0;
	m_State.m_MouseAccY = 0;
}

void CInputSystem::SetPrimaryUserId( int userId ) {
	AssertMsg( false, "TODO: `CInputSystem::SetPrimaryUserId( %d )` not implemented", userId );
}

const char* CInputSystem::ButtonCodeToString( ButtonCode_t code ) const {
	for ( auto entry : BUTTON_MAP ) {
		if ( entry.code == code ) {
			return entry.name;
		}
	}
	return "";
}

const char* CInputSystem::AnalogCodeToString( AnalogCode_t code ) const {
	AssertMsg( false, "TODO: `CInputSystem::AnalogCodeToString( %d )` not implemented", code );
	return nullptr;
}

ButtonCode_t CInputSystem::StringToButtonCode( const char* pString ) const {
	for ( auto& entry : BUTTON_MAP ) {
		if ( entry.name && Q_strcmp( entry.name, pString ) == 0 ) {
			return entry.code;
		}
	}
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
	// FIXME: This doesn't sound good
	if ( SDL_WarpMouseGlobal( x, y ) < 0 ) {
		DevWarning( "[AuroraSource|InputSystem] Failed to warp mouse pointer: %s", SDL_GetError() );
	}
}

void* CInputSystem::GetHapticsInterfaceAddress() const {
	AssertMsg( false, "TODO: `CInputSystem::GetHapticsInterfaceAddress()` not implemented" );
	return nullptr;
}

void CInputSystem::SetNovintPure( bool bPure ) {
	AssertMsg( false, "TODO: `CInputSystem::SetNovintPure( %d )` not implemented", bPure );
}

bool CInputSystem::GetRawMouseAccumulators( int& accumX, int& accumY ) {
	accumX = m_State.m_MouseAccX;
	m_State.m_MouseAccX = 0;

	accumY = m_State.m_MouseAccY;
	m_State.m_MouseAccY = 0;

	return true;
}

void CInputSystem::SetConsoleTextMode( bool bConsoleTextMode ) {
	m_ConsoleTextMode = bConsoleTextMode;
}

namespace {
	CInputSystem s_InputSystem{};
}
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CInputSystem, IInputSystem, INPUTSYSTEM_INTERFACE_VERSION, s_InputSystem )

int CInputSystem::CMessagePumpThread::Run() {
	SDL_Event sdlEvent;
	while ( s_InputSystem.m_Running ) {

		while ( SDL_PollEvent( &sdlEvent ) ) {
			InputEvent_t inputEvent{};
			switch ( sdlEvent.type ) {
				case SDL_EventType::SDL_EVENT_QUIT:
					inputEvent.m_nType = InputEventType_t::IE_Quit;
					break;
			}

			s_InputSystem.m_EventQueue.QueueMessage( inputEvent );
		}
	}
	return 0;
}
