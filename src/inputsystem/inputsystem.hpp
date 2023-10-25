//
// Created by ENDERZOMBI102 on 11/09/2023.
//

#pragma once

#include "inputsystem/iinputsystem.h"
#include "tier1/tier1.h"
#include <memory>


class CInputSystem : public CTier1AppSystem<IInputSystem> {
private:
	using BaseClass = CTier1AppSystem<IInputSystem>;
public:
	InitReturnVal_t Init() override;
	void Shutdown() override;

	// Attach, detach input system from a particular window
	// This window should be the root window for the application
	// Only 1 window should be attached at any given time.
	void AttachToWindow( void* hWnd ) override;
	void DetachFromWindow( ) override;

	// Enables/disables input. PollInputState will not update current
	// button/analog states when it is called if the system is disabled.
	void EnableInput( bool bEnable ) override;

	// Enables/disables the windows message pump. PollInputState will not
	// Peek/Dispatch messages if this is disabled
	void EnableMessagePump( bool bEnable ) override;

	// Polls the current input state
	void PollInputState() override;

	// Gets the time of the last polling in ms
	int GetPollTick() const override;

	// Is a button down? "Buttons" are binary-state input devices (mouse buttons, keyboard keys)
	bool IsButtonDown( ButtonCode_t code ) const override;

	// Returns the tick at which the button was pressed and released
	int GetButtonPressedTick( ButtonCode_t code ) const override;
	int GetButtonReleasedTick( ButtonCode_t code ) const override;

	// Gets the value of an analog input device this frame
	// Includes joysticks, mousewheel, mouse
	int GetAnalogValue( AnalogCode_t code ) const override;

	// Gets the change in a particular analog input device this frame
	// Includes joysticks, mousewheel, mouse
	int GetAnalogDelta( AnalogCode_t code ) const override;

	// Returns the input events since the last poll
	int GetEventCount() const override;
	const InputEvent_t* GetEventData( ) const override;

	// Posts a user-defined event into the event queue; this is expected
	// to be called in overridden wndprocs connected to the root panel.
	void PostUserEvent( const InputEvent_t &event ) override;

	// Returns the number of joysticks
	int GetJoystickCount() const override;

	// Enable/disable joystick, it has perf costs
	void EnableJoystickInput( int nJoystick, bool bEnable ) override;

	// Enable/disable diagonal joystick POV (simultaneous POV buttons down)
	void EnableJoystickDiagonalPOV( int nJoystick, bool bEnable ) override;

	// Sample the joystick and append events to the input queue
	void SampleDevices( void ) override;

	// FIXME: Currently force-feedback is only supported on the Xbox 360
	void SetRumble( float fLeftMotor, float fRightMotor, int userId = INVALID_USER_ID ) override;
	void StopRumble( void ) override;

	// Resets the input state
	void ResetInputState() override;

	// Sets a player as the primary user - all other controllers will be ignored.
	void SetPrimaryUserId( int userId ) override;

	// Convert back + forth between ButtonCode/AnalogCode + strings
	const char* ButtonCodeToString( ButtonCode_t code ) const override;
	const char* AnalogCodeToString( AnalogCode_t code ) const override;
	ButtonCode_t StringToButtonCode( const char *pString ) const override;
	AnalogCode_t StringToAnalogCode( const char *pString ) const override;

	// Sleeps until input happens. Pass a negative number to sleep infinitely
	void SleepUntilInput( int nMaxSleepTimeMS = -1 ) override;

	// Convert back + forth between codes + overrides
	// FIXME: This is a temporary piece of code
	ButtonCode_t VirtualKeyToButtonCode( int nVirtualKey ) const override;
	int ButtonCodeToVirtualKey( ButtonCode_t code ) const override;
	ButtonCode_t ScanCodeToButtonCode( int lParam ) const override;

	// How many times have we called PollInputState?
	int GetPollCount() const override;

	// Sets the cursor position
	void SetCursorPosition( int x, int y ) override;

	// NVNT get address to haptics interface
	void* GetHapticsInterfaceAddress() const override;

	void SetNovintPure( bool bPure ) override;

	// read and clear accumulated raw input values
	bool GetRawMouseAccumulators( int& accumX, int& accumY ) override;

	// tell the input system that we're not a game, we're console text mode.
	// this is used for dedicated servers to not initialize joystick system.
	// this needs to be called before CInputSystem::Init (e.g. in PreInit of
	// some system) if you want ot prevent the joystick system from ever
	// being initialized.
	void SetConsoleTextMode( bool bConsoleTextMode ) override;
private:
	class CMessagePumpThread : public CThread {
		int Run() override;
	};
	struct JoystickState_t {

	};
	struct State_t {
		bool m_bButtons[ButtonCode_t::BUTTON_CODE_COUNT] { false };
		int m_nMouseAccX{};
		int m_nMouseAccY{};
		JoystickState_t m_cJoysticks[4] {};
	};
	struct ButtonEntry_t {
		const char* name;
		const ButtonCode_t code;
	};
	SDL_Window* m_pSdlWindow{nullptr};


	bool m_bEnabled{false};
	bool m_bRunning{false};
	bool m_bConsoleTextMode{false};

	State_t m_cState{};
	static std::array<ButtonEntry_t, 107> BUTTON_MAP;

	CMessageQueue<InputEvent_t> m_cEventQueue{};
	CMessagePumpThread m_pEventPump{};
};

extern CInputSystem gInputSystem;