#include "GenericPlatform/GameFrameworkPCH.h"

#include "GenericPlatform/GameFramework.h"
#include "Misc/App.h"
#include <chrono>
static GameFramework* gs_pSingelton = nullptr;


GameFramework::GameFramework()
: m_RequestQuit( false )
{

    // Init spdlog.
    spdlog::init_thread_pool( 8192, 1 );
    auto stdout_sink   = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "logs/log.txt", 1024 * 1024 * 5, 3,
        true );  // Max size: 5MB, Max files: 3, Rotate on open: true
    auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();

    std::vector<spdlog::sink_ptr> sinks { stdout_sink, rotating_sink, msvc_sink };
    m_Logger = std::make_shared<spdlog::async_logger>( "GameFramework", sinks.begin(), sinks.end(),
                                                       spdlog::thread_pool(), spdlog::async_overflow_policy::block );
    spdlog::register_logger( m_Logger );
    spdlog::set_default_logger( m_Logger );
    
    m_SlateManager = SlateManager::Create();
    
}

GameFramework::~GameFramework()
{

}

GameFramework& GameFramework::Create()
{
    if ( !gs_pSingelton )
    {
        gs_pSingelton = new GameFramework();

        spdlog::info( "GameFramework class created." );
    }

    return *gs_pSingelton;
}

void GameFramework::Destroy()
{
    if ( gs_pSingelton )
    {
        delete gs_pSingelton;
        gs_pSingelton = nullptr;
        spdlog::info( "GameFramework class destroyed." );
    }
}

GameFramework& GameFramework::Get()
{
    assert( gs_pSingelton != nullptr );
    return *gs_pSingelton;
}

// Create loggers
// @see https://github.com/gabime/spdlog#asynchronous-logger-with-multi-sinks
Logger GameFramework::CreateLogger( const std::string& name )
{
    Logger logger = spdlog::get( name );
    if ( !logger )
    {
        logger = m_Logger->clone( name );
        spdlog::register_logger( logger );
    }

    return logger;
}



void GameFramework::UpdateTime()
{
    FApp::UpdateLastTime();
    auto now = std::chrono::steady_clock::now();
    FApp::SetCurrentTime(std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count()/ std::micro::den);
    auto lasttime=FApp::GetLastTime();
    FApp::SetDeltaTime(FApp::GetCurrentTime()- FApp::GetLastTime());
}


void GameFramework::Stop()
{
    // When called from another thread other than the main thread,
    // the WM_QUIT message goes to that thread and will not be handled
    // in the main thread. To circumvent this, we also set a boolean flag
    // to indicate that the user has requested to quit the application.
    m_RequestQuit = true;
}


void GameFramework::OnExit( EventArgs& e )
{
    // Invoke the Exit event.
    Exit( e );
}


