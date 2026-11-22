Settings = {
    Name                = "Card_Game",
    StartProject        = "Client_Launcher",
    Architecture        = "x64",
    Configurations      = { "Debug", "Release", "Retail" },
    TargetName          = "%{prj.name}",
    WorkspaceDir        = "../",
    SourceDir           = "../Source/",
    LibDir              = "../Lib/%{cfg.buildcfg}/",
    ExternalDir         = "../Source/External/",
    OutputDir           = "../Bin/%{cfg.buildcfg}/%{prj.name}/",
    TempDir             = "../Temp/%{cfg.buildcfg}/%{prj.name}/",
    User                = {
        ShowAllFiles        = true
    }
}