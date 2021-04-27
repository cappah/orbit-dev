#pragma once

#include "Events/Event.h"

namespace Orbit {

    enum LogLevel { 
        LOG_LEVEL_INFO = 0,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_CRITICAL
    };

    enum LogAction { 
        LOG_ACTION_NONE = 0,
        LOG_ACTION_SHOW,
        LOG_ACTION_TO_FILE,
        LOG_ACTION_QUIT,
        LOG_ACTION_TO_FILE_AND_QUIT,
    };

    struct LogEvent : Event {
        FORCEINLINE LogEvent(std::string message, LogLevel level, LogAction action) :
            _Level(level), _Action(action), _Message(message) { }

        FORCEINLINE virtual ~LogEvent() = default;

        FORCEINLINE LogLevel GetLevel() const {
            return _Level;
        }

        FORCEINLINE LogAction GetAction() const {
            return _Action;
        }

        FORCEINLINE std::string GetMessage() const {
            return _Message;
        }

    protected:
        LogLevel _Level;
        LogAction _Action;
        std::string _Message;
    };

    struct EditorLogEvent : LogEvent {
        EditorLogEvent(std::string message, LogLevel level, LogAction action) :
            LogEvent(message, level, action) {}
        ~EditorLogEvent() = default;
    };

    struct SystemLogEvent : LogEvent {
        SystemLogEvent(std::string message, LogLevel level, LogAction action) :
            LogEvent(message, level, action) {}
        ~SystemLogEvent() = default;
    };
}