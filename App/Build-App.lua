project "App"
    kind "ConsoleApp"     -- or "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    files {
        path.join(_SCRIPT_DIR, "Source/**.h"),
        path.join(_SCRIPT_DIR, "Source/**.cpp"),
        path.join(_SCRIPT_DIR, "Source/**.c"),
        path.join(_SCRIPT_DIR, "Source/**.glsl")
    }

    includedirs    {
        path.join(_SCRIPT_DIR, "Source"),
        path.join(_SCRIPT_DIR, "../Core/Source")
    }
    sysincludedirs { IncludeDir.DEPS }

    links { "Core" }

    -- WINDOWS: use glfw3.lib from Dependencies/lib
    filter "system:windows"
        libdirs { LibDir.BASE }
        links   { "glfw3" }
    filter {}

    -- macOS: try fat or arch-specific static libglfw3.a first; fall back to dylib name "glfw"
	filter { "system:macosx", "architecture:arm64" }
	    libdirs { LibDir.ARM, LibDir.FAT, LibDir.BASE }
	    links   { "glfw3" }  -- static lib name on mac
	    linkoptions {
	        "-framework Cocoa","-framework IOKit",
	        "-framework CoreVideo","-framework OpenGL"
	    }
	filter { "system:macosx", "architecture:x86_64" }
	    libdirs { LibDir.X64, LibDir.FAT, LibDir.BASE }
	    links   { "glfw3" }
	    linkoptions {
	        "-framework Cocoa","-framework IOKit",
	        "-framework CoreVideo","-framework OpenGL"
	    }
	filter {}

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
