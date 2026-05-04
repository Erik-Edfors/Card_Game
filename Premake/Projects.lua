include "Settings.lua"

launchersDir = Settings.SourceDir .. "Launchers/"
group("Launchers")

    Client_LauncherDir = launchersDir .. "Client_Launcher/"
    project("Client_Launcher")
        location(Client_LauncherDir)

        kind("WindowedApp")
        language("C++")
        cppdialect("C++20")

        links {
            "Client"
        }

        files {
    		Client_LauncherDir .. "**.h",
    		Client_LauncherDir .. "**.hpp",
    		Client_LauncherDir .. "**.cpp",
    	}
    
    Server_LauncherDir = launchersDir .. "Server_Launcher/"
    project("Server_Launcher")
        location(Server_LauncherDir)

        kind("WindowedApp")
        language("C++")
        cppdialect("C++20")

        links {
            "Server",
            "CommonUtilities"
        }

        files {
    		Server_LauncherDir .. "**.h",
    		Server_LauncherDir .. "**.hpp",
    		Server_LauncherDir .. "**.cpp",
    	}

toolsDir = Settings.SourceDir .. "Tools/"
group("Tools")
    
    fontBuilderDir = toolsDir .. "FontBuilder/"
    project("FontBuilder")
        location(fontBuilderDir)

        kind("ConsoleApp")
        language("C++")
        cppdialect("C++20")

        debugargs {
            "../../Assets/",
            "--regex=^.*\\.ttf$",
            "--size=96",
            "/rpd"
        }

        links {
            "GraphicsEngine",
            "CommonUtilities",

            "d3d11.lib",
            "d2d1.lib",
            "dwrite.lib",
            "dxguid.lib",
            "dxgi.lib",
        }

        files {
    		fontBuilderDir .. "**.h",
    		fontBuilderDir .. "**.hpp",
    		fontBuilderDir .. "**.cpp",
    	}

implementationsDir = Settings.SourceDir .. "Implementations/"
group("Implementations")
    
    SharedDir = implementationsDir .. "Shared/"
    project("Shared")
        location(SharedDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        links {
            "Logger"
        }

        includedirs {
            SharedDir
        }

        pchheader("Shared.pch.h")
    	pchsource(SharedDir .. "Shared.pch.cpp")

        files {
    		SharedDir .. "**.h",
    		SharedDir .. "**.hpp",
    		SharedDir .. "**.cpp",
    	}
    
    ClientDir = implementationsDir .. "Client/"
    project("Client")
        location(ClientDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        links {
            "Shared",
            "GraphicsEngine",
            "Engine",
            "CommonUtilities",
            "Logger",

            "d3d11.lib",
            "dxguid.lib",
            "dxgi.lib",
            "d3dcompiler.lib"
        }

        includedirs {
            ClientDir
        }

        pchheader("Client.pch.h")
    	pchsource(ClientDir .. "Client.pch.cpp")

        files {
    		ClientDir .. "**.h",
    		ClientDir .. "**.hpp",
    		ClientDir .. "**.cpp",
    	}
    
    ServerDir = implementationsDir .. "Server/"
    project("Server")
        location(ServerDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        links {
            "Shared",
            "GraphicsEngine",
            "Engine",
            "CommonUtilities",
            "Logger",

            "d3d11.lib",
            "dxguid.lib",
            "dxgi.lib",
            "d3dcompiler.lib"
        }

        includedirs {
            ServerDir
        }

        pchheader("Server.pch.h")
    	pchsource(ServerDir .. "Server.pch.cpp")

        files {
    		ServerDir .. "**.h",
    		ServerDir .. "**.hpp",
    		ServerDir .. "**.cpp",
    	}

coreDir = Settings.SourceDir .. "Core/"
group("Core")

    assetsDir = coreDir .. "Assets/"
    project("Assets")
        location(assetsDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        includedirs {
            assetsDir
        }

        files {
    		assetsDir .. "**.h",
    		assetsDir .. "**.hpp"
    	}

    loggerDir = coreDir .. "Logger/"
    project("Logger")
        location(loggerDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        includedirs {
            loggerDir
        }

        files {
    		loggerDir .. "**.h",
    		loggerDir .. "**.hpp",
    		loggerDir .. "**.cpp",
    	}

    commonUtilitiesDir = coreDir .. "CommonUtilities/"
    project("CommonUtilities")
        location(commonUtilitiesDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        links {
            "Logger"
        }

        files {
    		commonUtilitiesDir .. "**.h",
    		commonUtilitiesDir .. "**.hpp",
    		commonUtilitiesDir .. "**.cpp",
    	}

    renderHardwareInterfaceDir = coreDir .. "RenderHardwareInterface/"
    project("RenderHardwareInterface")
        location(renderHardwareInterfaceDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")
        
        links {
            "CommonUtilities",
            "Logger"
        }

        includedirs {
            renderHardwareInterfaceDir
        }

        pchheader("RenderHardwareInterface.pch.h")
    	pchsource(renderHardwareInterfaceDir .. "RenderHardwareInterface.pch.cpp")

        filter("files:**DDSTextureLoader11.cpp")
            enablepch("Off")
        filter {}

        files {
    		renderHardwareInterfaceDir .. "**.h",
    		renderHardwareInterfaceDir .. "**.hpp",
    		renderHardwareInterfaceDir .. "**.cpp",
    	}
    
    graphicsEngineDir = coreDir .. "GraphicsEngine/"
    project("GraphicsEngine")
        location(graphicsEngineDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")
        
        links {
            "CommonUtilities",
            "RenderHardwareInterface",
            "Logger"
        }

        includedirs {
            graphicsEngineDir
        }

        pchheader("GraphicsEngine.pch.h")
    	pchsource(graphicsEngineDir .. "GraphicsEngine.pch.cpp")

        files {
    		graphicsEngineDir .. "**.h",
    		graphicsEngineDir .. "**.hpp",
    		graphicsEngineDir .. "**.cpp",
    	}
    
    engineDir = coreDir .. "Engine/"
    project("Engine")
        location(engineDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")
        
        links {
            "CommonUtilities",
            "GraphicsEngine",
            "Logger"
        }

        includedirs {
            engineDir
        }

        pchheader("Engine.pch.h")
    	pchsource(engineDir .. "Engine.pch.cpp")

        files {
    		engineDir .. "**.h",
    		engineDir .. "**.hpp",
    		engineDir .. "**.cpp",
    	}
        
    shadersDir = coreDir .. "Shaders/"
    project("Shaders")
        location(shadersDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        shaderobjectfileoutput("../../../Assets/Shaders/%%(Filename).cso")

        files {
            shadersDir .. "**.hlsl",
            shadersDir .. "**.hlsli"
        }

        filter("files:**.hlsl")
            shadermodel("5.0")
            shaderoptions("/WX") -- Warnings as errors

        filter("files:**PS.hlsl")
            shadertype("Pixel")

        filter("files:**VS.hlsl")
            shadertype("Vertex")

        filter("files:**GS.hlsl")
            shadertype("Geometry")
        
        filter("files:**DS.hlsl")
            shadertype("Domain")

        filter("files:**CS.hlsl")
            shadertype("Compute")

        filter("files:**MS.hlsl")
            shadertype("Mesh")
        
        filter("files:**AS.hlsl")
            shadertype("Amplification")
        
        filter("files:**FX.hlsl")
            shadertype("Effect")
