#pragma once

#include "IPanel.h"
#include "Core/Application.h"
#include "ECS/Components/Common.h"
#include "Editor/Widgets/Widgets.h"
#include "Events/Categories/SceneEvents.h"

namespace Orbit::UI {	

	struct EntityElement {
		EntityElement() = default;
		~EntityElement() = default;

		std::string Name = EMPTY_STR;
		int32 EntityId = ECS::INVALID_HANDLE;

		FORCEINLINE operator uint32() const { return EntityId; }

		std::string GetField() {
			return (ICON_FA_DICE_D6 "  " + Name);
		}
	};

	class EntityPanel : public IPanel {
	public:
		FORCEINLINE EntityPanel(Scene* scene) : IPanel(scene), _SelectedEntityID(ECS::INVALID_HANDLE) {
			for (auto&& entity : _TargetScene->GetEntityList<ECS::Common>()) { 
				PushElement(entity); 
			}

			auto disp = Application::Dispatcher();
			disp->AddListener<EntityAddedEvent>(OB_BIND_FN(EntityPanel::OnEntityAdded));
			disp->AddListener<SceneLoadedEvent>(OB_BIND_FN(EntityPanel::OnSceneLoaded));
			disp->AddListener<EntityDestroyedEvent>(OB_BIND_FN(EntityPanel::OnEntityDestroyed));
			disp->AddListener<EntityNameChangedEvent>(OB_BIND_FN(EntityPanel::OnEntityNameChanged));
		}

		FORCEINLINE void  Show() {				
			ImGui::Begin(ICON_FA_LIST_ALT" Entities"); 
			{	
				ImGui::BeginChildFrame(ImGui::GetID("enttFrame"), ImVec2(0, 0));
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 6));
					for (auto& current : _EntityElements) {
						if (ObSelectable(current.GetField().c_str(), current, _SelectedEntityID)) {
							_SelectedEntityID = current;
							Application::Dispatcher()->Post<EntitySelectedEvent>(_TargetScene->ToEntity(_SelectedEntityID));
						}
					}
					ImGui::PopStyleVar();
				}
				ImGui::EndChildFrame();
			}
			ImGui::End();
		}
	
	private:
		FORCEINLINE void PushElement(ECS::Entity entity) {
			EntityElement element;
			element.EntityId = entity.GetHandle();
			element.Name = entity.GetComponent<ECS::Common>().Name;						
			_EntityElements.push_back(element);			
		}

		FORCEINLINE void OnEntityAdded(const EntityAddedEvent& e) {
			auto entity = e.GetEntity();
			PushElement(entity);
			_SelectedEntityID = entity.GetHandle();
			Application::Dispatcher()->Post<EntitySelectedEvent>(entity);
		}

		FORCEINLINE void OnEntityDestroyed(const EntityDestroyedEvent& e) {
			auto it = std::find(_EntityElements.begin(), _EntityElements.end(), e.GetEntity());

			if (it != _EntityElements.end()) {
				_EntityElements.erase(it);
				if (!_EntityElements.empty()) {
					_SelectedEntityID = _EntityElements.back();
					Application::Dispatcher()->Post<EntitySelectedEvent>(_TargetScene->ToEntity(_SelectedEntityID));
				}
			}			
		}

		FORCEINLINE void OnEntityNameChanged(const EntityNameChangedEvent& e) {
			const std::string name = e.GetValue();

			// search if any entity allready has the name (serach by name)
			auto copy = std::find_if(_EntityElements.begin(), _EntityElements.end(), 
				[&name](const EntityElement& el) { return el.Name == name; });

			if (copy != _EntityElements.end()) {
				OB_ERROR("Invalid entity name! Name [%s] allready used!", name.c_str());
				return;
			}

			// search by entity id
			auto element = std::find(_EntityElements.begin(), _EntityElements.end(), _SelectedEntityID);
			if (element != _EntityElements.end()) {
				element->Name = name;
			}
		}

		FORCEINLINE void OnSceneLoaded(const SceneLoadedEvent& e) {
			_EntityElements.clear();
			for (auto&& entity : _TargetScene->GetEntityList<ECS::Common>()) {
				PushElement(entity);
			}

			// refresh inspektor
			Application::Dispatcher()->Post<ClearInspectorEvent>();
		}

	private:
		int32 _SelectedEntityID;
		std::vector<EntityElement> _EntityElements;
	};
}

