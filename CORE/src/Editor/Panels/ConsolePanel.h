#pragma once

#include "IPanel.h"
#include "Editor/Widgets/Widgets.h"
#include "Events/Categories/EditorEvents.h"

namespace Orbit::UI {

    struct ImGuiLogApp {
        ImGuiTextBuffer Buffer;
        ImVector<int> LineOffsets;

        ImGuiLogApp() {
            Clear();
        }

        void Clear() {
            Buffer.clear();
            LineOffsets.clear();
            LineOffsets.push_back(0);
        }

        void Push(const char* fmt, ...) IM_FMTARGS(2) {
            int old_size = Buffer.size();
            va_list args;
            va_start(args, fmt);
            Buffer.appendfv(fmt, args);
            va_end(args);
            for (int new_size = Buffer.size(); old_size < new_size; old_size++) {
                if (Buffer[old_size] == '\n') {
                    LineOffsets.push_back(old_size + 1);
                }
            }
        }

        void Show(const char* title, bool* p_open = nullptr) {
            if (ImGui::Begin(title, p_open)) {

                ImGui::BeginChildFrame(ImGui::GetID("scrolling"), ImVec2(0, 0));
                {
                    if (ObToolButton(ICON_FA_BROOM)) {
                        this->Clear();
                    }

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                    const char* buf = Buffer.begin();
                    const char* buf_end = Buffer.end();

                    ImGuiListClipper clipper;
                    clipper.Begin(LineOffsets.Size);
                    while (clipper.Step()) {
                        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                            const char* line_start = buf + LineOffsets[line_no];
                            const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                            ImGui::TextUnformatted(line_start, line_end);
                        }
                    }
                    clipper.End();
                    ImGui::PopStyleVar();

                    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                        ImGui::SetScrollHereY(1.0f);
                    }
                }
                ImGui::EndChildFrame();
                ImGui::End();
            }
        }
    };

	class ConsolePanel : public IPanel {
	public:
		~ConsolePanel() = default;
		ConsolePanel(Scene* scene) : IPanel(scene) {
            Application::Dispatcher()->AddListener<ConsoleLogEvent>(OB_BIND_FN(ConsolePanel::OnConsoleLog));
        }

		void Show() {
            _Logger.Show(ICON_FA_CODE" Console", nullptr);		
		}

    private:
        void OnConsoleLog(const ConsoleLogEvent& e) {
            _Logger.Push(e.GetLogMessage().c_str());
        }

	private:
        ImGuiLogApp _Logger;
	};
}