--

--From stackoverflow, checks if file exists--
function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

--Generates a project file, customization can be added via project override file--
function generate_project(project_name, project_kind)
project (project_name)
	--General settings--
	kind		(project_kind)
	location	(PROJECT_PATH..project_name)
	targetname	(project_name)
	
	local project_files = FILES_PATH..project_name.."/"
	files{
		project_files.."src/**.cpp",
		project_files.."src/**.h",
		project_files.."include/**.h",
		project_files.."*.*"
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
	
project ("*")
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