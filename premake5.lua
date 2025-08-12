workspace "Shader Interface"
    configurations { "Debug", "Release", "Dist" }
    startproject "App"

    filter "system:windows"
        architecture "x64"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
    filter "system:macosx"
        architecture "arm64"   -- change to "x86_64" if you’re on Intel
        systemversion "latest"
    filter {}

-- absolute deps
local DepsDir = path.getabsolute("Dependencies")
IncludeDir = { DEPS = path.join(DepsDir, "include") }
LibDir     = {
    BASE = path.join(DepsDir, "lib"),
    ARM  = path.join(DepsDir, "lib-arm64"),
    X64  = path.join(DepsDir, "lib-x86_64"),
    FAT  = path.join(DepsDir, "lib-universal")
}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
    include "Core/Build-Core.lua"
group ""
    include "App/Build-App.lua"
