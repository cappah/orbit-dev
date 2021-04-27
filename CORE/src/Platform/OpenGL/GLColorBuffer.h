#pragma once

#include "Core/Common.h"

class GLColorBuffer {
public:
	FORCEINLINE GLColorBuffer(int32 width, int32 height) {
		glGenTextures(1, &_samplerID);
		glBindTexture(GL_TEXTURE_2D, _samplerID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach to fbo
		glGenFramebuffers(1, &_bufferID));
		glBindFramebuffer(GL_FRAMEBUFFER, _bufferID));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _samplerID, 0);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "FAILED TO CREATE FBO");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FORCEINLINE ~GLColorBuffer() {
		glDeleteTextures(1, &_samplerID);
		glDeleteFramebuffers(1, &_bufferID);
	}

	FORCEINLINE void SetSize(int32 width, int32 height) {
		glBindTexture(GL_TEXTURE_2D, _samplerID));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FORCEINLINE void Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, _bufferID);
	}

	void UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FORCEINLINE void Clear() {		
		glClear(GL_COLOR_BUFFER_BIT);
	}

	FORCEINLINE uint32 GetSamplerHandle() const {
		return _samplerID;
	}

	FORCEINLINE uint32 GetBufferHandle() const {
		return _bufferID;
	}

private:
	uint32 _bufferID;
	uint32 _samplerID;
};