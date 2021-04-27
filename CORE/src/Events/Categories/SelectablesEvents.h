#pragma once

#include "Events/Event.h"
#include "Resource/Resources.h"

namespace Orbit {

    //enum class SelectableAction { SELECT = 0, REMOVE, MOVE };
    //enum class SelectableType { TEXTURE = 0, ENTITY, MODEL, SCRIPT };

    //struct SelectableEvent: public Event {
    //    FORCEINLINE SelectableEvent(SelectableType type, SelectableAction action) : _Action(action), _Type(type) { }

    //    FORCEINLINE SelectableType GetType() const { return _Type; }
    //    FORCEINLINE SelectableAction GetAction() const { return _Action; }

    //private:
    //    SelectableType _Type;
    //    SelectableAction _Action;
    //};


    struct MaterialTextureSelectedEvent : public Event {
        FORCEINLINE MaterialTextureSelectedEvent(const Texture& texture) : _Texture(texture) { }

        FORCEINLINE const Texture& GetTexture() const { return _Texture; }

    private:
        Texture _Texture;
    };
}