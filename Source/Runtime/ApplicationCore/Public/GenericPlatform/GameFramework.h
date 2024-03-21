#pragma once

/**
 *  @file Application.h
 *  @date September 29, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief The application class is used to create windows for our application.
 */



#include <SDL.h>

#include <memory>
#include <unordered_map> 
#include <string> 

#include <TimeRecorder.h>
#include "Events.h"

extern const char* ENGINE_LOG_NAME;


class FSlateManager;
class FGameInstance;


class FGameFramework
{
public:
    /**
     * Create the singleton application instance.
     *
     * @parm hInst The application instance.
     * @returns A reference to the created instance.
     */
    static FGameFramework& Create();

    /**
     * Destroy the Application instance.
     */
    static void Destroy();

    /**
     * Get a reference to the application instance.
     * @returns A reference to the Application instance.
     */
    static FGameFramework& Get();

    FSlateManager* GetSlateManager() {
        return m_SlateManager.get();
    }

    void Tick();

    std::weak_ptr<FGameInstance> CreateGameInstance(std::string name);

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

    FGameFramework();
    virtual ~FGameFramework();

    // Application is going to close
    virtual void OnExit( EventArgs& e );
    void UpdateTime();
private:
    // Private and deleted. Please don't try to create copies
    // of this singleton!
    FGameFramework( const FGameFramework& ) = delete;
    FGameFramework( FGameFramework&& )      = delete;
    FGameFramework& operator=( FGameFramework& ) = delete;
    FGameFramework& operator=( FGameFramework&& ) = delete;



    // Should the application quit?
    std::atomic_bool m_RequestQuit;



    
    std::shared_ptr < FSlateManager> m_SlateManager;
    std::unordered_map <std::string, std::shared_ptr<FGameInstance>> GameInstances;
};