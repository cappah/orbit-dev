#include "pch.h"
#include "Application.h"
#include "Scene/Scene.h"
#include "Editor/Editor.h"
#include "Events/Categories/AppEvents.h"

namespace Orbit::Application {

	using namespace UI;

	static Scene s_Scene;
	static Editor s_Editor;
	static bool s_IsOpen = false;

	static PlatformInputs s_Inputs;
	static PlatformWindow s_Window;
	static EventDispatcher s_Dispatcher;

	FORCEINLINE static void OnCloseWindow(const WindowCloseEvent& e) {
		s_IsOpen = false;		
	}

	void Run() {
		s_Window = PlatformWindow("ORBIT", 1280, 700);
		s_Inputs = PlatformInputs(s_Window.GetHandle(), &s_Dispatcher);

		s_Dispatcher.AddListener<WindowCloseEvent>(OnCloseWindow);

		s_Scene.Init();
		s_Scene.Start();

		s_Editor.Init();
		s_IsOpen = true;
	
		while (s_IsOpen) {	
			s_Window.PoolEvents();
			s_Dispatcher.Process();

			s_Scene.Update();
			s_Editor.Show();

			s_Window.SwapBuffers();
		}

		// free memory
		s_Window.Free();
	}

	Scene* GetScene() { return &s_Scene; }

	PlatformWindow* Window() { return &s_Window; }

	PlatformInputs* Inputs() { return &s_Inputs; }

	EventDispatcher* Dispatcher() { return &s_Dispatcher; }

}
