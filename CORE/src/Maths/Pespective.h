#pragma once

#include "Maths.h"
#include "Platform/OpenGL/GLShader.h"

namespace Orbit {
	struct Pespective {
		FORCEINLINE Pespective() = default;
		FORCEINLINE ~Pespective() = default;
		FORCEINLINE Pespective(const Pespective&) = default;

		vec3f Eye = vec3f(0.0);
		float NearPlane = 1.0f;
		float FarPlane = 20.0f;
		float FieldOfView = 45.0f;
		float AspectRatio = 16.0f/9.0f;

		FORCEINLINE matrix4 GetMatrix(vec3f eye) {
			return glm::lookAt(eye, ORIGIN, WOLRD_UP) * glm::perspective(Maths::Radians(FieldOfView), AspectRatio, NearPlane, FarPlane);
		}
	};
}