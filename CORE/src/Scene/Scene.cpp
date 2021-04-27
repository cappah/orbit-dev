#include "pch.h"
#include "Scene.h"
#include "Core/Application.h"

// systems
#include "ECS/Systems/SpotLightSystem.h"
#include "ECS/Systems/PointLightSystem.h"
#include "ECS/Systems/SceneCameraSystem.h"
#include "ECS/Systems/GridRendererSystem.h"
#include "ECS/Systems/MeshRendererSystem.h"
#include "ECS/Systems/EditorCameraSystem.h"
#include "ECS/Systems/DepthSamplingSystem.h"
#include "ECS/Systems/SkyboxRendererSystem.h"
#include "ECS/Systems/DirectionalLightSystem.h"

namespace Orbit {

	// define all systems !
	static ECS::SpotLightSystem s_SpotLightSystem;
	static ECS::PointLightSystem s_PointLightSystem;
	static ECS::SceneCameraSystem s_SceneCameraSystem;
	static ECS::EditorCameraSystem s_EditorCameraSystem;
	static ECS::MeshRendererSystem s_MeshRendererSystem;
	static ECS::GridRendererSystem s_GridRendererSystem;
	static ECS::DepthSamplingSystem s_DepthSamplingSystem;
	static ECS::SkyboxRendererSystem s_SkyboxRendererSystem;
	static ECS::DirectionalLightSystem s_DirectionalLightSystem;	

	void Scene::Init() {
		_Registry = MakeUnique<ECS::Registry>();
		_Renderer = MakeUnique<PlatformRenderer>(1280, 720);
		_Serializer = MakeUnique<SceneSerializer>(_Registry.get(), &_Loader);

		// Register all components
		_Registry->RegisterComponent<ECS::Common>("Common");
		_Registry->RegisterComponent<ECS::Camera>("Camera");
		_Registry->RegisterComponent<ECS::Transform>("Transform");
		_Registry->RegisterComponent<ECS::SpotLight>("Spot Light");
		_Registry->RegisterComponent<ECS::PointLight>("Point Light");
		_Registry->RegisterComponent<ECS::MeshRenderer>("MeshRenderer");
		_Registry->RegisterComponent<ECS::SkyboxRenderer>("SkyboxRenderer");
		_Registry->RegisterComponent<ECS::DirectionalLight>("Directional Light");

		// initialise callbacks
		auto disp = Application::Dispatcher();
		disp->AddListener<OpenSceneEvent>(OB_BIND_FN(Scene::OnOpenScene));
		disp->AddListener<AddEntityEvent>(OB_BIND_FN(Scene::OnAddEntity));
		disp->AddListener<KeyPressedEvent>(OB_BIND_FN(Scene::OnKeyPressed));
		disp->AddListener<AddComponentEvent>(OB_BIND_FN(Scene::OnAddComponent));
		disp->AddListener<DestroyEntityEvent>(OB_BIND_FN(Scene::OnDestroyEntity));
		disp->AddListener<RemoveComponentEvent>(OB_BIND_FN(Scene::OnRemoveComponent));

		// load shaders
		_Loader.LoadShader("data/Shaders/PBR");
		_Loader.LoadShader("data/Shaders/FLAT");
		_Loader.LoadShader("data/Shaders/GRID");
		_Loader.LoadShader("data/Shaders/DEPTH");
		_Loader.LoadShader("data/Shaders/IRMAP");
		_Loader.LoadShader("data/Shaders/SKYBOX");
		_Loader.LoadShader("data/Shaders/HDR2MAP");

		// meshes
		_Loader.LoadMesh("data/Models/Basics/cube.fbx");
		_Loader.LoadMesh("data/Models/Basics/sphere.fbx");

		// cubemap
		const char* faces[6];
		faces[CUBEMAP_RIGHT] = "data/Skybox/right.png";
		faces[CUBEMAP_LEFT] = "data/Skybox/left.png";
		faces[CUBEMAP_TOP] = "data/Skybox/top.png";
		faces[CUBEMAP_BOTTOM] = "data/Skybox/bottom.png";
		faces[CUBEMAP_FRONT] = "data/Skybox/front.png";
		faces[CUBEMAP_BACK] = "data/Skybox/back.png";
		_Loader.LoadCubeMap(faces);

		// MATERIALS
		Material _default;
		_default.Name = "default";
		_default.Data.Metallic = 0.05;
		_default.Data.Roughness = 0.02;
		_default.Data.Albedo = vec3f(0.8f, 0.2f, 4.0f);
		_Loader.AddMaterial(_default);

		// init systems
		s_SpotLightSystem.Init();
		s_PointLightSystem.Init();
		s_SceneCameraSystem.Init();
		s_EditorCameraSystem.Init();
		s_GridRendererSystem.Init();
		s_MeshRendererSystem.Init();
		s_SkyboxRendererSystem.Init();
		s_DepthSamplingSystem.Init();
		s_DirectionalLightSystem.Init();			
	}

	void Scene::Start() {								
		// start all systems
		s_SpotLightSystem.Start();
		s_PointLightSystem.Start();
		s_SceneCameraSystem.Start();
		s_EditorCameraSystem.Start();
		s_MeshRendererSystem.Start();
		s_SkyboxRendererSystem.Start();
		s_DepthSamplingSystem.Start();
		s_DirectionalLightSystem.Start();
	}

	void Scene::Update() {	
		// lights
		s_SpotLightSystem.Update();
		s_PointLightSystem.Update();
		s_DirectionalLightSystem.Update();

		// depth sampling
		s_DepthSamplingSystem.Update();		

		// renderers
		_Renderer->GetFrameBuffer()->Clear();
		s_MeshRendererSystem.Update();
		s_GridRendererSystem.Update();
		s_SkyboxRendererSystem.Update();
	}

	void  Scene::DestroyEntity(ECS::EntityID handle) {
		_Registry->Destroy(handle);		
	}

	ECS::Entity Scene::AddEntity(std::string name, const vec3f& pos) {
		const ECS::EntityID handle = _Registry->AddEntity();
		if (name.empty()) { name = "Entity" + std::to_string(handle);}
		_Registry->AddComponent<ECS::Transform>(handle, pos);
		_Registry->AddComponent<ECS::Common>(handle, name);
		return ToEntity(handle);
	}

	ECS::EntityList Scene::GetEntityList(const ECS::EntitySignature& filter) {
		ECS::EntityList list;
		for (auto handle : _Registry->GetEntityHandleList(filter)) {
			list.push_back(ToEntity(handle));
		}
		return list;
	}	

	// EVENTS CALLBACKS

	void Scene::OnOpenScene(const OpenSceneEvent& e) {
		OB_INFO("trying to parse: %s", e.GetFilename().c_str());
		_Serializer->Load(e.GetFilename());
		Application::Dispatcher()->Prioritize<SceneLoadedEvent>();
	}

	void Scene::OnSceneResized(const SceneResizedEvent& e) {
		int32 w = e.Width();
		int32 h = e.Height();
		_Renderer->SetViewport(0, 0, w, h);		
		_Renderer->GetFrameBuffer()->Validate(w, h);		
		_Renderer->GetDepthBuffer()->Validate(w, h);		
	}		

	void Scene::OnAddEntity(const AddEntityEvent& e) {
		Application::Dispatcher()->Prioritize<EntityAddedEvent>(this->AddEntity(e.GetName()));
	}

	void Scene::OnKeyPressed(const KeyPressedEvent& e) {
		auto input = Application::Inputs();
		if (input->IsKeypress(KEY_LEFT_CONTROL) && input->IsKeypress(KEY_S)) {
			_Serializer->Save("Assets/Scenes/project.obproj");
		}
	}

	void Scene::OnAddComponent(const AddComponentEvent& e) {
		if (_Registry->IsEntityActive(e.GetEntity())) {
			std::string compTypeName = e.GetTypeName();
			_Registry->AddComponent(e.GetEntity(), compTypeName);
			Application::Dispatcher()->Prioritize<ComponentAddedEvent>(ToEntity(e.GetEntity()), compTypeName);
		}
	}

	void Scene::OnDestroyEntity(const DestroyEntityEvent& e) {		
		Application::Dispatcher()->Prioritize<EntityDestroyedEvent>(e.GetEntity());
		_Registry->Destroy(e.GetEntity());
	}

	void Scene::OnRemoveComponent(const RemoveComponentEvent& e) {
			e.GetEntity().RemoveComponent(e.GetTypeID());
			Application::Dispatcher()->Prioritize<RefreshInspectorEvent>(e.GetEntity());
			OB_INFO("Component %u removed", e.GetTypeID());
		}
}