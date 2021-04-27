#pragma once

#include "ImGui/FA.h"
#include "Maths/Maths.h"
#include "Editor/Colors.h"
#include "ImGui/imgui_internal.h"

namespace Orbit::UI {
	#define FIELD(fa, label) fa "  " label

	const float COLUMN_WIDTH = 110.0f;
	const int32 TEXT_BUFFER_SIZE = 32;

	const ImGuiWindowFlags OVERLAY_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

	const ImGuiColorEditFlags INPUT_TEXT_FLAGS = ImGuiInputTextFlags_EnterReturnsTrue | 
		ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_NoHorizontalScroll;

	const ImGuiColorEditFlags COLOR_EDIT_FLAGS = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoBorder;

	const ImGuiTreeNodeFlags COLLAPSE_HEADER_FLAGS = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap;

	// BASICS INPUTS

	FORCEINLINE static void ObTextField(const char* label) {
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[1]);
		ImGui::Text(label);
		ImGui::SetCurrentFont(ImGui::GetIO().Fonts->Fonts[0]);
	}

	FORCEINLINE static bool ObCheckBox(const char* id, bool& out) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, WHITEGRAY);
		bool hasChanged = ImGui::Checkbox(id, &out);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		return hasChanged;
	}
	
	FORCEINLINE static bool ObColorEdit3(const char* id, vec3f& out) {
		ImGui::PushStyleColor(ImGuiCol_Text, DARK);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, WHITEGRAY);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		bool hasChanged = ImGui::ColorEdit3(id, &out.r, COLOR_EDIT_FLAGS);
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
		return hasChanged;
	}

	// TEXT INPUTS

	FORCEINLINE static bool ObTextInput(char* buffer, uint32 length = TEXT_BUFFER_SIZE) {
		ImGui::PushStyleColor(ImGuiCol_Text, DARK);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, WHITEGRAY);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		bool isChanging = ImGui::InputText("##EnttName", buffer, length, INPUT_TEXT_FLAGS);
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
		return isChanging;
	}

	FORCEINLINE static bool ObComboBox(const char* name, const std::vector<std::string>& data, const char*& selected) {
		ImGui::PushStyleColor(ImGuiCol_Text, DARK);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, WHITEGRAY);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, WHITEGRAY);
		//ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, WHITE);

		ImGui::PushStyleColor(ImGuiCol_Header, WHITE);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, WHITEGRAY);

		ImGui::PushStyleColor(ImGuiCol_Button, WHITEGRAY);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, WHITEGRAY);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, WHITEGRAY);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

		// any changes made
		bool has_changed = false;

		if (ImGui::BeginCombo(name, selected)) {
			for (int32 n = 0; n < data.size(); n++) {
				const char* current = data[n].c_str();
				bool is_selected = strcmp(selected, current);

				if (ImGui::Selectable(current, is_selected)) {
					selected = current;	
					has_changed = true;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::PopStyleColor(8);
		ImGui::PopStyleVar();
		return has_changed;
	}

	// FLOAT INPUTS

	FORCEINLINE static bool ObDragFloat(const char* id, float& out, float speed = 0.1f, float min = 0.0f, float max = 0.0f, const char* format = "%.2f") {
		ImGui::PushStyleColor(ImGuiCol_Text, DARK);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, WHITEGRAY);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		bool isDragging = ImGui::DragFloat(id, &out, speed, min, max, format);
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
		return isDragging;
	}

	FORCEINLINE static bool ObVec3Input(const char* label, vec3f& output, float rValue = 0.0f, float speed = 0.1f, float min = 0.0f, float max = 0.0f) {		
		ImGui::PushID(label);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

		std::bitset<3> fields(0);
		std::bitset<3> buttons(0);

		ImGui::Columns(2, "FieldLayout", false);
		{
			ImGui::SetColumnWidth(0, COLUMN_WIDTH);
			ObTextField(label);
			ImGui::NextColumn();

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize(lineHeight, lineHeight);			
			float width = ImGui::GetContentRegionAvailWidth() - (COLUMN_WIDTH / 2.0f) - 12.0f;

			ImGui::PushMultiItemsWidths(3, width);

			// X-AXIS			

			ImGui::PushStyleColor(ImGuiCol_Button, RED);
			if (ImGui::Button("X", buttonSize)) {
				buttons.set(0);
				output.x = rValue;
			}
			ImGui::SameLine();
			fields.set(0, ObDragFloat("##x", output.x, speed, min, max));
			ImGui::PopItemWidth();

			// Y-AXIS

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, GREEN);
			if (ImGui::Button("Y", buttonSize)) {
				buttons.set(1);
				output.y = rValue;
			}
			ImGui::SameLine();
			fields.set(1, ObDragFloat("##y", output.y, speed, min, max));
			ImGui::PopItemWidth();

			// Z-AXIS

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, BLUE);
			if (ImGui::Button("Z", buttonSize)) {
				buttons.set(2);
				output.z = rValue;
			}
			ImGui::SameLine();
			fields.set(2, ObDragFloat("##z", output.z, speed, min, max));
			ImGui::PopItemWidth();
		}

		ImGui::Columns(1);
		ImGui::PopID();	

		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
		return fields.any() || buttons.any();
	};

	// SELECTABLE

	FORCEINLINE static bool ObSelectable(const char* name, int32 current, int32 selected) {	
		bool isSelected = ImGui::Selectable(name, (current == selected), ImGuiSelectableFlags_AllowItemOverlap | ImGuiSelectableFlags_SpanAvailWidth);
		return isSelected;
	}
	
	// IMAGE VIEW

	FORCEINLINE static void ObImageView(uint32 texid, ImVec2 size = ImVec2(40, 40)) {
		ImGui::Image((ImTextureID)texid, size, ImVec2(0, 1), ImVec2(1, 0), WHITE);
	}

	FORCEINLINE static void ObTextureView(uint32 texid) {
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Image((ImTextureID)texid, ImVec2(lineHeight, lineHeight), ImVec2(0, 1), ImVec2(1, 0), WHITE);
		ImGui::PopStyleVar();
	}
}