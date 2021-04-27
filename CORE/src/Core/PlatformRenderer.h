#pragma once

#include "Platform/OpenGL/GLRenderer.h"

namespace Orbit {
	#ifdef OPENGL_RENDERER_API
		using PlatformRenderer = GLRenderer;
	#endif 
}
