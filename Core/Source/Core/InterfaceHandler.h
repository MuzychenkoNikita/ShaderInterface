#pragma once

#include "Core/Core.h"
#include "Core/Framebuffer.h"
#include "Core/ShaderHandler.h"
#include "Core/Utilities.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

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

            //ImFont* defaultFont = io->Fonts->AddFontDefault();
            //ImFont* evangelionFont = io->Fonts->AddFontFromFileTTF("Source/Fonts/Times-New-Roman-MT-Std-Bold-Condensed.otf", 20.0f);

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
            
            SHAD::addTexture2D(addImportTextureID, "/Users/nikitamuzycenko/VSRepos/ShaderInterface/App/Source/Textures/icon.png");
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
            if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar)) {
                ImVec2 avail = ImGui::GetContentRegionAvail();
                const float aspect = mCustomFramebuffer.GetSize().x / mCustomFramebuffer.GetSize().y;

                float w = avail.x;
                float h = w / aspect;

                if (h > avail.y) {
                    h = avail.y;
                    w = h * aspect;
                }

                // center inside
                ImVec2 cursor = ImGui::GetCursorPos();
                ImGui::SetCursorPos(ImVec2(cursor.x + (avail.x - w) * 0.5f,
                                           cursor.y + (avail.y - h) * 0.5f));

                ImGui::Image(
                    (ImTextureID)(intptr_t)mCustomFramebuffer.GetTextureID(),
                    ImVec2((float)(int)w, (float)(int)h),
                    ImVec2(0.0f, 1.0f),
                    ImVec2(1.0f, 0.0f)
                );
            }
            ImGui::End();

            // Editor
            if (ImGui::Begin("Editor", NULL, NULL)) {
                static float textScrollY = 2.0f;
                std::string str = mMainShader.getFragmentCode();
                static char text[9256];
                // Copy string content
                std::strncpy(text, str.c_str(), sizeof(text));
                // Ensure null-termination
                text[sizeof(text) - 1] = '\0';
                
                ImGui::SetNextWindowScroll(ImVec2(0.0f, textScrollY));
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
                if (ImGui::BeginChild("TextArea", ImVec2(-FLT_MIN, -FLT_MIN), false, NULL))
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
                    textScrollY = ImGui::GetScrollY();
                    //ImGui::PopFont();
                }ImGui::EndChild();
            }ImGui::End();
            
            char timeBuffer[50];
            sprintf(timeBuffer, "%.2f", glfwGetTime());
            
            if (ImGui::Begin("Input", NULL, NULL)) {
                if (ImGui::Button("Reset Time")) {
                    glfwSetTime(0.0);
                }
                ImGui::SameLine();
                ImGui::TextUnformatted(timeBuffer);
                
                ImGui::SameLine();
                
                static int selectedResolution = -1;
                const char* resolutions[] = { "1920/1080", "1920/1440", "1000/1000", "custom..", };
                static const char* resolutionInfo = "1000/1000";

                // Simple selection popup (if you want to show the current selection inside the Button itself,
                // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
                if (ImGui::Button("Resolution"))
                    ImGui::OpenPopup("Resolution_popup");
                ImGui::SameLine();
                ImGui::TextUnformatted(resolutionInfo);
                if (ImGui::BeginPopup("Resolution_popup")) {
                    //ImGui::SeparatorText("Aquarium");
                    for (int i = 0; i < IM_ARRAYSIZE(resolutions); i++)
                        if (ImGui::Selectable(resolutions[i]))
                            selectedResolution = i;
                    ImGui::EndPopup();
                    switch (selectedResolution) {
                        case 0:
                            mCustomFramebuffer.SetSize(1920, 1080);
                            resolutionInfo = "1920/1080";
                            std::cout << "Resolution is set to 1920/1080\n";
                            break;
                        case 1:
                            mCustomFramebuffer.SetSize(1920, 1440);
                            resolutionInfo = "1920/1440";
                            std::cout << "Resolution is set to 1920/1440\n";
                            break;
                        case 2:
                            mCustomFramebuffer.SetSize(1000, 1000);
                            resolutionInfo = "1000/1000";
                            std::cout << "Resolution is set to 1000/1000\n";
                            break;
                        case 3:
                            ImGui::OpenPopup("Custom Resolution");
                            std::cout << "User used custom resolution\n";
                            break;
                        default:
                            break;
                    }
                }
                
                
                // iChannel Inputs
                ImGui::Dummy(ImVec2(0, 15));
                    // No tint
                static int iChannelsCount = 1;
                static std::string iChannelsCountString;
                for (int i = 0; i < iChannelsCount; i++){
                    
                    
                    /*if (ImGui::ImageButton("iChannel" + i, (ImTextureID)mCustomFramebuffer.GetTextureID(), size, uv0, uv1, bg_col, tint_col)) {
                        
                    }*/
                    std::string name = "iChannel" + std::to_string(i);
                    if (ImGui::Button(name.c_str(), ImVec2(200, 200))) {
                        ImGui::OpenPopup("iChannelPopUp");
                    }
                    
                    ImGui::SameLine();
                    ImGui::Dummy(ImVec2(15, 0));
                    ImGui::SameLine();
                }
                ImGui::SameLine();
                
                if (ImGui::Button("+", ImVec2(200, 200)))
                {
                    iChannelsCount++;
                    std::cout<<"Number of iChannels now is: " + iChannelsCount + '\n';
                }
                
                
                // Custom Resolution Pop Up
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                if (ImGui::BeginPopupModal("Custom Resolution", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Choose the viewport resolution");
                    ImGui::Separator();

                    static char bufW[6] = ""; ImGui::InputText("Width",   bufW, 6, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterNoDot);
                    //ImGui::SameLine();
                    ImGui::Separator();
                    static char bufH[6] = ""; ImGui::InputText("Height",   bufH, 6, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterNoDot);
                    
                    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); mCustomFramebuffer.SetSize(strtoul(bufW, NULL, 10), strtoul(bufH, NULL, 10));
                        selectedResolution = -1;
                        static std::string s;
                        s = bufW; s += '/'; s += bufH;
                        resolutionInfo = s.c_str();
                    }
                    
                    ImGui::EndPopup();
                }
                
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                if (ImGui::BeginPopupModal("iChannelPopUp", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Choose the iChannel Input");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(410);
                    if (ImGui::Button("x", ImVec2(0, 0))) { ImGui::CloseCurrentPopup();}
                    ImGui::Separator();

                    if (ImGui::Button("Texture", ImVec2(100, 100))) {
                        try {
                            const char* path = tinyfd_openFileDialog("Choose an apropriate iChannel format file", "", 0, NULL, NULL, 0);
                            if (!path) { throw std::runtime_error("Open canceled by user."); }
                            //=====
                            
                            //=====
                            std::cout << "iChannel input is now set to Texture";
                        } catch (const std::exception& e) { std::cerr << "[Open Error] " << e.what() << "\n"; }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cubemap", ImVec2(100, 100))) {
                        try {
                            const char* path = tinyfd_openFileDialog("Choose an apropriate iChannel format file", "", 0, NULL, NULL, 0);
                            if (!path) { throw std::runtime_error("Open canceled by user."); }
                            //=====
                            
                            //=====
                            std::cout << "iChannel input is now set to Cubemap";
                        } catch (const std::exception& e) { std::cerr << "[Open Error] " << e.what() << "\n"; }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Audio", ImVec2(100, 100))) {
                        try {
                            const char* path = tinyfd_openFileDialog("Choose an apropriate iChannel format file", "", 0, NULL, NULL, 0);
                            if (!path) { throw std::runtime_error("Open canceled by user."); }
                            //=====
                            
                            //=====
                            std::cout << "iChannel input is now set to Audio";
                        } catch (const std::exception& e) { std::cerr << "[Open Error] " << e.what() << "\n"; }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Video", ImVec2(100, 100))) {
                        try {
                            const char* path = tinyfd_openFileDialog("Choose an apropriate iChannel format file", "", 0, NULL, NULL, 0);
                            if (!path) { throw std::runtime_error("Open canceled by user."); }
                            //=====
                            
                            //=====
                            std::cout << "iChannel input is now set to Video";
                        } catch (const std::exception& e) { std::cerr << "[Open Error] " << e.what() << "\n"; }
                    }
                    // ================================================'
                    if (ImGui::Button("WebCam", ImVec2(100, 100))) {
                        
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Keyboard", ImVec2(100, 100))) {
                        
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Microphone", ImVec2(100, 100))) {
                        
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("BufferX", ImVec2(100, 100))) {
                        
                    }
                    
                    
                    
                    ImGui::EndPopup();
                }
                
                
            }ImGui::End();
            
            ImGui::ShowDemoWindow();

            ImGui::Render();
        }
        
        struct TextFilters
        {
            static int FilterNoDot(ImGuiInputTextCallbackData* data) {
                if (data->EventChar == '.' ) return 1; // reject dot
                return 0;                               // accept everything else allowed by the flag
            }
        };
        
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
            if (ImGui::MenuItem("New")) {
                try {
                    const char* path = tinyfd_saveFileDialog("", "", 0, NULL, NULL);
                    if (!path) { throw std::runtime_error("Save canceled by user."); }
                    mMainShader.setFragmentPath(path);
                    mMainShader.newFragmentFile();
                    std::cout << "New file created at " + std::string(mMainShader.getFragmentPath()) + "\n";
                } catch (const std::exception& e) { std::cerr << "[Save Error] " << e.what() << "\n"; }
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                try {
                    const char* path = tinyfd_openFileDialog("Choose a shader file", "", 0, NULL, ".glsl", 0);
                    if (!path) { throw std::runtime_error("Open canceled by user."); }
                    mMainShader.setFragmentPath(path);
                    mMainShader.openFragmentFile();
                    std::cout << "Opened file from " + std::string(mMainShader.getFragmentPath()) + "\n";
                } catch (const std::exception& e) { std::cerr << "[Open Error] " << e.what() << "\n"; }
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
                if (mMainShader.getHasPath()) {
                    mMainShader.saveFragmentFile();
                    std::cout << "File saved as " + std::string(mMainShader.getFragmentPath()) + "\n";
                }
                else {
                    try {
                        const char* path = tinyfd_saveFileDialog("", "", 0, NULL, NULL);
                        if (!path) { throw std::runtime_error("Save canceled by user."); }
                        mMainShader.setFragmentPath(path);
                        mMainShader.saveFragmentFile();
                        std::cout << "File saved as " + std::string(mMainShader.getFragmentPath()) + "\n";
                    } catch (const std::exception& e) { std::cerr << "[Save Error] " << e.what() << "\n"; }
                }
            }
            if (ImGui::MenuItem("Save As..")) {
                try {
                    const char* path = tinyfd_saveFileDialog("", "", 0, NULL, NULL);
                    if (!path) { throw std::runtime_error("Save canceled by user."); }
                    mMainShader.setFragmentPath(path);
                    mMainShader.saveFragmentFile();
                    std::cout << "File saved as " + std::string(mMainShader.getFragmentPath()) + "\n";
                } catch (const std::exception& e) { std::cerr << "[Save Error] " << e.what() << "\n"; }
            }
        }

    private:
        GLFWwindow* mWindow;
        Framebuffer& mCustomFramebuffer;
        Shader& mMainShader;
        float mFontMultiplier;
        ImGuiIO* io;
        uint32_t addImportTextureID;
    };


}
