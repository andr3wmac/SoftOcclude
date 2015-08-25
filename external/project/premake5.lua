local root      = "../../"
local getldflags = premake.tools.gcc.getldflags;
function premake.tools.gcc.getldflags(cfg)
    local ldflags = { pthread = "-pthread" }
    local r = getldflags(cfg);
    local r2 = table.translate(cfg.flags, ldflags);
    for _,v in ipairs(r2) do table.insert(r, v) end
    return r;
end
table.insert(premake.fields.flags.allowed, "pthread");

solution "SoftOcclude"

	location( root )
	objdir( root .. "bin/obj/" )
	debugdir( root .. "bin/" )
	
	configurations { "Debug", "Release" }

	platforms { "x64", "x32" }

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"	

    configuration "x32"
		targetdir( root .. "bin/x32/" )
		architecture "x32"

    configuration "x64"
		targetdir( root .. "bin/x64/" )
		architecture "x64"
		
	configuration "Debug"
		targetsuffix "d"
		defines "DEBUG"
		flags "Symbols"
		optimize "Off"

	configuration "Release"		
		flags "LinkTimeOptimization"
		optimize "Speed"
				
	configuration {}
			
	project "SoftOcclude-Minimal"
	
		location(  root .. "examples/minimal" )
		
		links "IntelOcclu"
		
		kind "ConsoleApp"
		flags "WinMain"
		defines "GTEST_HAS_TR1_TUPLE=0"
		
		includedirs {
			root .. "include/",
			root .. "external/"
			}	
		
		files { 
			root .. "examples/minimal/**.h",
			root .. "examples/minimal/*.cpp"
			}
			
		configuration { "Debug", "x32" }
			defines "PREFIX=X32D_"
		
		configuration { "Debug", "x64" }
			defines "PREFIX=X64D_"
		
		configuration { "Release", "x32" }
			defines "PREFIX=X32R_"
		
		configuration { "Release", "x64" }
			defines "PREFIX=X64R_"
			
	project "SoftOccludeCore"
	
		targetname "SoftOccludeCore"	
		
		kind "StaticLib"
		
		includedirs {
			root .. "include/",
			root .. "src/",
			}	
		
		files { 
			root .. "src/**.h",
			root .. "src/**.cpp"
			}
			
	project "IntelOcclu"
	
		targetname "IntelOcclu"	
		
		kind "StaticLib"
		
		includedirs {
			root .. "external/intel",
			root .. "external/intel/include",
			root .. "external/intel/example/IntelDemo/CPUT/CPUT"
			}	
		
		files { 
			root .. "external/intel/src/**.h",
			root .. "external/intel/src/**.cpp"
			}