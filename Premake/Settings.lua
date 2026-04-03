Settings = {
    ["Name"]                = "Card_Game",
    ["StartProject"]        = "Client_Launcher",
    ["Architecture"]        = "x64",
    ["Configurations"]      = { "Debug", "Release" },
    ["TargetName"]          = "%{prj.name}",
    ["WorkspaceDir"]        = "../",
    ["SourceDir"]           = "../Source/",
    ["OutputDir"]           = "../Bin/%{cfg.buildcfg}/",
    ["TempDir"]             = "../Temp/%{cfg.buildcfg}/%{prj.name}/",
}