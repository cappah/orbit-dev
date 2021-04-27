#pragma once

#include "Core/Common.h"

namespace Orbit {
	class GLFW_Window {
	public:
		FORCEINLINE GLFW_Window() = default;
		FORCEINLINE ~GLFW_Window() = default;
		FORCEINLINE GLFW_Window(const GLFW_Window&) = default;
		FORCEINLINE GLFW_Window(const char* title, int32 width, int32 height) {
			glfwInit();					
			auto display = glfwGetVideoMode(glfwGetPrimaryMonitor());

			glfwSwapInterval(1);
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
			glfwWindowHint(GLFW_RED_BITS, display->redBits);
			glfwWindowHint(GLFW_BLUE_BITS, display->blueBits);
			glfwWindowHint(GLFW_GREEN_BITS, display->greenBits);
			glfwWindowHint(GLFW_REFRESH_RATE, display->refreshRate);

			_Handle = glfwCreateWindow(width, height, title, NULL, NULL);

			if (!_Handle) {
				OB_ERROR("Failed to create GLFW window!");
				exit(-1);
			}	

			#ifdef OPENGL_RENDERER_API
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GL_TRUE);

				glfwMakeContextCurrent(_Handle);		

				if (glewInit() != GLEW_OK) {
					OB_ERROR("Failed to create opengl context!");
					return;
				}

				glewExperimental = GL_TRUE;
			#endif
		}

		FORCEINLINE void Free() {
			glfwTerminate();
			OB_INFO("Window destroyed!");
		}

		FORCEINLINE void PoolEvents() {
			glfwPollEvents();
		}

		FORCEINLINE void SwapBuffers() {			
			glfwSwapBuffers(_Handle);
		}

		FORCEINLINE GLFWwindow* GetHandle() { 
			return _Handle; 
		}

		FORCEINLINE void SetVsync(int8 interval) {
			glfwSwapInterval(interval);
		}

	private:
		GLFWwindow* _Handle = nullptr;
	};
}