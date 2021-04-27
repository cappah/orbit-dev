#pragma once

#include "Resource/Manager.h"
#include "Serializer/XMLSerializer.h"

#include "ECS/Components/Common.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/SpotLight.h"
#include "ECS/Components/PointLight.h"
#include "ECS/Components/MeshRenderer.h"
#include "ECS/Components/DirectionalLight.h"

namespace Orbit {
	class SceneSerializer {
	public:
		FORCEINLINE SceneSerializer(ECS::Registry* reg, ResourceManager* loader) : _Registry(reg), _Loader(loader) {
			_Writer = MakeUnique<XML::Writer>();
			_Reader = MakeUnique<XML::Reader>();
		}

		FORCEINLINE void Save(const char* filename) {
			_Writer->Open(filename);
			{
				for (auto& entity : _Registry->GetAllEntities()) {
					_Writer->PushTag("Entity");
					{
						// common
						this->SaveCommonComponent(entity);
						// mesh renderer
						this->SaveMeshComponent(entity);
						// transform
						this->SaveTransformComponent(entity);
						// camera
						this->SaveCameraComponent(entity);
						// spot light
						this->SaveSpotLightComponent(entity);
						// point light
						this->SavePointLightComponent(entity);
						// directional light
						this->SaveDirectLightComponent(entity);
					}
					_Writer->PopTag();
				}
			}
			_Writer->Close();
		}

		FORCEINLINE void Load(std::string filename) {
			_Registry->DestroyAll();
			_Registry->Clear();
			_Reader->Open(filename.c_str());
			
			for (auto& xmlEntt : _Reader->ChildElements()) {
				ECS::EntityID handle = _Registry->AddEntity();

				for (auto* e = xmlEntt->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {				
					// common
					ParseCommonComponent(e, handle);
					// transform
					ParseTransformComponent(e, handle);
					// spotlight
					ParseSpotLightComponent(e, handle);
					// point light
					ParsePointLightComponent(e, handle);
					// dir light
					ParseDirectLightComponent(e, handle);
					// camera
					ParseCameraComponent(e, handle);
					// mesh
					ParseMeshComponent(e, handle);
				}
			}			

			_Reader->Close();			
		}

		// ---------------------------------------------------------------

		void ParseMeshComponent(tinyxml2::XMLElement* component, ECS::EntityID handle) {
			if (component != nullptr && std::strcmp(component->Attribute("type"), "MeshRenderer") == 0) {
				std::string name = component->Attribute("name");
				std::string mtl = component->Attribute("material");
				_Registry->AddComponent<ECS::MeshRenderer>(handle, _Loader->GetMesh(name), _Loader->MaterialRef(mtl));
			}
		}

		FORCEINLINE void ParseCommonComponent(tinyxml2::XMLElement* component, ECS::EntityID handle) {
			if (component != nullptr && std::strcmp(component->Attribute("type"), "Common") == 0) {
				ECS::Common comp;
				comp.Name = component->Attribute("name");
				comp.IsActive = component->BoolAttribute("active");
				_Registry->AddComponent(handle, comp);
			}
		}

		FORCEINLINE void ParseSpotLightComponent(tinyxml2::XMLElement* component, ECS::EntityID handle) {
			if (component != nullptr && std::strcmp(component->Attribute("type"), "Spot Light") == 0) {
				ECS::SpotLight comp;
				comp.InnerCutOff = component->FloatAttribute("innercutoff");
				comp.OuterCutOff = component->FloatAttribute("outercutoff");
				comp.Intensity = component->FloatAttribute("intensity");
				comp.CastShadow = component->BoolAttribute("shadow");
				comp.Radiance.r = component->FloatAttribute("r");
				comp.Radiance.g = component->FloatAttribute("g");
				comp.Radiance.b = component->FloatAttribute("b");
				comp.Bias = component->FloatAttribute("bias");
				_Registry->AddComponent(handle, comp);
			}
		}
		
		FORCEINLINE void ParsePointLightComponent(tinyxml2::XMLElement* component, ECS::EntityID handle) {
			if (component != nullptr && std::strcmp(component->Attribute("type"), "Point Light") == 0) {
				ECS::PointLight comp;			
				comp.Intensity = component->FloatAttribute("intensity");
				comp.CastShadow = component->BoolAttribute("shadow");
				comp.Radiance.r = component->FloatAttribute("r");
				comp.Radiance.g = component->FloatAttribute("g");
				comp.Radiance.b = component->FloatAttribute("b");
				comp.Bias = component->FloatAttribute("bias");
				_Registry->AddComponent(handle, comp);
			}
		}

		FORCEINLINE void ParseDirectLightComponent(tinyxml2::XMLElement* component, ECS::EntityID handle) {
			if (component != nullptr && std::strcmp(component->Attribute("type"), "Directional Light") == 0) {
				ECS::DirectionalLight comp;
				comp.Intensity = component->FloatAttribute("intensity");
				comp.CastShadow = component->BoolAttribute("shadow");
				comp.Radiance.r = component->FloatAttribute("r");
				comp.Radiance.g = component->FloatAttribute("g");
				comp.Radiance.b = component->FloatAttribute("b");
				comp.Bias = component->FloatAttribute("bias");
				_Registry->AddComponent(handle, comp);
			}
		}

		FORCEINLINE void ParseCameraComponent(tinyxml2::XMLElement* component, ECS::EntityID handle) {
			if (component != nullptr && std::strcmp(component->Attribute("type"), "Camera") == 0) {
				ECS::Camera comp;
				comp.Handle.AspectRatio = component->FloatAttribute("aspect");
				comp.Handle.FieldOfView = component->FloatAttribute("fov");
				comp.Handle.NearPlane = component->FloatAttribute("near");
				comp.Handle.FarPlane = component->FloatAttribute("far");
				comp.IsPrimary = component->BoolAttribute("primary");
				_Registry->AddComponent(handle, comp);
			}
		}

		FORCEINLINE void ParseTransformComponent(tinyxml2::XMLElement* component, ECS::EntityID handle) {
			if (component != nullptr && std::strcmp(component->Attribute("type"), "Transform") == 0) {
				ECS::Transform comp;
				// translate
				comp.Position.x = component->FloatAttribute("px");
				comp.Position.y = component->FloatAttribute("py");
				comp.Position.z = component->FloatAttribute("pz");
				// rotation
				comp.Rotation.x = component->FloatAttribute("rx");
				comp.Rotation.y = component->FloatAttribute("ry");
				comp.Rotation.z = component->FloatAttribute("rz");
				// scale
				comp.Scale.x = component->FloatAttribute("sx");
				comp.Scale.y = component->FloatAttribute("sy");
				comp.Scale.z = component->FloatAttribute("sz");
				_Registry->AddComponent(handle, comp);
			}
		}

		// ---------------------------------------------------------------

		FORCEINLINE void SaveCommonComponent(ECS::EntityID entity) {
			if (_Registry->HasComponent<ECS::Common>(entity)) {
				const auto& common = _Registry->GetComponent<ECS::Common>(entity);
				_Writer->PushTag("Component");
				_Writer->PushStringAttr("type", "Common");
				_Writer->PushStringAttr("name", common.Name.c_str());
				_Writer->PushIntAttr("active", common.IsActive);
				_Writer->PopTag();
			}
		}

		FORCEINLINE void SaveMeshComponent(ECS::EntityID entity) {
			if (_Registry->HasComponent<ECS::MeshRenderer>(entity)) {
				const auto& mesh = _Registry->GetComponent<ECS::MeshRenderer>(entity);
				_Writer->PushTag("Component");
				_Writer->PushStringAttr("type", "MeshRenderer");
				_Writer->PushStringAttr("name", mesh.MeshData->Name.c_str());
				_Writer->PushStringAttr("material", mesh.Matrial->Name.c_str());
				_Writer->PopTag();
			}
		}

		FORCEINLINE void SaveCameraComponent(ECS::EntityID entity) {
			if (_Registry->HasComponent<ECS::Camera>(entity)) {
				const auto& camera = _Registry->GetComponent<ECS::Camera>(entity);
				_Writer->PushTag("Component");
				_Writer->PushStringAttr("type", "Camera");
				_Writer->PushIntAttr("primary", camera.IsPrimary);
				_Writer->PushFloatAttr("far", camera.Handle.FarPlane);
				_Writer->PushFloatAttr("near", camera.Handle.NearPlane);
				_Writer->PushFloatAttr("fov", camera.Handle.FieldOfView);
				_Writer->PushFloatAttr("aspect", camera.Handle.AspectRatio);
				_Writer->PopTag();
			}
		}

		FORCEINLINE void SaveTransformComponent(ECS::EntityID entity) {
			if (_Registry->HasComponent<ECS::Transform>(entity)) {
				const auto& transform = _Registry->GetComponent<ECS::Transform>(entity);
				_Writer->PushTag("Component");
				_Writer->PushStringAttr("type", "Transform");
				// translate
				_Writer->PushFloatAttr("px", transform.Position.x);
				_Writer->PushFloatAttr("py", transform.Position.y);
				_Writer->PushFloatAttr("pz", transform.Position.z);
				// translate
				_Writer->PushFloatAttr("rx", transform.Rotation.x);
				_Writer->PushFloatAttr("ry", transform.Rotation.y);
				_Writer->PushFloatAttr("rz", transform.Rotation.z);
				// translate
				_Writer->PushFloatAttr("sx", transform.Scale.x);
				_Writer->PushFloatAttr("sy", transform.Scale.y);
				_Writer->PushFloatAttr("sz", transform.Scale.z);
				_Writer->PopTag();
			}
		}

		FORCEINLINE void SaveSpotLightComponent(ECS::EntityID entity) {
			if (_Registry->HasComponent<ECS::SpotLight>(entity)) {
				const auto& light = _Registry->GetComponent<ECS::SpotLight>(entity);
				_Writer->PushTag("Component");
				_Writer->PushStringAttr("type", "Spot Light");
				_Writer->PushFloatAttr("r", light.Radiance.r);
				_Writer->PushFloatAttr("g", light.Radiance.g);
				_Writer->PushFloatAttr("b", light.Radiance.b);
				_Writer->PushFloatAttr("intensity", light.Intensity);
				_Writer->PushFloatAttr("bias", light.Bias);
				_Writer->PushFloatAttr("innercutoff", light.InnerCutOff);
				_Writer->PushFloatAttr("outercutoff", light.OuterCutOff);
				_Writer->PushIntAttr("shadow", light.CastShadow);
				_Writer->PopTag();
			}
		}

		FORCEINLINE void SavePointLightComponent(ECS::EntityID entity) {
			if (_Registry->HasComponent<ECS::PointLight>(entity)) {
				const auto& light = _Registry->GetComponent<ECS::PointLight>(entity);
				_Writer->PushTag("Component");
				_Writer->PushStringAttr("type", "Point Light");
				_Writer->PushFloatAttr("r", light.Radiance.r);
				_Writer->PushFloatAttr("g", light.Radiance.g);
				_Writer->PushFloatAttr("b", light.Radiance.b);
				_Writer->PushFloatAttr("intensity", light.Intensity);
				_Writer->PushFloatAttr("bias", light.Bias);
				_Writer->PushIntAttr("shadow", light.CastShadow);
				_Writer->PopTag();
			}
		}

		FORCEINLINE void SaveDirectLightComponent(ECS::EntityID entity) {
			if (_Registry->HasComponent<ECS::DirectionalLight>(entity)) {
				const auto& light = _Registry->GetComponent<ECS::DirectionalLight>(entity);
				_Writer->PushTag("Component");
				_Writer->PushStringAttr("type", "Directional Light");
				_Writer->PushFloatAttr("r", light.Radiance.r);
				_Writer->PushFloatAttr("g", light.Radiance.g);
				_Writer->PushFloatAttr("b", light.Radiance.b);
				_Writer->PushFloatAttr("intensity", light.Intensity);
				_Writer->PushFloatAttr("bias", light.Bias);
				_Writer->PushIntAttr("shadow", light.CastShadow);
				_Writer->PopTag();
			}
		}

	private:
		ResourceManager* _Loader;
		ECS::Registry* _Registry;
		Unique<XML::Writer> _Writer;
		Unique<XML::Reader> _Reader;
	};
}