#include "input.h"

void SWIFT::INPUT::update(MOUSE_UPDATE& mouse_updates, KEY_ARRAY<KEY_UPDATE>& key_updates)
{
    //Mouse
    m_mouse_state.in_window   = mouse_updates.in_window;
    m_mouse_state.posX        = mouse_updates.posX;
    m_mouse_state.posY        = mouse_updates.posY;
    m_mouse_state.wheelScroll = mouse_updates.wheelScroll;
    mouse_updates.wheelScroll = 0;

    for (auto& mouseEvent : mouse_updates.events)
    {
        m_mouse_event.invoke(mouseEvent);
    }
    mouse_updates.events.clear();

    //Keyboard
    for (auto i = 0u; i < KEY_COUNT; ++i)
    {
        //Set each element of key_status
        auto& key_status = m_key_status[i];
        auto& update = key_updates[i];

        //In any one frame, key can only be pressed or released, not both
        //If changed_since_last_frame, we must always set either released or pressed (depending on held)
        //Otherwise, just check whether held matches update
        key_status.released = key_status.held  && (update.changed_since_last_frame || !update.held);
        key_status.pressed  = !key_status.held && (update.changed_since_last_frame || update.held);
        key_status.held     = key_status.pressed || (key_status.held && !key_status.released);

        key_status.alt     = key_status.pressed && update.alt     || !key_status.pressed && key_status.alt;
        key_status.control = key_status.pressed && update.control || !key_status.pressed && key_status.control;
        key_status.shift   = key_status.pressed && update.shift   || !key_status.pressed && key_status.shift;
        key_status.system  = key_status.pressed && update.system  || !key_status.pressed && key_status.system;

        update.changed_since_last_frame = false;

        //TODO - Write this with branching and compare assembly

        if (key_status.pressed || key_status.released)
            m_key_event.invoke(KEY(i), key_status);
    }
}

bool SWIFT::INPUT::key_held(KEY key, bool alt, bool control, bool shift, bool system) const
{
    auto status = m_key_status[static_cast<int>(key)];
    return status.held 
        && status.alt     == alt 
        && status.control == control 
        && status.shift   == shift 
        && status.system  == system;
}

bool SWIFT::INPUT::key_pressed(KEY key, bool alt, bool control, bool shift, bool system) const
{
    auto status = m_key_status[static_cast<int>(key)];
    return status.pressed
        && status.alt == alt
        && status.control == control
        && status.shift == shift
        && status.system == system;
}

bool SWIFT::INPUT::key_released(KEY key, bool alt, bool control, bool shift, bool system) const
{
    auto status = m_key_status[static_cast<int>(key)];
    return status.released
        && status.alt == alt
        && status.control == control
        && status.shift == shift
        && status.system == system;
}

void SWIFT::INPUT::listen_for_keys(KEY_LISTENER& listener)
{
    m_key_event.add_listener(&listener);
}

void SWIFT::INPUT::stop_listening_for_keys(KEY_LISTENER& listener)
{
    m_key_event.remove_listener(&listener);
}

void SWIFT::INPUT::listen_for_mouse(MOUSE_LISTENER& listener)
{
    m_mouse_event.add_listener(&listener);
}

void SWIFT::INPUT::stop_listening_for_mouse(MOUSE_LISTENER& listener)
{
    m_mouse_event.remove_listener(&listener);
}
