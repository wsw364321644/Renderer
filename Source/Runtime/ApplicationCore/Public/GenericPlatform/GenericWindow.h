#pragma once
#include <cstdint> 
#include <memory>

#include <glm/vec2.hpp>

#include "GenericPlatform/Events.h"

struct GenericWindowDefinition;


enum class EWindowMode
{
	/** The window is in true fullscreen mode */
	Fullscreen,
	/** The window has no border and takes up the entire area of the screen */
	WindowedFullscreen,
	/** The window has a border and may not take up the entire screen area */
	Windowed,

	/** The total number of supported window modes */
	NumWindowModes
};



class  GenericWindow
{
public:

	GenericWindow();

	virtual ~GenericWindow();

	/** Native windows should implement ReshapeWindow by changing the platform-specific window to be located at (X,Y) and be the dimensions Width x Height. */
	virtual void ReshapeWindow(int32_t X, int32_t Y, int32_t Width, int32_t Height );

	/** Returns the rectangle of the screen the window is associated with */
	virtual bool GetFullScreenInfo( int32_t& X, int32_t& Y, int32_t& Width, int32_t& Height ) const;

	/** Native windows should implement MoveWindowTo by relocating the platform-specific window to (X,Y). */
	virtual void MoveWindowTo ( int32_t X, int32_t Y );

	/** Native windows should implement BringToFront by making this window the top-most window (i.e. focused). */
	virtual void BringToFront( bool bForce = false );

	/** @hack Force a window to front even if a different application is in front. */
	virtual void HACK_ForceToFront();

	/** Native windows should implement this function by asking the OS to destroy OS-specific resource associated with the window (e.g. Win32 window handle) */
	virtual void Destroy();

	/** Native window should implement this function by performing the equivalent of the Win32 minimize-to-taskbar operation */
	virtual void Minimize();

	/** Native window should implement this function by performing the equivalent of the Win32 maximize operation */
	virtual void Maximize();

	/** Native window should implement this function by performing the equivalent of the Win32 restore operation */
	virtual void Restore();

	/** Native window should make itself visible */
	virtual void Show();

	/** Native window should hide itself */
	virtual void Hide();

	/** Toggle native window between fullscreen and normal mode */
	virtual void SetWindowMode( EWindowMode InNewWindowMode );

	/** @return true if the native window is currently in fullscreen mode, false otherwise */
	virtual EWindowMode GetWindowMode() const;

	/** @return true if the native window is maximized, false otherwise */
	virtual bool IsMaximized() const;

	/** @return true if the native window is minimized, false otherwise */
	virtual bool IsMinimized() const;

	/** @return true if the native window is visible, false otherwise */
	virtual bool IsVisible() const;

	/**
	 * Populates the size and location of the window when it is restored.
	 * If the function fails, false is returned and X,Y,Width,Height will be undefined.
	 *
	 * @return true when the size and location and successfully retrieved; false otherwise.
	 */
	virtual bool GetRestoredDimensions(int32_t& X, int32_t& Y, int32_t& Width, int32_t& Height);

	/** 
	 * Native windows should implement SetWindowFocus to let the OS know that a window has taken focus.  
	 * Slate handles focus on a per widget basis internally but the OS still needs to know what window has focus for proper message routing
	 */
	virtual void SetWindowFocus();

	/**
	 * Sets the opacity of this window
	 *
	 * @param	InOpacity	The new window opacity represented as a floating point scalar
	 */
	virtual void SetOpacity( const float InOpacity );

	/** 
	 * Enables or disables the window.  If disabled the window receives no input
	 * 
	 * @param bEnable	true to enable the window, false to disable it.
	 */
	virtual void Enable( bool bEnable );

	/** @return true if native window exists underneath the coordinates */
	virtual bool IsPointInWindow( int32_t X, int32_t Y ) const;
	
	/** Gets OS specific window border size. This is necessary because Win32 does not give control over this size. */
	virtual int32_t GetWindowBorderSize() const;

	/** Gets OS specific window title bar size */
	virtual int32_t GetWindowTitleBarSize() const;

	/** Gets the OS Window handle in the form of a void pointer for other API's */
	virtual void* GetOSWindowHandle() const;

	/** @return true if the window is in the foreground */
	virtual bool IsForegroundWindow() const;

	/**
	 * Sets the window text - usually the title but can also be text content for things like controls
	 *
	 * @param Text	The window's title or content text
	 */
	virtual void SetText(const char* const Text);

	/** @return	The definition describing properties of the window */
	virtual const GenericWindowDefinition& GetDefinition() const;

	/** @return	Gives the native window a chance to adjust our stored window size before we cache it off */
	virtual void AdjustCachedSize( glm::vec2& Size ) const;

	virtual void OnDPIScaleChanged(DPIScaleEventArgs& e) ;
	virtual void OnResize(ResizeEventArgs& e) ;
	virtual void OnMinimized(ResizeEventArgs& e) {};
	virtual void OnMaximized(ResizeEventArgs& e) {};
	virtual void OnRestored(ResizeEventArgs& e) {};
	virtual void OnMouseEnter(MouseMotionEventArgs& e) {}
	virtual void OnMouseLeave(EventArgs& e) {}
	virtual void OnMouseMoved(MouseMotionEventArgs& e) {}
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) {}
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) {}

	float GetDPIScaling() const { return DPIScaling; };

protected:
	std::shared_ptr< GenericWindowDefinition > Definition;

	float DPIScaling;
	bool bIsFullscreen:1;
	bool bIsMinimized:1;
	bool bIsMaximized:1;
};
