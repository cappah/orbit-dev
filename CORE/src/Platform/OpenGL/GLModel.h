#pragma once

#include "GLMesh.h"
#include <assimp/scene.h>
#include "Resource/Material.h"
#include <assimp/Importer.hpp>
#include <assimp/pbrmaterial.h>
#include <assimp/postprocess.h>
#include "Platform/Shared/AssimpHelpers.h"

namespace Orbit {
	class GLModel {
	public:
		FORCEINLINE ~GLModel() { 
			_Meshes.clear(); 
			_Materials.clear();
		}

		FORCEINLINE GLModel() = default;

		FORCEINLINE GLModel(const GLModel&) = default;

		FORCEINLINE GLModel(const char* path) {
			Assimp::Importer loader;
			const aiScene* scene = loader.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				OB_ERROR("failed to load model: %s", loader.GetErrorString());
				return;
			}
			ParseNode(scene->mRootNode, scene);
		}

		FORCEINLINE const std::vector<Material>& GetMaterials() {
			return _Materials;
		}

		FORCEINLINE const std::vector<GLMesh>& GetMeshes() {
			return _Meshes;
		}

		FORCEINLINE void ClearMaterials() {
			_Materials.clear();
			_Materials.shrink_to_fit();
		}

		FORCEINLINE void Render() {
			for (auto& mesh : _Meshes) {
				mesh.DrawElements();
			}
		}

	private:
		FORCEINLINE void ParseMaterial(aiMaterial* aMaterial) {			
			std::string name = aMaterial->GetName().C_Str();
			auto it = std::find_if(_Materials.begin(), _Materials.end(), 
				[&name](const Material& mtl) { return name == mtl.Name; });

			// return if already parsed
			if (it != _Materials.end()) { return;}

			Material material;
			material.Name = name;
			ai_real factor = 0.0f;
			aiColor3D color(0.0f, 0.0f, 0.0f);

			if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == 0) {
				material.Data.Albedo = vec3f(color.r, color.g, color.b);				
			}
			
			if (aMaterial->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, factor) == 0) {
				material.Data.Metallic = factor;				
			}

			if (aMaterial->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, factor) == 0) {
				material.Data.Roughness = factor;				
			}

			_Materials.push_back(material);
		}

		FORCEINLINE void ParseNode(aiNode* node, const aiScene* scene) {
			for (GLuint i = 0; i < node->mNumMeshes; i++) {								
				ParseMesh(scene->mMeshes[node->mMeshes[i]], node->mTransformation, scene);
			}

			for (GLuint i = 0; i < node->mNumChildren; i++) {
				ParseNode(node->mChildren[i], scene);
			}
		}

		FORCEINLINE void ParseMesh(aiMesh* mesh, aiMatrix4x4 transform, const aiScene* scene) {
			std::vector<GLVertex> vertices;
			std::vector<uint32> indices;
			GLVertex vertex;

			// vertices
			for (uint32 i = 0; i < mesh->mNumVertices; i++) {
				// positions
				if (mesh->HasPositions()) {
					vertex.Position = AssimpHelpers::ToGLM_Vec3(mesh->mVertices[i]);					
				}

				// normals
				if (mesh->HasNormals()) {
					vertex.Normal = AssimpHelpers::ToGLM_Vec3(mesh->mNormals[i]);
				}

				// texcoords
				if (mesh->HasTextureCoords(0)) {
					vertex.Texcoords.x = mesh->mTextureCoords[0][i].x;
					vertex.Texcoords.y = mesh->mTextureCoords[0][i].y;
				}

				// tangents & bitangents
				if (mesh->HasTangentsAndBitangents()) {
					vertex.Tangent = AssimpHelpers::ToGLM_Vec3(mesh->mTangents[i]);
					vertex.Bitangent = AssimpHelpers::ToGLM_Vec3(mesh->mBitangents[i]);
				}				
				
				vertices.push_back(vertex);
			}

			// indices	
			for (uint32 i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (GLuint j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			// material
			auto newMesh = GLMesh(vertices, indices);

			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* aMaterial = scene->mMaterials[mesh->mMaterialIndex];
				ParseMaterial(aMaterial);				
			}						

			newMesh.SetName(mesh->mName.C_Str());
			_Meshes.push_back(newMesh);
		 }

	private:
		std::vector<GLMesh> _Meshes;
		std::vector<Material> _Materials;
	};
}