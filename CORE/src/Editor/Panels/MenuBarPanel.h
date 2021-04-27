#pragma once

#include "IPanel.h"
#include <NFD/nfd.hpp>
#include "Core/Application.h"
#include "Editor/Widgets/Widgets.h"
#include "Events/Categories/SceneEvents.h"

namespace Orbit::UI {
	class MenuBarPanel : public IPanel {
	public:
		~MenuBarPanel() = default;
		MenuBarPanel(Scene* scene) : IPanel(scene) { }

		FORCEINLINE void Show() {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5)); 
			ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[1]);
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem(ICON_FA_PLUS_CIRCLE " New Scene", "Ctrl+N")) { NewProject(); }
					if (ImGui::MenuItem(ICON_FA_FOLDER " Open Scene", "Ctrl+O")) { OpenProject(); }
					if (ImGui::MenuItem(ICON_FA_SAVE " Save", "Ctrl+S")) { OpenProject(); }
					if (ImGui::MenuItem(ICON_FA_SAVE " Save As..")) {}
					if (ImGui::MenuItem(ICON_FA_UPLOAD " Import", "Ctrl+I")) { ImportMultipleFiles(); }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Editor")) {
					if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
					if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "CTRL+X")) {}
					if (ImGui::MenuItem("Copy", "CTRL+C")) {}
					if (ImGui::MenuItem("Paste", "CTRL+V")) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Scene")) {
					if (ImGui::MenuItem("New Entity", "CTRL+A")) {}
					if (ImGui::MenuItem("Add Skybox", "CTRL+L")) {}
					if (ImGui::MenuItem("Add Spot Light", "CTRL+L")) {}
					if (ImGui::MenuItem("Add Point Light", "CTRL+L")) {}
					if (ImGui::MenuItem("Add Directional Light", "CTRL+L")) {}
					ImGui::Separator();
					if (ImGui::MenuItem("Clear", "CTRL+X")) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Settings")) {
					if (ImGui::MenuItem("Lighting", "Ctrl+T")) {}
					if (ImGui::MenuItem("Preferences", "Ctrl+T")) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help")) {
					if (ImGui::MenuItem("View Help", "Ctrl+H")) {}
					if (ImGui::MenuItem("About Orbit")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
				ImGui::PopStyleVar();
				ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[0]);
			}
		}

		FORCEINLINE void OpenProject() {
			NFD::Guard nfdGuard;
			NFD::UniquePath filePath;
			nfdfilteritem_t filter = { "project", "obproj" };
			nfdresult_t result = NFD::OpenDialog(filePath, &filter, 1, "Assets/Scenes");

			if (result == NFD_OKAY) {
				Application::Dispatcher()->Post<OpenSceneEvent>(filePath.get());				
			}
			else if (result != NFD_CANCEL) {
				OB_ERROR("File dialog error: %s", NFD::GetError());
			}			
		}

		FORCEINLINE void NewProject() {
			NFD::Guard nfdGuard;
			NFD::UniquePath targetPath;
			nfdfilteritem_t filter = { "Project", "obproj" };
			nfdresult_t result = NFD::SaveDialog(targetPath, &filter, 1, "Assets/Scenes","Untitled.obproj");

			if (result == NFD_OKAY) {
				Application::Dispatcher()->Post<NewSceneEvent>(targetPath.get());
				OB_INFO(targetPath.get());
			}
			else if (result != NFD_CANCEL) {
				OB_ERROR(NFD::GetError());
			}
		}

		FORCEINLINE void ImportMultipleFiles() {
			NFD::Guard nfdGuard;
			NFD::UniquePathSet outPaths;
			nfdfilteritem_t filterItem[2] = { { "Images", "png, jpg, tga, jpeg" } };

			nfdresult_t result = NFD::OpenDialogMultiple(outPaths, filterItem, 1);
			if (result == NFD_OKAY) {
				nfdpathsetsize_t numPaths;
				NFD::PathSet::Count(outPaths, numPaths);
				nfdpathsetsize_t i;
				for (i = 0; i < numPaths; i++) {
					NFD::UniquePathSetPath path;
					NFD::PathSet::GetPath(outPaths, i, path);
				}
			}
			else if (result == NFD_CANCEL) {
				OB_ERROR("user pressed cancel.");				
			}
			else {
				OB_ERROR("nfd error %s", NFD::GetError());				
			}
		}
	};
}