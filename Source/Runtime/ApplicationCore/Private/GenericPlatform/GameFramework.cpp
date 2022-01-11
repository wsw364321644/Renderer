#include "GenericPlatform/GameFrameworkPCH.h"

#include <GenericPlatform/GameFramework.h>

static GameFramework* gs_pSingelton = nullptr;


GameFramework::GameFramework(std::shared_ptr<GenericApplication> inGenericApplication)
:  m_bIsRunning( false )
, m_RequestQuit( false )
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

    // Init gainput.
    m_KeyboardDevice = m_InputManager.CreateDevice<gainput::InputDeviceKeyboard>();
    m_MouseDevice    = m_InputManager.CreateDevice<gainput::InputDeviceMouse>();
    for ( unsigned i = 0; i < gainput::MaxPadCount; ++i )
    { m_GamepadDevice[i] = m_InputManager.CreateDevice<gainput::InputDevicePad>( i ); }

    // This will prevent normalization of mouse coordinates.
    m_InputManager.SetDisplaySize(1, 1);



    m_GenericApplication = inGenericApplication;
    m_SlateManager = std::make_shared< SlateManager>();
    m_GenericApplication->SetMessageHandler(m_SlateManager);
}

GameFramework::~GameFramework()
{

}

GameFramework& GameFramework::Create(std::shared_ptr<GenericApplication> inGenericApplication)
{
    if ( !gs_pSingelton )
    {
        gs_pSingelton = new GameFramework(inGenericApplication);

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

gainput::DeviceId GameFramework::GetKeyboardId() const
{
    return m_KeyboardDevice;
}

gainput::DeviceId GameFramework::GetMouseId() const
{
    return m_MouseDevice;
}

gainput::DeviceId GameFramework::GetPadId( unsigned index /*= 0 */ ) const
{
    assert( index >= 0 && index < gainput::MaxPadCount );
    return m_GamepadDevice[index];
}

std::shared_ptr<gainput::InputMap> GameFramework::CreateInputMap( const char* name )
{
    return std::make_shared<gainput::InputMap>( m_InputManager, name );
}

int32_t GameFramework::Run()
{
    assert( !m_bIsRunning );

    m_bIsRunning = true;

    MSG msg = {};
    while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) && msg.message != WM_QUIT )
    {
        ::TranslateMessage( &msg );
        ::DispatchMessage( &msg );

        m_InputManager.HandleMessage( msg );

        // Check to see of the application wants to quit.
        if ( m_RequestQuit )
        {
            ::PostQuitMessage( 0 );
            m_RequestQuit = false;
        }
    }

    m_bIsRunning = false;

    return static_cast<int32_t>( msg.wParam );
}

void GameFramework::SetDisplaySize( int width, int height )
{
    m_InputManager.SetDisplaySize( width, height );
}

void GameFramework::ProcessInput()
{
    m_InputManager.Update();
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


