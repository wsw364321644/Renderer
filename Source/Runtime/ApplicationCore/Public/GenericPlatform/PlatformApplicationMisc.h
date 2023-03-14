#pragma once

#include "GenericPlatform/GenericPlatformApplicationMisc.h"

struct FPlatformApplicationMisc : public FGenericPlatformApplicationMisc
{
	static void LoadStartupModules();
	//static class FOutputDeviceConsole* CreateConsoleOutputDevice();
	//static class FOutputDeviceError* GetErrorOutputDevice();
	//static class FFeedbackContext* GetFeedbackContext();
	static class GenericApplication* CreateApplication();
	static void RequestMinimize();
	static bool IsThisApplicationForeground();
	static int32_t GetAppIcon();
	static void PumpMessages(bool bFromMainLoop);
	static void RequestExitWithStatus(bool Force, uint8_t ReturnCode);
	static void PreventScreenSaver();
	static glm::vec4 GetScreenPixelColor(const glm::vec2& InScreenPos, float InGamma = 1.0f);
	static void SetHighDPIMode();

	static float GetDPIScaleFactorAtPoint(float X, float Y);
	static void ClipboardCopy(const char* Str);
	//static void ClipboardPaste(class FString& Dest);

	/** Windows platform only */
	/** Function should retrieve the DPI value for the provided monitor information structure */
	static int32_t GetMonitorDPI(const FMonitorInfo& MonitorInfo);
	static bool ProbablyHasIntegratedGPU();
	/** End Windows platform only */
};

