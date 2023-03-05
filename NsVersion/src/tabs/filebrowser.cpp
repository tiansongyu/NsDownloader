#include <algorithm>
#include <cstring>

#include "imgui.h"
#include "imgui_internal.h"
#include "tabs.hpp"
#include <iostream>
namespace Tabs {
static const ImVec2 tex_size = ImVec2(21, 21);

typedef struct {
  std::vector<bool> checked;
  std::vector<bool> checked_copy;
  std::string cwd = "";
  std::string device = "";
  u64 count = 0;
} WindowCheckboxData;

typedef struct {
  WINDOW_STATES state = WINDOW_STATE_FILEBROWSER;
  u64 selected = 0;
  std::vector<FsDirectoryEntry> entries;
  WindowCheckboxData checkbox_data;
  s64 used_storage = 0;
  s64 total_storage = 0;
  long unsigned int frame_count = 0;
  float zoom_factor = 1.0f;
} WindowData;

/// Directory entry.
typedef struct {
  char name[FS_MAX_PATH];  ///< Entry name.
  u8 pad[3];
  s8 type;        ///< See FsDirEntryType.
  u8 pad2[3];     ///< ?
  s64 file_size;  ///< File size.
} FsDirectoryEntry;

void FileBrowser() {
  FsDirectoryEntry dir_1;
  WindowData data;

  if (ImGui::BeginTabItem("File Browser")) {
    ImGui::Dummy(ImVec2(0.0f, 1.0f));  // Spacing

    ImGui::PushID("device_list");
    ImGui::PushItemWidth(160.f);
    if (ImGui::BeginCombo("", "TEST")) {
      ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::PopID();

    ImGui::SameLine();

    // Display current working directory
    ImGui::Text("/");

    if (ImGui::BeginTable("Directory List", 2, 0)) {
      // Make header always visible
      // ImGui::TableSetupScrollFreeze(0, 1);

      ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoSort |
                                      ImGuiTableColumnFlags_NoHeaderLabel |
                                      ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Filename", ImGuiTableColumnFlags_DefaultSort);
      ImGui::TableHeadersRow();
      ImGui::TableNextRow();

      std::vector<std::string> vec_name{"test_1", "test_2", "test_3"};
      for (u64 i = 0; i < vec_name.size(); i++) {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::PushID(i);

        ImGui::PopID();

        ImGui::TableNextColumn();

        // ImGui::SameLine();

        if (ImGui::Selectable(vec_name[i].data(), false)) {
          if (true) {
            // Reset navigation ID -- TODO: Scroll to top
            ImGuiContext& g = *GImGui;
            ImGui::SetNavID(ImGui::GetID(vec_name[i].data(), 0), g.NavLayer,
                            0, ImRect());

            // Reapply sort
            ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs();
            sorts_specs->SpecsDirty = true;
          } else {
            std::cout << " fail" << std::endl;
          }
        }

        if (ImGui::IsItemHovered()) data.selected = i;
      }

      ImGui::EndTable();
    }

    // Draw storage bar
    ImGui::Dummy(ImVec2(0.0f, 1.0f));  // Spacing
    ImGui::Dummy(ImVec2(0.0f, 1.0f));  // Spacing

    // ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable |
    // ImGuiTableFlags_Sortable | ImGuiTableFlags_BordersInner |
    //     ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchProp |
    //     ImGuiTableFlags_ScrollY;

    ImGui::EndTabItem();
  }
}
}  // namespace Tabs
