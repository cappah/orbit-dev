#pragma once

#include "ImGui/FA.h"
#include "Editor/Colors.h"

namespace Orbit::UI {

	const float TOOL_BUTTON_SIZE = 24.0f;

	FORCEINLINE static bool ObSmallButton(const char* label, ImVec4 color = DARKGRAY) {
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[2]);
		bool isClicked = ImGui::ButtonEx(label, ImVec2(lineHeight, lineHeight));
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[0]);

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		return isClicked;
	}

	FORCEINLINE static bool ObToolButton(const char* id, ImVec4 color = DARK, ImVec2 size = ImVec2(TOOL_BUTTON_SIZE + 1, TOOL_BUTTON_SIZE), float rounding = 2.0f) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[2]);
		bool isClicked = ImGui::ButtonEx(id, size);
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[0]);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		return isClicked;
	}

	FORCEINLINE static bool ObCenteredButton(const char* label, ImVec4 color = DARKGRAY, float height = 28.0f, float padding = 50.0f, float rounding = 2.0f) {
		ImGui::Dummy(ImVec2(padding, 0));
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));

		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[1]);
		bool isClicked = ImGui::ButtonEx(label, ImVec2(-padding, height));
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[0]);

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		return isClicked;
	}

	FORCEINLINE static bool ObButton(const char* label, ImVec4 color = DARKGRAY, float width = 120, float height = 28.0f, float rounding = 1.0f) {
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);

		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[1]);
		bool isClicked = ImGui::ButtonEx(label, ImVec2(width, height));
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[0]);

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		return isClicked;
	}
}
