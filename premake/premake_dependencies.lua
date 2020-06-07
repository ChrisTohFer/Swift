--This file contains functions for importing third party or external projects--

function import_sfml()
	local sfml_folder = THIRD_PARTY_PATH.."SFML-2.5.1-windows-vc15-64-bit/SFML-2.5.1/"
	
	--include folder
	includedirs(sfml_folder.."include")
	
	--link .libs
	libdirs(sfml_folder.."lib")
	
	filter "configurations:debug"
		links{
			"sfml-audio-d",
			"sfml-graphics-d",
			"sfml-main-d",
			"sfml-network-d",
			"sfml-system-d",
			"sfml-window-d",
		}
	filter "configurations:release"
		links{
			"sfml-audio",
			"sfml-graphics",
			"sfml-main",
			"sfml-network",
			"sfml-system",
			"sfml-window",
		}
	filter{}
	
	--copy dlls
	copy_file(sfml_folder.."bin/openal32.dll", 			BINARY_PATH.."release/openal32.dll")
	copy_file(sfml_folder.."bin/sfml-audio-2.dll", 		BINARY_PATH.."release/sfml-audio-2.dll")
	copy_file(sfml_folder.."bin/sfml-graphics-2.dll", 	BINARY_PATH.."release/sfml-graphics-2.dll")
	copy_file(sfml_folder.."bin/sfml-network-2.dll", 	BINARY_PATH.."release/sfml-network-2.dll")
	copy_file(sfml_folder.."bin/sfml-system-2.dll", 	BINARY_PATH.."release/sfml-system-2.dll")
	copy_file(sfml_folder.."bin/sfml-window-2.dll", 	BINARY_PATH.."release/sfml-window-2.dll")
	
	copy_file(sfml_folder.."bin/openal32.dll", 			BINARY_PATH.."debug/openal32.dll")
	copy_file(sfml_folder.."bin/sfml-audio-d-2.dll", 	BINARY_PATH.."debug/sfml-audio-d-2.dll")
	copy_file(sfml_folder.."bin/sfml-graphics-d-2.dll", BINARY_PATH.."debug/sfml-graphics-d-2.dll")
	copy_file(sfml_folder.."bin/sfml-network-d-2.dll", 	BINARY_PATH.."debug/sfml-network-d-2.dll")
	copy_file(sfml_folder.."bin/sfml-system-d-2.dll", 	BINARY_PATH.."debug/sfml-system-d-2.dll")
	copy_file(sfml_folder.."bin/sfml-window-d-2.dll", 	BINARY_PATH.."debug/sfml-window-d-2.dll")
end