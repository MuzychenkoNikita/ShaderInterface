#pragma once

#include "Core/Core.h"
#include "Core/Framebuffer.h"
#include "Core/ShaderHandler.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Core/tinyfiledialogs/tinyfiledialogs.h"


namespace SHAD 
{
    class Interface
    {
    public:
        Interface(GLFWwindow* window, Framebuffer& customFramebuffer, Shader& mainShader)
            : mWindow(window)
            , mCustomFramebuffer(customFramebuffer)
            , mMainShader(mainShader)
            , mFontMultiplier(1.)
            , io(nullptr)
        {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            io = &ImGui::GetIO(); (void)io;
            io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
            io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

            ImFont* defaultFont = io->Fonts->AddFontDefault();
            ImFont* evangelionFont = io->Fonts->AddFontFromFileTTF("Source/Fonts/Times-New-Roman-MT-Std-Bold-Condensed.otf", 20.0f);

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsLight();

            // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
            ImGuiStyle& mStyle = ImGui::GetStyle();
            mStyle.FramePadding = ImVec2(4, 2);
            mStyle.ItemSpacing = ImVec2(6, 2);
            mStyle.ItemInnerSpacing = ImVec2(6, 4);
            mStyle.Alpha = 0.95f;
            mStyle.WindowRounding = 4.0f;
            mStyle.FrameRounding = 2.0f;
            mStyle.IndentSpacing = 6.0f;
            mStyle.ItemInnerSpacing = ImVec2(2, 4);
            mStyle.ColumnsMinSpacing = 50.0f;
            mStyle.GrabMinSize = 14.0f;
            mStyle.GrabRounding = 16.0f;
            mStyle.ScrollbarSize = 12.0f;
            mStyle.ScrollbarRounding = 16.0f;

            ImGuiStyle& style = mStyle;
            style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
            style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
            style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
            style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);

            if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;

            }

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    #ifdef __EMSCRIPTEN__
            ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
    #endif
            ImGui_ImplOpenGL3_Init("#version 330");
        }
        
        ~Interface() {}

        inline void renderInterface() {
            // ImGui Render
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport();


            // Menu
            if (ImGui::BeginMainMenuBar())
            {
                ImGui::SetWindowFontScale(1.2);
                if (ImGui::BeginMenu("File"))
                {
                    showFileMenu();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                    if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                    ImGui::Separator();
                    if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                    if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                    if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("=")){
                    glfwSetWindowSize(mWindow, 1920 / 2, 1080 / 2);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("X")) {
                    glfwSetWindowShouldClose(mWindow, true);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            // Viewport
            if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar)) {
                //ImGui::SetWindowFontScale(1.2);
                ImVec2 windowSize = ImGui::GetContentRegionAvail();
                ImGui::Image((void*)mCustomFramebuffer.GetTextureID(), ImVec2(windowSize.x, windowSize.x * 9 / 16), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
            }ImGui::End();

            // Editor
            if (ImGui::Begin("Editor", NULL, ImGuiWindowFlags_NoScrollbar)) {
                std::string str = mMainShader.getFragmentCode();
                static char text[9256];
                // Copy string content
                std::strncpy(text, str.c_str(), sizeof(text));
                // Ensure null-termination
                text[sizeof(text) - 1] = '\0';

                // Line Numbers
                if (ImGui::BeginChild("LineNumbers", ImVec2(20 * mFontMultiplier, -FLT_MIN), false, ImGuiWindowFlags_NoScrollbar)) {
                    ImGui::SetWindowFontScale(mFontMultiplier);
                    int lineCount = 1;
                    for (const char* c = text; *c; c++) {
                        if (*c == '\n') lineCount++;
                    }

                    for (int i = 1; i <= lineCount; ++i) {
                        ImGui::SetCursorPosY((i - 1) * mFontMultiplier * 13 + 2.5);
                        ImGui::SetCursorPosX(20 * mFontMultiplier - (ImGui::CalcTextSize("1").x * std::to_string(i).length()));
                        ImGui::Text("%d", i);
                    }
                }ImGui::EndChild();

                ImGui::SameLine();

                // Text Area
                if (ImGui::BeginChild("TextArea", ImVec2(-FLT_MIN, -FLT_MIN), false, ImGuiWindowFlags_NoScrollbar))
                {
                    if (ImGui::Shortcut(ImGuiModFlags_Ctrl + ImGuiKey_MouseWheelY, NULL)) {
                        mFontMultiplier += (float)io->MouseWheel / 10.;
                        if (mFontMultiplier > 0) ImGui::SetWindowFontScale(mFontMultiplier);
                    }

                    //ImGui::PushFont(evangelionFont);
                    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_NoHorizontalScroll;
                    ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, -FLT_MIN), flags);
                    str = text;
                    mMainShader.setFragmentCode(str);
                    mMainShader.setup();
                    //ImGui::PopFont();
                }ImGui::EndChild();
            }ImGui::End();

            // processes all the UI elements
            ImGui::Render();
        }

        inline void deleteInterface() {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }

        void showFileMenu() {
            ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("New")) {
                mMainShader.setFragmentPath(tinyfd_saveFileDialog("", "", 0, NULL, NULL));
                mMainShader.newFragmentFile();
                std::cout << "New file created at " + std::string(mMainShader.getFragmentPath()) + "\n";
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) { 
                mMainShader.setFragmentPath(tinyfd_openFileDialog("Bruh", "", 0, NULL, NULL, 0));
                mMainShader.openFragmentFile();
                std::cout << "Opened file from " + std::string(mMainShader.getFragmentPath()) + "\n";
            }
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("fish_hat.c");
                ImGui::MenuItem("fish_hat.inl");
                ImGui::MenuItem("fish_hat.h");
                if (ImGui::BeginMenu("More.."))
                {
                    ImGui::MenuItem("Hello");
                    ImGui::MenuItem("Sailor");
                    if (ImGui::BeginMenu("Recurse.."))
                    {
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                mMainShader.saveFragmentFile();
                std::cout << "File saved as " + std::string(mMainShader.getFragmentPath()) + "\n";
            }
            if (ImGui::MenuItem("Save As..")) {
                mMainShader.setFragmentPath(tinyfd_saveFileDialog("", "", 0, NULL, NULL));
                mMainShader.saveFragmentFile();
                std::cout << "File saved as " + std::string(mMainShader.getFragmentPath()) + "\n";
            }
        }

    private:
        GLFWwindow* mWindow;
        Framebuffer& mCustomFramebuffer;
        Shader& mMainShader;
        float mFontMultiplier;
        ImGuiIO* io;
    };


}