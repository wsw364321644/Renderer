#pragma once
#include <glm/glm.hpp>
#include <string>
#include "SCompoundWidget.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/Events.h"

class FGameInstance;
class SWindow :public SCompoundWidget {

    
public:
	SWindow();
    virtual ~SWindow();

	void SetPlatformWindow(std::shared_ptr<FGenericWindow> window);
    std::shared_ptr<FGenericWindow> GetNativeWindow() { return NativeWindow; };
    std::shared_ptr<const FGenericWindow> GetNativeWindow() const { return NativeWindow; };

    void ShowWindow();
    std::string GetTitle() const;
    void SetTitle(const std::string& InTitle);

    void SetWindowMode(EWindowMode WindowMode);
    EWindowMode GetWindowMode() const { return NativeWindow->GetWindowMode(); }







    // The DPI scaling of the window has changed.
    virtual void OnDPIScaleChanged(DPIScaleEventArgs& e) ;

    // Window was closed.
    virtual void OnClose(WindowCloseEventArgs& e) ;

    // Window was resized.
    virtual void OnResize(ResizeEventArgs& e) ;

    // Window was minimized.
    virtual void OnMinimized(ResizeEventArgs& e) ;

    // Window was maximized.
    virtual void OnMaximized(ResizeEventArgs& e) ;

    // Window was restored.
    virtual void OnRestored(ResizeEventArgs& e) ;

    virtual void OnUpdate(UpdateEventArgs& e);

    /**
  * The DPI scaling of the window has changed.
  */
    DPIScaleEvent DPIScaleChanged;

    /**
     * Window close event is fired when the window is about to be closed.
     */
    WindowCloseEvent Close;

    /**
     * Invoked when the window is resized.
     */
    ResizeEvent Resize;

    /**
     * Invoked when the window is minimized.
     */
    ResizeEvent Minimized;

    /**
     * Invoked when the window is maximized.
     */
    ResizeEvent Maximized;

    /**
     * Invoked when the window is restored.
     */
    ResizeEvent Restored;


    std::string WindowTitle;
    glm::vec2 ScreenPosition;
    glm::vec2 ClientSize;
    std::weak_ptr<FGameInstance> GameInstance;
protected:
    

private:

	std::shared_ptr<FGenericWindow> NativeWindow;
};