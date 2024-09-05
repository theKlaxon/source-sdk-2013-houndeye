//
// Created by ENDERZOMBI102 on 11/09/2023.
//

#pragma once

#include "inputsystem/iinputsystem.h"
#include "tier1/tier1.h"
#include <memory>


class CInputSystem : public CTier1AppSystem<IInputSystem> {
	using BaseClass = CTier1AppSystem<IInputSystem>;
public:  // IAppSystem
	InitReturnVal_t Init() override;
	void Shutdown() override;
public:  // IInputSystem
	void AttachToWindow( void* hWnd ) override;
	void DetachFromWindow() override;

	void EnableInput( bool bEnable ) override;

	void EnableMessagePump( bool bEnable ) override;

	void PollInputState() override;

	int GetPollTick() const override;

	bool IsButtonDown( ButtonCode_t code ) const override;

	int GetButtonPressedTick( ButtonCode_t code ) const override;
	int GetButtonReleasedTick( ButtonCode_t code ) const override;

	int GetAnalogValue( AnalogCode_t code ) const override;

	int GetAnalogDelta( AnalogCode_t code ) const override;

	int GetEventCount() const override;
	const InputEvent_t* GetEventData( ) const override;

	void PostUserEvent( const InputEvent_t &event ) override;

	int GetJoystickCount() const override;

	void EnableJoystickInput( int nJoystick, bool bEnable ) override;

	void EnableJoystickDiagonalPOV( int nJoystick, bool bEnable ) override;

	void SampleDevices() override;

	void SetRumble( float fLeftMotor, float fRightMotor, int userId = INVALID_USER_ID ) override;
	void StopRumble() override;

	void ResetInputState() override;

	void SetPrimaryUserId( int userId ) override;

	const char* ButtonCodeToString( ButtonCode_t code ) const override;
	const char* AnalogCodeToString( AnalogCode_t code ) const override;
	ButtonCode_t StringToButtonCode( const char *pString ) const override;
	AnalogCode_t StringToAnalogCode( const char *pString ) const override;

	void SleepUntilInput( int nMaxSleepTimeMS = -1 ) override;

	ButtonCode_t VirtualKeyToButtonCode( int nVirtualKey ) const override;
	int ButtonCodeToVirtualKey( ButtonCode_t code ) const override;
	ButtonCode_t ScanCodeToButtonCode( int lParam ) const override;

	int GetPollCount() const override;

	void SetCursorPosition( int x, int y ) override;

	void* GetHapticsInterfaceAddress() const override;

	void SetNovintPure( bool bPure ) override;

	bool GetRawMouseAccumulators( int& accumX, int& accumY ) override;

	void SetConsoleTextMode( bool bConsoleTextMode ) override;
private:
	class CMessagePumpThread : public CThread {
		int Run() override;
	};
	struct JoystickState_t {

	};
	struct State_t {
		bool m_Buttons[ButtonCode_t::BUTTON_CODE_COUNT] { false };
		int m_MouseAccX{};
		int m_MouseAccY{};
		JoystickState_t m_Joysticks[4] {};
	};
	SDL_Window* m_SdlWindow{nullptr};


	bool m_Enabled{false};
	bool m_Running{false};
	bool m_ConsoleTextMode{false};

	State_t m_State{};

	CMessageQueue<InputEvent_t> m_EventQueue{};
	CMessagePumpThread m_EventPump{};
};
