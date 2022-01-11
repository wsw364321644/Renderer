#include "Windows/WindowsApplication.h"
#include "Windows/WindowsWindow.h"

#include "GenericPlatform/GameFrameworkPCH.h"
#include "GenericPlatform/GameFramework.h"
#include "GenericPlatform/GenericWindow.h"

#include "Windows\WindowsDeviceInputManager.h"

#include <memory>
#include <map>
#include <iostream>


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <comdef.h> // For _com_error class (used to decode HR result codes).

#include <fcntl.h> // For Translation mode constants _O_TEXT (https://docs.microsoft.com/en-us/cpp/c-runtime-library/translation-mode-constants)
#include <Shlwapi.h>  // For PathFindFileNameW (used in ReadDirectoryChanges.cpp)

#pragma comment( lib, "Shlwapi.lib" )
#pragma comment( lib,"Setupapi.lib")
#include <SetupAPI.h>
#include <cfgmgr32.h>
#include "resource.h"



static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Convert the message ID into a ButtonState.
static ButtonState DecodeButtonState(UINT messageID)
{
    ButtonState buttonState = ButtonState::Pressed;

    switch (messageID)
    {
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
        buttonState = ButtonState::Released;
        break;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
        buttonState = ButtonState::Pressed;
        break;
    }

    return buttonState;
}

// Convert wParam of the WM_SIZE events to a WindowState.
static EWindowState DecodeWindowState(WPARAM wParam)
{
    EWindowState windowState = EWindowState::Restored;

    switch (wParam)
    {
    case SIZE_RESTORED:
        windowState = EWindowState::Restored;
        break;
    case SIZE_MINIMIZED:
        windowState = EWindowState::Minimized;
        break;
    case SIZE_MAXIMIZED:
        windowState = EWindowState::Maximized;
        break;
    default:
        break;
    }

    return windowState;
}



// Set the name of an std::thread.
// Useful for debugging.
const DWORD MS_VC_EXCEPTION = 0x406D1388;

// Set the name of a running thread (for debugging)
#pragma pack( push, 8 )
typedef struct tagTHREADNAME_INFO
{
    DWORD  dwType;      // Must be 0x1000.
    LPCSTR szName;      // Pointer to name (in user addr space).
    DWORD  dwThreadID;  // Thread ID (-1=caller thread).
    DWORD  dwFlags;     // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack( pop )

inline void SetThreadName(std::thread& thread, const char* threadName)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = ::GetThreadId(reinterpret_cast<HANDLE>(thread.native_handle()));
    info.dwFlags = 0;

    __try
    {
        ::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

constexpr int MAX_CONSOLE_LINES = 500;

using WindowMap = std::map<HWND, std::shared_ptr<GenericWindow>>;
using WindowMapByName = std::map<std::string, std::shared_ptr<GenericWindow>>;
static WindowMap       gs_WindowMap;
static WindowMapByName gs_WindowMapByName;

static std::mutex gs_WindowHandlesMutex;


/**
 * Create a console window (consoles are not automatically created for Windows
 * subsystems)
 */
static void CreateConsole()
{
    // Allocate a console.
    if (AllocConsole())
    {
        HANDLE lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        // Increase screen buffer to allow more lines of text than the default.
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(lStdHandle, &consoleInfo);
        consoleInfo.dwSize.Y = MAX_CONSOLE_LINES;
        SetConsoleScreenBufferSize(lStdHandle, consoleInfo.dwSize);
        SetConsoleCursorPosition(lStdHandle, { 0, 0 });

        // Redirect unbuffered STDOUT to the console.
        int   hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
        FILE* fp = _fdopen(hConHandle, "w");
        freopen_s(&fp, "CONOUT$", "w", stdout);
        setvbuf(stdout, nullptr, _IONBF, 0);

        // Redirect unbuffered STDIN to the console.
        lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
        hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
        fp = _fdopen(hConHandle, "r");
        freopen_s(&fp, "CONIN$", "r", stdin);
        setvbuf(stdin, nullptr, _IONBF, 0);

        // Redirect unbuffered STDERR to the console.
        lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
        hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
        fp = _fdopen(hConHandle, "w");
        freopen_s(&fp, "CONOUT$", "w", stderr);
        setvbuf(stderr, nullptr, _IONBF, 0);

        // Clear the error state for each of the C++ standard stream objects. We
        // need to do this, as attempts to access the standard streams before
        // they refer to a valid target will cause the iostream objects to enter
        // an error state. In versions of Visual Studio after 2005, this seems
        // to always occur during startup regardless of whether anything has
        // been read from or written to the console or not.
        std::wcout.clear();
        std::cout.clear();
        std::wcerr.clear();
        std::cerr.clear();
        std::wcin.clear();
        std::cin.clear();
    }
}



WindowsApplication::WindowsApplication(const HINSTANCE inInstance, const HICON IconHandle) : GenericApplication(), m_hInstance(inInstance), m_bTerminateDirectoryChangeThread(false)
{
    // Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
// Using this awareness context allows the client area of the window
// to achieve 100% scaling while still allowing non-client window content to
// be rendered in a DPI sensitive fashion.
// @see https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setthreaddpiawarenesscontext
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

#if defined( _DEBUG )
    // Create a console window for std::cout
    CreateConsole();
#endif

    // Initializes the COM library for use by the calling thread, sets the thread's concurrency model, and creates a new
// apartment for the thread if one is required.
// This must be called at least once for each thread that uses the COM library.
// @see https://docs.microsoft.com/en-us/windows/win32/api/objbase/nf-objbase-coinitialize
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        _com_error err(hr);  // I hope this never happens.
        spdlog::critical("CoInitialize failed: {}", err.ErrorMessage());
        throw new std::exception(err.ErrorMessage());
    }

    WNDCLASSEXW wndClass = { 0 };

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = &WndProc;
    wndClass.hInstance = m_hInstance;
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hIcon = LoadIcon(wndClass.hInstance, MAKEINTRESOURCE(APP_ICON));
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = WindowsWindow::WINDOW_CLASS_NAME;
    wndClass.hIconSm = LoadIcon(wndClass.hInstance, MAKEINTRESOURCE(APP_ICON));

    if (!RegisterClassExW(&wndClass))
    {
        MessageBoxA(NULL, "Unable to register the window class.", "Error", MB_OK | MB_ICONERROR);
    }

    m_DirectoryChangeListenerThread = std::thread(&WindowsApplication::CheckFileChanges, this);
    SetThreadName(m_DirectoryChangeListenerThread, "Check File Changes");

    m_WindowsDeviceInputManager = std::make_shared<WindowsDeviceInputManager>();
}

WindowsApplication::~WindowsApplication()
{
    m_bTerminateDirectoryChangeThread = true;
    if (m_DirectoryChangeListenerThread.joinable())
    {
        m_DirectoryChangeListenerThread.join();
    }

    gs_WindowMap.clear();
    gs_WindowMapByName.clear();
}

std::shared_ptr<GenericWindow> WindowsApplication::MakeWindow()
{
    std::shared_ptr<GenericWindow> pWindow(new WindowsWindow());

    return pWindow;
}

void WindowsApplication::InitializeWindow(const std::shared_ptr<GenericWindow>& InWindow, const std::shared_ptr<GenericWindowDefinition>& InDefinition, const std::shared_ptr<GenericWindow>& InParent, const bool bShowImmediately)
{
    auto Window = std::dynamic_pointer_cast<WindowsWindow>(InWindow);
    auto Parent = std::dynamic_pointer_cast<WindowsWindow>(InParent);
    Window->Initialize(this, InDefinition, m_hInstance, Parent, bShowImmediately);
    gs_WindowMap.insert(WindowMap::value_type(Window->GetWindowHandle(), Window));
    gs_WindowMapByName.insert(WindowMapByName::value_type(Window->GetDefinition().Title, Window));
}


void WindowsApplication::RegisterDirectoryChangeListener(const std::wstring& dir, bool recursive)
{
    scoped_lock lock(m_DirectoryChangeMutex);
    m_DirectoryChanges.AddDirectory(dir, recursive, FILE_NOTIFY_CHANGE_LAST_WRITE);
}

int32_t WindowsApplication::ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
    
    std::shared_ptr<WindowsWindow> pWindow=std::dynamic_pointer_cast<WindowsWindow>( GetWindowByHandle(hwnd).lock());


    if (pWindow)
    {

        if (m_WindowsDeviceInputManager->ProcessMessage(hwnd, msg, wParam, lParam)) {
            return 0;
        }
        switch (msg)
        {
        case WM_DPICHANGED:
        {
            float             dpiScaling = HIWORD(wParam) / 96.0f;
            DPIScaleEventArgs dpiScaleEventArgs(dpiScaling);
            pWindow->OnDPIScaleChanged(dpiScaleEventArgs);
            MessageHandler->HandleDPIScaleChanged(pWindow);
        }
        break;
        case WM_PAINT:
        {
            m_Timer.Tick();
            MessageHandler->OnOSPaint(pWindow);

        }
        break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
      
        break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
      
        break;
        // The default window procedure will play a system notification sound
        // when pressing the Alt+Enter keyboard combination if this message is
        // not handled.
        case WM_SYSCHAR:
            break;
        case WM_KILLFOCUS:
        {
            MessageHandler->OnWindowFocusChanged(pWindow, false);
        }
        break;
        case WM_SETFOCUS:
        {
            MessageHandler->OnWindowFocusChanged(pWindow, true);
        }
        break;
        case WM_MOUSEMOVE:

        break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:

        break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:

        break;
        case WM_MOUSEWHEEL:

        break;
        case WM_MOUSELEAVE:
            break;
        // NOTE: Not really sure if these next set of messages are working
        // correctly. Not really sure HOW to get them to work correctly.
        // TODO: Try to fix these if I need them ;)
        case WM_CAPTURECHANGED:
        {
            EventArgs mouseBlurEventArgs;
            //pWindow->OnMouseBlur(mouseBlurEventArgs);
        }
        break;
        case WM_MOUSEACTIVATE:
        {
            EventArgs mouseFocusEventArgs;
            //pWindow->OnMouseFocus(mouseFocusEventArgs);
        }
        break;

        case WM_SIZE:
        {
            EWindowState windowState = DecodeWindowState(wParam);

            int width = ((int)(short)LOWORD(lParam));
            int height = ((int)(short)HIWORD(lParam));

            ResizeEventArgs resizeEventArgs(width, height, windowState);
            pWindow->OnResize(resizeEventArgs);

            MessageHandler->OnWindowAction(pWindow, windowState);

        }
        break;
        case WM_CLOSE:
        {
            WindowCloseEventArgs windowCloseEventArgs;
            //pWindow->OnClose(windowCloseEventArgs);

            // Check to see if the user canceled the close event.
            if (windowCloseEventArgs.ConfirmClose)
            {
                // DestroyWindow( hwnd );
                // Just hide the window.
                // Windows will be destroyed when the application quits.
                pWindow->Hide();
            }
            // Called when the OS close button is pressed
            MessageHandler->OnWindowClose(pWindow);

        }
        break;
        case WM_DESTROY:
        {
            std::lock_guard<std::mutex> lock(gs_WindowHandlesMutex);
            WindowMap::iterator         iter = gs_WindowMap.find(hwnd);
            if (iter != gs_WindowMap.end())
            {
                gs_WindowMapByName.erase(gs_WindowMapByName.find(iter->second->GetDefinition().Title));
                gs_WindowMap.erase(iter);

            }
        }
        break;
        default:
            return ::DefWindowProcW(hwnd, msg, wParam, lParam);
        }
    }
    else
    {
        switch (msg)
        {
        case WM_CREATE:
            break;
        default:
            return ::DefWindowProcW(hwnd, msg, wParam, lParam);
        }
    }

    return 0;
}

std::weak_ptr<GenericWindow> WindowsApplication::GetWindowByName(const std::string& windowName) const
{

    auto iter = gs_WindowMapByName.find(windowName);
    if (iter != gs_WindowMapByName.end())
    {
        return iter->second;
    }
  
    return std::weak_ptr<GenericWindow>();
}

std::weak_ptr<GenericWindow> WindowsApplication::GetWindowByHandle(HWND handle) const
{
    auto iter = gs_WindowMap.find(handle);
    if (iter != gs_WindowMap.end())
    {
        return iter->second;
    }
    return std::weak_ptr<GenericWindow>();
}

void WindowsApplication::SetMessageHandler(const std::shared_ptr<GenericApplicationMessageHandler>& InMessageHandler)
{
    GenericApplication::SetMessageHandler(InMessageHandler);

}

// This is the directory change listener thread entry point.
void WindowsApplication::CheckFileChanges()
{
    while (!m_bTerminateDirectoryChangeThread)
    {
        scoped_lock lock(m_DirectoryChangeMutex);

        DWORD waitSignal = ::WaitForSingleObject(m_DirectoryChanges.GetWaitHandle(), 0);
        switch (waitSignal)
        {
        case WAIT_OBJECT_0:
            // A file has been modified
            if (m_DirectoryChanges.CheckOverflow())
            {
                // This could happen if a lot of modifications occur at once.
                spdlog::warn("Directory change overflow occurred.");
            }
            else
            {
                DWORD        action;
                std::wstring fileName;
                m_DirectoryChanges.Pop(action, fileName);

                FileAction fileAction = FileAction::Unknown;
                switch (action)
                {
                case FILE_ACTION_ADDED:
                    fileAction = FileAction::Added;
                    break;
                case FILE_ACTION_REMOVED:
                    fileAction = FileAction::Removed;
                    break;
                case FILE_ACTION_MODIFIED:
                    fileAction = FileAction::Modified;
                    break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    fileAction = FileAction::RenameOld;
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
                    fileAction = FileAction::RenameNew;
                    break;
                default:
                    break;
                }

                FileChangedEventArgs fileChangedEventArgs(fileAction, fileName);
                OnFileChange(fileChangedEventArgs);
            }

            break;
        default:
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // std::this_thread::yield();
    }
}

void WindowsApplication::OnFileChange(FileChangedEventArgs& e)
{
    FileChanged(e);
}







static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    WindowsApplication* app = dynamic_cast<WindowsApplication*>(GameFramework::Get().GetGenericApplication());
    return app->ProcessMessage(hwnd, message, wParam, lParam);
}




static BOOL CALLBACK MonitorEnumProc(HMONITOR Monitor, HDC MonitorDC, LPRECT Rect, LPARAM UserData)
{
    MONITORINFOEX MonitorInfoEx;
    MonitorInfoEx.cbSize = sizeof(MonitorInfoEx);
    GetMonitorInfo(Monitor, &MonitorInfoEx);

    FMonitorInfo* Info = (FMonitorInfo*)UserData;
    if (Info->Name == MonitorInfoEx.szDevice)
    {
        Info->DisplayRect.Bottom = MonitorInfoEx.rcMonitor.bottom;
        Info->DisplayRect.Left = MonitorInfoEx.rcMonitor.left;
        Info->DisplayRect.Right = MonitorInfoEx.rcMonitor.right;
        Info->DisplayRect.Top = MonitorInfoEx.rcMonitor.top;

        Info->WorkArea.Bottom = MonitorInfoEx.rcWork.bottom;
        Info->WorkArea.Left = MonitorInfoEx.rcWork.left;
        Info->WorkArea.Right = MonitorInfoEx.rcWork.right;
        Info->WorkArea.Top = MonitorInfoEx.rcWork.top;

        return FALSE;
    }

    return TRUE;
}





/**
 * Extracts EDID data from the given registry key and reads out native display with and height
 * @param hDevRegKey - Registry key where EDID is stored
 * @param OutWidth - Reference to output variable for monitor native width
 * @param OutHeight - Reference to output variable for monitor native height
 * @returns 'true' if data was extracted successfully, 'false' otherwise
 **/
static bool GetMonitorSizeFromEDID(const HKEY hDevRegKey, int32_t& OutWidth, int32_t& OutHeight)
{
    static const uint32_t NameSize = 512;
    static TCHAR ValueName[NameSize];

    DWORD Type;
    DWORD ActualValueNameLength = NameSize;

    BYTE EDIDData[1024];
    DWORD EDIDSize = sizeof(EDIDData);

    for (LONG i = 0, RetValue = ERROR_SUCCESS; RetValue != ERROR_NO_MORE_ITEMS; ++i)
    {
        RetValue = RegEnumValue(hDevRegKey,
            i,
            &ValueName[0],
            &ActualValueNameLength, NULL, &Type,
            EDIDData,
            &EDIDSize);

        if (RetValue != ERROR_SUCCESS || (std::strcmp(ValueName, TEXT("EDID")) != 0))
        {
            continue;
        }

        // EDID data format documented here:
        // http://en.wikipedia.org/wiki/EDID

        int DetailTimingDescriptorStartIndex = 54;
        OutWidth = ((EDIDData[DetailTimingDescriptorStartIndex + 4] >> 4) << 8) | EDIDData[DetailTimingDescriptorStartIndex + 2];
        OutHeight = ((EDIDData[DetailTimingDescriptorStartIndex + 7] >> 4) << 8) | EDIDData[DetailTimingDescriptorStartIndex + 5];

        return true; // valid EDID found
    }

    return false; // EDID not found
}





/**
 * Locate registry record for the given display device ID and extract native size information
 * @param TargetDevID - Name of taret device
 * @praam OutWidth - Reference to output variable for monitor native width
 * @praam OutHeight - Reference to output variable for monitor native height
 * @returns TRUE if data was extracted successfully, FALSE otherwise
 **/
inline bool GetSizeForDevID(const std::string& TargetDevID, int32_t& Width, int32_t& Height)
{
    static const GUID ClassMonitorGuid = { 0x4d36e96e, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18} };

    HDEVINFO DevInfo = SetupDiGetClassDevsEx(
        &ClassMonitorGuid, //class GUID
        NULL,
        NULL,
        DIGCF_PRESENT,
        NULL,
        NULL,
        NULL);

    if (NULL == DevInfo)
    {
        return false;
    }

    bool bRes = false;

    for (ULONG MonitorIndex = 0; ERROR_NO_MORE_ITEMS != GetLastError(); ++MonitorIndex)
    {
        SP_DEVINFO_DATA DevInfoData;
        ZeroMemory(&DevInfoData, sizeof(DevInfoData));
        DevInfoData.cbSize = sizeof(DevInfoData);

        if (SetupDiEnumDeviceInfo(DevInfo, MonitorIndex, &DevInfoData) == TRUE)
        {
            TCHAR Buffer[MAX_DEVICE_ID_LEN];
            if (CM_Get_Device_ID(DevInfoData.DevInst, Buffer, MAX_DEVICE_ID_LEN, 0) == CR_SUCCESS)
            {
                std::string DevID(Buffer);
                DevID = DevID.substr(8, DevID.find(TEXT("\\"), 9) - 8);
                if (DevID == TargetDevID)
                {
                    HKEY hDevRegKey = SetupDiOpenDevRegKey(DevInfo, &DevInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

                    if (hDevRegKey && hDevRegKey != INVALID_HANDLE_VALUE)
                    {
                        bRes = GetMonitorSizeFromEDID(hDevRegKey, Width, Height);
                        RegCloseKey(hDevRegKey);
                        break;
                    }
                }
            }
        }
    }

    if (SetupDiDestroyDeviceInfoList(DevInfo) == FALSE)
    {
        bRes = false;
    }

    return bRes;
}

/**
 * Extract hardware information about connect monitors
 * @param OutMonitorInfo - Reference to an array for holding records about each detected monitor
 **/
static void GetMonitorsInfo(std::vector<FMonitorInfo>& OutMonitorInfo)
{
    DISPLAY_DEVICE DisplayDevice;
    DisplayDevice.cb = sizeof(DisplayDevice);
    DWORD DeviceIndex = 0; // device index

    FMonitorInfo* PrimaryDevice = nullptr;
    OutMonitorInfo.reserve(2); // Reserve two slots, as that will be the most common maximum

    std::string DeviceID;
    while (EnumDisplayDevices(0, DeviceIndex, &DisplayDevice, 0))
    {
        if ((DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) > 0)
        {
            DISPLAY_DEVICE Monitor;
            ZeroMemory(&Monitor, sizeof(Monitor));
            Monitor.cb = sizeof(Monitor);
            DWORD MonitorIndex = 0;

            while (EnumDisplayDevices(DisplayDevice.DeviceName, MonitorIndex, &Monitor, 0))
            {
                if (Monitor.StateFlags & DISPLAY_DEVICE_ACTIVE &&
                    !(Monitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
                {
                    FMonitorInfo Info;

                    Info.Name = DisplayDevice.DeviceName;
                    EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)&Info);

                    Info.ID = std::printf(TEXT("%s"), Monitor.DeviceID);
                    Info.Name = Info.ID.substr(8, Info.ID.find(("\\"), 9) - 8);

                    if (GetSizeForDevID(Info.Name, Info.NativeWidth, Info.NativeHeight))
                    {
                        Info.ID = Monitor.DeviceID;
                        Info.bIsPrimary = (DisplayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) > 0;
                        OutMonitorInfo.push_back(Info);

                        if (PrimaryDevice == nullptr && Info.bIsPrimary)
                        {
                            PrimaryDevice = &OutMonitorInfo.back();
                        }
                    }
                }
                MonitorIndex++;

                ZeroMemory(&Monitor, sizeof(Monitor));
                Monitor.cb = sizeof(Monitor);
            }
        }

        ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
        DisplayDevice.cb = sizeof(DisplayDevice);
        DeviceIndex++;
    }
}


void FDisplayMetrics::GetDisplayMetrics(struct FDisplayMetrics& OutDisplayMetrics)
{
    // Total screen size of the primary monitor
    OutDisplayMetrics.PrimaryDisplayWidth = ::GetSystemMetrics(SM_CXSCREEN);
    OutDisplayMetrics.PrimaryDisplayHeight = ::GetSystemMetrics(SM_CYSCREEN);

    // Get the screen rect of the primary monitor, excluding taskbar etc.
    RECT WorkAreaRect;
    if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0))
    {
        WorkAreaRect.top = WorkAreaRect.bottom = WorkAreaRect.left = WorkAreaRect.right = 0;
    }

    OutDisplayMetrics.PrimaryDisplayWorkAreaRect.Left = WorkAreaRect.left;
    OutDisplayMetrics.PrimaryDisplayWorkAreaRect.Top = WorkAreaRect.top;
    OutDisplayMetrics.PrimaryDisplayWorkAreaRect.Right = WorkAreaRect.right;
    OutDisplayMetrics.PrimaryDisplayWorkAreaRect.Bottom = WorkAreaRect.bottom;

    // Virtual desktop area
    OutDisplayMetrics.VirtualDisplayRect.Left = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
    OutDisplayMetrics.VirtualDisplayRect.Top = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
    OutDisplayMetrics.VirtualDisplayRect.Right = OutDisplayMetrics.VirtualDisplayRect.Left + ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
    OutDisplayMetrics.VirtualDisplayRect.Bottom = OutDisplayMetrics.VirtualDisplayRect.Top + ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // Get connected monitor information
    GetMonitorsInfo(OutDisplayMetrics.MonitorInfo);

    // Apply the debug safe zones
    OutDisplayMetrics.ApplyDefaultSafeZones();
}
