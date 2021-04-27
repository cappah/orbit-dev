#include "pch.h"
#include "Editor.h"
#include "Core/Application.h"
#include "Modals/TexturesModal.h"

#include "Panels/ScenePanel.h"
#include "Panels/EntityPanel.h"
#include "Panels/MenuBarPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/ResourcePanel.h"
#include "Panels/InspektorPanel.h"

#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace Orbit::UI {
	void Editor::Init() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& IO = ImGui::GetIO(); (void)IO;
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		this->InitStyle();
		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH = true;
		static const ImWchar ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		const char* font = "data/Fonts/Roboto-Medium.ttf";
		const char* fonticon = "data/Fonts/FontAwesome-Solid-900.otf";

		// big icons
		config.GlyphOffset = ImVec2(0, 3);
		IO.Fonts->AddFontFromFileTTF(font, 15.0f);
		IO.Fonts->AddFontFromFileTTF(fonticon, 20.0f, &config, ranges);

		// small icons
		config.GlyphOffset = ImVec2(0, 1);
		IO.Fonts->AddFontFromFileTTF(font, 15.0f);
		IO.Fonts->AddFontFromFileTTF(fonticon, 14.0f, &config, ranges);

		// small icons
		config.GlyphOffset = ImVec2(0, 0);
		IO.Fonts->AddFontFromFileTTF(font, 14.0f);
		IO.Fonts->AddFontFromFileTTF(fonticon, 10.0f, &config, ranges);

		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui_ImplGlfw_InitForOpenGL(Application::Window()->GetHandle(), true);

		_DockFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

		Scene* scene = Application::GetScene();
		_Panels.push_back(MakeUnique<ScenePanel>(scene));
		_Panels.push_back(MakeUnique<MenuBarPanel>(scene));
		_Panels.push_back(MakeUnique<InspektorPanel>(scene));
		_Panels.push_back(MakeUnique<EntityPanel>(scene));
		_Panels.push_back(MakeUnique<ResourcePanel>(scene));
		_Panels.push_back(MakeUnique<ConsolePanel>(scene));

		// register callbacks
		Application::Dispatcher()->AddListener<ShowMaterialTextureModalEvent>(OB_BIND_FN(Editor::OnModal));
	}

	void Editor::Show() {
		this->NewFrame();
		for (auto& panel : _Panels) { panel->Show(); }
		for (auto& popup : _PopUps) { popup->Show(); }
		ImGui::ShowDemoWindow();
		this->EndFrame();
	}

	void Editor::InitStyle() {
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		colors[ImGuiCol_Tab] = TITLE_BG;
		colors[ImGuiCol_TabActive] = TITLE_BG;
		colors[ImGuiCol_TabUnfocused] = TITLE_BG;
		colors[ImGuiCol_Text] = TEXTCOLOR;
		colors[ImGuiCol_Button] = CHILDBG;
		colors[ImGuiCol_ButtonActive] = CHILDBG;
		colors[ImGuiCol_FrameBg] = FRAME_BG;
		colors[ImGuiCol_FrameBgHovered] = WHITEGRAY;
		colors[ImGuiCol_FrameBgActive] = WHITEGRAY;
		colors[ImGuiCol_ChildBg] = CHILDBG;
		colors[ImGuiCol_CheckMark] = DARK;
		colors[ImGuiCol_WindowBg] = WINBG;
		colors[ImGuiCol_Border] = BORDER;
		colors[ImGuiCol_BorderShadow] = DARK;

		colors[ImGuiCol_Header] = HEADER;
		colors[ImGuiCol_HeaderHovered] = LIGHTGRAY;
		colors[ImGuiCol_HeaderActive] = LIGHTGRAY;

		colors[ImGuiCol_MenuBarBg] = DARK;
		colors[ImGuiCol_ScrollbarBg] = DARK;
		colors[ImGuiCol_TitleBg] = TITLE_BG;
		colors[ImGuiCol_TitleBgActive] = TITLE_BG;
		colors[ImGuiCol_TabUnfocusedActive] = TITLE_BG;

		//style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ColorButtonPosition = ImGuiDir_Left;
		style.ItemInnerSpacing = ImVec2(3, 3);		
		style.WindowPadding = ImVec2(3, 3);
		style.FramePadding = ImVec2(5, 5);
		style.ItemSpacing = ImVec2(3, 3);		

		style.WindowBorderSize = 0;
		style.ChildBorderSize = 0;
		style.FrameBorderSize = 0;
		style.TabBorderSize = 0;		

		style.ScrollbarSize = 12;

		style.WindowRounding = 0;	
		style.ChildRounding = 0;
		style.FrameRounding = 0;		
		style.PopupRounding = 1;			
		style.TabRounding = 1;
	}

	void Editor::NewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		ImGuiViewport* vp = ImGui::GetMainViewport();	
		ImGui::SetNextWindowPos((vp->Pos));
		ImGui::SetNextWindowSize(vp->Size);
		ImGui::SetNextWindowViewport(vp->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		
		ImGui::Begin("Dockspace", nullptr, _DockFlags); {			
			ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0,0), ImGuiDockNodeFlags_PassthruCentralNode);
		}
		ImGui::PopStyleVar(3);
	}

	void Editor::EndFrame() {
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backupContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupContext);
		}
	}

	void Editor::OnModal(const ShowMaterialTextureModalEvent& e) {
		_PopUps.push_back(MakeUnique<TexturesModal>());
	}
}

