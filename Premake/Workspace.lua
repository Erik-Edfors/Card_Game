include("Settings.lua")
include("Extensions.lua")

workspace(Settings.Name)

    location(Settings.WorkspaceDir)
    startproject(Settings.StartProject)
    architecture(Settings.Architecture)

    configurations(Settings.Configurations)

    -- Default project settings
    debugdir(Settings.OutputDir)
	targetdir(Settings.OutputDir)
	targetname(Settings.TargetName)
	objdir(Settings.TempDir)

    -- Default project user settings
    usershowallfiles(Settings.User.ShowAllFiles)

    staticruntime("off")

    multiprocessorcompile("On")
    warnings("Extra")
    fatalwarnings("All")
    symbols("Default")

    includedirs {
        Settings.SourceDir,
        Settings.ExternalDir
    }

    libdirs {
        Settings.LibDir
    }

    defines {
        "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING",
        "RAPIDJSON_HAS_CXX11_NOEXCEPT"
    }

    filter("configurations:Debug")
        defines {
            "_DEBUG"
        }
        optimize("Off")
        runtime("Debug")
        
    filter("configurations:Release")
        defines {
            "NDEBUG",
            "_RELEASE"
        }
        optimize("On")
        runtime("Release")

    filter("configurations:Retail")
        defines {
            "NDEBUG",
            "_RELEASE",
            "_RETAIL"
        }
        optimize("On")
        runtime("Release")
        outputcppruntime(true)
        
    filter { "configurations:Debug", "action:vs*", "not action:vs2015", "not action:vs2013", "not action:vs2012", "not action:vs2010", "not action:vs2008", "not action:vs2005" }
        symbols("Full")

    filter { "configurations:Release", "action:vs*", "not action:vs2013", "not action:vs2012", "not action:vs2010", "not action:vs2008", "not action:vs2005" }
        symbols("FastLink")

    filter { "configurations:Retail", "action:vs*", "not action:vs2013", "not action:vs2012", "not action:vs2010", "not action:vs2008", "not action:vs2005" }
        symbols("FastLink")

    filter("system:windows")
        defines {
            "WIN32"
        }
        systemversion("latest")


    -- Disables filter
    filter {}

    -- Projects
    include("Projects.lua")