#pragma once

#include "Resources.h"

namespace Orbit {	
	class ResourceManager {
	public:
		FORCEINLINE ResourceManager() = default;

		FORCEINLINE ~ResourceManager() {
			_HDR.clear();
			_Meshes.clear();
			_Shaders.clear();
			_Textures.clear();
			_CubeMaps.clear();
			_Materials.clear();
			OB_INFO("Resource cleaned!");
		}

		// SHADER

		FORCEINLINE const std::vector<Shader>& GetShaderList() {
			return _Shaders;
		}

		FORCEINLINE const Shader& GetShader(std::string name) {
			auto search = [&name](const Shader& shader) { return !name.compare(shader.Name); };
			auto it = std::find_if(_Shaders.begin(), _Shaders.end(), search);
			ASSERT(it != _Shaders.end() && "Shader out of range!");
			return *it;
		}

		void LoadShader(std::string filename);

		// MATERIAL 

		FORCEINLINE const std::vector<Material>& GetMaterialList() {
			return _Materials;
		}

		FORCEINLINE void AddMaterial(const Material& material) {
			auto search = [&material](const Material& mtl) { return !material.Name.compare(mtl.Name); };
			auto it = std::find_if(_Materials.begin(), _Materials.end(), search);
			_Materials.push_back(material);
		}

		FORCEINLINE Material* MaterialRef(std::string name) {
			auto search = [&name](const Material& mtl) { return !name.compare(mtl.Name); };
			auto it = std::find_if(_Materials.begin(), _Materials.end(), search);
			ASSERT(it != _Materials.end() && "Material out of range!");
			return &(*it);
		}

		// CUBEMAP

		FORCEINLINE const std::vector<CubeMap>& GetCubeMapList() {
			return _CubeMaps;
		}

		FORCEINLINE const CubeMap& GetCubeMap(std::string name) {
			auto it = std::find_if(_CubeMaps.begin(), _CubeMaps.end(), [&name](const CubeMap& map) { return !name.compare(map.Name); });
			ASSERT(it != _CubeMaps.end() && "CubeMap out of range!");
			return *it;
		}

		void LoadCubeMap(const char* faces[6]);

		// TEXTURE

		FORCEINLINE const std::vector<Texture>& GetTextureList() {
			return _Textures;
		}

		FORCEINLINE const Texture& GetTexture(std::string name) {
			auto it = std::find_if(_Textures.begin(), _Textures.end(), [&name](const Texture& tex) { return !name.compare(tex.Name); });
			ASSERT(it != _Textures.end() && "texture out of range!");
			return *it;
		}

		void LoadTexture(std::string path);

		// MESH

		FORCEINLINE const std::vector<Model>& GetMeshList() {
			return _Meshes;
		}

		Model* GetMesh(std::string name) {
			auto it = std::find_if(_Meshes.begin(), _Meshes.end(), [&name](const Model& mesh) { return !name.compare(mesh.Name); });
			ASSERT(it != _Meshes.end() && "Mesh out of range!");
			return &(*it);
		}

		void LoadMesh(std::string path);

		// HDR

		FORCEINLINE const std::vector<HDR>& GetHdrList() {
			return _HDR;
		}

		FORCEINLINE const HDR& GetHDR(std::string name) {
			auto it = std::find_if(_HDR.begin(), _HDR.end(), [&name](const HDR& hdr) { return !name.compare(hdr.Name); });
			ASSERT(it != _HDR.end() && "HDR out of range!");
			return *it;
		}

		void LoadHDR(std::string path);

	private:
		std::string PrepareShaderSource(std::string filename);

		uint32 CompileShaderSource(const char* src, GLenum type);

	private:
		std::vector<HDR> _HDR;
		std::vector<Model> _Meshes;
		std::vector<Shader> _Shaders;
		std::vector<CubeMap> _CubeMaps;
		std::vector<Texture> _Textures;
		std::vector<Material> _Materials;
	};
}