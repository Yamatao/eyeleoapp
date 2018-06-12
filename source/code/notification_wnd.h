#ifndef NOTIFICATION_WND_H
#define NOTIFICATION_WND_H

#include "wx/wx.h"
#include "wx/timer.h"
#include "task_mgr.h"

class NotificationWindowLook;
class NotificationWindow : public wxFrame, public ITask
{
	enum EState
	{
		STATE_SHOWING,
		STATE_ACTIVE,
		STATE_HIDING,
		STATE_DONE
	};

	static bool isInstanceExist; // существует ли хоть инстанс окна

	EState _state;

	NotificationWindowLook * _controlsWnd;

	int _timeLeft;
	int _showCount;

	int _alpha;
	bool _preventClosing;

	DECLARE_EVENT_TABLE()

public:
	NotificationWindow(unsigned int showCount = 0);
	virtual ~NotificationWindow();

	void Init(int displayInd = 0);
	void SetTime(int timeBeforeLongBreakMs); // сколько времени осталось до большого перерыва

	virtual bool Hide();

	static bool hasAnyInstance() { return isInstanceExist; }
	
private:
	void ExecuteTask(float f, long time_went);
	void OnClose(wxCloseEvent& event);
};

class NotificationWindowLook : public wxFrame
{
public:
	NotificationWindowLook(wxWindow * parent = 0);
	virtual ~NotificationWindowLook();

	void Init(int displayInd, unsigned int showCount);

	void Update();
	
	void UpdateTimeLabel(int msLeft);

private:
	void OnClose(wxCloseEvent& event);
	void OnMouseTap(wxMouseEvent& evt);
	
	wxStaticBitmap * _eyeleoLogo;

	wxStaticText * _txtTime;

	int _alpha;
	bool _preventClosing;
	
	DECLARE_EVENT_TABLE()
};

#endif