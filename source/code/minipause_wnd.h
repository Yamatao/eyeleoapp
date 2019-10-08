#ifndef MINIPAUSE_WND_H
#define MINIPAUSE_WND_H

#include "wx/wx.h"
#include "wx/timer.h"
#include "task_mgr.h"
#include "oscapabilities.h"

class MiniPauseControls;
class MiniPauseWindow : public wxFrame, public ITask
{
	enum EState
	{
		STATE_SHOWING,
		STATE_ACTIVE,
		STATE_HIDING,
		STATE_DONE
	};

public:
	MiniPauseWindow(const DisplayData& displayData, unsigned int showCount = 0);
	virtual ~MiniPauseWindow();

	virtual void Init();

	virtual void Hide();
	void HideQuick();
	
private:
	void ExecuteTask(float f, long time_went);
	void OnClose(wxCloseEvent& event);

	EState _state;
	
	MiniPauseControls * _controlsWnd;

	int _timeLeft;
	int _showCount;
    DisplayData _displayData;

	float _alpha;
	bool _preventClosing;
	
	DECLARE_EVENT_TABLE()
};

class ExcerciseAnim;
class MiniPauseControls : public wxFrame
{
	enum
	{
		ID_MINIPAUSE_TEXT = 100,
		ID_MINIPAUSE_PORTRAIT,
		ID_MINIPAUSE_LOGO,
		ID_MINIPAUSE_TIMER
	};

public:
	MiniPauseControls(wxWindow * parent = 0);
	virtual ~MiniPauseControls();

	void Init(const DisplayData& displayData, unsigned int showCount = 0);

	void Update();

	static int _excercise;
	static int _line;
	static int _lastExcercise;

	void UpdateTimeLabel(int msLeft);

private:
	void OnMouseTap(wxMouseEvent&);
	void OnClose(wxCloseEvent& event);
	
	wxStaticBitmap * _excerciseImg;
	ExcerciseAnim * _excerciseAnim;
	
	wxStaticText * _txtTime;
	
	int _alpha;
	bool _preventClosing;
	
	DECLARE_EVENT_TABLE()
};

#endif