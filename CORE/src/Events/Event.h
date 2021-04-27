#pragma once

#include "Core/RuntimeType.h"

namespace Orbit {
	using KeyCode = int32;
	using MouseButton = int32;
	using Keyboard = std::bitset<1024>;
	using MouseButtonState = std::bitset<7>;

	// struct for mouse data
	struct MouseData {
		double posX = 0;
		double posY = 0;
		double deltaX = 0;
		double deltaY = 0;
		double scrollX = 0;
		double scrollY = 0;
		MouseButtonState buttons = { 0 };
	};

	enum EventState : int { NOT_HANDLED = 0, HANDLED };
	enum EventPriority : int { PRIORITY_NORMAL = 0, PRIORITY_HIGH = 1 };


	struct Event {
		Event() = default;
		virtual ~Event() = default;
	};

	template<typename T>
	FORCEINLINE const T& CastEvent(const Event& e) {
		STATIC_ASSERT(std::is_base_of<Event, T>::value && "invalid template type!");
		return static_cast<const T&>(e);
	}
}