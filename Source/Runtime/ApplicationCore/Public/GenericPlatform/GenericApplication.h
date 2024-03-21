#pragma once
#include<vector>
#include<functional>
#include<glm/vec2.hpp>

#include "DeviceInputManager.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericPlatform/GenericWindow.h"
#include "PlayerID.h"
#include "Misc/HighResolutionTimer.h"
#include "Misc/CoreMiscDefines.h"
class FSlateApplication;
class IAnalyticsProvider;

class IInputInterface;
class ITextInputMethodSystem;
class IForceFeedbackSystem;



namespace EPopUpOrientation
{
	enum Type
	{
		Horizontal,
		Vertical,
	};
}


/**
 * FModifierKeysState stores the pressed state of keys that are commonly used as modifiers
 */



struct FPlatformRect
{
	int32_t Left{ 0 };
	int32_t Top{ 0 };
	int32_t Right{ 0 };
	int32_t Bottom{ 0 };

	FPlatformRect() {}
	FPlatformRect(int32_t InLeft, int32_t InTop, int32_t InRight, int32_t InBottom) : Left(InLeft), Top(InTop), Right(InRight), Bottom(InBottom) {}

	bool operator==(const FPlatformRect& Other) const
	{
		return Left == Other.Left && Top == Other.Top && Right == Other.Right && Bottom == Other.Bottom;
	}
};


/**
 * Contains info on a physical monitor connected to the display device
 */
struct FMonitorInfo
{
	std::string Name;
	std::string ID;
	int32_t NativeWidth;
	int32_t NativeHeight;
	FPlatformRect DisplayRect;
	FPlatformRect WorkArea;
	bool bIsPrimary;
};


/**
 * Contains metrics information for the desktop area
 */
struct FDisplayMetrics
{
	FDisplayMetrics() : TitleSafePaddingSize(0, 0, 0, 0)
		, ActionSafePaddingSize(0, 0, 0, 0) { }

	/** Width of the primary display in pixels */
	int32_t PrimaryDisplayWidth;

	/** Height of the primary display in pixels */
	int32_t PrimaryDisplayHeight;

	/** Info on connected physical monitors. Only available on platforms where this information is accessible (PC currently) */
	std::vector<FMonitorInfo> MonitorInfo;

	/** Area of the primary display not covered by task bars or other docked widgets */
	FPlatformRect PrimaryDisplayWorkAreaRect;

	/** Virtual display coordinate range (includes all active displays) */
	FPlatformRect VirtualDisplayRect;

	/** The safe area for all content on TVs (see http://en.wikipedia.org/wiki/Safe_area_%28television%29) - content will be inset TitleSafePaddingSize.X on left _and_ right */
	glm::vec4 TitleSafePaddingSize;

	/** The safe area for less important spill over on TVs (see TitleSafePaddingSize) */
	glm::vec4 ActionSafePaddingSize;

	static void GetDisplayMetrics(struct FDisplayMetrics& OutDisplayMetrics);

	/** Gets the monitor work area from a position in the global display rect */
	FPlatformRect GetMonitorWorkAreaFromPoint(const glm::vec2& Point) const;

	/** Logs out display metrics */
	void PrintToLog() const;

protected:
	// The title safe zone ratio that will be returned by FDisplayMetrics::GetDisplayMetrics on platforms that don't have a defined safe zone
	static float GetDebugTitleSafeZoneRatio();

	// The action safe zone ratio that will be returned by FDisplayMetrics::GetDisplayMetrics on platforms that don't have a defined safe zone
	static float GetDebugActionSafeZoneRatio();

	// Apply the debug/default safe zones
	void ApplyDefaultSafeZones();
};


/**
 * Enumerates available horizontal alignments for window title bars.
 */
namespace EWindowTitleAlignment
{
	enum Type
	{
		/** The window title should be aligned to the left. */
		Left,
		/** The window title should be centered within the title bar. */
		Center,
		/** The window title should be aligned to the right. */
		Right
	};
}


/**
 * Generic platform application interface
 */
class GenericApplication 
{
public:

	////DECLARE_MULTICAST_DELEGATE_OneParam( FOnConsoleCommandAdded, const FString& /*Command*/ );
	//typedef FOnConsoleCommandAdded::FDelegate FOnConsoleCommandListener;
	typedef void FOnConsoleCommandListener(std::string);


	GenericApplication()
		:MessageHandler(std::make_shared<GenericApplicationMessageHandler>() )
	{

	}

	virtual ~GenericApplication() {}

	virtual void SetMessageHandler( const std::shared_ptr< GenericApplicationMessageHandler >& InMessageHandler ) { MessageHandler = InMessageHandler; }

	std::shared_ptr< GenericApplicationMessageHandler > GetMessageHandler() { return MessageHandler; }

	virtual void PollGameDeviceState( const float TimeDelta ) { }

	virtual void PumpMessages( const float TimeDelta ) { }

	virtual void ProcessDeferredEvents( const float TimeDelta ) { }

	virtual void Tick ( const float TimeDelta ) { }

	virtual std::shared_ptr< FGenericWindow > MakeWindow() { return std::shared_ptr< FGenericWindow >(new FGenericWindow()) ; }

	virtual void InitializeWindow(const std::shared_ptr< FGenericWindow >& Window, const std::shared_ptr< GenericWindowDefinition >& InDefinition, const std::shared_ptr< FGenericWindow >& InParent, const bool bShowImmediately ) { }

	virtual void SetCapture( const std::shared_ptr< FGenericWindow >& InWindow ) { }

	virtual void* GetCapture( void ) const { return NULL; }

	virtual FModifierKeysState GetModifierKeys() const  { return FModifierKeysState(); }

	/** @return true if the system cursor is currently directly over a slate window. */
	virtual bool IsCursorDirectlyOverSlateWindow() const { return true; }

	/** @return Native window under the mouse cursor. */
	virtual std::shared_ptr< FGenericWindow > GetWindowUnderCursor() { return std::shared_ptr< FGenericWindow >( nullptr ); }

	virtual void SetHighPrecisionMouseMode( const bool Enable, const std::shared_ptr< FGenericWindow >& InWindow ) { };

	virtual bool IsUsingHighPrecisionMouseMode() const { return false; }
	
	virtual bool IsUsingTrackpad() const { return false; }

	virtual bool IsMouseAttached() const { return true; }

	virtual bool IsGamepadAttached() const { return false; }

	virtual void RegisterConsoleCommandListener(const 	std::function<FOnConsoleCommandListener>& InListener) {}

	virtual void AddPendingConsoleCommand(const std::string& InCommand) {}

	virtual FPlatformRect GetWorkArea( const FPlatformRect& CurrentWindow ) const
	{
		FPlatformRect OutRect;
		OutRect.Left = 0;
		OutRect.Top = 0;
		OutRect.Right = 0;
		OutRect.Bottom = 0;

		return OutRect;
	}

	virtual bool TryCalculatePopupWindowPosition( const FPlatformRect& InAnchor, const glm::vec2& InSize, const glm::vec2& ProposedPlacement, const EPopUpOrientation::Type Orientation, /*OUT*/ glm::vec2* const CalculatedPopUpPosition ) const { return false; }

	//DECLARE_EVENT_OneParam(GenericApplication, FOnDisplayMetricsChanged, const FDisplayMetrics&);
	typedef CommonDelegate<void(const FDisplayMetrics&)> OnDisplayMetricsChangedEvent;
	/** Notifies subscribers when any of the display metrics change: e.g. resolution changes or monitor sare re-arranged. */
	OnDisplayMetricsChangedEvent& OnDisplayMetricsChanged(){ return DisplayMetricsChangedEvent; }

	virtual void GetInitialDisplayMetrics( FDisplayMetrics& OutDisplayMetrics ) const { FDisplayMetrics::GetDisplayMetrics(OutDisplayMetrics); }

	
	/** Delegate for virtual keyboard being shown/hidden in case UI wants to slide out of the way */
	//DECLARE_EVENT_OneParam(FSlateApplication, FVirtualKeyboardShownEvent, FPlatformRect);
	typedef CommonDelegate<void(FPlatformRect)> OnVirtualKeyboardShownEvent;
	OnVirtualKeyboardShownEvent& OnVirtualKeyboardShown()  { return VirtualKeyboardShownEvent; }
	
	//DECLARE_EVENT(FSlateApplication, FVirtualKeyboardHiddenEvent);
	typedef CommonDelegate<void()> OnVirtualKeyboardHiddenEvent;
	OnVirtualKeyboardHiddenEvent& OnVirtualKeyboardHidden()  { return VirtualKeyboardHiddenEvent; }

	
	/** Gets the horizontal alignment of the window title bar's title text. */
	virtual EWindowTitleAlignment::Type GetWindowTitleAlignment() const
	{
		return EWindowTitleAlignment::Left;
	}

	virtual EWindowTransparency GetWindowTransparencySupport() const
	{
		return EWindowTransparency::None;
	}

	virtual void DestroyApplication() { }

	virtual IInputInterface* GetInputInterface() { return nullptr; }	

	/** Function to return the current implementation of the Text Input Method System */
	virtual ITextInputMethodSystem *GetTextInputMethodSystem() { return NULL; }
	
	/** Send any analytics captured by the application */
	virtual void SendAnalytics(IAnalyticsProvider* Provider) { }

	virtual bool SupportsSystemHelp() const { return false; }

	virtual void ShowSystemHelp() {}

	virtual bool ApplicationLicenseValid(FPlatformUserId PlatformUser = PLATFORMUSERID_NONE) { return true; }

public:

	HighResolutionTimer& GetTimer() { return m_Timer; }
	virtual FDeviceInputManager* GetDeviceInputManager() = 0;
protected:

	std::shared_ptr< GenericApplicationMessageHandler > MessageHandler;
	
	/** Trigger the OnDisplayMetricsChanged event with the argument 'InMetrics' */
	void BroadcastDisplayMetricsChanged( const FDisplayMetrics& InMetrics ){ DisplayMetricsChangedEvent( InMetrics ); }
	
	// Notifies subscribers when any of the display metrics change: e.g. resolution changes or monitor sare re-arranged.
	OnDisplayMetricsChangedEvent DisplayMetricsChangedEvent;

	/** Delegate for virtual keyboard being shown */
	OnVirtualKeyboardShownEvent VirtualKeyboardShownEvent;
	
	/** Delegate for virtual keyboard being shown */
	OnVirtualKeyboardHiddenEvent VirtualKeyboardHiddenEvent;
	

	HighResolutionTimer m_Timer;
};
