#include "pch.h"
#include "RuntimeType.h"

namespace Orbit {
	static RuntimeType s_Type = 1u;
	const RuntimeType UniqueRuntimeType() { return s_Type++; }
}