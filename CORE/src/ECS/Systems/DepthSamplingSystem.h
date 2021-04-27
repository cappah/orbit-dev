#pragma once

#include "ECS/ISystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/MeshRenderer.h"

namespace Orbit::ECS {
	class DepthSamplingSystem : public ISystem {
	public:
		FORCEINLINE void Init() {
			_Shader = GLShader(_Loader->GetShader("DEPTH").Program);
		}

		FORCEINLINE void Start() {}

		void Update() {		
			_Scene->Renderer()->GetDepthBuffer()->Clear();

			glCullFace(GL_FRONT);
			for (auto& entity : _Scene->GetEntityList<MeshRenderer>()) {
				entity.GetComponent<Transform>().SetUniform(_Shader);
				entity.GetComponent<MeshRenderer>().MeshData->Data.Render();
			}
			glCullFace(GL_BACK);

			_Scene->Renderer()->GetDepthBuffer()->UnBind();
		}

	private:
		GLShader _Shader;
	};
}