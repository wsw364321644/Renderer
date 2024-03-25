#include "Windows/WindowsWindow.h"
#include "GenericPlatform/GameFramework.h"
#include "Windows/WindowsApplication.h"
#include <LoggerHelper.h>
#include <string_convert.h>

FWindowsWindow::FWindowsWindow()
    : m_hWnd(NULL)
    , m_PreviousMouseX(0)
    , m_PreviousMouseY(0)
    , m_bHasKeyboardFocus(false)
{

}


void FWindowsWindow::Initialize(FWindowsApplication* const Application, const std::shared_ptr<GenericWindowDefinition>& InDefinition, HINSTANCE InHInstance, const std::shared_ptr<FWindowsWindow>& InParent, const bool bShowImmediately)
{
    Definition = InDefinition;
    WindowsApplication = Application;
    int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

    RECT windowRect = { 0, 0, static_cast<LONG>(Definition->WidthDesiredOnScreen), static_cast<LONG>(Definition->HeightDesiredOnScreen) };

    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    uint32_t width = windowRect.right - windowRect.left;
    uint32_t height = windowRect.bottom - windowRect.top;

    int windowX = std::max<int>(0, (screenWidth - (int)width) / 2);
    int windowY = std::max<int>(0, (screenHeight - (int)height) / 2);

    auto titleu16=U8ToU16(Definition->Title.c_str());
    m_hWnd = ::CreateWindowExW(NULL, WINDOW_CLASS_NAME, (LPCWSTR)titleu16.c_str(), WS_OVERLAPPEDWINDOW, windowX,
        windowY, width, height, NULL, NULL, InHInstance, NULL);

    if (!m_hWnd)
    {
        GetLogger(ENGINE_LOG_NAME)->error("Failed to create window.");
        return ;
    }
    DPIScaling = ::GetDpiForWindow(m_hWnd) / 96.0f;

}

void FWindowsWindow::OnMouseEnter(MouseMotionEventArgs& e)
{
    // Track mouse leave events.
    TRACKMOUSEEVENT trackMouseEvent = {};
    trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    trackMouseEvent.hwndTrack = m_hWnd;
    trackMouseEvent.dwFlags = TME_LEAVE;
    TrackMouseEvent(&trackMouseEvent);

    m_bInClientRect = true;
}

void FWindowsWindow::OnMouseLeave(EventArgs& e)
{
    m_bInClientRect = false;
}

void FWindowsWindow::OnMouseMoved(MouseMotionEventArgs& e)
{
    if (!m_bInClientRect)
    {
        m_PreviousMouseX = e.X;
        m_PreviousMouseY = e.Y;
        m_bInClientRect = true;
        // Mouse re-entered the client area.
        OnMouseEnter(e);
    }

    e.RelX = e.X - m_PreviousMouseX;
    e.RelY = e.Y - m_PreviousMouseY;

    m_PreviousMouseX = e.X;
    m_PreviousMouseY = e.Y;
}

void FWindowsWindow::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
    // Capture mouse movement until the button is released.
    ::SetCapture(m_hWnd);
}

void FWindowsWindow::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
    // Stop capturing the mouse.
    ::ReleaseCapture();
}



FWindowsWindow::~FWindowsWindow()
{
    ::DestroyWindow( m_hWnd );
}

HWND FWindowsWindow::GetWindowHandle() const
{
    return m_hWnd;
}

void* FWindowsWindow::GetOSWindowHandle() const
{
    return GetWindowHandle();
}


void FWindowsWindow::Show()
{
    ::ShowWindow( m_hWnd, SW_SHOW );
}

void FWindowsWindow::Hide()
{
    ::ShowWindow( m_hWnd, SW_HIDE );
}

void FWindowsWindow::SetText(const char* const Text)
{
    WindowsApplication->UpdateWindowTitle(this, Text);
}

void FWindowsWindow::SetWindowMode(EWindowMode InNewWindowMode)
{
    if (WindowMode == InNewWindowMode) { return; }
    ::GetWindowRect(m_hWnd, &m_WindowRect);
    switch(InNewWindowMode){
    case EWindowMode::Fullscreen: {
        // Store the current window dimensions so they can be restored
 // when switching out of fullscreen state.
        

        // Set the window style to a borderless fullscreen window so the
        // client area fills the entire screen.
        UINT windowStyle =
            WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

        ::SetWindowLongW(m_hWnd, GWL_STYLE, windowStyle);

        // Query the name of the nearest display device for the window.
        // This is required to set the fullscreen dimensions of the window
        // when using a multi-monitor setup.
        HMONITOR      hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFOEX monitorInfo = {};
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        ::GetMonitorInfo(hMonitor, &monitorInfo);

        ::SetWindowPos(m_hWnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
            monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ::ShowWindow(m_hWnd, SW_MAXIMIZE);
    }
                                break;
    default: {
        // Restore all the window decorators.
        ::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

        ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_WindowRect.left, m_WindowRect.top,
            m_WindowRect.right - m_WindowRect.left, m_WindowRect.bottom - m_WindowRect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ::ShowWindow(m_hWnd, SW_NORMAL);
    }
    }
    WindowMode = InNewWindowMode;
}

EWindowMode FWindowsWindow::GetWindowMode() const
{
    return WindowMode;
}


