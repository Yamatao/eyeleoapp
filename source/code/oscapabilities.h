#ifndef OSCAPABILITIES_H
#define OSCAPABILITIES_H

#include "wx/gdicmn.h" // wxRect
#include <vector>

struct DisplayData
{
	bool primary;
	wxRect clientArea;
	wxRect geometry;
};

struct OSCapabilities
{
	bool transparent_windows;
	bool semi_transparent_windows;

	int numDisplays;
	bool multiDisplay;
	int primaryDisplayInd;

	std::vector<DisplayData> displays;
};

extern OSCapabilities osCaps;

void fillOSCapabilities();
void refillResolutionParams();

#endif