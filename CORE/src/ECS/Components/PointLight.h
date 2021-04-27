#pragma once

#include "Maths/Pespective.h"

namespace Orbit::ECS {
	struct PointLight {
		FORCEINLINE PointLight() = default;
		FORCEINLINE ~PointLight() = default;
		FORCEINLINE PointLight(const PointLight&) = default;

		Pespective Projection;
		bool CastShadow = true;

		float Bias = 0.05f;
		float Intensity = 1.0f;
		vec3f Radiance = vec3f(1.0f);
	};
}