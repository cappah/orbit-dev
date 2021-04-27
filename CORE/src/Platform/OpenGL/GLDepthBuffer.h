#pragma once

#include "Core/Common.h"

namespace Orbit {
	class GLDepthBuffer {
	public:
		FORCEINLINE GLDepthBuffer(int32 width, int32 height) {
			glGenFramebuffers(1, &_Buffer);
			glBindFramebuffer(GL_FRAMEBUFFER, _Buffer);

			glGenTextures(1, &_DepthMap);
			glBindTexture(GL_TEXTURE_2D, _DepthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _DepthMap, 0);
			glDrawBuffer(GL_NONE); // No color buffer is drawn to.
			glReadBuffer(GL_NONE); // No color buffer is drawn to.

			// check if framebuffer is ok
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				glDeleteTextures(1, &_DepthMap);
				glDeleteFramebuffers(1, &_Buffer);
				OB_INFO("FAILED TO CREATE DEPTH BUFFER");
			}
		}

		FORCEINLINE ~GLDepthBuffer() {
			glDeleteTextures(1, &_DepthMap);
			glDeleteFramebuffers(1, &_Buffer);
			OB_INFO("Depth Buffer destroyed!");
		}

		FORCEINLINE void Validate(int32 width, int32 height) {
			glBindTexture(GL_TEXTURE_2D, _DepthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		FORCEINLINE void Bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, _Buffer);
			glCullFace(GL_FRONT);
		}

		FORCEINLINE void UnBind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glCullFace(GL_BACK);
		}

		FORCEINLINE void Clear() {		
			glBindFramebuffer(GL_FRAMEBUFFER, _Buffer);
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		FORCEINLINE uint32 GetDepthMap() {
			return _DepthMap;
		}

		FORCEINLINE uint32 GetBuffer() {
			return _Buffer;
		}

	private:
		uint32 _Buffer = 0;
		uint32 _DepthMap = 0;
	};
}