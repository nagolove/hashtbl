workspace "xcaustic"
    configurations { "Debug", "Release" }

    includedirs { 
    }
    buildoptions { 
        "-ggdb3",
        "-fPIC",
        "-Wall",
        "-Werror",
        "-Wno-strict-aliasing",
        --"-Wno-unused-function",
    }
    links { 
    }
    libdirs { 
    }
    language "C"
    kind "ConsoleApp"
    --targetdir "bin/%{cfg.buildcfg}"
    --targetdir "bin/%{cfg.buildcfg}"
    targetprefix ""
    targetdir "."

    project "fifo_circular"
        files { "**.h", "fifo_circular.c" }

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
