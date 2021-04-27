#pragma once

#include "IModal.h"
#include "Editor/Widgets/Selectable.h"

namespace Orbit::UI {
	class TexturesModal : public IModal {
	public:
		~TexturesModal() = default;
		TexturesModal() : IModal() {
			Application::Dispatcher()->AddListener<MaterialTextureSelectedEvent>(OB_BIND_FN(TexturesModal::OnSelected));

			const auto& list = Application::GetScene()->Loader()->GetTextureList();
			for (auto& texture : list) {
				_Items.push_back(ObTextureSelectable(texture, Application::Dispatcher()));
			}
		}

		FORCEINLINE void Show() {
			ImGui::PushStyleColor(ImGuiCol_TitleBg, BLUE);
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, BLUE);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));

			ImGui::Begin(ICON_FA_IMAGES " Textures", &_Open, flags);
			{	
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar();

				ImGui::BeginChildFrame(ImGui::GetID("content"), ImVec2(267, 300));
				{
					int column = 0;
					for (auto& texture : _Items) {
						column++;
						texture.Show();
						if (column < 2) { ImGui::SameLine(); }
						else { column = 0; }
					}

					ImGui::Dummy(ImVec2(0, 0));
					ImGui::EndChildFrame();
				}

				ImGui::Separator();
				ImGui::SetCursorPosX(150.0f);
				if (ObButton(ICON_FA_TIMES_CIRCLE " Cancel", BLUE, 120.0f)) {

				}
			}
			ImGui::End();
		}

		FORCEINLINE void OnSelected(const MaterialTextureSelectedEvent& e) {
			auto tex = e.GetTexture();
			OB_INFO("texture selected %s %u", tex.Name.c_str(), tex.TextureID);
		}

	private:
		std::vector<ObTextureSelectable> _Items;
	};
}