require('vstudio')

local function premakeCopyRuntimeToOut(cfg)
    if cfg.outputcppruntime then
        premake.vstudio.vc2010.element("CopyCppRuntimeToOutputDir", nil, 'true')
    end
end

local function showAllFiles(cfg)
    if cfg.usershowallfiles then
        premake.x('<ShowAllFiles>true</ShowAllFiles>')
    end
end

premake.override(premake.vstudio.vc2010.elements, "configurationProperties", function(base, cfg)
    local calls = base(cfg)
    
    if cfg.Kind ~= premake.UTILITY then
        table.insert(calls, premakeCopyRuntimeToOut)
    end

    return calls
end)

premake.override(premake.vstudio.vc2010.elements, "user", function(base, cfg)
    local calls = base(cfg)

    table.insert(calls, showAllFiles)

    return calls
end)

premake.api.register {
    name = "outputcppruntime",
    scope = "config",
    kind = "boolean",
    default = false
}

premake.api.register {
    name = "usershowallfiles",
    scope = "config",
    kind = "boolean",
    default = false
}


-- -- The following override makes VS "Show All Files" ON by default.
-- -- Usually you don't need to do this but generateUser contains a pop and
-- -- we need to inject text in the middle of generateUser before it pops. 
-- -- Therefore we temporarily wrap premake.pop to inject or PropertyGroup 
-- -- and ShowAllFiles before calling the real pop. 
-- -- Then we set premake.pop back to the real pop.
-- -- This code was adapted from a ChatGPT suggestion. I wouldn't have been
-- -- able to figure it out myself and asking on forums would take too long.
-- -- -Elias
-- premake.override(premake.vstudio.vc2010, "generateUser", function(base, prj)
--     local realPop = premake.pop
--     premake.pop = function(tag)
--         if tag == "</Project>" then
--             premake.push("<PropertyGroup>")
--             premake.w("<ShowAllFiles>true</ShowAllFiles>")
--             premake.pop("</PropertyGroup>")
--         end
--         realPop(tag)
--     end
--     base(prj)
--     premake.pop = realPop
-- end)


-- ####################################################################################################
-- ##       Stuff under here is copied from TGE, it enables Visual Studio's "SDLChecks" option.      ##
-- ####################################################################################################

premake.api.register { -- No clue what this specific thing does, doesn't seem to be used in the override -Elias
    name = "sdlchecks",
    scope = "config",
    kind = "boolean",
    default = true
}
-- needed a way to set SDLChecks which isn't provided in premake
-- might be better ways to do it, right now I just opted to push it in
-- where it is in the project settings.
premake.override(premake.vstudio.vc2010, "multiProcessorCompilation", function(base, prj)
	premake.w('<SDLCheck>' .. tostring(true) .. '</SDLCheck>')
	base(prj)
end)