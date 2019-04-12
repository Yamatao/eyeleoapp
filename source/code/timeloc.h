#ifndef TIMELOC_H
#define TIMELOC_H
#include "wx/string.h"

enum ETimeUnit
{
    MILLISECONDS,
    SECONDS,
    MINUTES,
    HOURS
};

wxString getTimeStr(int value, ETimeUnit unit, wxString const &lang);
void splitTime(int value, ETimeUnit unit, int *pHours, int *pMinutes, int *pSeconds, int *pMilliseconds);

#endif