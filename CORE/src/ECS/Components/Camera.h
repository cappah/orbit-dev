#pragma once

#include "Platform/OpenGL/GLCamera.h"

namespace Orbit::ECS {
	struct Camera {
		FORCEINLINE Camera() = default;
		FORCEINLINE ~Camera() = default;
		FORCEINLINE Camera(const Camera&) = default;
		GLCamera Handle;
		bool IsPrimary = true;
	};
}