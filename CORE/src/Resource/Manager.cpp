#include "pch.h"
#include "Manager.h"
#include "Core/Application.h"
#include "Events/Categories/ResourceEvents.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace Orbit {

	// MESH

	void ResourceManager::LoadMesh(std::string path) {
		auto seach = [&path](const Model& mesh) { return !path.compare(mesh.Path); };
		if (std::find_if(_Meshes.begin(), _Meshes.end(), seach) == _Meshes.end()) {
			Model mesh;
			mesh.Path = path;
			mesh.Data = GLModel(path.c_str());
			mesh.Name = path.substr(path.find_last_of("/\\") + 1);
			
			for (auto& mtl : mesh.Data.GetMaterials()) {
				_Materials.push_back(mtl);
			}
			_Meshes.push_back(mesh);
			mesh.Data.ClearMaterials();
			Application::Dispatcher()->Prioritize<ResourceEvent>(mesh.Name, ResourceType::MESH, ResourceAction::LOADED);
		}		
		else {
			OB_WARNING("Mesh [ %s ] allready loaded!", path.c_str());
		}
	}

	// HDR

	void ResourceManager::LoadHDR(std::string path) {
		HDR hdr;
		hdr.Path = path;
		hdr.Format = GL_RGBA;
		hdr.LevelOfDetail = 0;
		hdr.InternalFormat = GL_RGB32F;
		hdr.Name = path.substr(path.find_last_of("/\\") + 1);

		float* pixels = nullptr;
		stbi_set_flip_vertically_on_load(true);
		pixels = stbi_loadf(hdr.Path.c_str(), &hdr.Width, &hdr.Height, &hdr.NumChannels, 0);

		if (!pixels) {
			OB_INFO("Failed to load HDR: %s ", hdr.Path.c_str());
			return;
		}

		glGenTextures(1, &hdr.TextureID);
		glBindTexture(GL_TEXTURE_2D, hdr.TextureID);
		glTexImage2D(GL_TEXTURE_2D, hdr.LevelOfDetail, hdr.InternalFormat, hdr.Width, hdr.Height, 0, hdr.Format, GL_FLOAT, pixels);
		stbi_image_free(pixels);									

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		Application::Dispatcher()->Prioritize<ResourceEvent>(hdr.Name, ResourceType::HDR, ResourceAction::LOADED);
		_HDR.push_back(hdr);
	}

	// TEXTURE

	void ResourceManager::LoadTexture(std::string path) {
		Texture texture;
		texture.Path = path;
		texture.Name = path.substr(path.find_last_of("/\\") + 1);

		uint8* buffer = nullptr;
		stbi_set_flip_vertically_on_load(true);
		buffer = stbi_load(texture.Path.c_str(), &texture.Width, &texture.Height, &texture.NumChannels, 4);

		if (!buffer) {
			OB_ERROR("Failed to load texture: %s", texture.Path.c_str());
			return;
		}
			
		glGenTextures(1, &texture.TextureID);
		glBindTexture(GL_TEXTURE_2D, texture.TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width, texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		stbi_image_free(buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		Application::Dispatcher()->Prioritize<ResourceEvent>(texture.Name, ResourceType::TEXTURE, ResourceAction::LOADED);
		_Textures.push_back(texture);
	}

	// CUBEMAP

	void ResourceManager::LoadCubeMap(const char* faces[6]) {
		CubeMap cubemap;	
		cubemap.Format = GL_RGBA;
		cubemap.InternalFormat = GL_RGB32F;
		cubemap.Name = ("CubeMap_" + std::to_string(_CubeMaps.size()));

		glGenTextures(1, &cubemap.CubeMapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.CubeMapID);

		int32 width, height = 0;
		stbi_set_flip_vertically_on_load(false);

		for (uint32 i = 0; i < 6; i++) {
			cubemap.Faces[i] = faces[i];

			uint8* buffer = nullptr;
			buffer = stbi_load(faces[i], &width, &height, &cubemap.NumChannels, 0);

			if (!buffer) {
				glDeleteTextures(1, &cubemap.CubeMapID);
				OB_ERROR("Failed to load cubemap face: [ %s ]", faces[i]);
				return;
			}

			glTexImage2D((GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), cubemap.LevelOfDetail, cubemap.InternalFormat, width, height, 0, cubemap.Format, GL_UNSIGNED_BYTE, buffer);
			stbi_image_free(buffer);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		Application::Dispatcher()->Prioritize<ResourceEvent>(cubemap.Name, ResourceType::CUBEMAP, ResourceAction::LOADED);
		_CubeMaps.push_back(cubemap);
	}

	// SHADER

	void ResourceManager::LoadShader(std::string path) {
		auto seach = [&path](const Shader& shader) { return !path.compare(shader.Path); };
		if (std::find_if(_Shaders.begin(), _Shaders.end(), seach) != _Shaders.end()) {
			OB_WARNING("Shader [ %s ] allready loaded!", path.c_str());
			return;
		}

		uint32 vertex = CompileShaderSource(PrepareShaderSource(path + "_V.glsl").c_str(), GL_VERTEX_SHADER);
		uint32 fragment = CompileShaderSource(PrepareShaderSource(path + "_F.glsl").c_str(), GL_FRAGMENT_SHADER);

		int32 status;
		char info_log[512];

		Shader shader;
		shader.Name = path.substr(path.find_last_of("/\\") + 1).c_str();
		shader.Program = glCreateProgram();

		glAttachShader(shader.Program, vertex);
		glAttachShader(shader.Program, fragment);
		glLinkProgram(shader.Program);
		glGetProgramiv(shader.Program, GL_LINK_STATUS, &status);

		if (!status) {
			glGetProgramInfoLog(shader.Program, 512, nullptr, info_log);
			OB_ERROR("Failed to link shader: [ %s ] -> %s", path.c_str(), info_log);
			glDeleteProgram(shader.Program);			
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		_Shaders.push_back(shader);
	}

	std::string ResourceManager::PrepareShaderSource(std::string filename) {
		std::ifstream fileStream;
		fileStream.exceptions(std::ifstream::failbit | std::fstream::badbit);
		try {
			fileStream.open(filename);
			std::stringstream stringStream;
			stringStream << fileStream.rdbuf();
			fileStream.close();
			return stringStream.str();
		}
		catch (const std::exception&) {
			OB_ERROR("failed to load shader source: %s", filename.c_str());
		}
		return std::string();
	}

	uint32 ResourceManager::CompileShaderSource(const char* src, GLenum type) {
		GLint status;
		GLchar info_log[512];
		uint32 shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, &src, nullptr);
		glCompileShader(shaderId);

		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
		if (!status) {
			glGetShaderInfoLog(shaderId, 512, nullptr, info_log);
			OB_ERROR("Failed to compile shader: %s", info_log);
		}
		return shaderId;
	}
}