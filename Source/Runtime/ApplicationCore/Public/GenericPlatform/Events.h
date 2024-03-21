#pragma once

/**
 *  @file Events.h
 *  @date September 29, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief Application and Window events.
 */

#include "keyCodes.h"

#include "Templates/Event.h"
#include <string>


 /**
  * Base class for all event args.
  */
class EventArgs
{
public:
    EventArgs() = default;
};

// Define the default event.
using Event = CommonDelegate<void(EventArgs&)>;

/**
 * Update event args.
 */
class UpdateEventArgs : public EventArgs
{
public:
    UpdateEventArgs( double deltaTime, double totalTime )
    : DeltaTime( deltaTime )
    , TotalTime( totalTime )
    {}

    // The elapsed time (in seconds)
    double DeltaTime;
    // Total time the application has been running (in seconds).
    double TotalTime;
};

using UpdateEvent = CommonDelegate<void(UpdateEventArgs&)>;

class DPIScaleEventArgs : public EventArgs
{
public:
    typedef EventArgs base;
    DPIScaleEventArgs( float dpiScale )
    : DPIScale( dpiScale )
    {}

    float DPIScale;
};

using DPIScaleEvent = CommonDelegate<void(DPIScaleEventArgs&)>;

/**
 * EventArgs for a WindowClosing event.
 */
class WindowCloseEventArgs : public EventArgs
{
public:
    using base = EventArgs;
    WindowCloseEventArgs()
    : base()
    , ConfirmClose( true )
    {}

    /**
     * The user can cancel a window closing operation by registering for the
     * Window::Close event on the Window and setting the
     * WindowCloseEventArgs::ConfirmClose property to false if the window should
     * be kept open (for example, if closing the window would cause unsaved
     * file changes to be lost).
     */
    bool ConfirmClose;
};

using WindowCloseEvent = CommonDelegate<void(WindowCloseEventArgs&)>;

enum class KeyState
{
    Released = 0,
    Pressed = 1,
};



enum class EModifierKey : uint8_t
{
    LeftShift,		// VK_LSHIFT
    RightShift,		// VK_RSHIFT
    LeftControl,	// VK_LCONTROL
    RightControl,	// VK_RCONTROL
    LeftAlt,		// VK_LMENU
    RightAlt,		// VK_RMENU
    CapsLock,		// VK_CAPITAL
    Count,
};

class FModifierKeysState
{
    typedef uint16_t ModifierKeysStateMask;
public:

    /**
     * Constructor.  Events are immutable once constructed.
     *
     * @param  bInIsLeftShiftDown  True if left shift is pressed
     * @param  bInIsRightShiftDown  True if right shift is pressed
     * @param  bInIsLeftControlDown  True if left control is pressed
     * @param  bInIsRightControlDown  True if right control is pressed
     * @param  bInIsLeftAltDown  True if left alt is pressed
     * @param  bInIsRightAltDown  True if right alt is pressed
     */
    FModifierKeysState(const bool bInIsLeftShiftDown,
        const bool bInIsRightShiftDown,
        const bool bInIsLeftControlDown,
        const bool bInIsRightControlDown,
        const bool bInIsLeftAltDown,
        const bool bInIsRightAltDown,
        const bool bInIsLeftCommandDown,
        const bool bInIsRightCommandDown,
        const bool bInAreCapsLocked)
        : bIsLeftShiftDown(bInIsLeftShiftDown),
        bIsRightShiftDown(bInIsRightShiftDown),
        bIsLeftControlDown(bInIsLeftControlDown),
        bIsRightControlDown(bInIsRightControlDown),
        bIsLeftAltDown(bInIsLeftAltDown),
        bIsRightAltDown(bInIsRightAltDown),
        bIsLeftCommandDown(bInIsLeftCommandDown),
        bIsRightCommandDown(bInIsRightCommandDown),
        bAreCapsLocked(bInAreCapsLocked)
    {
    }

    FModifierKeysState()
        : bIsLeftShiftDown(false),
        bIsRightShiftDown(false),
        bIsLeftControlDown(false),
        bIsRightControlDown(false),
        bIsLeftAltDown(false),
        bIsRightAltDown(false),
        bIsLeftCommandDown(false),
        bIsRightCommandDown(false),
        bAreCapsLocked(false)
    {
    }

    /**
     * Returns true if either shift key was down when this event occurred
     *
     * @return  True if shift is pressed
     */
    bool IsShiftDown() const
    {
        return bIsLeftShiftDown || bIsRightShiftDown;
    }

    /**
     * Returns true if left shift key was down when this event occurred
     *
     * @return  True if left shift is pressed
     */
    bool IsLeftShiftDown() const
    {
        return bIsLeftShiftDown;
    }

    /**
     * Returns true if right shift key was down when this event occurred
     *
     * @return  True if right shift is pressed
     */
    bool IsRightShiftDown() const
    {
        return bIsRightShiftDown;
    }

    /**
     * Returns true if either control key was down when this event occurred
     *
     * @return  True if control is pressed
     */
    bool IsControlDown() const
    {
        return bIsLeftControlDown || bIsRightControlDown;
    }

    /**
     * Returns true if left control key was down when this event occurred
     *
     * @return  True if left control is pressed
     */
    bool IsLeftControlDown() const
    {
        return bIsLeftControlDown;
    }

    /**
     * Returns true if right control key was down when this event occurred
     *
     * @return  True if right control is pressed
     */
    bool IsRightControlDown() const
    {
        return bIsRightControlDown;
    }

    /**
     * Returns true if either alt key was down when this event occurred
     *
     * @return  True if alt is pressed
     */
    bool IsAltDown() const
    {
        return bIsLeftAltDown || bIsRightAltDown;
    }

    /**
     * Returns true if left alt key was down when this event occurred
     *
     * @return  True if left alt is pressed
     */
    bool IsLeftAltDown() const
    {
        return bIsLeftAltDown;
    }

    /**
     * Returns true if right alt key was down when this event occurred
     *
     * @return  True if right alt is pressed
     */
    bool IsRightAltDown() const
    {
        return bIsRightAltDown;
    }

    /**
     * Returns true if either command key was down when this event occurred
     *
     * @return  True if command is pressed
     */
    bool IsCommandDown() const
    {
        return bIsLeftCommandDown || bIsRightCommandDown;
    }

    /**
     * Returns true if left command key was down when this event occurred
     *
     * @return  True if left command is pressed
     */
    bool IsLeftCommandDown() const
    {
        return bIsLeftCommandDown;
    }

    /**
     * Returns true if right command key was down when this event occurred
     *
     * @return  True if right command is pressed
     */
    bool IsRightCommandDown() const
    {
        return bIsRightCommandDown;
    }


    /**
     * @return  true if the Caps Lock key has been toggled to the enabled state.
     */
    bool AreCapsLocked() const
    {
        return bAreCapsLocked;
    }

    /**
     * @param ModifierKeys the modifier keys to test to see if they are pressed.  Returns true if no modifiers are specified.
     * @return true if all modifier keys are pressed specified in the modifier keys.
     */
    bool AreModifersDown(ModifierKeysStateMask inModifierKeysStateMask) const
    {
        bool AllModifersDown = true;

        if (inModifierKeysStateMask & EModifierKeyToMask(EModifierKey::LeftShift))
        {
            AllModifersDown &= IsLeftShiftDown();
        }
        if (inModifierKeysStateMask & EModifierKeyToMask(EModifierKey::RightShift))
        {
            AllModifersDown &= IsRightShiftDown();
        }
        if (inModifierKeysStateMask & EModifierKeyToMask(EModifierKey::LeftControl))
        {
            AllModifersDown &= IsLeftControlDown();
        }
        if (inModifierKeysStateMask & EModifierKeyToMask(EModifierKey::RightControl))
        {
            AllModifersDown &= IsRightControlDown();
        }
        if (inModifierKeysStateMask & EModifierKeyToMask(EModifierKey::LeftAlt))
        {
            AllModifersDown &= IsLeftAltDown();
        }
        if (inModifierKeysStateMask & EModifierKeyToMask(EModifierKey::RightAlt))
        {
            AllModifersDown &= IsRightAltDown();
        }

        return AllModifersDown;
    }
    static inline ModifierKeysStateMask EModifierKeyToMask(EModifierKey type) {
        return 1 << ((uint8_t)type + 1);
    }
    static inline ModifierKeysStateMask FromBools(const bool bLShift, const bool bRShift, const bool bLControl, const bool bRControl, const bool bLAlt, const bool bRAlt)
    {
        ModifierKeysStateMask ModifierMask = 0;
        if (bLShift)	ModifierMask |= EModifierKeyToMask( EModifierKey::LeftShift);
        if (bRShift)	ModifierMask |= EModifierKeyToMask( EModifierKey::RightShift);
        if (bLControl)		ModifierMask |= EModifierKeyToMask(EModifierKey::LeftControl);
        if (bRControl)		ModifierMask |= EModifierKeyToMask(EModifierKey::RightControl);
        if (bLAlt)		ModifierMask |= EModifierKeyToMask(EModifierKey::LeftAlt);
        if (bRAlt)		ModifierMask |= EModifierKeyToMask(EModifierKey::RightAlt);
        return ModifierMask;
    }
private:

    /** True if the left shift key was down when this event occurred. */
    uint16_t bIsLeftShiftDown : 1;

    /** True if the right shift key was down when this event occurred. */
    uint16_t bIsRightShiftDown : 1;

    /** True if the left control key was down when this event occurred. */
    uint16_t bIsLeftControlDown : 1;

    /** True if the right control key was down when this event occurred. */
    uint16_t bIsRightControlDown : 1;

    /** True if the left alt key was down when this event occurred. */
    uint16_t bIsLeftAltDown : 1;

    /** True if the right alt key was down when this event occurred. */
    uint16_t bIsRightAltDown : 1;

    /** True if the left command key was down when this event occurred. */
    uint16_t bIsLeftCommandDown : 1;

    /** True if the right command key was down when this event occurred. */
    uint16_t bIsRightCommandDown : 1;

    /** True if the Caps Lock key has been toggled to the enabled state. */
    uint16_t bAreCapsLocked : 1;
};

/**
 * KeyEventArgs are used for keyboard key pressed/released events.
 */
class KeyEventArgs : public EventArgs
{
public:
    using base = EventArgs;
    KeyEventArgs( KeyCode key, unsigned int c, KeyState state, bool lcontrol, bool rcontrol, bool lshift, bool rshift, bool lalt, bool ralt, bool capsLock)
    : base()
    , Key( key )
    , Char( c )
    , State( state )
    , ModifierKeysState(lshift, rshift, lcontrol, rcontrol, lalt, ralt,false,false, capsLock)
    {}

    KeyCode      Key;   // The Key Code that was pressed or released.
    unsigned int Char;  // The 32-bit character code that was pressed. This
                        // value will be 0 if it is a non-printable character.
    KeyState State;     // Was the key pressed or released?

    FModifierKeysState ModifierKeysState;
};

using KeyboardEvent = CommonDelegate<void(KeyEventArgs&)>;

/**
 * MouseMotionEventArgs are used to indicate the mouse moved or was dragged over
 * the window.
 */
class MouseMotionEventArgs : public EventArgs
{
public:
    using base = EventArgs;
    MouseMotionEventArgs( bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y )
    : base()
    , LeftButton( leftButton )
    , MiddleButton( middleButton )
    , RightButton( rightButton )

    , X( x )
    , Y( y )
    , RelX( 0 )
    , RelY( 0 )
    , ModifierKeysState(shift, shift, control, control, false, false,false,false, false)
    {}

    bool LeftButton;    // Is the left mouse button down?
    bool MiddleButton;  // Is the middle mouse button down?
    bool RightButton;   // Is the right mouse button down?

    int  X;             // The X-position of the cursor relative to the upper-left corner of
                        // the client area (in pixels).
    int Y;              // The Y-position of the cursor relative to the upper-left corner of
                        // the client area (in pixels).
    int RelX;           // How far the mouse moved since the last event (in pixels).
    int RelY;           // How far the mouse moved since the last event (in pixels).

    FModifierKeysState ModifierKeysState;
};

using MouseMotionEvent = CommonDelegate<void(MouseMotionEventArgs&)>;

enum class EMouseButton:uint8_t
{
    Left = 0,
    Middle,
    Right,
    Thumb01,
    Thumb02,

    Invalid
};

enum class ButtonState
{
    Released = 0,
    Pressed  = 1,
    DoubleClick=2
};

class MouseButtonEventArgs : public EventArgs
{
public:
    using base = EventArgs;

    MouseButtonEventArgs( EMouseButton button, ButtonState state, bool leftButton, bool middleButton, bool rightButton,
                          bool control, bool shift, int x, int y )
    : base()
    , Button( button )
    , State( state )
    , LeftButton( leftButton )
    , MiddleButton( middleButton )
    , RightButton( rightButton )
        , ModifierKeysState(shift, shift, control, control, false, false, false, false, false)
    , X( x )
    , Y( y )
    {}

    EMouseButton Button;        // The mouse button that was pressed or released.
    ButtonState State;         // Was the button pressed or released?
    bool        LeftButton;    // Is the left mouse button down?
    bool        MiddleButton;  // Is the middle mouse button down?
    bool        RightButton;   // Is the right mouse button down?

    int X;  // The X-position of the cursor relative to the upper-left corner of
            // the client area.
    int Y;  // The Y-position of the cursor relative to the upper-left corner of
            // the client area.

    FModifierKeysState ModifierKeysState;
};

using MouseButtonEvent = CommonDelegate<void(MouseButtonEventArgs&)>;

/**
 * MouseWheelEventArgs indicates if the mouse wheel was moved and how much.
 */
class MouseWheelEventArgs : public EventArgs
{
public:
    using base = EventArgs;

    MouseWheelEventArgs( float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool control,
                         bool shift, int x, int y )
    : base()
    , WheelDelta( wheelDelta )
    , LeftButton( leftButton )
    , MiddleButton( middleButton )
    , RightButton( rightButton )
    , Control( control )
    , Shift( shift )
    , X( x )
    , Y( y )
    {}

    float WheelDelta;   // How much the mouse wheel has moved. A positive value
                        // indicates that the wheel was moved to the right. A
                        // negative value indicates the wheel was moved to the
                        // left.
    bool LeftButton;    // Is the left mouse button down?
    bool MiddleButton;  // Is the middle mouse button down?
    bool RightButton;   // Is the right mouse button down?
    bool Control;       // Is the CTRL key down?
    bool Shift;         // Is the Shift key down?
    int  X;             // The X-position of the cursor relative to the upper-left corner of
                        // the client area.
    int Y;              // The Y-position of the cursor relative to the upper-left corner of
                        // the client area.
};
using MouseWheelEvent = CommonDelegate<void(MouseWheelEventArgs&)>;

enum class EWindowState
{
    Restored  = 0,  // The window has been resized.
    Minimized = 1,  // The window has been minimized.
    Maximized = 2,  // The window has been maximized.
    ClickedNonClientArea =3,
    WindowMenu=4

};

/**
 * Event args to indicate the window has been resized.
 */
class ResizeEventArgs : public EventArgs
{
public:
    using base = EventArgs;

    ResizeEventArgs( int width, int height, EWindowState state )
    : base()
    , Width( width )
    , Height( height )
    , State( state )
    {}

    // The new width of the window
    int Width;
    // The new height of the window.
    int Height;
    // If the window was minimized or maximized.
    EWindowState State;
};
using ResizeEvent = CommonDelegate<void(ResizeEventArgs&)>;

/**
 * Generic user event args.
 */
class UserEventArgs : public EventArgs
{
public:
    using base = EventArgs;
    UserEventArgs( int code, void* data1, void* data2 )
    : base()
    , Code( code )
    , Data1( data1 )
    , Data2( data2 )
    {}

    int   Code;
    void* Data1;
    void* Data2;
};
using UserEvent = CommonDelegate<void(UserEventArgs&)>;

/**
 * Used to notify a runtime error occurred.
 */
class RuntimeErrorEventArgs : public EventArgs
{
public:
    using base = EventArgs;

    RuntimeErrorEventArgs( const std::string& errorString, const std::string& compilerError )
    : base()
    , ErrorString( errorString )
    , CompilerError( compilerError )
    {}

    std::string ErrorString;
    std::string CompilerError;
};
using RuntimeErrorEvent = CommonDelegate<void(RuntimeErrorEventArgs&)>;

enum class FileAction
{
    Unknown,    // An unknown action triggered this event. (Should not happen, but
                // I guess it's possible)
    Added,      // A file was added to a directory.
    Removed,    // A file was removed from a directory.
    Modified,   // A file was modified. This might indicate the file's timestamp
                // was modified or another attribute was modified.
    RenameOld,  // The file was renamed and this event stores the previous name.
    RenameNew,  // The file was renamed and this event stores the new name.
};

class FileChangedEventArgs : public EventArgs
{
public:
    using base = EventArgs;

    FileChangedEventArgs( FileAction action, const std::wstring& path )
    : base()
    , Action( action )
    , Path( path )
    {}

    FileAction   Action;  // The action that triggered this event.
    std::wstring Path;    // The file or directory path that was modified.
};
using FileChangeEvent = CommonDelegate<void(FileChangedEventArgs&)>;
