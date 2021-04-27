#pragma once

#include "Core/Common.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLM/ext/matrix_clip_space.hpp>

namespace Orbit {

	using vec2f = glm::fvec2;
	using vec3f = glm::fvec3;
	using vec4f = glm::fvec4;

	using vec2i = glm::ivec2;
	using vec3i = glm::ivec3;
	using vec4i = glm::ivec4;

	using vec2u = glm::uvec2;
	using vec3u = glm::uvec3;
	using vec4u = glm::uvec4;

	using matrix4 = glm::mat4;
	using matrix3 = glm::mat3;
	using quaternion = glm::quat;

	#define VAL_PTR glm::value_ptr	

	constexpr vec3f AXIS_X =	vec3f(1, 0, 0);
	constexpr vec3f AXIS_Y =	vec3f(0, 1, 0);
	constexpr vec3f AXIS_Z =	vec3f(0, 0, 1);

	constexpr vec3f RIGHT =		vec3f(1, 0, 0);
	constexpr vec3f FRONT =		vec3f(0, 0,-1);
	constexpr vec3f ORIGIN =	vec3f(0, 0, 0);
	constexpr vec3f WOLRD_UP =	vec3f(0, 1, 0);

	namespace Maths {				
		FORCEINLINE static double Cos(double angle) { return glm::cos(angle); }
		FORCEINLINE static double Sin(double angle) { return glm::sin(angle); }
		FORCEINLINE static float Radians(float angle) { return glm::radians(angle); }		
		FORCEINLINE static const vec3f& Radians(const vec3f& v) { return glm::radians(v); }
		FORCEINLINE static const vec3f& Degrees(const vec3f& v) { return glm::degrees(v); }
	}
}