#pragma once

#include "Maths/Maths.h"
#include "GLDepthBuffer.h"
#include "GLFrameBuffer.h"

namespace Orbit {
	class GLRenderer {
	public:
		FORCEINLINE GLRenderer() = default;
		FORCEINLINE ~GLRenderer() = default;
		FORCEINLINE GLRenderer(const GLRenderer&) = default;

		FORCEINLINE GLRenderer(int32 width, int32 height) {			
			glViewport(0, 0, width, height);
			_DepthBuffer = MakeUnique<GLDepthBuffer>(width, height);
			_FrameBuffer = MakeUnique<GLFrameBuffer>(width, height);
			
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);			

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);					
			glEnable(GL_BLEND);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			glEnable(GL_MULTISAMPLE);	
		}		

		FORCEINLINE void Enable(GLenum cap) { glEnable(cap); }

		FORCEINLINE void Disable(GLenum cap) { glDisable(cap); }

		FORCEINLINE GLFrameBuffer* GetFrameBuffer() { return _FrameBuffer.get(); }

		FORCEINLINE GLDepthBuffer* GetDepthBuffer() { return _DepthBuffer.get(); }
		
		FORCEINLINE void SetViewport(int32 x, int32 y, int32 w, int32 h) { glViewport(x, y, w, h); }

	private:
		Unique<GLFrameBuffer> _FrameBuffer;
		Unique<GLDepthBuffer> _DepthBuffer;
	};
}