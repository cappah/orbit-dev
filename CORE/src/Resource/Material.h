#pragma once

#include "Platform/OpenGL/GLMaterial.h"

namespace Orbit {
	struct Material {
		GLMaterial Data;
		std::string Name = EMPTY_STR;
		std::string Path = EMPTY_STR;
		FORCEINLINE operator GLMaterial() const { return Data; }
	};
}
