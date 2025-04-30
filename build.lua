workspace "StarsOfRedemption"
	architecture "x86_64"
	configurations { "Debug", "Release", "Dist" }
	startproject "WalnutApp"

	-- Workspace-wide build options for MSVC
	filter "system:windows"
		buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "bin/%{cfg.buildcfg}-%{cfg.architecture}"
WalnutNetworkingBinDir = "Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}/"

include "Walnut/Build-Walnut-External.lua"

group "StarsOfRedemption"
	include "StarsOfRedemption-Common/build-app-common.lua"
    include "StarsOfRedemption-Client/build-app-client.lua"
    include "StarsOfRedemption-Server/build-app-server.lua"
group ""