#include "pch.h"
#include "GLFWInputs.h"
#include "Events/Categories/AppEvents.h"
#include "Events/Categories/KeyEvents.h"
#include "Events/Categories/MouseEvents.h"

namespace Orbit {
	MouseData GLFWInputs::_Mouse;
	Keyboard GLFWInputs::_Keyboard;
	EventDispatcher* GLFWInputs::_Dispatcher = nullptr;

	GLFWInputs::GLFWInputs(GLFWwindow* window, EventDispatcher* disp) {
		_Dispatcher = disp;
		glfwSetKeyCallback(window, keyEventCallback);
		glfwSetScrollCallback(window, MouseScrollCallback);
		glfwSetCursorPosCallback(window, MouseMotionCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetWindowCloseCallback(window, WindowCloseCallback);
		glfwSetFramebufferSizeCallback(window, WindowResizedCallback);
	}

	bool GLFWInputs::IsMousedown(MouseButton button) const {
		return _Mouse.buttons.test(button);
	}

	bool GLFWInputs::IsKeypress(KeyCode key) const {
		return _Keyboard.test(key);
	}

	// -----

	void GLFWInputs::WindowCloseCallback(GLFWwindow* window) {
		_Dispatcher->Post<WindowCloseEvent>();
	}

	void GLFWInputs::WindowResizedCallback(GLFWwindow* window, int width, int height) {
		_Dispatcher->Post<WindowResizeEvent>(width, height);
	}

	// ----

	void GLFWInputs::MouseMotionCallback(GLFWwindow* window, double x, double y) {
		_Mouse.deltaX = _Mouse.posX - x;
		_Mouse.deltaY = _Mouse.posY - y;
		_Mouse.posX = x;
		_Mouse.posY = y;

		if (_Mouse.buttons.test(GLFW_MOUSE_BUTTON_LEFT)) {
			_Dispatcher->Post<MouseDragEvent>(_Mouse);
		}
	}

	void GLFWInputs::MouseScrollCallback(GLFWwindow* window, double x, double y) {
		_Mouse.scrollX = x;
		_Mouse.scrollY = y;
		_Dispatcher->Post<MouseScrollEvent>(_Mouse);
	}

	void GLFWInputs::MouseButtonCallback(GLFWwindow* window, int button, int action, int mod) {
		switch (action) {
		case GLFW_PRESS:
			_Mouse.buttons.set(button);
			_Dispatcher->Post<MouseDownEvent>(_Mouse);
			break;

		case GLFW_RELEASE:
			_Mouse.buttons.reset(button);
			_Dispatcher->Post<MouseReleaseEvent>(_Mouse);
			break;
		}
	}

	// -----

	void GLFWInputs::keyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		switch (action) {
		case GLFW_PRESS:
			_Keyboard.set(key);
			_Dispatcher->Post<KeyPressedEvent>(key);
			_Dispatcher->Post<KeyRepeatEvent>(key);
			break;

		case GLFW_RELEASE:
			_Keyboard.reset(key);
			_Dispatcher->Post<KeyReleasedEvent>(key);
			break;

		case GLFW_REPEAT:
			_Keyboard.set(key);
			_Dispatcher->Post<KeyRepeatEvent>(key);
			break;
		}		
	}
}