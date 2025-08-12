project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    files {
        path.join(_SCRIPT_DIR, "Source/**.h"),
        path.join(_SCRIPT_DIR, "Source/**.cpp"),
        path.join(_SCRIPT_DIR, "Source/**.c")
    }

    includedirs    { path.join(_SCRIPT_DIR, "Source") }
    sysincludedirs { IncludeDir.DEPS }   -- makes <glad/glad.h>, <imgui/imgui.h> work

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir   ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"
