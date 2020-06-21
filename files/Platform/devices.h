#pragma once

#include <optional>

//Provides an interface to windows or other platform api
//Intended to avoid windows syntax littered around the codebase

namespace SWIFT::PLAT
{
	struct MONITOR_DIMENSIONS
	{
		int posX;
		int posY;
		int width;
		int height;
	};

	MONITOR_DIMENSIONS current_monitor_dimensions(void* windowHandle);
	MONITOR_DIMENSIONS main_monitor_dimensions();
}