#include "notification_wnd.h"
#include "excercises.h"
#include "image_resources.h"
#include "language_set.h"
#include "main.h"
#include "oscapabilities.h"
#include "settings.h"
#include "wx/button.h"

BEGIN_EVENT_TABLE(NotificationWindow, wxFrame)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(NotificationWindowLook, wxFrame)
EVT_CLOSE(NotificationWindowLook::OnClose)
END_EVENT_TABLE()

////////////////////////////////////////////////////////////////////////

bool NotificationWindow::isInstanceExist(false);

NotificationWindow::NotificationWindow(unsigned int showCount)
    : wxFrame(NULL, -1, L"", wxDefaultPosition, wxDefaultSize, wxFRAME_TOOL_WINDOW | wxFRAME_SHAPED | wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP)
    , _preventClosing(true)
    , _controlsWnd(0)
    , _showCount(showCount)
    , _timeLeft(3000)
    , _alpha(0.0f)
    , _state(STATE_SHOWING) {
    SetName("NotificationWindow");
}

void NotificationWindow::Init(int displayInd) {
    assert(!NotificationWindow::isInstanceExist);
    NotificationWindow::isInstanceExist = true;

    refillResolutionParams();

    wxRegion region(*_backBitmap_notification, *wxWHITE);
    SetShape(region);

    SetBackgroundColour(wxColour(0, 0, 0));
    SetTransparent(0);

    SetSize(_backBitmap_notification->GetSize());
    assert(displayInd < osCaps.numDisplays);
    wxRect displayRect = osCaps.displays[displayInd].clientArea;
    SetPosition(wxPoint(displayRect.GetRight() - GetSize().GetX(), displayRect.GetBottom() - GetSize().GetY()));

    _state = NotificationWindow::STATE_SHOWING;
    g_TaskMgr->AddTask(GetName(), 20);
    _alpha = 0.0f;

    Show(true);

    _controlsWnd = new NotificationWindowLook(this);
    _controlsWnd->Init(displayInd, _showCount);
    _controlsWnd->Show(true);
}

NotificationWindow::~NotificationWindow() {
    if (!getApp()->isFinished())
        g_TaskMgr->RemoveTasks(GetName());
    _controlsWnd = 0;

    NotificationWindow::isInstanceExist = false;

    getApp()->OnNotificationWindowClosed();
}

void NotificationWindow::SetTime(int timeBeforeLongBreakMs) {
    _timeLeft = timeBeforeLongBreakMs;
}

void NotificationWindow::ExecuteTask(float f, long time_went) {
    switch (_state) {
    case NotificationWindow::STATE_SHOWING: {
        _alpha += 11.0f * f;
        if (_alpha >= 210.0f) {
            _alpha = 210.0f;
            _state = NotificationWindow::STATE_ACTIVE;

            _controlsWnd->UpdateTimeLabel(_timeLeft);

            g_TaskMgr->AddTask(GetName(), 100);
        } else {
            g_TaskMgr->AddTask(GetName(), 20);
        }
        SetTransparent((int)_alpha);
        break;
    }

    case NotificationWindow::STATE_ACTIVE: {
        _timeLeft -= time_went;

        _controlsWnd->Update();

        if (_timeLeft <= eyeleo::settings::timeForLongBreakConfirmation * 1000) {
            _state = NotificationWindow::STATE_HIDING;
            _controlsWnd->Show(false);
            _controlsWnd->Destroy();
            g_TaskMgr->AddTask(GetName(), 20);
        } else {
            g_TaskMgr->AddTask(GetName(), 100);
        }

        _controlsWnd->UpdateTimeLabel(_timeLeft);
        break;
    }

    case NotificationWindow::STATE_HIDING: {
        _alpha -= 15.0f * f;
        if (_alpha <= 0.0f) {
            _alpha = 0.0f;
            _state = NotificationWindow::STATE_DONE;
            _preventClosing = false;
            SetTransparent(0);

            Close();
        } else {
            SetTransparent((int)_alpha);
            g_TaskMgr->AddTask(GetName(), 20);
        }
        break;
    }
    }
}

bool NotificationWindow::Hide() {
    if (_state == NotificationWindow::STATE_HIDING)
        return false;

    _state = NotificationWindow::STATE_HIDING;
    
    _controlsWnd->Show(false);
    _controlsWnd->Destroy();

    Close();
    return true;
}

void NotificationWindow::OnClose(wxCloseEvent &event) {
    if (!_preventClosing) {
        event.Skip(true);
    }
}

////////////////////////////////////////////////////////////////////////

NotificationWindowLook::NotificationWindowLook(wxWindow *parent)
    : wxFrame(parent, -1, L"", wxDefaultPosition, wxDefaultSize, wxFRAME_TOOL_WINDOW | wxFRAME_SHAPED | wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP)
    , _preventClosing(true)
    , _eyeleoLogo(0)
    , _txtTime(nullptr)
    , _alpha(0.0f) {
}

void NotificationWindowLook::Init(int displayInd, unsigned int showCount) {
    (void)showCount;

    if ((GetExtraStyle() & WS_EX_LAYERED) == 0)
        SetWindowLong(GetHWND(), GWL_EXSTYLE, GetExtraStyle() | WS_EX_LAYERED);
    SetLayeredWindowAttributes(GetHWND(), RGB(32, 33, 34), 0, LWA_COLORKEY);
    SetBackgroundColour(wxColour(32, 33, 34));

    // Set position to right bottom side of the screen
    SetSize(_backBitmap_notification->GetSize());
    assert(displayInd < osCaps.numDisplays);
    wxRect displayRect = osCaps.displays[displayInd].clientArea;
    SetPosition(wxPoint(displayRect.GetRight() - GetSize().GetX(), displayRect.GetBottom() - GetSize().GetY()));

    wxStaticText *txt = new wxStaticText(this, wxID_ANY, L"", wxDefaultPosition, wxSize(100, 25), wxALIGN_CENTER);
    txt->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
    txt->SetForegroundColour(wxColour(255, 255, 255, 255));
    txt->SetPosition(wxPoint(89, 13));
    txt->SetLabel(langPack->Get(L"notification_wnd_label"));

    _txtTime = new wxStaticText(this, wxID_ANY, L"", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    _txtTime->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
    _txtTime->SetForegroundColour(wxColour(255, 200, 70, 255));
    _txtTime->SetPosition(wxPoint(132, 38));
    _txtTime->SetSize(wxSize(40, 30));

    wxStaticBitmap *logo = new wxStaticBitmap(this, wxID_ANY, *_bmpNotificationLeopard);
    logo->SetPosition(wxPoint(12, 16));

    //
    txt->Bind(wxEVT_RIGHT_UP, &NotificationWindowLook::OnMouseTap, this);
    _txtTime->Bind(wxEVT_RIGHT_UP, &NotificationWindowLook::OnMouseTap, this);
    logo->Bind(wxEVT_RIGHT_UP, &NotificationWindowLook::OnMouseTap, this);
    Bind(wxEVT_RIGHT_UP, &NotificationWindowLook::OnMouseTap, this);
}

NotificationWindowLook::~NotificationWindowLook() {
}

void NotificationWindowLook::UpdateTimeLabel(int msLeft) {
    int secsLeft = (msLeft - 1) / 1000 + 1;
    if (secsLeft < 0)
        secsLeft = 0;
    wxString newStr = wxString::Format(L"%d", secsLeft);
    if (_txtTime->GetLabel() != newStr)
        _txtTime->SetLabel(newStr);
}

void NotificationWindowLook::Update() {
}

void NotificationWindowLook::OnClose(wxCloseEvent &event) {
    if (!_preventClosing) {
        event.Skip(true);
    }
}

void NotificationWindowLook::OnMouseTap(wxMouseEvent &) {
    if (getApp()->GetCanCloseNotificationsSetting()) {
        NotificationWindow *wnd = dynamic_cast<NotificationWindow *>(GetParent());
        if (wnd)
            wnd->Hide();
    }
}
