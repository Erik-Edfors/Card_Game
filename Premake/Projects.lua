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

implementationsDir = Settings.SourceDir .. "Implementations/"
group("Implementations")
    
    ClientDir = implementationsDir .. "Client/"
    project("Client")
        location(ClientDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        links {
            "Shared",
            "CommonUtilities"
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
            "CommonUtilities"
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

    SharedDir = coreDir .. "Shared/"
    project("Shared")
        location(SharedDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

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

    commonUtilitiesDir = coreDir .. "CommonUtilities/"
    project("CommonUtilities")
        location(commonUtilitiesDir)

        kind("StaticLib")
        language("C++")
        cppdialect("C++20")

        files {
    		commonUtilitiesDir .. "**.h",
    		commonUtilitiesDir .. "**.hpp",
    		commonUtilitiesDir .. "**.cpp",
    	}
