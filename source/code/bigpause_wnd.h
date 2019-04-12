#ifndef BIGPAUSE_WND_H
#define BIGPAUSE_WND_H

#include "task_mgr.h"
#include "wx/timer.h"
#include "wx/wx.h"

enum
{
    ID_BTN_SKIP = 1
};

class BigPauseWindow : public wxFrame, public ITask {
public:
    BigPauseWindow(int displayInd = 0);
    virtual ~BigPauseWindow();

    virtual void Init();
    void SetBreakDuration(int minutes);

    virtual void Hide();

private:
    void OnPaint(wxPaintEvent &evt);
    void OnErase(wxEraseEvent &evt);
    void ExecuteTask(float f, long time_went);
    void OnClose(wxCloseEvent &event);
    void OnKillFocus(wxFocusEvent &);
    void OnSkipClicked(wxCommandEvent &);

    void UpdateTimeLabel();

    virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

    bool _showing;
    bool _hiding;
    float _alpha;
    bool _preventClosing;
    bool _restoreFocus;
    float _breakTimeLeft;
    float _breakTimeFull;
    bool _primary;

    int _displayInd;

    wxStaticText *_timeText;
    wxBoxSizer *_sizer;

    DECLARE_EVENT_TABLE()
};

#endif