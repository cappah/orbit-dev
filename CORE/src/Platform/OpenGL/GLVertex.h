#pragma once

#include "Maths/Maths.h"
#define MAX_BONE_WEIGHTS 4

namespace Orbit {
	struct GLVertex {
		GLVertex() = default;
		~GLVertex() = default;
		GLVertex(const GLVertex&) = default;

		vec3f Position = vec3f(0);
		vec3f Normal = vec3f(0);
		vec3f Tangent = vec3f(0);
		vec3f Bitangent = vec3f(0);
		vec2f Texcoords = vec2f(0);
	};
}
