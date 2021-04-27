#pragma once

#include "GLVertex.h"
#include <assimp/matrix4x4.h>

namespace Orbit {
	class GLMesh {
	public:
		FORCEINLINE GLMesh() = default;
		FORCEINLINE ~GLMesh() = default;
		FORCEINLINE GLMesh(const GLMesh&) = default;

		FORCEINLINE GLMesh(const std::vector<GLVertex>& vertices, const std::vector<uint32>& indices, GLenum usage = GL_STATIC_DRAW) :
			_NumOfIndices((uint32)indices.size()), _NumOfVertices((uint32)vertices.size()) {

			glGenVertexArrays(1, &_BufferArray);
			glBindVertexArray(_BufferArray);

			uint32 vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, _NumOfVertices * sizeof(GLVertex), vertices.data(), usage);
			uint32 ebo;
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _NumOfIndices * sizeof(uint32), indices.data(), usage);

			// positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, Position));

			// normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, Normal));

			// texcoords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, Texcoords));

			// tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, Tangent));

			// bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, Bitangent));

			glBindVertexArray(0);
		}

		FORCEINLINE void Bind() {
			glBindVertexArray(_BufferArray);
		}

		FORCEINLINE void UnBind() {
			glBindVertexArray(0);
		}

		FORCEINLINE void Destroy() {
			glDeleteVertexArrays(1, &_BufferArray);
		}

		FORCEINLINE void SetName(std::string name) {
			_Name = name;
		}

		FORCEINLINE std::string GetName() {
			return _Name;
		}

		FORCEINLINE void DrawArrays(GLenum mode = GL_TRIANGLES) {
			glBindVertexArray(_BufferArray);
			glDrawArrays(mode, 0, _NumOfVertices);
			glBindVertexArray(0);
		}

		FORCEINLINE void DrawElements(GLenum mode = GL_TRIANGLES) {
			glBindVertexArray(_BufferArray);
			glDrawElements(mode, _NumOfIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		std::string _Name;	
		uint32 _BufferArray = 0;
		uint32 _NumOfIndices = 0;
		uint32 _NumOfVertices = 0;
	};
}