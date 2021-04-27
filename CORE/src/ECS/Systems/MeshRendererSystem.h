#pragma once

#include "ECS/ISystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/MeshRenderer.h"

namespace Orbit::ECS {

	constexpr float OUTLINE_STROKE = 0.05f;

	class MeshRendererSystem : public ISystem {
	public:
		FORCEINLINE ~MeshRendererSystem() = default;

		FORCEINLINE MeshRendererSystem() = default;

		FORCEINLINE void Init() {
			_PbrShader = GLShader(_Loader->GetShader("PBR").Program);		
			_FlatColorShader = GLShader(_Loader->GetShader("FLAT").Program);
			_Dispatcher->AddListener<EntitySelectedEvent>(OB_BIND_FN(MeshRendererSystem::OnEntitySelected));
		}

		FORCEINLINE void Start() { }

		FORCEINLINE void Update() {		
			_Scene->Renderer()->GetFrameBuffer()->Bind();
			{
				_PbrShader.Bind();
				_PbrShader.SetSample2D("u_ShadowMap", _Scene->Renderer()->GetDepthBuffer()->GetDepthMap(), 8);

				for (auto& entity : _Scene->GetEntityList<MeshRenderer>()) {
					auto& renderer = entity.GetComponent<MeshRenderer>();
					auto& transform = entity.GetComponent<Transform>();

					renderer.Matrial->Data.SetUniform(_PbrShader);
					transform.SetUniform(_PbrShader);
					renderer.MeshData->Data.Render();

					/*if (_SelectedEntity != entity) {
						renderer.Material->SetUniform(_PbrShader);
						transform.SetUniform(_PbrShader);
						renderer.Mesh.Render();
						continue;
					}
					else {
						Highlight(renderer, transform);
					}*/
				}
			}
			_Scene->Renderer()->GetFrameBuffer()->UnBind();
		}

	private:
		FORCEINLINE void Highlight(MeshRenderer& renderer, Transform transform) {
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			{
				renderer.Matrial->Data.SetUniform(_PbrShader);
				transform.SetUniform(_PbrShader);
				renderer.MeshData->Data.Render();
			}
			
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glDisable(GL_DEPTH_TEST);
			{
				transform.Scale += OUTLINE_STROKE;
				transform.SetUniform(_FlatColorShader);
				renderer.MeshData->Data.Render();
			}		
			glEnable(GL_DEPTH_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}

		FORCEINLINE void OnEntitySelected(const EntitySelectedEvent& e) {
			_SelectedEntity = e.GetEntity();
		}

	private:
		GLShader _PbrShader;			
		Entity _SelectedEntity;
		GLShader _FlatColorShader;
	};
}