#include "oscapabilities.h"
#include <wx/display.h>

#ifdef WIN32
#include "windows.h"
#endif

OSCapabilities osCaps;

void fillOSCapabilities()
{
	// retrieve monitor count
	osCaps.numDisplays = wxDisplay::GetCount();
	osCaps.multiDisplay = osCaps.numDisplays > 1;
	
	refillResolutionParams();
}

void refillResolutionParams()
{
	osCaps.displays.clear();
	
	for (int ind = 0; ind < osCaps.numDisplays; ++ind)
	{
		osCaps.displays.push_back(DisplayData());
		DisplayData & disp = osCaps.displays[ind];
		
		wxDisplay display(ind);
		//display.ReinitializeImpl();
		disp.primary = display.IsPrimary();
		disp.clientArea = display.GetClientArea();
		disp.geometry = display.GetGeometry();
		if (disp.primary)
			osCaps.primaryDisplayInd = ind;
	}
}