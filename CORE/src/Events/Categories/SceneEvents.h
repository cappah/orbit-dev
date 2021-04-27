#pragma once

#include "Events/Event.h"
#include "Maths/Maths.h"

namespace Orbit {

    // ADD, ADDED ENTITY

    struct AddEntityEvent : public Event {
        FORCEINLINE ~AddEntityEvent() = default;
        FORCEINLINE AddEntityEvent(std::string name = EMPTY_STR): _EntityName(name) { }
        FORCEINLINE std::string GetName() const { return _EntityName; }

    private:
        std::string _EntityName;
    };

    struct EntityAddedEvent : public Event {
        FORCEINLINE ~EntityAddedEvent() = default;
        FORCEINLINE EntityAddedEvent(ECS::Entity entity) : _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

    private:
        ECS::Entity _Entity;
    };

    // DESTROY, DETROYED ENTITY

    struct DestroyEntityEvent : public Event {
        FORCEINLINE ~DestroyEntityEvent() = default;
        FORCEINLINE DestroyEntityEvent(ECS::Entity entity) : _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

    private:
        ECS::Entity _Entity;
    };

    struct EntityDestroyedEvent : public Event {
        FORCEINLINE ~EntityDestroyedEvent() = default;
        FORCEINLINE EntityDestroyedEvent(ECS::Entity entity) : _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

    private:
        ECS::Entity _Entity;
    };

    // DESABLE, DESABLE ENTITY

    struct DesableEntityEvent : public Event {
        FORCEINLINE ~DesableEntityEvent() = default;
        FORCEINLINE DesableEntityEvent() = default;
    };

    struct EntityDesabledEvent : public Event {
        FORCEINLINE ~EntityDesabledEvent() = default;
        FORCEINLINE EntityDesabledEvent(ECS::Entity entity) : _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

    private:
        ECS::Entity _Entity;
    };

    // ENABLE, ENABLED ENTITY

    struct EnableEntityEvent : public Event {
        FORCEINLINE ~EnableEntityEvent() = default;
        FORCEINLINE EnableEntityEvent() = default;
    };

    struct EntityEnabledEvent : public Event {
        FORCEINLINE ~EntityEnabledEvent() = default;
        FORCEINLINE EntityEnabledEvent(ECS::Entity entity) : _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

    private:
        ECS::Entity _Entity;
    };

    // ADD, REMOVE COMPONENT

    struct AddComponentEvent : public Event {
        FORCEINLINE ~AddComponentEvent() = default;
        FORCEINLINE AddComponentEvent(ECS::Entity entity, std::string typeName) : 
            _CompTypeName(typeName), _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

        FORCEINLINE std::string GetTypeName() const { return _CompTypeName; }

    private:
        ECS::Entity _Entity;
        std::string _CompTypeName;
    };

    struct ComponentAddedEvent : public Event {
        FORCEINLINE ~ComponentAddedEvent() = default;
        FORCEINLINE ComponentAddedEvent(ECS::Entity entity, std::string typeName) :
            _CompTypeName(typeName), _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

        FORCEINLINE std::string GetTypeName() const { return _CompTypeName; }

    private:
        ECS::Entity _Entity;
        std::string _CompTypeName;
    };

    struct RemoveComponentEvent : public Event {
        FORCEINLINE ~RemoveComponentEvent() = default;
        FORCEINLINE RemoveComponentEvent(const ECS::Entity& entity, const RuntimeType type) : 
            _ComponentTypeID(type), _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

        FORCEINLINE const RuntimeType GetTypeID() const { return _ComponentTypeID; }

    private:
        ECS::Entity _Entity;
        RuntimeType _ComponentTypeID;
    };
}