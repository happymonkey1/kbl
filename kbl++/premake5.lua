project "kbl++"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	debugdir "%{wks.location}/"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/**.h",
		"src/**.cpp"
	}

	defines
	{

	}

	includedirs
	{
		"include",
		"src"
	}

	links
	{

	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		links
        {
            
        }
	
	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		links
        {
            
        }
