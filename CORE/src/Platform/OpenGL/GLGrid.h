#pragma once

#include "GLVertex.h"

namespace Orbit {
	class GLGrid {
	public:
		FORCEINLINE ~GLGrid() = default;
		FORCEINLINE GLGrid(const GLGrid&) = default;

		FORCEINLINE GLGrid(uint32 size = 10) {
            std::vector<vec3f> vertices;
            std::vector<vec4u> indices;

            for (int j = 0; j <= size; ++j) {
                for (int i = 0; i <= size; ++i) {
                    float x = (float)i / size;
                    float y = 0;
                    float z = (float)j / size;
                    vertices.push_back(glm::vec3(x, y, z));
                }
            }

            for (int j = 0; j < size; ++j) {
                for (int i = 0; i < size; ++i) {

                    int row1 = j * (size + 1);
                    int row2 = (j + 1) * (size + 1);

                    indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
                    indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));
                }
            }

            glGenVertexArrays(1, &_BufferArray);
            glBindVertexArray(_BufferArray);

            uint32 vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3f), vertices.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            uint32 ibo;
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vec4u), indices.data(), GL_STATIC_DRAW);

            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            _IndiceCount = (uint32)indices.size() * sizeof(uint32);

            vertices.clear();
            indices.clear();
		}

		FORCEINLINE void Render() {
			glBindVertexArray(_BufferArray);
			glDrawElements(GL_LINES, _IndiceCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		uint32 _BufferArray = 0;
		uint32 _IndiceCount = 0;
	};
}