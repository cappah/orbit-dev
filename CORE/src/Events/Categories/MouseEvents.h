#pragma once

#include"Events/Event.h"

namespace Orbit {

    // mouse events
    struct MouseEvent : public Event {
        virtual ~MouseEvent() = default;
        MouseEvent(const MouseData& d) :
            _posX(d.posX), _posY(d.posY), 
            _deltaX(d.deltaX), _deltaY(d.deltaY), 
            _scrollX(d.scrollX), _scrollY(d.scrollY) { }

        FORCEINLINE double PosX() const { return _posX; }
        FORCEINLINE double PosY() const { return _posY; }

        FORCEINLINE double DeltaX() const { return _deltaX; }
        FORCEINLINE double DeltaY() const { return _deltaY; }

        FORCEINLINE double ScrollX() const { return _scrollX; }
        FORCEINLINE double ScrollY() const { return _scrollY; }

    protected:
        double _posX, _posY;
        double _deltaX, _deltaY;
        double _scrollX, _scrollY;
    };

    // down event
    struct MouseDownEvent : public MouseEvent {
        ~MouseDownEvent() = default;
        MouseDownEvent(const MouseData& data) : MouseEvent(data) { }
    };

    // up event
    struct MouseReleaseEvent : public MouseEvent {
        ~MouseReleaseEvent() = default;
        MouseReleaseEvent(const MouseData& data) : MouseEvent(data) { }
    };

    // motion event
    struct MotionEvent : public MouseEvent {
        ~MotionEvent() = default;
        MotionEvent(const MouseData& data) : MouseEvent(data) { }
    };

    // motion event
    struct MouseDragEvent : public MouseEvent {
        ~MouseDragEvent() = default;
        MouseDragEvent(const MouseData& data) : MouseEvent(data) { }
    };

    // scroll event
    struct MouseScrollEvent : public MouseEvent {
        ~MouseScrollEvent() = default;
        MouseScrollEvent(const MouseData& data) : MouseEvent(data) { }
    };
}