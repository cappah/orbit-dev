#pragma once

#include "Events/Event.h"

namespace Orbit {

    // PLAY / STOP

    struct StartGameEvent : public Event {

        FORCEINLINE ~StartGameEvent() = default;
        FORCEINLINE StartGameEvent(bool mode = false) : _Fullscreen(mode) { }

        FORCEINLINE bool GetMode() const { return _Fullscreen; }

    private:
        bool _Fullscreen;
    };

    struct StopGameEvent : public Event {
        FORCEINLINE StopGameEvent() = default;
        FORCEINLINE ~StopGameEvent() = default;
    };

    // SELECT 

    struct EntitySelectedEvent : public Event {
        FORCEINLINE ~EntitySelectedEvent() = default;
        FORCEINLINE EntitySelectedEvent(const ECS::Entity& entity) : _Entity(entity) { }

        FORCEINLINE ECS::Entity GetEntity() const { return _Entity; }

    private:
        ECS::Entity _Entity;
    };

    struct EntityNameChangedEvent : public Event {
        FORCEINLINE ~EntityNameChangedEvent() = default;
        FORCEINLINE EntityNameChangedEvent(std::string name): _Name(name) { }

        std::string GetValue() const { return _Name; }

    private:
        std::string _Name;
    };

    // INSPEKTOR
    struct ClearInspectorEvent : public Event { };

    struct RefreshInspectorEvent : public Event {
        FORCEINLINE ~RefreshInspectorEvent() = default;
        FORCEINLINE RefreshInspectorEvent(const  ECS::Entity& entity) : _Entity(entity) {}

        FORCEINLINE const ECS::Entity& GetEntity() const { return _Entity; }

    private:
        ECS::Entity _Entity;
    };

    struct TransformControlEvent : public Event {
        FORCEINLINE TransformControlEvent() = default;
        FORCEINLINE ~TransformControlEvent() = default;
    };

    struct RenderMaterialPreviewEvent : public Event {
        FORCEINLINE RenderMaterialPreviewEvent(std::string name, int32 frameWidth, int32 frameHeight) : 
            _MaterialName(name), _FrameWidth(frameWidth), _FrameHeight(frameHeight){ }
        FORCEINLINE ~RenderMaterialPreviewEvent() = default;

        FORCEINLINE int32 GetFrameWidth() const { return _FrameWidth;}
        FORCEINLINE int32 GetFrameHeight() const { return _FrameHeight;}
        FORCEINLINE std::string GetMaterialName() const { return _MaterialName;}

    private:
       std::string _MaterialName;
       int32 _FrameWidth, _FrameHeight;
    };

    // PROJECTS

    struct OpenSceneEvent : public Event {
        FORCEINLINE ~OpenSceneEvent() = default;
        FORCEINLINE OpenSceneEvent(std::string filename) : _Filename(filename) { }
        FORCEINLINE std::string GetFilename() const { return _Filename; }

    private:
        std::string _Filename;
    };

    struct SceneLoadedEvent : public Event { };

    struct NewSceneEvent : public Event {
        FORCEINLINE ~NewSceneEvent() = default;
        FORCEINLINE NewSceneEvent(const char* filename) : _Filename(filename) { }
        FORCEINLINE const char* GetFilename() const { return _Filename; }

    private:
        const char* _Filename;
    };

    struct SaveSceneEvent : public Event {
        FORCEINLINE SaveSceneEvent(std::string filename) : _Filename(filename) { }
        FORCEINLINE std::string GetFilename() const { return _Filename; }

    private:
        std::string _Filename;
    };

    // OTHERS

    struct FrameBufferEvent : public Event {
        FORCEINLINE FrameBufferEvent(uint32 samplerid) : _SampleID(samplerid) { }
        FORCEINLINE ~FrameBufferEvent() = default;
        FORCEINLINE uint32 GetSampleID() const { return _SampleID; }

    private:
       uint32 _SampleID;
    };

    struct SceneResizedEvent : public Event {
        FORCEINLINE ~SceneResizedEvent() = default;
        FORCEINLINE SceneResizedEvent(float w, float h) :
            _Width(w), _Height(h) { }

        FORCEINLINE float Width() const { return _Width; }
        FORCEINLINE float Height() const { return _Height; }

    private:
        float _Width, _Height;
    };

    // CONSOLE EVENT

    struct ConsoleLogEvent : public Event {
        FORCEINLINE ~ConsoleLogEvent() = default;
        FORCEINLINE ConsoleLogEvent(std::string message) : _Message(message) {}

        FORCEINLINE std::string GetLogMessage() const { return _Message; }

    private:
        std::string _Message;
    };
}