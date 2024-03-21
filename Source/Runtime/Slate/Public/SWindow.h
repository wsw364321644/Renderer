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





    //todo remove event

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

    // A keyboard key was pressed.
    virtual void OnKeyPressed(KeyEventArgs& e) { KeyPressed(e); }
    // A keyboard key was released
    virtual void OnKeyReleased(KeyEventArgs& e) { KeyReleased(e); }
    // Window gained keyboard focus
    virtual void OnKeyboardFocus(EventArgs& e) { KeyboardFocus(e); }
    // Window lost keyboard focus
    virtual void OnKeyboardBlur(EventArgs& e) { KeyboardBlur(e); }

    // The mouse was moved
    virtual void OnMouseMoved(MouseMotionEventArgs& e) { MouseMoved(e); }
    // A button on the mouse was pressed
    virtual void OnMouseButtonPressed(MouseButtonEventArgs& e) { MouseButtonPressed(e); }
    // A button on the mouse was released
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) { MouseButtonReleased(e); }
    // The mouse wheel was moved.
    virtual void OnMouseWheel(MouseWheelEventArgs& e) { MouseWheel(e); }

    // The mouse entered the client area.
    virtual void OnMouseEnter(MouseMotionEventArgs& e) { MouseEnter(e); }
    // The mouse left the client are of the window.
    virtual void OnMouseLeave(EventArgs& e) { MouseLeave(e); }
    // The application window has received mouse focus
    virtual void OnMouseFocus(EventArgs& e) { MouseFocus(e); }
    // The application window has lost mouse focus
    virtual void OnMouseBlur(EventArgs& e) { MouseBlur(e); }
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



    /**
* Invoked when the game should be updated.
*/
    UpdateEvent Update;

    /**
     * Invoked when a keyboard key is pressed while the window has focus.
     */
    KeyboardEvent KeyPressed;

    /**
     * Invoked when a keyboard key is released while the window has focus.
     */
    KeyboardEvent KeyReleased;

    /**
     * Invoked when the window gains keyboard focus.
     */
    Event KeyboardFocus;

    /**
     * Invoked when the window loses keyboard focus.
     */
    Event KeyboardBlur;

    /**
     * Invoked when the mouse is moved over the window.
     */
    MouseMotionEvent MouseMoved;

    /**
     * Invoked when the mouse enters the client area.
     */
    MouseMotionEvent MouseEnter;

    /**
     * Invoked when the mouse button is pressed over the window.
     */
    MouseButtonEvent MouseButtonPressed;

    /**
     * Invoked when the mouse button is released over the window.
     */
    MouseButtonEvent MouseButtonReleased;

    /**
     * Invoked when the mouse wheel is scrolled over the window.
     */
    MouseWheelEvent MouseWheel;

    /**
     * Invoked when the mouse cursor leaves the client area.
     */
    Event MouseLeave;

    /**
     * Invoked when the window gains mouse focus.
     */
    Event MouseFocus;

    /**
     * Invoked when the window looses mouse focus.
     */
    Event MouseBlur;



    std::string WindowTitle;
    glm::vec2 ScreenPosition;
    glm::vec2 ClientSize;
    std::weak_ptr<FGameInstance> GameInstance;
protected:
    

private:

	std::shared_ptr<FGenericWindow> NativeWindow;
};