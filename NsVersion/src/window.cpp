#include <algorithm>
#include <cstring>

#include "imgui.h"
#include "../include/windows.hpp"
#include "../include/tabs.hpp"
namespace Windows {

    void SetupWindow(void) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(1280.0f, 720.0f), ImGuiCond_Once);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    };
    
    void ExitWindow(void) {
        ImGui::End();
        ImGui::PopStyleVar();
    };
    
    void MainWindow(u64 &key, bool progress) {
        Windows::SetupWindow();
        if (ImGui::Begin("NX-Shell", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
            if (ImGui::BeginTabBar("NX-Shell-tabs")) {
              Tabs::FileBrowser();
              Tabs::Settings();
              ImGui::EndTabBar();
            }
        }
        Windows::ExitWindow();

        if (progress)
            return;

    }
}
