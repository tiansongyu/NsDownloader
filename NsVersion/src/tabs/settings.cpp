#include "imgui.h"
#include "imgui_internal.h"
#include "tabs.hpp"

namespace Tabs {
// static void Indent(const std::string &title) {
//     ImGui::Dummy(ImVec2(0.0f, 5.0f)); // Spacing
//     ImGui::TextColored(ImGui::GetStyle().Colors[ImGuiCol_CheckMark],
//     title.c_str()); ImGui::Indent(20.f); ImGui::Dummy(ImVec2(0.0f, 5.0f)); //
//     Spacing
// }

static void Separator(void) {
  ImGui::Dummy(ImVec2(0.0f, 5.0f));  // Spacing
  ImGui::Unindent();
  ImGui::Separator();
}

void Settings() {
  if (ImGui::BeginTabItem("Settings")) {
    // Developer Options Checkbox
    Tabs::Separator();

    // About

    ImGui::Text("NX-Shell");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));  // Spacing
    ImGui::EndTabItem();
  }
}
}  // namespace Tabs
