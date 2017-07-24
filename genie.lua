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

	project "GraphicCookieEngine"
		targetname "GraphicCookieEngine"
		language "C++"
		kind "WindowedApp"
		files{
			"./src/*.cpp",
			"./include/*.hpp"
		}
		
		includedirs{
			"$(DXSDK_DIR)Include"
		}

		libdirs{
			"$(DXSDK_DIR)Lib/x64"
		}

		links{
			"dxgi",
			"d3d11",
			"d3dx11",
			"d3dx10"
		}

		configuration "Debug"
			defines     { "DEBUG_CONFIG" }
			flags       { "Symbols" }

		configuration "Release"
			defines     { "RELEASE_CONFIG" }
			flags       { "OptimizeSize" }
