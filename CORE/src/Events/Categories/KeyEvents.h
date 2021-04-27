#pragma once

#include "Events/Event.h"

namespace Orbit {

    // key pressed event
    struct KeyEvent : public Event {
        virtual ~KeyEvent() = default;

        FORCEINLINE KeyEvent(int32 key) : _Key(key) { }

        FORCEINLINE int KeyCode() const { return _Key; }

    protected:
        int32 _Key;
    };

    // key pressed event
    struct KeyPressedEvent : public KeyEvent {
        KeyPressedEvent(int32 key) : KeyEvent(key) { }
        ~KeyPressedEvent() = default;
    };

    // key realesed event
    struct KeyReleasedEvent : public KeyEvent {
        KeyReleasedEvent(int key): KeyEvent(key) { }
        ~KeyReleasedEvent() = default;
    }; 
    
    // key repeat event
    struct KeyRepeatEvent : public KeyEvent {
        KeyRepeatEvent(int key): KeyEvent(key) {}
        ~KeyRepeatEvent() = default;
    };
}