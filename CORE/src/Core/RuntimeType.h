#pragma once

#include "Common.h"

/// <summary>
/// This will generate a static runtime type for each
/// type passed as template argument.
/// </summary>

namespace Orbit {
	using RuntimeType = uint32;
	const RuntimeType INVALID_RUNTIME_TYPE = 0;

	const RuntimeType UniqueRuntimeType();

	template<typename T>
	FORCEINLINE constexpr RuntimeType TypeOf() noexcept {
		static const RuntimeType s_RuntimeType = UniqueRuntimeType();		
		return s_RuntimeType;
	}
}