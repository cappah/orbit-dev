#pragma once

#include "Resource/Resources.h"


namespace Orbit::ECS {
	static Model* defaultmesh = new Model();
	static Material* defaultmtl = new Material();

	struct MeshRenderer {
		FORCEINLINE MeshRenderer() = default;
		FORCEINLINE ~MeshRenderer() = default;
		FORCEINLINE MeshRenderer(const MeshRenderer&) = default;
		FORCEINLINE MeshRenderer(Model* mesh): MeshData(mesh) { }
		FORCEINLINE MeshRenderer(Model* mesh, Material* mtl) : MeshData(mesh), Matrial(mtl) { }

		Model* MeshData = defaultmesh;
		Material* Matrial = defaultmtl;
	};
}
