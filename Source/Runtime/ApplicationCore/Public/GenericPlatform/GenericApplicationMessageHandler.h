// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "Misc/Optional.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>
class FGenericWindow;

namespace EMouseButtons
{
	enum Type
	{
		Left = 0,
		Middle,
		Right,
		Thumb01,
		Thumb02,

		Invalid,
	};
}

struct  FGamepadKeyNames
{
	typedef std::string Type;

	static const std::string Invalid;

	static const std::string LeftAnalogX;
	static const std::string LeftAnalogY;
	static const std::string RightAnalogX;
	static const std::string RightAnalogY;
	static const std::string LeftTriggerAnalog;
	static const std::string RightTriggerAnalog;

	static const std::string LeftThumb;
	static const std::string RightThumb;
	static const std::string SpecialLeft;
	static const std::string SpecialLeft_X;
	static const std::string SpecialLeft_Y;
	static const std::string SpecialRight;
	static const std::string FaceButtonBottom;
	static const std::string FaceButtonRight;
	static const std::string FaceButtonLeft;
	static const std::string FaceButtonTop;
	static const std::string LeftShoulder;
	static const std::string RightShoulder;
	static const std::string LeftTriggerThreshold;
	static const std::string RightTriggerThreshold;
	static const std::string DPadUp;
	static const std::string DPadDown;
	static const std::string DPadRight;
	static const std::string DPadLeft;

	static const std::string LeftStickUp;
	static const std::string LeftStickDown;
	static const std::string LeftStickRight;
	static const std::string LeftStickLeft;

	static const std::string RightStickUp;
	static const std::string RightStickDown;
	static const std::string RightStickRight;
	static const std::string RightStickLeft;
};

enum class EWindowActivation : uint8_t
{
	Activate,
	ActivateByMouse,
	Deactivate
};

namespace EWindowZone
{
	/**
	 * The Window Zone is the window area we are currently over to send back to the operating system
	 * for operating system compliance.
	 */
	enum Type
	{
		NotInWindow			= 0,
		TopLeftBorder		= 1,
		TopBorder			= 2,
		TopRightBorder		= 3,
		LeftBorder			= 4,
		ClientArea			= 5,
		RightBorder			= 6,
		BottomLeftBorder	= 7,
		BottomBorder		= 8,
		BottomRightBorder	= 9,
		TitleBar			= 10,
		MinimizeButton		= 11,
		MaximizeButton		= 12,
		CloseButton			= 13,
		SysMenu				= 14,

		/** No zone specified */
		Unspecified	= 0,
	};
}


namespace EWindowAction
{
	enum Type
	{
		ClickedNonClientArea	= 1,
		Maximize				= 2,
		Restore					= 3,
		WindowMenu				= 4,
	};
}


/**
 * 
 */
namespace EDropEffect
{
	enum Type
	{
		None = 0,
		Copy = 1,
		Move = 2,
		Link = 3,
	};
}


enum class EGestureEvent : uint8_t
{
	None,
	Scroll,
	Magnify,
	Swipe,
	Rotate,
	LongPress,
	Count
};


/** Defines the minimum and maximum dimensions that a window can take on. */
struct FWindowSizeLimits
{
public:
	FWindowSizeLimits& SetMinWidth(TOptional<float> InValue){ MinWidth = InValue; return *this; }
	const TOptional<float>& GetMinWidth() const { return MinWidth; }

	FWindowSizeLimits& SetMinHeight(TOptional<float> InValue){ MinHeight = InValue; return *this; }
	const TOptional<float>& GetMinHeight() const { return MinHeight; }

	FWindowSizeLimits& SetMaxWidth(TOptional<float> InValue){ MaxWidth = InValue; return *this; }
	const TOptional<float>& GetMaxWidth() const { return MaxWidth; }

	FWindowSizeLimits& SetMaxHeight(TOptional<float> InValue){ MaxHeight = InValue; return *this; }
	const TOptional<float>& GetMaxHeight() const { return MaxHeight; }

private:
	TOptional<float> MinWidth;
	TOptional<float> MinHeight;
	TOptional<float> MaxWidth;
	TOptional<float> MaxHeight;
};

/** 
 * Context scope that indicates which IInputDevice is currently being handled. 
 * This can be used to determine hardware-specific information when handling input from FGenericApplicationMessageHandler subclasses.
 * This is generally set during SendControllerEvents or Tick and is only valid on the game thread.
 */
class  FInputDeviceScope
{
public:
	/** The specific InputDevice that is currently being polled. This is only valid within the current function scope and may be null */
	class IInputDevice* InputDevice;

	/** Logical name of the input device interface. This is not translated but is platform-specific */
	std::string InputDeviceName;

	/** A system-specific device id, this is not the same as controllerId and represents a physical device instead of logical user. -1 represents an unknown device */
	int32_t HardwareDeviceHandle;

	/** Logical string identifying the hardware device. This is not translated and is system-specific, it may be empty */
	std::string HardwareDeviceIdentifier;

	/** Constructor, this should only be allocated directly on the stack */
	FInputDeviceScope(IInputDevice* InInputDevice, std::string InInputDeviceName, int32_t InHardwareDeviceHandle = -1, std::string InHardwareDeviceIdentifier = std::string());
	~FInputDeviceScope();

	/** Cannot be copied/moved */
	FInputDeviceScope() = delete;
	FInputDeviceScope(const FInputDeviceScope&) = delete;
	FInputDeviceScope& operator=(const FInputDeviceScope&) = delete;
	FInputDeviceScope(FInputDeviceScope&&) = delete;
	FInputDeviceScope& operator=(FInputDeviceScope&&) = delete;

	/** Returns the currently active InputDeviceScope. This is only valid to call on the game thread and may return null */
	static const FInputDeviceScope* GetCurrent();

private:
	static std::vector<FInputDeviceScope*> ScopeStack;
};

/** Interface that defines how to handle interaction with a user via hardware input and output */
class FGenericApplicationMessageHandler
{
public:

	virtual ~FGenericApplicationMessageHandler() {}

	virtual bool ShouldProcessUserInputMessages( const std::shared_ptr < FGenericWindow >& PlatformWindow ) const
	{
		return false;
	}

	virtual bool OnKeyChar( const char Character, const bool IsRepeat )
	{
		return false;
	}

	virtual bool OnKeyDown( const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat ) 
	{
		return false;
	}

	virtual bool OnKeyUp( const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat )
	{
		return false;
	}

	virtual void OnInputLanguageChanged()
	{
	}

	virtual bool OnMouseDown( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button )
	{
		return false;
	}

	virtual bool OnMouseDown( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button, const DirectX::XMFLOAT2 CursorPos )
	{
		return false;
	}

	virtual bool OnMouseUp( const EMouseButtons::Type Button )
	{
		return false;
	}

	virtual bool OnMouseUp( const EMouseButtons::Type Button, const DirectX::XMFLOAT2 CursorPos )
	{
		return false;
	}

	virtual bool OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button )
	{
		return false;
	}

	virtual bool OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button, const DirectX::XMFLOAT2 CursorPos )
	{
		return false;
	}

	virtual bool OnMouseWheel( const float Delta )
	{
		return false;
	}

	virtual bool OnMouseWheel( const float Delta, const DirectX::XMFLOAT2 CursorPos )
	{
		return false;
	}

	virtual bool OnMouseMove()
	{
		return false;
	}

	virtual bool OnRawMouseMove( const int32_t X, const int32_t Y )
	{
		return false;
	}

	virtual bool OnCursorSet()
	{
		return false;
	}

	virtual bool OnControllerAnalog( FGamepadKeyNames::Type KeyName, int32_t ControllerId, float AnalogValue )
	{
		return false;
	}

	virtual bool OnControllerButtonPressed( FGamepadKeyNames::Type KeyName, int32_t ControllerId, bool IsRepeat )
	{
		return false;
	}

	virtual bool OnControllerButtonReleased( FGamepadKeyNames::Type KeyName, int32_t ControllerId, bool IsRepeat )
	{
		return false;
	}

    virtual void OnBeginGesture()
    {
    }

	virtual bool OnTouchGesture( EGestureEvent GestureType, const DirectX::XMFLOAT2& Delta, float WheelDelta, bool bIsDirectionInvertedFromDevice )
	{
		return false;
	}
    
    virtual void OnEndGesture()
    {
    }



	virtual bool OnTouchStarted( const std::shared_ptr< FGenericWindow >& Window, const DirectX::XMFLOAT2& Location, float Force, int32_t TouchIndex, int32_t ControllerId )
	{
		return false;
	}


	virtual bool OnTouchMoved( const DirectX::XMFLOAT2& Location, float Force, int32_t TouchIndex, int32_t ControllerId )
	{
		return false;
	}

	virtual bool OnTouchEnded( const DirectX::XMFLOAT2& Location, int32_t TouchIndex, int32_t ControllerId )
	{
		return false;
	}

	virtual bool OnTouchForceChanged(const DirectX::XMFLOAT2& Location, float Force, int32_t TouchIndex, int32_t ControllerId)
	{
		return false;
	}

	virtual bool OnTouchFirstMove(const DirectX::XMFLOAT2& Location, float Force, int32_t TouchIndex, int32_t ControllerId)
	{
		return false;
	}

	virtual void ShouldSimulateGesture(EGestureEvent Gesture, bool bEnable)
	{

	}

	virtual bool OnMotionDetected( const DirectX::XMFLOAT3 & Tilt, const DirectX::XMFLOAT3& RotationRate, const DirectX::XMFLOAT3& Gravity, const DirectX::XMFLOAT3& Acceleration, int32_t ControllerId )
	{
		return false;
	}

	virtual bool OnSizeChanged( const std::shared_ptr< FGenericWindow >& Window, const int32_t Width, const int32_t Height, bool bWasMinimized = false )
	{
		return false;
	}

	virtual void OnOSPaint( const std::shared_ptr<FGenericWindow>& Window )
	{
	
	}

	virtual FWindowSizeLimits GetSizeLimitsForWindow( const std::shared_ptr<FGenericWindow>& Window ) const
	{
		return FWindowSizeLimits();
	}

	virtual void OnResizingWindow( const std::shared_ptr< FGenericWindow >& Window )
	{

	}

	virtual bool BeginReshapingWindow( const std::shared_ptr< FGenericWindow >& Window )
	{
		return true;
	}

	virtual void FinishedReshapingWindow( const std::shared_ptr< FGenericWindow >& Window )
	{

	}

	virtual void HandleDPIScaleChanged( const std::shared_ptr< FGenericWindow >& Window )
	{

	}

	virtual void SignalSystemDPIChanged(const std::shared_ptr< FGenericWindow >& Window)
	{

	}	

	virtual void OnMovedWindow( const std::shared_ptr< FGenericWindow >& Window, const int32_t X, const int32_t Y )
	{

	}

	virtual bool OnWindowActivationChanged( const std::shared_ptr< FGenericWindow >& Window, const EWindowActivation ActivationType )
	{
		return false;
	}

	virtual bool OnApplicationActivationChanged( const bool IsActive )
	{
		return false;
	}

	virtual bool OnConvertibleLaptopModeChanged()
	{
		return false;
	}

	virtual EWindowZone::Type GetWindowZoneForPoint( const std::shared_ptr< FGenericWindow >& Window, const int32_t X, const int32_t Y )
	{
		return EWindowZone::NotInWindow;
	}

	virtual void OnWindowClose( const std::shared_ptr< FGenericWindow >& Window )
	{

	}

	virtual EDropEffect::Type OnDragEnterText( const std::shared_ptr< FGenericWindow >& Window, const std::string& Text )
	{
		return EDropEffect::None;
	}

	virtual EDropEffect::Type OnDragEnterFiles( const std::shared_ptr< FGenericWindow >& Window, const std::vector< std::string >& Files )
	{
		return EDropEffect::None;
	}

	virtual EDropEffect::Type OnDragEnterExternal( const std::shared_ptr< FGenericWindow >& Window, const std::string& Text, const std::vector< std::string >& Files )
	{
		return EDropEffect::None;
	}

	virtual EDropEffect::Type OnDragOver( const std::shared_ptr< FGenericWindow >& Window )
	{
		return EDropEffect::None;
	}

	virtual void OnDragLeave( const std::shared_ptr< FGenericWindow >& Window )
	{

	}

	virtual EDropEffect::Type OnDragDrop( const std::shared_ptr< FGenericWindow >& Window )
	{
		return EDropEffect::None;
	}

	virtual bool OnWindowAction( const std::shared_ptr< FGenericWindow >& Window, const EWindowAction::Type InActionType)
	{
		return true;
	}

	virtual void SetCursorPos(const DirectX::XMFLOAT2& MouseCoordinate)
	{

	}
};
