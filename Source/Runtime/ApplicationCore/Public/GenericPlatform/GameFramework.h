#pragma once

/**
 *  @file Application.h
 *  @date September 29, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief The application class is used to create windows for our application.
 */



#include <SDL.h>
#include <spdlog/logger.h>


#include <cstdint>      // for uint32_t
#include <limits>       // for std::numeric_limits
#include <memory>       // for std::shared_ptr
#include <mutex>        // for std::mutex
#include <string>       // for std::wstring
#include <thread>       // for std::thread
#include <type_traits>  // for std::enable_if

#include "Events.h"

#include "SlateManager.h"



class GenericWindow;

using Logger = std::shared_ptr<spdlog::logger>;


class GameFramework
{
public:
    /**
     * Create the singleton application instance.
     *
     * @parm hInst The application instance.
     * @returns A reference to the created instance.
     */
    static GameFramework& Create();

    /**
     * Destroy the Application instance.
     */
    static void Destroy();

    /**
     * Get a reference to the application instance.
     * @returns A reference to the Application instance.
     */
    static GameFramework& Get();

    SlateManager* GetSlateManager() { return m_SlateManager.get(); }
    /**
     * Create a named logger or get a previously created logger with the same
     * name.
     */
    Logger CreateLogger( const std::string& name );


    /**
     * Start the main application run loop.
     *
     * @returns The error code (if an error occurred).
     */
    void UpdateTime();

    /**
     * Inform the input manager of changes to the size of the display.
     * This is needed for gainput to normalize mouse inputs.
     * NOTE: Only use this on a single window's Resize event!
     *
     * @param width The width of the window's client area (in pixels).
     * @param height The height of the window's client area (in pixels).
     */
    void SetDisplaySize( int width, int height );

    /**
     * Process joystick and keyboard events. This should be called once per
     * frame before updating the game logic.
     */
    void ProcessInput();

    /**
     * Stop the application.
     */
    void Stop();
    bool GetRequestStop() { return m_RequestQuit; }

    /**
     * Application is exiting.
     */
    Event Exit;

protected:

    GameFramework();
    virtual ~GameFramework();

    // Application is going to close
    virtual void OnExit( EventArgs& e );

private:
    // Private and deleted. Please don't try to create copies
    // of this singleton!
    GameFramework( const GameFramework& ) = delete;
    GameFramework( GameFramework&& )      = delete;
    GameFramework& operator=( GameFramework& ) = delete;
    GameFramework& operator=( GameFramework&& ) = delete;


    Logger m_Logger;


    // Should the application quit?
    std::atomic_bool m_RequestQuit;



    
    std::shared_ptr < SlateManager> m_SlateManager;
};