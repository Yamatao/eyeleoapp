#include "beforepause_wnd.h"
#include "main.h"
#include "oscapabilities.h"
#include "wx/button.h"
#include "image_resources.h"
#include "language_set.h"
#include "DataLog.h"
#include "settings.h"

BEGIN_EVENT_TABLE(BeforePauseWindow, wxFrame)
	EVT_PAINT(BeforePauseWindow::OnPaint)
	EVT_CLOSE(BeforePauseWindow::OnClose)
END_EVENT_TABLE()


BeforePauseWindow::BeforePauseWindow(int displayInd, int postponeCount) :
	wxFrame(NULL, -1, L"", wxDefaultPosition, wxDefaultSize, wxFRAME_TOOL_WINDOW | wxFRAME_SHAPED | wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP),
	_preventClosing(true),
	_readyTimer((float)eyeleo::settings::timeForLongBreakConfirmation),
	_result(RESULT_NONE),
	_postponeCount(postponeCount),
	_displayInd(displayInd),
	_btnReady(nullptr),
	_showing(true),
	_hiding(false),
	_alpha(0.0f)
{
	SetName("BeforePauseWindow");
}

void BeforePauseWindow::Init()
{
	wxRegion region(*_backBitmap, *wxWHITE);
	SetShape(region);

	refillResolutionParams();

	_showing = true;
	_hiding = false;
	_alpha = 0.0f;

	SetTransparent((int)_alpha);

	// Set position to center of the screen
	SetSize(wxSize(_backBitmap->GetWidth(), _backBitmap->GetHeight()));
	assert(_displayInd < osCaps.numDisplays);
	wxRect displayRect = osCaps.displays[_displayInd].clientArea;
	SetPosition(wxPoint(displayRect.GetX() + displayRect.GetWidth() / 2 - GetSize().GetX() / 2, displayRect.GetY() + displayRect.GetHeight() / 2 - GetSize().GetY() / 2));

	wxStaticText * txt = new wxStaticText(this, wxID_ANY, langPack->get("before_pause_text"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	txt->SetFont(font);
	txt->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	txt->SetBackgroundColour(wxColour(0, 0, 0, 0));
	txt->SetForegroundColour(wxColour(255, 255, 255, 255));
	txt->SetPosition(wxPoint(40, 35));
	txt->SetSize(390, 65);

	_btnReady = new wxButton(this, ID_BEFORE_PAUSE_IAM_READY, _(""));
	_btnReady->SetSize(110, _btnReady->GetSize().y);
	_btnReady->SetBackgroundColour(wxColor(255, 255, 255, 0));
	_btnReady->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	UpdateReadyTimer();

	wxButton * btnPostpone = new wxButton(this, ID_BEFORE_PAUSE_GIVE_ME_TIME, langPack->get("before_pause_postpone_button"));
	btnPostpone->SetBackgroundColour(wxColor(255, 255, 255, 0));
	btnPostpone->SetBackgroundStyle(wxBG_STYLE_COLOUR);

	if (_postponeCount < 1)
	{
		btnPostpone->SetPosition(wxPoint(70, 102));
		_btnReady->SetPosition(wxPoint(270, 102));
	}
	else
	{
		wxButton * btnRefuse = new wxButton(this, ID_BEFORE_PAUSE_NO_THANKS, langPack->get("before_pause_refuse_button"));
		btnRefuse->SetBackgroundColour(wxColor(255, 255, 255, 0));
		btnRefuse->SetBackgroundStyle(wxBG_STYLE_COLOUR);

		btnRefuse->SetPosition(wxPoint(50, 102));
		btnPostpone->SetPosition(wxPoint(160, 102));
		_btnReady->SetPosition(wxPoint(300, 102));
	}

	Connect(ID_BEFORE_PAUSE_NO_THANKS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BeforePauseWindow::OnRefuseClicked));
	Connect(ID_BEFORE_PAUSE_IAM_READY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BeforePauseWindow::OnReadyClicked));
	Connect(ID_BEFORE_PAUSE_GIVE_ME_TIME, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BeforePauseWindow::OnPostponeClicked));

	g_TaskMgr->AddTask(GetName(), 20);
}

BeforePauseWindow::~BeforePauseWindow()
{
	LogMsg("BeforePauseWindow::~BeforePauseWindow");

	if (!getApp()->isFinished())
		g_TaskMgr->RemoveTasks(GetName());

	getApp()->OnCloseBeforePauseWnd(this);
}

void BeforePauseWindow::UpdateReadyTimer()
{
	int iReadyTimer = (int)(_readyTimer + 1.0f);
	wxString readyText = wxString::Format(langPack->get("before_pause_accept_button"), iReadyTimer);
	_btnReady->SetLabel(readyText);
}

void BeforePauseWindow::ExecuteTask(float f, long time_went)
{
	(void)time_went;

	if (!_showing && !_hiding && _alpha > 0.0f)
	{
		_readyTimer -= 0.1f * f;
		
		UpdateReadyTimer();
		
		if (_readyTimer <= 0)
		{
			g_TaskMgr->AddTask(GetName(), 20);
			_result = RESULT_ACCEPT;
			_hiding = true;
		}
		else
		{
			g_TaskMgr->AddTask(GetName(), 100);
		}
	}

	if (_showing)
	{
		_alpha += 11.0f * f;
		if (_alpha >= 210.0f)
		{
			_alpha = 210.0f;
			_showing = false;
			
			g_TaskMgr->AddTask(GetName(), 100);
		}
		else
		{
			g_TaskMgr->AddTask(GetName(), 20);
		}
		SetTransparent((int)_alpha);
	}

	if (_hiding)
	{
		_alpha -= 15.0f * f;
		if (_alpha <= 0.0f)
		{
			_alpha = 0.0f;
			_hiding = false;
			_preventClosing = false;
			
			if (_result == RESULT_ACCEPT)
			{
				getApp()->StartBigPause();
			}
			else if (_result == RESULT_POSTPONE)
			{
				getApp()->PostponeBigPause();
			}
			else if (_result == RESULT_REFUSE)
			{
				getApp()->RefuseBigPause();
			}
			
			Close();
			g_TaskMgr->RemoveTasks(GetName());
		}
		else
		{
			SetTransparent((int)_alpha);
			g_TaskMgr->AddTask(GetName(), 20);
		}
	}
}

void BeforePauseWindow::OnClose(wxCloseEvent& event)
{
	if (!_preventClosing)
		event.Skip(true);
}

void BeforePauseWindow::OnRefuseClicked(wxCommandEvent &)
{
	_result = RESULT_REFUSE;
	g_TaskMgr->AddTask(GetName(), 20);
	_hiding = true;
}

void BeforePauseWindow::OnReadyClicked(wxCommandEvent &)
{
	_result = RESULT_ACCEPT;
	g_TaskMgr->AddTask(GetName(), 20);
	_hiding = true;
}

void BeforePauseWindow::OnPostponeClicked(wxCommandEvent &)
{
	_result = RESULT_POSTPONE;
	g_TaskMgr->AddTask(GetName(), 20);
	_hiding = true;
}

void BeforePauseWindow::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxPaintDC dc(this);
	dc.DrawBitmap(*_backBitmap, 0, 0, true);
}

void BeforePauseWindow::Hide()
{
	LogMsg("BeforePauseWindow::Hide");
	Destroy();
}
