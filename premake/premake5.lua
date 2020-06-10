--Define various global values and functions--
dofile "premake_paths.lua"
dofile "premake_functions.lua"
dofile "premake_dependencies.lua"

--Begin Solution--
workspace "SwiftEngine"
	
	--Solution settings--
	location(SOLUTION_PATH)
	language			"C++"
	cppdialect			"C++17"
	characterset		"Unicode"
	
	TARGETING_X64 = 	false;	--Affects dependencies
	if TARGETING_X64 then
		architecture "x86_64"
	else
		architecture "x86"
	end
	
	configurations {
		"debug",
		"release"
	}
	filter "configurations:debug"
		runtime		"Debug"
	filter "configurations:release"
		runtime		"Release"
	filter{}
	
	--Preprocessor--
	filter "configurations:debug"
		defines{
			"CONFIGURATION=DEBUG",
		}
	filter "configurations:release"
		defines{
			"CONFIGURATION=RELEASE",
		}
	filter{}
	
	--Compiler--
	warnings			"Extra"
	stringpooling		"On"
	exceptionhandling	"Off"
	
	filter "configurations:debug"
		optimize 	"Off"
	filter "configurations:release"
		optimize 	"On"
	filter{}
	
	--Linker--
	filter "configurations:debug"
		libdirs		(BINARY_PATH.."debug")
		targetdir	(BINARY_PATH.."debug")
	filter "configurations:release"
		libdirs		(BINARY_PATH.."release")
		targetdir	(BINARY_PATH.."release")
	filter{}
	
	--Projects--
	group			"Core Projects"
	generate_project("GlobalHeaders", "StaticLib")
	generate_project("Types", "StaticLib")
	generate_project("FileIO", "StaticLib")
	generate_project("ECSystem", "StaticLib")
	
	group			""
	generate_project("Startup", "ConsoleApp")
	
---------------------------------------------

print("Finished executing premake5.lua")