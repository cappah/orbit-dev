#pragma once

#include "Maths.h"
#include "Platform/OpenGL/GLShader.h"

namespace Orbit {
	struct Ortho {
		FORCEINLINE Ortho() = default;
		FORCEINLINE ~Ortho() = default;
		FORCEINLINE Ortho(const Ortho&) = default;

		float Width = 10.0f;
		float Height = 10.0f;
		float FarPlane = 7.5f;
		float NearPlane = 1.0f;

		FORCEINLINE matrix4 GetMatrix(vec3f eye) {
			matrix4 view = glm::lookAt(eye, ORIGIN, WOLRD_UP);
			matrix4 proj = glm::ortho(-Width, Width, -Height, Height, NearPlane, FarPlane);
			return view * proj;
		}
	};
}