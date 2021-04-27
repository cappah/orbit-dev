#pragma once

#include "Maths/Maths.h"

namespace Orbit {
	class GLShader {
	public:
		FORCEINLINE GLShader(const GLShader& other) : _Handle(other._Handle) { }

		FORCEINLINE GLShader(const uint32 handle) : _Handle(handle) { }

		FORCEINLINE GLShader() : _Handle(0) { }

		FORCEINLINE ~GLShader() = default;

		FORCEINLINE void Bind() {
			glUseProgram(_Handle);
		}

		FORCEINLINE void Bnbind() {
			glUseProgram(0);
		}

		FORCEINLINE void SetSample2D(const char* uName, uint32 tex2d, int32 unit = 0) {
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, tex2d);
			glUniform1i(glGetUniformLocation(_Handle, uName), unit);
		}

		FORCEINLINE void SetSampleCube(const char* uName, uint32 cube, int32 unit = 0) {
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cube);
			glUniform1i(glGetUniformLocation(_Handle, uName), unit);
		}

		FORCEINLINE void Set1i(const char* uName, int32 value) {
			glUniform1i(glGetUniformLocation(_Handle, uName), value);
		}

		FORCEINLINE void Set1f(const char* uName, float value) {
			glUniform1f(glGetUniformLocation(_Handle, uName), value);
		}

		FORCEINLINE void Set2f(const char* uName, float x, float y) {
			glUniform2f(glGetUniformLocation(_Handle, uName), x, y);
		}

		FORCEINLINE void SetV2f(const char* uName, glm::vec2 v) {
			Set2f(uName, v.x, v.y);
		}

		FORCEINLINE void Set3f(const char* uName, float x, float y, float z) {
			glUniform3f(glGetUniformLocation(_Handle, uName), x, y, z);
		}

		FORCEINLINE void SetV3f(const char* uName, glm::vec3 v) {
			Set3f(uName, v.x, v.y, v.z);
		}

		FORCEINLINE void Set4f(const char* uName, float x, float y, float z, float w) {
			glUniform4f(glGetUniformLocation(_Handle, uName), x, y, z, w);
		}

		FORCEINLINE void SetV4f(const char* uName, glm::vec4 v) {
			Set4f(uName, v.x, v.y, v.z, v.z);
		}

		FORCEINLINE void SetMat4f(const char* uName, const float* mtx) {
			glUniformMatrix4fv(glGetUniformLocation(_Handle, uName), 1, GL_FALSE, mtx);
		}

		FORCEINLINE const uint32 GetHandle() const { 
			return _Handle; 
		}

	private:
		uint32 _Handle;
	};
}