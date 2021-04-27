#pragma once

#include "Maths/Ortho.h"
#include "Maths/Pespective.h"

namespace Orbit::ECS {
	struct DirectionalLight {
		FORCEINLINE DirectionalLight() = default;
		FORCEINLINE ~DirectionalLight() = default;
		FORCEINLINE DirectionalLight(const DirectionalLight&) = default;

		Ortho Projection;
		bool CastShadow = true;

		float Bias = 0.05f;
		float Intensity = 1.0f;
		vec3f Radiance = vec3f(1.0f);
	};
}
