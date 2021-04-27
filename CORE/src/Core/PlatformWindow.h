#pragma once

#include "Platform/GLFW/GLFWWindow.h"	

namespace Orbit {
	#ifdef PLATFORM_WINDOWS
		using PlatformWindow = GLFW_Window;
	#endif
}