#include "oscapabilities.h"
#include <wx/display.h>
#include <mutex>

#ifdef WIN32
#include "windows.h"
#endif

std::vector<DisplayData> displays;
std::mutex displaysVectorMutex;

void refillResolutionParams()
{
	std::lock_guard<std::mutex> displaysVectorLock(displaysVectorMutex);
	displays.clear();
	
	for (unsigned ind = 0; ind < wxDisplay::GetCount(); ++ind)
	{
		wxDisplay display(ind);
		//display.ReinitializeImpl();
		DisplayData data = {
			static_cast<int>(ind),
			display.IsPrimary(),
			display.GetClientArea(),
			display.GetGeometry()
		};

		displays.push_back(std::move(data));
	}
}

std::vector<DisplayData> getDisplays()
{
	std::lock_guard<std::mutex> displaysVectorLock(displaysVectorMutex);
	return displays;
}