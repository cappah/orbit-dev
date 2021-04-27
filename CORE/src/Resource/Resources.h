#pragma once
#include "Platform/OpenGL/GLModel.h"
#include "Platform/OpenGL/GLShader.h"

namespace Orbit {
	// mesh
	struct Model {
		GLModel Data;
		std::string Name = EMPTY_STR;
		std::string Path = EMPTY_STR;
		FORCEINLINE operator GLModel() const { return Data; }
	};

	// texture
	struct Texture {
		int32 Width = 0;
		int32 Height = 0;
		uint32 TextureID = 0;
		std::string Name = EMPTY_STR;
		std::string Path = EMPTY_STR;

		int32 Format = 0;
		int32 NumChannels = 0;
		int32 LevelOfDetail = 0;
		int32 InternalFormat = 0;
		FORCEINLINE operator uint32() const { return TextureID; }
	};

	// hdr 
	struct HDR {
		int32 Width = 0;
		int32 Height = 0;
		uint32 TextureID = 0;
		std::string Path = EMPTY_STR;
		std::string Name = EMPTY_STR;

		int32 Format = 0;
		int32 NumChannels = 0;
		int32 LevelOfDetail = 0;
		int32 InternalFormat = 0;
		FORCEINLINE operator uint32() const { return TextureID; }
	};

	// cubemap
	enum CubeMapFaces : int32 {
		CUBEMAP_RIGHT = 0,
		CUBEMAP_LEFT = 1,
		CUBEMAP_TOP = 2,
		CUBEMAP_BOTTOM = 3,
		CUBEMAP_BACK = 4,
		CUBEMAP_FRONT = 5,
	};

	struct CubeMap {
		std::string Faces[6];
		uint32 CubeMapID = 0;
		std::string Name = EMPTY_STR;

		int32 Format = 0;
		int32 NumChannels = 0;
		int32 LevelOfDetail = 0;
		int32 InternalFormat = 0;	
		FORCEINLINE operator uint32() const { return CubeMapID; }
	};

	// shader
	struct Shader {
		uint32 Program = 0;
		std::string Name = EMPTY_STR;
		std::string Path = EMPTY_STR;
		FORCEINLINE operator uint32() const { return Program; }
	};
}