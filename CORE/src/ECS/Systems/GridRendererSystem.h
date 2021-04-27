#pragma once

#include "ECS/ISystem.h"
#include "Platform/OpenGL/GLGrid.h"
#include "ECS/Components/Transform.h"
#include "Events/Categories/CameraEvents.h"

namespace Orbit::ECS {
	class GridRendererSystem : public ISystem {
	public:
		FORCEINLINE void Init() {
			_Shader = GLShader(_Loader->GetShader("GRID").Program);

			ScaleFactor = 100.0f;
			Transform transform;
			transform.Scale = vec3f(ScaleFactor);
			transform.Position = vec3f(-ScaleFactor/2.0f, 0.0, -ScaleFactor/2.0f);
			transform.SetUniform(_Shader);   
			_Grid = MakeUnique<GLGrid>();
		}

		FORCEINLINE void Start() {

		}

        FORCEINLINE void Update() {
            _Scene->Renderer()->GetFrameBuffer()->Bind(); 
			_Shader.Bind();
            _Grid->Render();
            _Scene->Renderer()->GetFrameBuffer()->UnBind();
        } 

	private:
		GLShader _Shader;
		float ScaleFactor;
        Unique<GLGrid> _Grid;
	};
}