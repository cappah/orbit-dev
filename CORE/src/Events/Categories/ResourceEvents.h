#pragma once

#include "Events/Event.h"

namespace Orbit {

    enum class ResourceType { TEXTURE = 0, CUBEMAP, HDR, MESH };
    enum class ResourceAction { LOADED = 0, DELETED, MODIFIED };

    struct ResourceEvent : public Event {
        FORCEINLINE ~ResourceEvent() = default;
        FORCEINLINE ResourceEvent(std::string name, ResourceType type, ResourceAction action) :
            _Name(name), _Type(type), _Action(action) { }

        FORCEINLINE std::string GetName() const { return _Name; }
        FORCEINLINE ResourceType GetType() const { return _Type; }
        FORCEINLINE ResourceAction GetAction() const { return _Action; }

    private:
        std::string _Name;
        ResourceType _Type;
        ResourceAction _Action;
    };
}