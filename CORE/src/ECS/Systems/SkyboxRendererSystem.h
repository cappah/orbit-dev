#pragma

#include "ECS/ISystem.h"
#include "ECS/Components/SkyboxRenderer.h"
#include "Events/Categories/CameraEvents.h"

namespace Orbit::ECS {
	class SkyboxRendererSystem : public ISystem {
	public:
		FORCEINLINE void Init() {
			_PbrShader = GLShader(_Loader->GetShader("PBR").Program);
			_SkyboxShader = GLShader(_Loader->GetShader("SKYBOX").Program);
			_IrradianceShader = GLShader(_Loader->GetShader("IRMAP").Program);
			_HdrToCubemapShader = GLShader(_Loader->GetShader("HDR2MAP").Program);
		}	
		
		void Start() {
			
		}

		void Update() {		
			_Scene->Renderer()->GetFrameBuffer()->Bind();

			for (auto& skybox : _Scene->GetEntityList<SkyboxRenderer>()) {
				auto& list = _Scene->GetEntityList<SkyboxRenderer>();
				auto& renderer = skybox.GetComponent<SkyboxRenderer>();
				uint32 cubemap = _Scene->Loader()->GetCubeMap("CubeMap_0");

				_PbrShader.Bind();
				_PbrShader.SetSampleCube("u_IrradianceCubeMap", cubemap, 6);
				_PbrShader.SetSampleCube("u_SpecularIrradianceCubeMap", cubemap, 7);

				_SkyboxShader.Bind();
				_SkyboxShader.SetSampleCube("u_SkyBoxCubeMap", cubemap);

				glDepthMask(GL_FALSE);
				glDepthFunc(GL_LEQUAL);
				renderer.Mesh.Render();
				glDepthFunc(GL_LESS);
				glDepthMask(GL_TRUE);
			}
			
			_Scene->Renderer()->GetFrameBuffer()->UnBind();
		}

	private:
		uint32 _IrradianceMap = 0;
		uint32 _EnvironmentMap = 0;

		GLShader _PbrShader;
		GLShader _SkyboxShader;
		GLShader _IrradianceShader;
		GLShader _HdrToCubemapShader;
	};
}