#pragma once

#include "GLVertexArray.h"

namespace Orbit {
	class GLQuad {
	public:
		FORCEINLINE GLQuad() {
			std::vector<GLVertex> vertices {
				GLVertex(vec3f(-0.5f, -0.5f, 0), vec2f(0, 0)),
				GLVertex(vec3f( 0.5f, -0.5f, 0), vec2f(1, 0)),
				GLVertex(vec3f( 0.5f,  0.5f, 0), vec2f(1, 1)),
				GLVertex(vec3f(-0.5f,  0.5f, 0), vec2f(0, 1))
			};

			std::vector<uint32> indices {
				0, 1, 2,
				2, 3, 0
			};

			_buffer = GLVertexArray(vertices, indices);
		}

		FORCEINLINE ~GLQuad() = default;

		FORCEINLINE void Draw() {
			_buffer.DrawElements();
		}

	private:
		GLVertexArray _buffer;
	};
}