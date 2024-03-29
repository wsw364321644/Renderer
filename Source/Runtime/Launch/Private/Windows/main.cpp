#include "Editor.h"
#include "Windows/WindowsApplication.h"
#include "LaunchEngineLoop.h"
#include <GenericPlatform/GameFramework.h>
#include <Device.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <shellapi.h>

using namespace dx12lib;

FEngineLoop	GEngineLoop;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{

#if defined( _DEBUG )
    Device::EnableDebugLayer();
#endif

    int retCode = 0;

    WCHAR path[MAX_PATH];

    int     argc = 0;
    LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);
    if (argv)
    {
        for (int i = 0; i < argc; ++i)
        {
            // -wd Specify the Working Directory.
            if (wcscmp(argv[i], L"-wd") == 0)
            {
                wcscpy_s(path, argv[++i]);
                SetCurrentDirectoryW(path);
            }
        }
        LocalFree(argv);
    }



    FGameFramework::Create();
    {
        std::unique_ptr<Editor> demo = std::make_unique<Editor>("Editor", 1920, 1080, true);
        retCode = demo->Init();
        GEngineLoop.Run();
        demo->UnloadContent();
    }
    FGameFramework::Destroy();

    atexit(&Device::ReportLiveObjects);

    return retCode;
}