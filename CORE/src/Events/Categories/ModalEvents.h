#pragma once

#include "Events/Event.h"

namespace Orbit {

    enum class MaterialTextureType { AO, ALBEDO, NORMAL, ROUGHNESS, METALNESS, SPECBRDF };

    struct ShowMaterialTextureModalEvent : public Event {
        FORCEINLINE ShowMaterialTextureModalEvent(MaterialTextureType type) : _Type(type) { }

        FORCEINLINE ShowMaterialTextureModalEvent GetType() const { return _Type; }

    private:
        MaterialTextureType _Type;
    };
}