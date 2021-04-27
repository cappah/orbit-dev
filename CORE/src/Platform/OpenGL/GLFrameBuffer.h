#pragma once

#include "Core/Common.h"

namespace Orbit {
	class GLFrameBuffer {
	public:
		FORCEINLINE GLFrameBuffer() = default;
		FORCEINLINE GLFrameBuffer(const GLFrameBuffer&) = default;
		FORCEINLINE GLFrameBuffer(int32 width, int32 height, bool sampling = false) {

			glGenTextures(1, &_TextureID);
			glGenFramebuffers(1, &_BufferID);
			glGenRenderbuffers(1, &_RenderID);

			glBindFramebuffer(GL_FRAMEBUFFER, _BufferID);
			glBindRenderbuffer(GL_RENDERBUFFER, _RenderID);

			glBindTexture(GL_TEXTURE_2D, _TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _TextureID, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RenderID);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				OB_ERROR("ERROR::FRAMEBUFFER::IS NOT COMPLETE!");
			}
		}

		FORCEINLINE ~GLFrameBuffer() {
			glDeleteTextures(1, &_TextureID);
			glDeleteRenderbuffers(1, &_RenderID);
			glDeleteFramebuffers(1, &_BufferID);
			OB_INFO("Frame Buffer destroyed!");
		}

		FORCEINLINE void Validate(int32 width, int32 height) {
			glBindTexture(GL_TEXTURE_2D, _TextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glBindRenderbuffer(GL_RENDERBUFFER, _RenderID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		}

		FORCEINLINE void Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, _BufferID);
		}

		FORCEINLINE void UnBind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		FORCEINLINE void Clear(vec3f color = vec3f(0.2f, 0.2f, 0.2f)) {
			glBindFramebuffer(GL_FRAMEBUFFER, _BufferID);
			glClearColor(color.r, color.g, color.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		FORCEINLINE uint32 GetFrameMap() {
			return _TextureID;
		}

		FORCEINLINE uint32 GetBuffer() {
			return _BufferID;
		}

		FORCEINLINE void BlitTo(GLuint fbo, int32 width, int32 height) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _BufferID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}

	private:
		uint32 _BufferID = 0;
		uint32 _RenderID = 0;
		uint32 _TextureID = 0;
	};
}