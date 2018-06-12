#ifndef WAITING_WND_H
#define WAITING_WND_H

#include "wx/wx.h"
#include "wx/timer.h"
#include "task_mgr.h"

class WaitingFullscreenWindow : public wxFrame, public ITask
{
public:
	WaitingFullscreenWindow();
	virtual ~WaitingFullscreenWindow();

	void Init(int displayInd = 0);

	void Hide();

private:
	virtual void OnPaint(wxPaintEvent& evt);
	void ExecuteTask(float f, long time_went);
	void OnClose(wxCloseEvent& event);

	bool _showing;
	bool _hiding;
	float _alpha;
	bool _preventClosing;
	
	DECLARE_EVENT_TABLE()

	static bool bitmapInitialized;
};


#endif