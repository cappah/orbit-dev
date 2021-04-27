#pragma once

#include "Callback.h"

namespace Orbit {
	
	using CallbackStack = std::vector<Unique<ICallback>>;

	class EventDispatcher {
	public:
		FORCEINLINE EventDispatcher() = default;

		FORCEINLINE ~EventDispatcher() {
			_RegisteredEventCallbacks.clear();
			_TriggeredEvents.clear();
		}

		FORCEINLINE void Process() {
			if (_TriggeredEvents.size()) {
				for (auto& [type, event] : _TriggeredEvents) {
					for (auto& callback : _RegisteredEventCallbacks[type]) {
						callback->Call(*event.get());
					}
				}

				_TriggeredEvents.clear();
			}
		}

		template<typename T, typename... Args>
		FORCEINLINE void Post(Args&&... args) {
			STATIC_ASSERT(std::is_base_of<Event, T>::value && "invalid template type");
			static const RuntimeType type = TypeOf<T>();
			_TriggeredEvents[type] = std::move(MakeUnique<T>(std::forward<Args>(args)...));
		}

		template<typename T, typename... Args>
		FORCEINLINE void Prioritize(Args&&... args) {
			STATIC_ASSERT(std::is_base_of<Event, T>::value && "invalid template type");
			static const RuntimeType type = TypeOf<T>();
			for (auto& callback : _RegisteredEventCallbacks[type]) {
				callback->Call(T(std::forward<Args>(args)...));
			}
		}

		template<typename T>
		FORCEINLINE void AddListener(CallbackFunc<T>&& func) {
			STATIC_ASSERT(std::is_base_of<Event, T>::value && "invalid template type");
			static const RuntimeType type = TypeOf<T>();
			_RegisteredEventCallbacks[type].push_back(std::move(MakeUnique<Callback<T>>(func)));
		}

	private:
		std::unordered_map<RuntimeType, Unique<Event>> _TriggeredEvents;
		std::unordered_map<RuntimeType, CallbackStack> _RegisteredEventCallbacks;
	};
}