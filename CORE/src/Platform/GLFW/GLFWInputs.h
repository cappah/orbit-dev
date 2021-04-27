#pragma once

#include "Events/EventDispatcher.h"

namespace Orbit {
	class GLFWInputs {
	public:
		GLFWInputs() = default;
		~GLFWInputs() = default;
		GLFWInputs(const GLFWInputs&) = default;
		GLFWInputs(GLFWwindow* window, EventDispatcher* disp);

		bool IsKeypress(KeyCode key) const;

		bool IsMousedown(MouseButton button) const;

	private:
		static void WindowCloseCallback(GLFWwindow* window);
		static void MouseMotionCallback(GLFWwindow* window, double x, double y);
		static void MouseScrollCallback(GLFWwindow* window, double x, double y);
		static void WindowResizedCallback(GLFWwindow* window, int width, int height);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mod);
		static void keyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	private:
		static MouseData _Mouse;
		static Keyboard _Keyboard;
		static EventDispatcher* _Dispatcher;
	};
}