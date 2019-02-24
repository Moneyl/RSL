#include "ScriptSelectGui.h"
#include "ScriptManager.h"
#include "TextEditor.h"

ScriptSelectGui::ScriptSelectGui()
{

}

ScriptSelectGui::~ScriptSelectGui()
{

}

void ScriptSelectGui::Initialize(bool* _OpenState)
{
	OpenState = _OpenState;

	WindowFlags = 0;
	//WindowFlags |= ImGuiWindowFlags_NoTitleBar;
	//WindowFlags |= ImGuiWindowFlags_NoScrollbar;
	//WindowFlags |= ImGuiWindowFlags_MenuBar;
	//WindowFlags |= ImGuiWindowFlags_NoMove;
	//WindowFlags |= ImGuiWindowFlags_NoResize;
	//WindowFlags |= ImGuiWindowFlags_NoCollapse;
	//WindowFlags |= ImGuiWindowFlags_NoNav;
	//WindowFlags |= ImGuiWindowFlags_NoBackground;
	//WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void ScriptSelectGui::Draw(const char* Title)
{
	if (!*OpenState)
	{
		return;
	}
	if (!ImGui::Begin(Title, OpenState, WindowFlags))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Rescan"))
	{
		Scripts->ScanScriptsFolder();
	}
	for (auto i = Scripts->Scripts.begin(); i != Scripts->Scripts.end(); ++i)
	{
		ImGui::Columns(2);
		ImGui::Text(i->Name.c_str()); ImGui::SameLine();
		ImGui::NextColumn();
		ImGui::SetColumnWidth(-1, 200.0f);

		//ImGui::PushID(0);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4());
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4());
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.556f, 0.823f, 0.541f, 1.0f));
		//ImGui::Columns(2);
		if (ImGui::Button(std::string(std::string(ICON_FA_PLAY) + u8"##" + i->FullPath).c_str()))
		{
			size_t ScriptIndex = std::distance(Scripts->Scripts.begin(), i);
			bool Result = Scripts->RunScript(ScriptIndex);
			//Logger::Log("Result from running " + Scripts->Scripts[ScriptIndex].Name + ": " + std::to_string(Result), LogInfo);
		}
		ImGui::PopStyleColor(1);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.952f, 0.545f, 0.462f, 1.0f));
		ImGui::SameLine();
		if (ImGui::Button(std::string(std::string(ICON_FA_STOP) + u8"##" + i->FullPath).c_str()))
		{

		}
		ImGui::PopStyleColor();
		ImGui::SameLine();
		if (ImGui::Button(std::string(std::string(ICON_FA_EDIT) + u8"##" + i->FullPath).c_str()))
		{
			ScriptEditor->LoadScript(i->FullPath, i->Name);
			*ScriptEditorState = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();
		ImGui::NextColumn();
		//ImGui::PopID();
	}
	ImGui::End();
}
