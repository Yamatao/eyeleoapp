#ifndef DEBUG_WND_H
#define DEBUG_WND_H

#include "wx/wx.h"

class DebugWindow : public wxFrame
{
public:
	DebugWindow();

	wxStaticText * _timeLeftToBigPause;
	wxStaticText * _timeLeftToMiniPause;
	wxStaticText * _inactivityTime;
	wxStaticText * _relaxingTimeLeft;

private:
	void OnClose(wxCloseEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif