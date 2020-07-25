#pragma once

#include "Types/event.h"
#include "Types/vector.h"

#include <array>
#include <vector>

namespace SWIFT
{
    //values copied from SFML for convenience
    enum class KEY
    {
        Unknown = -1, ///< Unhandled key
        A = 0,        ///< The A key
        B,            ///< The B key
        C,            ///< The C key
        D,            ///< The D key
        E,            ///< The E key
        F,            ///< The F key
        G,            ///< The G key
        H,            ///< The H key
        I,            ///< The I key
        J,            ///< The J key
        K,            ///< The K key
        L,            ///< The L key
        M,            ///< The M key
        N,            ///< The N key
        O,            ///< The O key
        P,            ///< The P key
        Q,            ///< The Q key
        R,            ///< The R key
        S,            ///< The S key
        T,            ///< The T key
        U,            ///< The U key
        V,            ///< The V key
        W,            ///< The W key
        X,            ///< The X key
        Y,            ///< The Y key
        Z,            ///< The Z key
        Num0,         ///< The 0 key
        Num1,         ///< The 1 key
        Num2,         ///< The 2 key
        Num3,         ///< The 3 key
        Num4,         ///< The 4 key
        Num5,         ///< The 5 key
        Num6,         ///< The 6 key
        Num7,         ///< The 7 key
        Num8,         ///< The 8 key
        Num9,         ///< The 9 key
        Escape,       ///< The Escape key
        LControl,     ///< The left Control key
        LShift,       ///< The left Shift key
        LAlt,         ///< The left Alt key
        LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        RControl,     ///< The right Control key
        RShift,       ///< The right Shift key
        RAlt,         ///< The right Alt key
        RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        Menu,         ///< The Menu key
        LBracket,     ///< The [ key
        RBracket,     ///< The ] key
        Semicolon,    ///< The ; key
        Comma,        ///< The , key
        Period,       ///< The . key
        Quote,        ///< The ' key
        Slash,        ///< The / key
        Backslash,    ///< The \ key
        Tilde,        ///< The ~ key
        Equal,        ///< The = key
        Hyphen,       ///< The - key (hyphen)
        Space,        ///< The Space key
        Enter,        ///< The Enter/Return keys
        Backspace,    ///< The Backspace key
        Tab,          ///< The Tabulation key
        PageUp,       ///< The Page up key
        PageDown,     ///< The Page down key
        End,          ///< The End key
        Home,         ///< The Home key
        Insert,       ///< The Insert key
        Delete,       ///< The Delete key
        Add,          ///< The + key
        Subtract,     ///< The - key (minus, usually from numpad)
        Multiply,     ///< The * key
        Divide,       ///< The / key
        Left,         ///< Left arrow
        Right,        ///< Right arrow
        Up,           ///< Up arrow
        Down,         ///< Down arrow
        Numpad0,      ///< The numpad 0 key
        Numpad1,      ///< The numpad 1 key
        Numpad2,      ///< The numpad 2 key
        Numpad3,      ///< The numpad 3 key
        Numpad4,      ///< The numpad 4 key
        Numpad5,      ///< The numpad 5 key
        Numpad6,      ///< The numpad 6 key
        Numpad7,      ///< The numpad 7 key
        Numpad8,      ///< The numpad 8 key
        Numpad9,      ///< The numpad 9 key
        F1,           ///< The F1 key
        F2,           ///< The F2 key
        F3,           ///< The F3 key
        F4,           ///< The F4 key
        F5,           ///< The F5 key
        F6,           ///< The F6 key
        F7,           ///< The F7 key
        F8,           ///< The F8 key
        F9,           ///< The F9 key
        F10,          ///< The F10 key
        F11,          ///< The F11 key
        F12,          ///< The F12 key
        F13,          ///< The F13 key
        F14,          ///< The F14 key
        F15,          ///< The F15 key
        Pause,        ///< The Pause key

        KeyCount     ///< Keep last -- the total number of keyboard keys
    };

    const inline size_t KEY_COUNT = static_cast<int>(KEY::KeyCount);

    enum class MOUSE_BUTTON
    {
        LEFT,
        RIGHT,
        MIDDLE,
        BACK,
        FORWARD,

        COUNT
    };

    const inline size_t MOUSE_BUTTON_COUNT = static_cast<int>(MOUSE_BUTTON::COUNT);

    //Convenient way to create arrays with an element for each key
    template<typename KEY_TYPE>
    using KEY_ARRAY = std::array<KEY_TYPE, KEY_COUNT>;

    //Contains update information for a specific key
    struct KEY_UPDATE
    {
        bool   alt                      = false;
        bool   control                  = false;
        bool   shift                    = false;
        bool   system                   = false;

        bool   held                     = false;
        bool   changed_since_last_frame = false;    //This is to catch when a key is pressed and released on a single frame (mostly in case the game chugs)
    };

    struct MOUSE_EVENT
    {
        VECTOR2F pos()
        {
            return VECTOR2F(float(posX), float(posY));
        }

        MOUSE_BUTTON button;
        int posX;
        int posY;
        bool pressed;   //Pressed or released
    };

    struct MOUSE_UPDATE
    {
        bool   in_window = false;
        int    posX;
        int    posY;
        int    wheelScroll;

        std::vector<MOUSE_EVENT> events;
    };

    class INPUT
    {
    public:
        //Holds the status of a specific key
        struct KEY_STATUS
        {
            bool   alt      = false;
            bool   control  = false;
            bool   shift    = false;
            bool   system   = false;

            bool   held     = false;
            bool   pressed  = false;
            bool   released = false;
        };

        struct MOUSE_STATE
        {
            bool   in_window   = false;
            int    posX        = 0;
            int    posY        = 0;
            int    wheelScroll = 0;
        };

        using KEY_LISTENER   = EVENT<KEY, KEY_STATUS const&>::LISTENER;
        using MOUSE_LISTENER = EVENT<MOUSE_EVENT>::LISTENER;

        //Call once per main loop cycle, update status of all keys
        void update(MOUSE_UPDATE&, KEY_ARRAY<KEY_UPDATE>&);

        //Methods to check key status
        //Single key presses are distinguished from ctrl + key or shift + key etc with boolean flags
        bool key_held(KEY, bool = false, bool = false, bool = false, bool = false) const;
        bool key_pressed(KEY, bool = false, bool = false, bool = false, bool = false) const;
        bool key_released(KEY, bool = false, bool = false, bool = false, bool = false) const;

        void listen_for_keys(KEY_LISTENER&);
        void stop_listening_for_keys(KEY_LISTENER&);
        void listen_for_mouse(MOUSE_LISTENER&);
        void stop_listening_for_mouse(MOUSE_LISTENER&);

    private:
        MOUSE_STATE                       m_mouse_state;
        EVENT<MOUSE_EVENT>                m_mouse_event;

        KEY_ARRAY<KEY_STATUS>             m_key_status;
        EVENT<KEY, KEY_STATUS const&>     m_key_event;
    };
}