#pragma once

#include "IPanel.h"
#include "Editor/Widgets/Widgets.h"

namespace Orbit::UI {
	class ResourcePanel : public IPanel {
	public:
		FORCEINLINE ~ResourcePanel() = default;

		FORCEINLINE ResourcePanel(Scene* scene) : IPanel(scene) {
			_DirectoryPath = "data";
		}

		FORCEINLINE void Show() {
			ImGui::Begin(ICON_FA_FOLDER" Resources", nullptr);
			{
				uint32_t count = 0;
				for (const auto& entry : std::filesystem::recursive_directory_iterator(_DirectoryPath)) {
					count++;
				}

				static int selection_mask = 0;
				auto clickState = DirectoryTreeViewRecursive(_DirectoryPath, &count, &selection_mask);
			}
			ImGui::End();
		}

	private:
		FORCEINLINE std::pair<bool, uint32_t> DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask) {

			ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_OpenOnArrow;

			bool any_node_clicked = false;
			uint32_t node_clicked = 0;

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				ImGuiTreeNodeFlags node_flags = base_flags;
				const bool is_selected = (*selection_mask & *count) != 0;
				if (is_selected)
					node_flags |= ImGuiTreeNodeFlags_Selected;

				std::string name = entry.path().string();

				auto lastSlash = name.find_last_of("/\\");
				lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
				name = name.substr(lastSlash, name.size() - lastSlash);

				bool entryIsFile = !std::filesystem::is_directory(entry.path());
				if (entryIsFile) {
					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				}

				name = ICON_FA_FOLDER " " + name;
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)(*count), node_flags, name.c_str());

				if (ImGui::IsItemClicked()) {
					node_clicked = *count;
					any_node_clicked = true;
				}

				(*count)--;

				if (!entryIsFile) {
					if (node_open) {
						auto clickState = DirectoryTreeViewRecursive(entry.path(), count, selection_mask);

						if (!any_node_clicked) {
							any_node_clicked = clickState.first;
							node_clicked = clickState.second;
						}

						ImGui::TreePop();
					}
					else {
						for (const auto& e : std::filesystem::recursive_directory_iterator(entry.path()))
							(*count)--;
					}
				}
			}
			return { any_node_clicked, node_clicked };
		}

	private:
		std::string _DirectoryPath;
	};
}