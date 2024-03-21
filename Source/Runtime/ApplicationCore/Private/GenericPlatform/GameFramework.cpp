#include "GenericPlatform/GameFrameworkPCH.h"
#include "GenericPlatform/GameFramework.h"
#include "SlateManager.h"
#include "GameInstance.h"
#include "LocalPlayer.h"
#include "Misc/App.h"
#include <chrono>
#include <LoggerHelper.h>
static FGameFramework* gs_pSingelton = nullptr;
const char* ENGINE_LOG_NAME = "Engine";

FGameFramework::FGameFramework()
: m_RequestQuit( false )
{

    // Init spdlog.
    //spdlog::init_thread_pool( 8192, 1 );
    //auto stdout_sink   = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    //auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
    //    "logs/log.txt", 1024 * 1024 * 5, 3,
    //    true );  // Max size: 5MB, Max files: 3, Rotate on open: true
    //auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();

    //std::vector<spdlog::sink_ptr> sinks { stdout_sink, rotating_sink, msvc_sink };
    //m_Logger = std::make_shared<spdlog::async_logger>( "GameFramework", sinks.begin(), sinks.end(),
    //                                                   spdlog::thread_pool(), spdlog::async_overflow_policy::block );
    //spdlog::register_logger( m_Logger );
    //spdlog::set_default_logger( m_Logger );
    m_SlateManager = FSlateManager::Create();
    
}

FGameFramework::~FGameFramework()
{

}

FGameFramework& FGameFramework::Create()
{
    if ( !gs_pSingelton )
    {
        gs_pSingelton = new FGameFramework();
        SIMPLELOG_LOGGER_TRACE(ENGINE_LOG_NAME, "GameFramework class created.");
    }

    return *gs_pSingelton;
}

void FGameFramework::Destroy()
{
    if ( gs_pSingelton )
    {
        delete gs_pSingelton;
        gs_pSingelton = nullptr;
        SIMPLELOG_LOGGER_TRACE(ENGINE_LOG_NAME,"GameFramework class created.");
    }
}

FGameFramework& FGameFramework::Get()
{
    assert( gs_pSingelton != nullptr );
    return *gs_pSingelton;
}

void FGameFramework::Tick()
{
    UpdateTime();
}

std::weak_ptr<FGameInstance> FGameFramework::CreateGameInstance(std::string name)
{
    auto itr=GameInstances.find(name);
    if (itr != GameInstances.end()) {
        return itr->second;
    }
    auto res = GameInstances.emplace(name, std::make_shared< FGameInstance>());
    if (!res.second) {
        return std::weak_ptr<FGameInstance>();
    }
    auto pGameInstance=res.first->second;
    
    pGameInstance->AddLocalPlayer(std::make_shared<FLocalPlayer>());
    return pGameInstance;
}

void FGameFramework::UpdateTime()
{
    FApp::UpdateLastTime();
    auto now = std::chrono::steady_clock::now();
    FApp::SetCurrentTime(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count()/ std::micro::den);
    auto lasttime=FApp::GetLastTime();
    FApp::SetDeltaTime(FApp::GetCurrentTime()- FApp::GetLastTime());
}


void FGameFramework::Stop()
{
    // When called from another thread other than the main thread,
    // the WM_QUIT message goes to that thread and will not be handled
    // in the main thread. To circumvent this, we also set a boolean flag
    // to indicate that the user has requested to quit the application.
    m_RequestQuit = true;
}


void FGameFramework::OnExit( EventArgs& e )
{
    // Invoke the Exit event.
    Exit( e );
}


