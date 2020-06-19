--This file contains functions for importing third party or external projects--

function include_sfml()
	local sfml_folder = THIRD_PARTY_PATH.."SFML-2.5.1/"
	includedirs(sfml_folder.."include")
end

function import_sfml()
	local sfml_folder = THIRD_PARTY_PATH.."SFML-2.5.1/"
	local sfml_bin_path = ""
	
	if TARGETING_X64 then
		sfml_bin_path = sfml_folder.."x64/bin/"
		libdirs(sfml_folder.."x64/lib")
	else
		sfml_bin_path = sfml_folder.."Win32/bin/"
		libdirs(sfml_folder.."Win32/lib")
	end
	
	--include folder
	include_sfml()
	
	--link libs
	
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
	copy_file(sfml_bin_path.."openal32.dll", 			BINARY_PATH.."release/openal32.dll")
	copy_file(sfml_bin_path.."sfml-audio-2.dll", 		BINARY_PATH.."release/sfml-audio-2.dll")
	copy_file(sfml_bin_path.."sfml-graphics-2.dll", 	BINARY_PATH.."release/sfml-graphics-2.dll")
	copy_file(sfml_bin_path.."sfml-network-2.dll", 		BINARY_PATH.."release/sfml-network-2.dll")
	copy_file(sfml_bin_path.."sfml-system-2.dll", 		BINARY_PATH.."release/sfml-system-2.dll")
	copy_file(sfml_bin_path.."sfml-window-2.dll", 		BINARY_PATH.."release/sfml-window-2.dll")
	
	copy_file(sfml_bin_path.."openal32.dll", 			BINARY_PATH.."debug/openal32.dll")
	copy_file(sfml_bin_path.."sfml-audio-d-2.dll", 		BINARY_PATH.."debug/sfml-audio-d-2.dll")
	copy_file(sfml_bin_path.."sfml-graphics-d-2.dll", 	BINARY_PATH.."debug/sfml-graphics-d-2.dll")
	copy_file(sfml_bin_path.."sfml-network-d-2.dll", 	BINARY_PATH.."debug/sfml-network-d-2.dll")
	copy_file(sfml_bin_path.."sfml-system-d-2.dll", 	BINARY_PATH.."debug/sfml-system-d-2.dll")
	copy_file(sfml_bin_path.."sfml-window-d-2.dll", 	BINARY_PATH.."debug/sfml-window-d-2.dll")
end