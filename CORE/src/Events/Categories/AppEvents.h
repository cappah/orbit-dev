#pragma once

#include "Events/Event.h"

namespace  Orbit {
    struct WindowResizeEvent : public Event {    
        ~WindowResizeEvent() = default;
        WindowResizeEvent(uint32 width, uint32 height) :
            _width(width), _height(height) { }

        FORCEINLINE uint32 Width() const { return _width; }
        FORCEINLINE uint32 Height() const { return _height; }

    private:
        uint32 _width, _height;
    };

    struct WindowCloseEvent : public Event {
        WindowCloseEvent() = default;
        ~WindowCloseEvent() = default;
    };
}