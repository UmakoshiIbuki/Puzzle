#pragma once

inline void ImGuiShowHelp(const std::string& Text, bool ShowIcon = false)
{
	if (ShowIcon)
	{
		ImGui::SameLine();
		ImGui::TextDisabled(("?"));
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(Text.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}