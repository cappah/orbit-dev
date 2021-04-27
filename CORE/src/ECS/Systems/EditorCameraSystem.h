#pragma once

#include "ECS/ISystem.h"
#include "Platform/OpenGL/GLCamera.h"

#include "Events/Categories/KeyEvents.h"
#include <Events/Categories/AppEvents.h>
#include "Events/Categories/MouseEvents.h"
#include "Events/Categories/CameraEvents.h"

namespace Orbit::ECS {
	class EditorCameraSystem : public ISystem {
	public:
		FORCEINLINE ~EditorCameraSystem() = default;

		FORCEINLINE EditorCameraSystem() = default;

		FORCEINLINE void Init() {
			_Shaders.push_back(_Loader->GetShader("PBR"));
			_Shaders.push_back(_Loader->GetShader("GRID"));
			_Shaders.push_back(_Loader->GetShader("FLAT"));
			_Shaders.push_back(_Loader->GetShader("SKYBOX"));

			_Dispatcher->AddListener<CameraZoomEvent>(OB_BIND_FN(EditorCameraSystem::OnZoomed));
			_Dispatcher->AddListener<CameraMovedEvent>(OB_BIND_FN(EditorCameraSystem::OnMoved));
			_Dispatcher->AddListener<CameraFocusEvent>(OB_BIND_FN(EditorCameraSystem::OnFocus));
			_Dispatcher->AddListener<SceneResizedEvent>(OB_BIND_FN(EditorCameraSystem::OnSceneResized));
		}
		
		FORCEINLINE void Start() {
			Update();
		}

		void Update() {
			for (auto& shader : _Shaders) { _Camera.SetUniform(GLShader(shader.Program)); }
			_Dispatcher->Prioritize<CameraEvent>(_Camera.View(), _Camera.Projection());
		}		

	private:
		FORCEINLINE void OnSceneResized(const SceneResizedEvent& e) {
			_Camera.AspectRatio = e.Width()/e.Height();
			this->Update();
		}

		FORCEINLINE void OnMoved(const CameraMovedEvent& e) {
			if (Application::Inputs()->IsKeypress(KEY_LEFT_CONTROL)) {			
				_Camera.Location.x += (float)e.DeltaX();
				_Camera.Location.y -= (float)e.DeltaY();
			}
			else {
				_Camera.Orientation.x -= (float)e.DeltaY();
				_Camera.Orientation.y -= (float)e.DeltaX();
			}
			this->Update();			
		}

		FORCEINLINE void OnZoomed(const CameraZoomEvent& e) {
			_Camera.Location.z -= (float)e.DeltaZ();
			this->Update();			
		}
		
		FORCEINLINE void OnFocus(const CameraFocusEvent& e) {
			_Camera.Location = e.Target();
			this->Update();			
		}
		
	private:
		GLCamera _Camera;
		std::vector<Shader> _Shaders;
	};
}