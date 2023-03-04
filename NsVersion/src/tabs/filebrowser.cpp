#include <algorithm>
#include <cstring>

#include "imgui.h"
#include "imgui_internal.h"
#include "tabs.hpp"



namespace Tabs {
    static const ImVec2 tex_size = ImVec2(21, 21);

    void FileBrowser() {
        if (ImGui::BeginTabItem("File Browser")) {
            ImGui::Dummy(ImVec2(0.0f, 1.0f)); // Spacing

            ImGui::PushID("device_list");
            ImGui::PushItemWidth(160.f);
            if (ImGui::BeginCombo("","TEST")) {

                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
            ImGui::PopID();
            
            ImGui::SameLine();

            // Display current working directory
            ImGui::Text("/");
            
            // Draw storage bar
            ImGui::Dummy(ImVec2(0.0f, 1.0f)); // Spacing
            ImGui::Dummy(ImVec2(0.0f, 1.0f)); // Spacing

            // ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable | ImGuiTableFlags_BordersInner |
            //     ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY;
            
         
            ImGui::EndTabItem();
        }
    }
}
