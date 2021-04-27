#pragma once

#include "Common.h"

namespace Orbit {
	FORCEINLINE double GetPlatformTime() {
		#ifdef PLATFORM_WINDOWS
			return glfwGetTime();
		#endif
	}
}