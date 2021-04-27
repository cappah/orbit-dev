#pragma once

#include "Maths/Maths.h"
#include "Platform/OpenGL/GLShader.h"

namespace Orbit::ECS {
	struct Transform  {
		FORCEINLINE Transform() = default;
		FORCEINLINE ~Transform() = default;
		FORCEINLINE Transform(const Transform&) = default;		

		FORCEINLINE Transform(float x, float y, float z) : Position(x, y, z) { }

		FORCEINLINE Transform(const vec3f& pos, const vec3f& scale, const vec3f& rotation) :
			Position(pos), Scale(scale), Rotation(rotation) {}

		FORCEINLINE Transform(const vec3f& pos, const vec3f& scale) :
			Position(pos), Scale(scale) {}

		FORCEINLINE Transform(const vec3f& pos) :
			Position(pos) {}

		FORCEINLINE matrix4 TrMatrix() {
			return (glm::translate(matrix4(1.0f), Position) * 
				glm::toMat4(quaternion(Maths::Radians(Rotation))) *
				glm::scale(matrix4(1.0f), Scale));
		}

		FORCEINLINE void SetUniform(GLShader& shader) {
			shader.Bind();
			shader.SetMat4f("u_Model", VAL_PTR(TrMatrix()));
		}

		vec3f Position = vec3f(0.0f);
		vec3f Rotation = vec3f(0.0f);
		vec3f Scale = vec3f(1.0f);
	};
}