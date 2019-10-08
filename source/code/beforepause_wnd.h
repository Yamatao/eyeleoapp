#ifndef BEFOREPAUSE_WND_H
#define BEFOREPAUSE_WND_H

#include "wx/wx.h"
#include "wx/timer.h"
#include "task_mgr.h"
#include "oscapabilities.h"

enum
{
	ID_BEFORE_PAUSE_NO_THANKS = 1,
	ID_BEFORE_PAUSE_IAM_READY = 2,
	ID_BEFORE_PAUSE_GIVE_ME_TIME = 3
};

enum EResult
{
	RESULT_NONE,
	RESULT_REFUSE,
	RESULT_POSTPONE,
	RESULT_ACCEPT,
};

class BeforePauseWindow : public wxFrame, public ITask
{
public:
	BeforePauseWindow(const DisplayData& displayData, int postponeCount = 0);
	virtual ~BeforePauseWindow();

	void Init();

	void Hide();

private:
	void OnPaint(wxPaintEvent& evt);
	void ExecuteTask(float f, long time_went);
	void OnClose(wxCloseEvent& event);
	
	void OnRefuseClicked(wxCommandEvent &);
	void OnReadyClicked(wxCommandEvent &);
	void OnPostponeClicked(wxCommandEvent &);

	void UpdateReadyTimer();

	DisplayData _displayData;
	int _postponeCount;

	bool _showing;
	bool _hiding;
	float _alpha;
	bool _preventClosing;

	float _readyTimer;

	EResult _result;

	wxButton * _btnReady;
	
	DECLARE_EVENT_TABLE()

	static bool bitmapInitialized;
};

#endif