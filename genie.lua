solution "GraphicCookie"
	
	location "./build"

	configurations{
		"Release",
		"Debug"
	}

	platforms{
		"x32",
		"x64"
	}

	flags{
		"WinMain"
	}

	location ".."

	project "GraphicCookieEngine"
		targetname "GraphicCookieEngine"
		language "C++"
		kind "WindowedApp"
		files{
			"./src/**.cpp",
			"./include/**.hpp"
		}
		
		includedirs{
			"$(DXSDK_DIR)Include",
			"./include"
		}

		links{
			"dxgi",
			"d3d11",
			"d3dx11",
			"d3dx10"
		}

		postbuildcommands { "xcopy /c /s /i /y .\\data .\\build\\data" }

		configuration "Debug"
			defines     { "DEBUG_CONFIG" }
			flags       { "Symbols" }
			targetdir  "bin/debug" 
			postbuildcommands { "xcopy /c /s /i /y .\\data .\\bin\\debug\\data" }

		configuration "Release"
			defines     { "RELEASE_CONFIG" }
			flags       { "OptimizeSize" }
			targetdir   "bin/release" 
			postbuildcommands { "xcopy /c /s /i /y .\\data .\\bin\\release\\data" }

		configuration "x32"
			libdirs     { "$(DXSDK_DIR)Lib/x86" }

		configuration "x64"
			libdirs     { "$(DXSDK_DIR)Lib/x64" }