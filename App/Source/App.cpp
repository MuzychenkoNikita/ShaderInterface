#include "Core/Core.h"
#include "Core/Framebuffer.h"
#include "Core/ShaderHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float fontMultiplier = 1.;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shader Interface", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    SHAD::Shader mainShader("Source/Shaders/vert.glsl", "Source/Shaders/frag.glsl"); // you can name your shader files however you like

    float vertices[] = {
        // positions
        -1.0f,  1.0f, 0.0f,  // top left
        -1.0f, -1.0f, 0.0f,  // bottom left
         1.0f, -1.0f, 0.0f,  // bottom right

        -1.0f,  1.0f, 0.0f,  // top left
         1.0f, -1.0f, 0.0f,  // bottom right
         1.0f,  1.0f, 0.0f   // top right
    };

    // set up Buffers
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    SHAD::Framebuffer customFramebuffer(1920, 1080);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImFont* defaultFont = io.Fonts->AddFontDefault();
    ImFont* evangelionFont = io.Fonts->AddFontFromFileTTF("Source/Fonts/Times-New-Roman-MT-Std-Bold-Condensed.otf", 20.0f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init("#version 330");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // ImGui Render
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        // Viewport
        if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImVec2 windowSize = ImGui::GetContentRegionAvail();
            ImGui::Image((void*)customFramebuffer.GetTextureID(), ImVec2(windowSize.x, windowSize.x * 9 / 16), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        }ImGui::End();

        // Editor
        if (ImGui::Begin("Editor", NULL, ImGuiWindowFlags_MenuBar)) {
            //float lineHeightRatio = ImGui::GetTextLineHeightWithSpacing() / ImGui::GetTextMu();
            std::string str = mainShader.getFragmentCode();
            static char text[9256];
            // Copy string content
            std::strncpy(text, str.c_str(), sizeof(text));
            // Ensure null-termination
            text[sizeof(text) - 1] = '\0';

            // Line Numbers
            if (ImGui::BeginChild("LineNumbers", ImVec2(20 * fontMultiplier, -FLT_MIN), false, ImGuiWindowFlags_NoScrollbar)) {
                ImGui::SetWindowFontScale(fontMultiplier);
                int lineCount = 1;
                for (const char* c = text; *c; c++) {
                    if (*c == '\n') lineCount++;
                }

                for (int i = 1; i <= lineCount; ++i) {
                    ImGui::SetCursorPosY((i - 1) * fontMultiplier * 13 + 2.5);
                    ImGui::SetCursorPosX(20 * fontMultiplier - (ImGui::CalcTextSize("1").x * std::to_string(i).length()));
                    ImGui::Text("%d", i);
                }
            }ImGui::EndChild();

            ImGui::SameLine();

            // Text Area
            if (ImGui::BeginChild("TextArea", ImVec2(-FLT_MIN, -FLT_MIN), false, ImGuiWindowFlags_NoScrollbar))
            {
                if (ImGui::Shortcut(ImGuiModFlags_Ctrl + ImGuiKey_MouseWheelY, NULL)) {
                    fontMultiplier += (float)io.MouseWheel / 10.;
                    if (fontMultiplier > 0) ImGui::SetWindowFontScale(fontMultiplier);
                }

                //ImGui::PushFont(evangelionFont);
                static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
                ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, -FLT_MIN), flags);
                str = text;
                mainShader.setFragmentCode(str);
                mainShader.setup();
                //ImGui::PopFont();
            }ImGui::EndChild();
        }ImGui::End();

        // processes all the UI elements
        ImGui::Render();

        // render
        // ------
        customFramebuffer.BindBuffer();

        mainShader.use();
        mainShader.setFloat("iTime", glfwGetTime());
        mainShader.setVec2("iResolution", glm::vec2(SCR_WIDTH, SCR_HEIGHT));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        customFramebuffer.UnBindBuffer();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}