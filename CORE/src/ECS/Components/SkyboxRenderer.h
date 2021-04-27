#pragma once

#include "Platform/OpenGL/GLModel.h"

namespace Orbit::ECS {
	struct SkyboxRenderer {
		FORCEINLINE SkyboxRenderer() = default;
		FORCEINLINE ~SkyboxRenderer() = default;
		FORCEINLINE SkyboxRenderer(const SkyboxRenderer&) = default;
		FORCEINLINE SkyboxRenderer(const GLModel& mesh, uint32 cubemap, bool isActive = false) :
			Mesh(mesh), CubeMapID(cubemap), IsActive(true) { }

		GLModel Mesh;
		uint32 CubeMapID = 0;
		bool IsActive = false;
	};
}

