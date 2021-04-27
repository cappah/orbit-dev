#pragma once

#include "Inputs/KeyCodes.h"
#include "Inputs/MouseCodes.h"
#include "Platform/GLFW/GLFWInputs.h"

namespace Orbit {
	#ifdef PLATFORM_WINDOWS
		using PlatformInputs = GLFWInputs;
	#endif 
}