#include "devices.h"

#include <Windows.h>

namespace
{
	SWIFT::PLAT::MONITOR_DIMENSIONS monitor_dimensions(HMONITOR monitor)
	{
		MONITORINFO info;
		info.cbSize = sizeof(info);
		GetMonitorInfo(monitor, &info);

		return {
			info.rcMonitor.left,
			info.rcMonitor.top,
			info.rcMonitor.right - info.rcMonitor.left,
			info.rcMonitor.bottom - info.rcMonitor.top
		};
	}
}

SWIFT::PLAT::MONITOR_DIMENSIONS SWIFT::PLAT::current_monitor_dimensions(void* windowHandle)
{
	auto monitorHandle = MonitorFromWindow((HWND)windowHandle, MONITOR_DEFAULTTONEAREST);
	return monitor_dimensions(monitorHandle);
}

SWIFT::PLAT::MONITOR_DIMENSIONS SWIFT::PLAT::main_monitor_dimensions()
{
	auto monitorHandle = MonitorFromPoint({ 0,0 }, MONITOR_DEFAULTTOPRIMARY);
	return monitor_dimensions(monitorHandle);
}
