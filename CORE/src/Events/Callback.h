#pragma once

#include "Event.h"

namespace Orbit {

	class ICallback;
	template<typename T>
	using CallbackFunc = std::function<void(const T&)>;

	class ICallback {
	public:
		ICallback() = default;
		virtual ~ICallback() = default;
		virtual void Call(const Event& e) = 0;
	};

	template<typename T>
	class Callback : public ICallback {
	public:
		FORCEINLINE ~Callback() = default;

		FORCEINLINE Callback(CallbackFunc<T> func) : _Func(func) { }

		FORCEINLINE void Call(const Event& e) {
			_Func(CastEvent<T>(e));
		}

	private:
		CallbackFunc<T> _Func;
	};
}