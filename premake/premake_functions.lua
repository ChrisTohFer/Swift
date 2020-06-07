--This file defines functions which are used to construct the solution

--Checks if file exists. From stackoverflow--
function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

--Check if table contains a value--
function table_contains(tab, value)
	for k,v in pairs(tab) do
		if(v == value) then
			return true
		end
	end
	
	return false
end

--Move a file from source to destination; keeps track of which files are moved and won't repeat moves--
local copied_files = {}
function copy_file(source, destination)
	source = source:gsub("/","\\")
	destination = destination:gsub("/","\\")

	local move_key = source..">"..destination
	
	if not table_contains(copied_files, move_key) then
		os.execute("echo F | xcopy /y \""..source.."\" \""..destination.."\" > nul")
		table.insert(copied_files, move_key)
	end
end

--Generates a project file, customization can be added via project override file--
function generate_project(project_name, project_kind)
project (project_name)
	--General settings--
	kind		(project_kind)
	location	(PROJECT_PATH)
	targetname	(project_name)
	
	local project_files = FILES_PATH..project_name.."/"
	files{
		project_files.."src/**.cpp",
		project_files.."src/**.h",
		project_files.."include/**.h",
		project_files.."*.*"
	}
	
	includedirs{
		FILES_PATH,
		project_files,
		project_files.."src",
		project_files.."include"
	}
	
	--Config specific directories--
	filter "configurations:debug"
		objdir		(INTERMEDIATE_PATH.."debug/"..project_name)
	filter "configurations:release"
		objdir		(INTERMEDIATE_PATH.."release/"..project_name)
	filter{}
	
	--Run override file if it exists--
	local project_override = OVERRIDE_PATH..project_name.."_premake5.lua"
	if file_exists(project_override) then
		dofile(project_override)
	end
end

function import_projects(project_names)
	if type(elem) == "table" then
		for key, value in pairs(project_names) do
			links(value)
		end
	else
		links(project_names)
	end
end