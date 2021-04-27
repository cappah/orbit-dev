#pragma once

#include "Events/Event.h"

namespace Orbit {
    struct CameraEvent : public Event {
        FORCEINLINE ~CameraEvent() = default;
        FORCEINLINE CameraEvent(matrix4 view, matrix4 proj) : _View(view), _Projection(proj) { }
        FORCEINLINE matrix4 GetProjection() const { return _Projection; }
        FORCEINLINE matrix4 GetView() const { return _View; }

    private:
        matrix4 _View;
        matrix4 _Projection;
    };

    struct CameraZoomEvent : public Event {
        FORCEINLINE ~CameraZoomEvent() = default;
        FORCEINLINE CameraZoomEvent(double dz) : _deltaZ(dz) { }
        FORCEINLINE double DeltaZ() const { return _deltaZ; }

    private:
        double _deltaZ;
    };

    struct CameraMovedEvent : public Event {
        FORCEINLINE ~CameraMovedEvent() = default;
        FORCEINLINE CameraMovedEvent(double dx, double dy) : _deltaX(dx), _deltaY(dy) { }

        FORCEINLINE double DeltaX() const { return _deltaX; }
        FORCEINLINE double DeltaY() const { return _deltaY; }

    private:
        double _deltaX, _deltaY;
    };

    struct CameraFocusEvent : public Event {
        FORCEINLINE ~CameraFocusEvent() = default;
        FORCEINLINE CameraFocusEvent(vec3f target) : _target(target) { }

        FORCEINLINE vec3f Target() const { return _target; }

    private:
        vec3f _target;
    };
}
