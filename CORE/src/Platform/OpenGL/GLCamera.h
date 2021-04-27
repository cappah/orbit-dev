#pragma once

#include "GLShader.h"

namespace Orbit {
	struct GLCamera {
		GLCamera() = default;
		~GLCamera() = default;

		vec3f Location = vec3f(0, 3, 5);
		vec3f Orientation = vec3f(0);

		float NearPlane = 0.3f;
		float FarPlane = 1000.0f;
		float FieldOfView = 45.0f;
		float AspectRatio = 16.0f/9.0f;

		FORCEINLINE matrix4 View() {
			return glm::lookAt(Location, (Location + FRONT), WOLRD_UP) * glm::toMat4(quaternion(Orientation));
		}

		FORCEINLINE matrix4 Projection() {
			return glm::perspective(FieldOfView, AspectRatio, NearPlane, FarPlane);
		}

		FORCEINLINE void SetUniform(GLShader& shader) {
			shader.Bind();
			shader.SetV3f("u_CameraPosition", Location);
			shader.SetMat4f("u_View", VAL_PTR(View()));
			shader.SetMat4f("u_Projection", VAL_PTR(Projection()));
		}
	};
}