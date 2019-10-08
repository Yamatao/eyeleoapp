#ifndef OSCAPABILITIES_H
#define OSCAPABILITIES_H

#include "wx/gdicmn.h" // wxRect
#include <vector>

struct DisplayData
{
    int ind;
	bool primary;
	wxRect clientArea;
	wxRect geometry;
};

std::vector<DisplayData> getDisplays();
void refillResolutionParams();

#endif