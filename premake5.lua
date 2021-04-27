--premake5.lua

workspace "ORBIT_ENGINE"
    architecture "x86_64"
    startproject "APP"
    configurations { "Debug", "Release" }

    -- ENGINE CORE DLL 

    project "CORE"
        language "C++"    
        kind "SharedLib"
        targetname "ORBIT"
        staticruntime "on"
        location "%{prj.name}"    				
        targetdir "APP/bin/%{cfg.buildcfg}"  
        objdir ("APP/bin/%{cfg.buildcfg}/obj")
        
        pchheader "pch.h"
        pchsource "%{prj.name}/src/pch.cpp"  

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs {
            "%{prj.name}/src",
            "%{prj.name}/src/Vendor",            
			"%{prj.name}/libs/includes",
        }          

        libdirs {
            "%{prj.name}/libs/lib/Shared",  
            "%{prj.name}/libs/lib/%{cfg.buildcfg}"        
        }

        links {
            "nfd.lib",
		    "glew32s.lib",
		    "glfw3dll.lib",
		    "opengl32.lib",
	    }           

        defines {
            "GLEW_STATIC",
            "OPENGL_RENDERER_API"
        }       

        -- Not working and I don't know why!
        filter "files:%{prj.name}/src/ImGui/**.cpp" 
            flags { "NoPCH" }

        filter "system:windows"   
            staticruntime "On"        
            cppdialect "C++17"
            systemversion "latest"
            defines { "OB_BUILD_DLL", "PLATFORM_WINDOWS" }

        filter "action:vs*"
            defines { "COMPILER_MSVC" }
        
        filter "configurations:Debug"
            symbols "On"	
            runtime "Debug"
			optimize "Speed"            
            defines { "OB_DEBUG" }            
            links {  "tinyxml2d.lib", "assimp-vc142-mtd.lib" }

        filter "configurations:Release"
            optimize "Speed"
            runtime "release"
            defines "OB_RELEASE"			
            flags{"LinkTimeOptimization"}
            links { "tinyxml2.lib", "assimp-vc142-mt.lib" }
    
    -- ENGINE BOOTSRAP APP 

    project "APP"
        language "C++"
		targetname "APP"
        kind "ConsoleApp"
        staticruntime "on"
        location "%{prj.name}"
        targetdir "%{prj.name}/bin/%{cfg.buildcfg}"
        objdir "%{prj.name}/bin/%{cfg.buildcfg}/obj"

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs { 
           "CORE/src",
           "%{prj.name}/src",
           "CORE/libs/includes",           
        }

        links { 
            "CORE"
        }

        defines {
            "GLEW_STATIC",
            "OPENGL_RENDERER_API"
        }       

        filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"        
        defines { "PLATFORM_WINDOWS" }
        
        filter "configurations:Debug"
            symbols "On"	
            runtime "Debug"           
            targetname "APP"

        filter "configurations:Release"          
            targetname "APP"		
            runtime "Release"
            flags{"LinkTimeOptimization"}
