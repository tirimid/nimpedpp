workspace "nimped++"
  configurations {"Debug", "Release"}
  
  project "nimped++"
    kind "ConsoleApp"
    
    language "C++"
    cppdialect "C++20"
    
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"
    files {"**.hh", "**.cc"}
    includedirs "."
    
    warnings "Extra"
    
    filter "configurations:Debug"
      runtime "Debug"
      symbols "On"
      optimize "Off"
      sanitize "address"
    
    filter "configurations:Release"
      runtime "Release"
      symbols "Off"
      optimize "Speed"
