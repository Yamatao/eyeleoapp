#ifndef WAITING_WND_H
#define WAITING_WND_H

#include "wx/wx.h"
#include "wx/timer.h"
#include "task_mgr.h"
#include "oscapabilities.h"

class WaitingFullscreenWindow : public wxFrame, public ITask
{
	enum class State
	{
		Showing,
		Active,
		Hiding,
		Dead
	};

public:
	WaitingFullscreenWindow();
	virtual ~WaitingFullscreenWindow();

	void Init(const DisplayData& displayData);

	void Hide();
	void HideQuick();

private:
	virtual void OnPaint(wxPaintEvent& evt);
	void ExecuteTask(float f, long time_went);
	void OnClose(wxCloseEvent& event);

	void OnMouseTap(wxMouseEvent &);

	State _state;

	float _alpha;
	bool _preventClosing;
	
	DECLARE_EVENT_TABLE()

	static bool bitmapInitialized;
};


#endif