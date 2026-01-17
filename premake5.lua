workspace "nimped++"
  configurations {"debug", "release"}
  
  project "nimped++"
    kind "ConsoleApp"
    
    language "C++"
    cppdialect "C++20"
    
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"
    files {"**.hh", "**.cc"}
    includedirs "."
    
    warnings "Extra"
    
    filter "configurations:debug"
      runtime "debug"
      symbols "On"
      optimize "Off"
      sanitize "address"
    
    filter "configurations:release"
      runtime "release"
      symbols "Off"
      optimize "Speed"
