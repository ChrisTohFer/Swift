#include "input.h"

void SWIFT::INPUT::update(KEY_ARRAY<KEY_UPDATE>& updates)
{
    for (auto i = 0u; i < KEY_COUNT; ++i)
    {
        //Set each element of key_status
        auto& status = m_key_status[i];
        auto& update = updates[i];

        //In any one frame, key can only be pressed or released, not both
        //If changed_since_last_frame, we must always set either released or pressed (depending on held)
        //Otherwise, just check whether held matches update
        status.released = status.held  && (update.changed_since_last_frame || !update.held);
        status.pressed  = !status.held && (update.changed_since_last_frame || update.held);
        status.held     = status.pressed || (status.held && !status.released);

        status.alt     = status.pressed && update.alt     || !status.pressed && status.alt;
        status.control = status.pressed && update.control || !status.pressed && status.control;
        status.shift   = status.pressed && update.shift   || !status.pressed && status.shift;
        status.system  = status.pressed && update.system  || !status.pressed && status.system;

        update.changed_since_last_frame = false;

        //TODO - Write this with branching and compare assembly

        if (status.pressed || status.released)
            m_key_event.invoke(KEY(i), status);
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
