#include "GenericPlatform/PlatformApplicationMisc.h"
#include "Windows/WindowsApplication.h"

#include "dxgi1_3.h"
#include "dxgi1_4.h"
#include "dxgi1_6.h"
#include "resource.h"
#include <stdio.h>
typedef HRESULT(STDAPICALLTYPE *GetDpiForMonitorProc)(HMONITOR Monitor, int32_t DPIType, uint32_t *DPIX, uint32_t *DPIY);
GetDpiForMonitorProc GetDpiForMonitor;

void FPlatformApplicationMisc::LoadStartupModules()
{
}

GenericApplication* FPlatformApplicationMisc::CreateApplication()
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	HICON AppIconHandle = LoadIcon( hInstance, MAKEINTRESOURCE( GetAppIcon() ) );
	if( AppIconHandle == NULL )
	{
		AppIconHandle = LoadIcon( (HINSTANCE)NULL, IDI_APPLICATION ); 
	}

	return new FWindowsApplication( hInstance, AppIconHandle );
}

void FPlatformApplicationMisc::RequestMinimize()
{
	::ShowWindow(::GetActiveWindow(), SW_MINIMIZE);
}

bool FPlatformApplicationMisc::IsThisApplicationForeground()
{
	uint32_t ForegroundProcess;
	::GetWindowThreadProcessId(GetForegroundWindow(), (::DWORD *)&ForegroundProcess);
	return (ForegroundProcess == GetCurrentProcessId());
}

int32_t FPlatformApplicationMisc::GetAppIcon()
{
	return APP_ICON;
}

static void WinPumpMessages()
{
	{
		MSG Msg;
		while( PeekMessage(&Msg,NULL,0,0,PM_REMOVE) )
		{
			TranslateMessage( &Msg );
			DispatchMessage( &Msg );
		}
	}
}


void FPlatformApplicationMisc::PumpMessages(bool bFromMainLoop)
{
	WinPumpMessages();
}

void FPlatformApplicationMisc::RequestExitWithStatus(bool Force, uint8_t ReturnCode)
{
	if (Force)
	{
		exit(0);
	}else{
		PostQuitMessage(ReturnCode);
	}
}

void FPlatformApplicationMisc::PreventScreenSaver()
{
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dx = 0;
	Input.mi.dy = 0;	
	Input.mi.mouseData = 0;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	Input.mi.time = 0;
	Input.mi.dwExtraInfo = 0; 	
	SendInput(1,&Input,sizeof(INPUT));
}

glm::vec4 FPlatformApplicationMisc::GetScreenPixelColor(const glm::vec2& InScreenPos, float /*InGamma*/)
{
	COLORREF PixelColorRef = GetPixel(GetDC(HWND_DESKTOP), InScreenPos.x, InScreenPos.y);


	// Assume the screen color is coming in as sRGB space
	return glm::vec4((PixelColorRef & 0xFF),
			((PixelColorRef & 0xFF00) >> 8),
			((PixelColorRef & 0xFF0000) >> 16),
			255);
}

void FPlatformApplicationMisc::SetHighDPIMode()
{
	//if (IsHighDPIAwarenessEnabled())
	//{
	//	if (void* ShCoreDll = FPlatformProcess::GetDllHandle(TEXT("shcore.dll")))
	//	{
	//		typedef enum _PROCESS_DPI_AWARENESS {
	//			PROCESS_DPI_UNAWARE = 0,
	//			PROCESS_SYSTEM_DPI_AWARE = 1,
	//			PROCESS_PER_MONITOR_DPI_AWARE = 2
	//		} PROCESS_DPI_AWARENESS;

	//		typedef HRESULT(STDAPICALLTYPE *SetProcessDpiAwarenessProc)(PROCESS_DPI_AWARENESS Value);
	//		SetProcessDpiAwarenessProc SetProcessDpiAwareness = (SetProcessDpiAwarenessProc)FPlatformProcess::GetDllExport(ShCoreDll, TEXT("SetProcessDpiAwareness"));
	//		GetDpiForMonitor = (GetDpiForMonitorProc)FPlatformProcess::GetDllExport(ShCoreDll, TEXT("GetDpiForMonitor"));

	//		typedef HRESULT(STDAPICALLTYPE *GetProcessDpiAwarenessProc)(HANDLE hProcess, PROCESS_DPI_AWARENESS* Value);
	//		GetProcessDpiAwarenessProc GetProcessDpiAwareness = (GetProcessDpiAwarenessProc)FPlatformProcess::GetDllExport(ShCoreDll, TEXT("GetProcessDpiAwareness"));

	//		if (SetProcessDpiAwareness && GetProcessDpiAwareness && !IsRunningCommandlet() && !FApp::IsUnattended())
	//		{
	//			PROCESS_DPI_AWARENESS CurrentAwareness = PROCESS_DPI_UNAWARE;

	//			GetProcessDpiAwareness(nullptr, &CurrentAwareness);

	//			if (CurrentAwareness != PROCESS_PER_MONITOR_DPI_AWARE)
	//			{
	//				UE_LOG(LogInit, Log, TEXT("Setting process to per monitor DPI aware"));
	//				HRESULT Hr = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE); // PROCESS_PER_MONITOR_DPI_AWARE_VALUE
	//																					// We dont care about this warning if we are in any kind of headless mode
	//				if (Hr != S_OK)
	//				{
	//					UE_LOG(LogInit, Warning, TEXT("SetProcessDpiAwareness failed.  Error code %x"), Hr);
	//				}
	//			}
	//		}

	//		FPlatformProcess::FreeDllHandle(ShCoreDll);
	//	}
	//	else if (void* User32Dll = FPlatformProcess::GetDllHandle(TEXT("user32.dll")))
	//	{
	//		typedef BOOL(WINAPI *SetProcessDpiAwareProc)(void);
	//		SetProcessDpiAwareProc SetProcessDpiAware = (SetProcessDpiAwareProc)FPlatformProcess::GetDllExport(User32Dll, TEXT("SetProcessDPIAware"));

	//		if (SetProcessDpiAware && !IsRunningCommandlet() && !FApp::IsUnattended())
	//		{
	//			UE_LOG(LogInit, Log, TEXT("Setting process to DPI aware"));

	//			BOOL Result = SetProcessDpiAware();
	//			if (Result == 0)
	//			{
	//				UE_LOG(LogInit, Warning, TEXT("SetProcessDpiAware failed"));
	//			}
	//		}

	//		FPlatformProcess::FreeDllHandle(User32Dll);
	//	}
	//}
}


int32_t FPlatformApplicationMisc::GetMonitorDPI(const FMonitorInfo& MonitorInfo)
{
	int32_t DisplayDPI = 96;

	if (IsHighDPIAwarenessEnabled())
	{
		if (GetDpiForMonitor)
		{
			RECT MonitorDim;
			MonitorDim.left = MonitorInfo.DisplayRect.Left;
			MonitorDim.top = MonitorInfo.DisplayRect.Top;
			MonitorDim.right = MonitorInfo.DisplayRect.Right;
			MonitorDim.bottom = MonitorInfo.DisplayRect.Bottom;

			HMONITOR Monitor = MonitorFromRect(&MonitorDim, MONITOR_DEFAULTTONEAREST);
			if (Monitor)
			{
				uint32_t DPIX = 0;
				uint32_t DPIY = 0;
				if (SUCCEEDED(GetDpiForMonitor(Monitor, 0 /*MDT_EFFECTIVE_DPI*/, &DPIX, &DPIY)))
				{
					DisplayDPI = DPIX;
				}
			}
		}
		else
		{
			HDC Context = GetDC(nullptr);
			DisplayDPI = GetDeviceCaps(Context, LOGPIXELSX);
			ReleaseDC(nullptr, Context);
		}
	}

	return DisplayDPI;
}

// Looks for an adapter with >= 512 MB of dedicated video memory and assumes we use it.
bool FPlatformApplicationMisc::ProbablyHasIntegratedGPU()
{ 
	IDXGIFactory1* DXGIFactory1;
	if (CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory1) != S_OK || !DXGIFactory1)
	{
		return false;
	}
	IDXGIAdapter* TempAdapter;
	for (uint32_t AdapterIndex = 0; DXGIFactory1->EnumAdapters(AdapterIndex, &TempAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
	{
		if (TempAdapter)
		{
			DXGI_ADAPTER_DESC Desc;
			TempAdapter->GetDesc(&Desc);
			TempAdapter->Release();
			const int MIN_GPU_MEMORY = 512 * 1024 * 1024;
			if (Desc.DedicatedVideoMemory >= MIN_GPU_MEMORY)
			{
				DXGIFactory1->Release();
				return false;
			}
		}
	}
	DXGIFactory1->Release();
	return true;
}

float FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(float X, float Y)
{
	float Scale = 1.0f;

	if (IsHighDPIAwarenessEnabled())
	{
		if (GetDpiForMonitor)
		{
			POINT Position = { static_cast<LONG>(X), static_cast<LONG>(Y) };
			HMONITOR Monitor = MonitorFromPoint(Position, MONITOR_DEFAULTTONEAREST);
			if (Monitor)
			{
				uint32_t DPIX = 0;
				uint32_t DPIY = 0;
				if (SUCCEEDED(GetDpiForMonitor(Monitor, 0 /*MDT_EFFECTIVE_DPI*/, &DPIX, &DPIY)))
				{
					Scale = (float)DPIX / 96.0f;
				}
			}
		}
		else
		{
			HDC Context = GetDC(nullptr);
			int32_t DPI = GetDeviceCaps(Context, LOGPIXELSX);
			Scale = (float)DPI / 96.0f;
			ReleaseDC(nullptr, Context);
		}
	}

	return Scale;
}

// Disabling optimizations helps to reduce the frequency of OpenClipboard failing with error code 0. It still happens
// though only with really large text buffers and we worked around this by changing the editor to use an intermediate
// text buffer for internal operations.

void FPlatformApplicationMisc::ClipboardCopy(const char* Str)
{
	if( OpenClipboard(GetActiveWindow()) )
	{
		if (!EmptyClipboard()) {
			return;
		}
		HGLOBAL GlobalMem;
		int32_t StrLen =strlen(Str);
		GlobalMem = GlobalAlloc( GMEM_MOVEABLE, sizeof(char)*(StrLen+1) );
		if (!GlobalMem) {
			return;
		}
		char* Data = (char*) GlobalLock( GlobalMem );
		strcpy( Data, Str );
		GlobalUnlock( GlobalMem );
		if( SetClipboardData( CF_UNICODETEXT, GlobalMem ) == NULL )
			printf("SetClipboardData failed with error code %i", (uint32_t)GetLastError() );
		if (!CloseClipboard()) {
			printf("CloseClipboard error ");
		}
	}
}

//void FWindowsPlatformApplicationMisc::ClipboardPaste(class FString& Result)
//{
//	if( OpenClipboard(GetActiveWindow()) )
//	{
//		HGLOBAL GlobalMem = NULL;
//		bool Unicode = 0;
//		GlobalMem = GetClipboardData( CF_UNICODETEXT );
//		Unicode = 1;
//		if( !GlobalMem )
//		{
//			GlobalMem = GetClipboardData( CF_TEXT );
//			Unicode = 0;
//		}
//		if( !GlobalMem )
//		{
//			Result = TEXT("");
//		}
//		else
//		{
//			void* Data = GlobalLock( GlobalMem );
//			check( Data );	
//			if( Unicode )
//				Result = (TCHAR*) Data;
//			else
//			{
//				ANSICHAR* ACh = (ANSICHAR*) Data;
//				int32 i;
//				for( i=0; ACh[i]; i++ );
//				TArray<TCHAR> Ch;
//				Ch.AddUninitialized(i+1);
//				for( i=0; i<Ch.Num(); i++ )
//					Ch[i]=CharCast<TCHAR>(ACh[i]);
//				Result.GetCharArray() = MoveTemp(Ch);
//			}
//			GlobalUnlock( GlobalMem );
//		}
//		verify(CloseClipboard());
//	}
//	else 
//	{
//		Result=TEXT("");
//	}
//}

