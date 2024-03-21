#pragma once


/**
 *  @file Window.h
 *  @date September 29, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief A Window class for the GameFramework.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <memory>
#include <string>
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericWindowDefinition.h"
// Forward declarations
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM,
                          LPARAM );  // Defined in the Application class.

class WindowsWindow : public FGenericWindow
{
public:
    static constexpr wchar_t     WINDOW_CLASS_NAME[]= L"RenderWindow";
    /**
     * Get a handle to the OS window instance.
     *
     * @returns The handle to the OS window instance.
     */
    HWND GetWindowHandle() const;
    virtual void* GetOSWindowHandle() const override;

    virtual void SetText(const char* const Text) override;

    virtual void SetWindowMode(EWindowMode InNewWindowMode);
    virtual EWindowMode GetWindowMode() const;



    virtual void Show() override;

    virtual void Hide() override;
    

    void Initialize(class WindowsApplication* const Application, const std::shared_ptr< GenericWindowDefinition >& InDefinition, HINSTANCE InHInstance, const std::shared_ptr< WindowsWindow >& InParent, const bool bShowImmediately);

    virtual void OnMouseEnter(MouseMotionEventArgs& e) override;
    virtual void OnMouseLeave(EventArgs& e) override;
    virtual void OnMouseMoved(MouseMotionEventArgs& e) override;
    virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) override;
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e)override;

    virtual ~WindowsWindow();
protected:
    friend class WindowsApplication;
    // This is needed to allow the WndProc function to call event callbacks on
    // the window.
    friend LRESULT CALLBACK ::WndProc( HWND, UINT, WPARAM, LPARAM );

    // Only the Application class can create Windows.
    WindowsWindow();

    



private:
    HWND m_hWnd;

    int32_t m_PreviousMouseX;
    int32_t m_PreviousMouseY;
    bool m_bInClientRect;

    RECT m_WindowRect;
    EWindowMode WindowMode;

    // This is set to true when the window receives keyboard focus.
    bool m_bHasKeyboardFocus;

};
