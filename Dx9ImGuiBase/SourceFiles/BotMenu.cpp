#include "ImGui/imgui.h"
#include <Windows.h>
#include <string>
#include <BotMenus.h>
#include <Console.h>

void BotMenu()
{
    ImGui::SetNextWindowBgAlpha(0.725f);
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Menu", NULL, ImGuiWindowFlags_MenuBar))
    {   
        //if (ImGui::BeginMenuBar())
        //{
        //    if (ImGui::BeginMenu("View"))
        //    {
        //        if (ImGui::BeginMenu("Config"))
        //        {
        //            ImGui::Text("Test:)");
        //            ImGui::EndMenu();
        //        }
        //        ImGui::Separator();
        //        ImGui::EndMenu();
        //    }
        //}
        //ImGui::EndMenuBar();
    }

    if (ImGui::CollapsingHeader("Player Info"))
    {
        ImGui::TreePush("PlayerInfoTree"); 
        //int GUID = *((int*)0xCA1238);
        ////char* PlayerName = *((char**)0xC79D18);
        //const char* playerName = (const char*)0xC79D18;
        //ImGui::Text("Player GUID: %d", GUID);
        //ImGui::Text("Player Name: %s", playerName);
        ImGui::Text("Hello!", 0);
        ImGui::TreePop();
    }

    ImGui::End();
}