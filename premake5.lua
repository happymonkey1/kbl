include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "kbl"
    architecture "x86_64"
    startproject "kbl++"

    configurations
    {
        "Debug",
        "Release"
    }

    solution_items
    {
        
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"



group "Dependencies"
    include "vendor/premake"
group ""

include "kbl++"