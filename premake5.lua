workspace "nimped++"
  configurations {"Debug", "Release"}
  
  project "nimped++"
    kind "WindowedApp"
    
    language "C++"
    cppdialect "C++20"
    
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"
    files {"**.hh", "**.cc"}
    includedirs "."
    
    warnings "Extra"
    
    filter "system:linux"
      buildoptions {"`pkg-config --cflags sdl2 SDL2_ttf`"}
      linkoptions {"`pkg-config --libs sdl2 SDL2_ttf`"}
    
    -- Windows support needs to be considered
    
    filter "configurations:Debug"
      runtime "Debug"
      symbols "On"
      optimize "Off"
    
    filter "configurations:Release"
      runtime "Release"
      symbols "Off"
      optimize "Size"
