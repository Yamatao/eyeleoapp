#include "bigpause_wnd.h"
#include "main.h"
#include "oscapabilities.h"
#include "wx/statbmp.h"
#include "image_resources.h"
#include "language_set.h"
#include "timeloc.h"
#include "excercises.h"
#include "logging.h"

#ifdef WIN32
	#include <Wtsapi32.h>
	#include <VersionHelpers.h>
#endif

BEGIN_EVENT_TABLE(BigPauseWindow, wxFrame)
	EVT_CLOSE(BigPauseWindow::OnClose)
	EVT_KILL_FOCUS(BigPauseWindow::OnKillFocus)
END_EVENT_TABLE()

BigPauseWindow::BigPauseWindow(const DisplayData& displayData) :
	wxFrame(NULL, -1, L"", wxDefaultPosition, wxDefaultSize, wxFRAME_SHAPED | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP),
	_preventClosing(true),
	_breakTimeLeft(0),
	_breakTimeFull(0),
	_timeText(0),
	_restoreFocus(false),
	_displayData(displayData),
	_showing(true),
	_hiding(false),
	_alpha(0.0f),
	_primary(false),
	_sizer(nullptr)
{
	SetName(std::string("BigPauseWindow") + (char)('0' + displayData.ind));

	logging::msg("BigPauseWindow::BigPauseWindow");
}

void BigPauseWindow::Init()
{
	refillResolutionParams();

	wxRect displayRect = _displayData.geometry;

	SetPosition(displayRect.GetPosition());

	//logging::msg(wxString::Format("BigPauseWindow init displayInd=%d, x=%d, y=%d, w=%d, h=%d", _displayInd, displayRect.GetPosition().x, displayRect.GetPosition().y, displayRect.GetSize().GetWidth(), displayRect.GetSize().GetHeight()));

	_showing = true;
	_hiding = false;
	_alpha = 0.0f;
	_primary = _displayData.primary;

	SetBackgroundColour(wxColour(0, 0, 0));
	SetTransparent((int)_alpha);

	if (_primary)
	{
		wxStaticBitmap * imgTitle = new wxStaticBitmap(this, wxID_ANY, *_bmpTitle);
		imgTitle->SetPosition(wxPoint(17, 15));

		wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

		sizer->AddSpacer(200);
		wxBoxSizer * vsizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(vsizer, wxSizerFlags().Center());

		wxStaticBitmap * imgCtrl = new wxStaticBitmap(this, wxID_ANY, *g_Personage->_default);
		vsizer->Add(imgCtrl, wxSizerFlags().Center());
		vsizer->AddSpacer(20);

		wxStaticText * text = new wxStaticText(this, wxID_ANY, L"", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
		wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
		text->SetFont(font);
		text->SetBackgroundStyle(wxBG_STYLE_COLOUR);
		text->SetBackgroundColour(wxColour(0, 0, 0, 0));
		text->SetForegroundColour(wxColour(255, 255, 255, 255));
		text->Wrap(600);

		int ind = 1;
		while (ind < 20)
		{
			if (!langPack->Has(wxString::Format(L"big_pause_text_%d", ind)))
			{
				ind--;
				break;
			}
			++ind;
		}

		if (ind > 0)
			text->SetLabel(langPack->Get(wxString::Format(L"big_pause_text_%d", rand() % ind + 1)));

		vsizer->Add(text);
		vsizer->AddSpacer(50);

		wxStaticText * timeText = new wxStaticText(this, wxID_ANY, L"", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
		wxFont font2(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
		timeText->SetFont(font2);
		timeText->SetBackgroundStyle(wxBG_STYLE_COLOUR);
		timeText->SetBackgroundColour(wxColour(0, 0, 0, 0));
		timeText->SetForegroundColour(wxColour(210, 210, 210, 255));
		timeText->SetSize(200, 30);
		_timeText = timeText;

		wxButton * btnSkip = new wxButton(this, ID_BTN_SKIP, langPack->Get("big_pause_skip_button"));
		btnSkip->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
		btnSkip->SetBackgroundColour(wxColor(255, 255, 255, 0));
		btnSkip->SetBackgroundStyle(wxBG_STYLE_COLOUR);
		btnSkip->SetSize(45, 10);

		if (getApp()->GetStrictModeEnabled())
			btnSkip->Hide();

		vsizer->Add(timeText, wxSizerFlags().Center());
		vsizer->AddSpacer(20);
		vsizer->Add(btnSkip, wxSizerFlags().Center());
		_sizer = vsizer;

		SetSizerAndFit(sizer);
	}

	SetSize(displayRect.GetSize());

	Connect(ID_BTN_SKIP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(BigPauseWindow::OnSkipClicked));

	g_TaskMgr->AddTask(GetName(), 20);
	
#ifdef WIN32
	if (IsWindowsVistaOrGreater())
		WTSRegisterSessionNotification(GetHandle(), NOTIFY_FOR_THIS_SESSION);
#endif
}

BigPauseWindow::~BigPauseWindow()
{
	if (!getApp()->isFinished())
		g_TaskMgr->RemoveTasks(GetName());

	getApp()->OnBigPauseWindowClosed(this);

	logging::msg("BigPauseWindow::~BigPauseWindow end");
}

void BigPauseWindow::SetBreakDuration(int seconds)
{
	_breakTimeLeft = seconds;
	_breakTimeFull = seconds;
}

void BigPauseWindow::UpdateTimeLabel()
{
	if (!_timeText)
		return;
	
	if ( fabsf(_breakTimeLeft) < 0.000001f )
	{
		_timeText->SetLabel(L"");
	}
	else
	{
		wxString str = wxString::Format(langPack->Get("big_pause_time_left_text"), getTimeStr(_breakTimeLeft, SECONDS, getApp()->getLang()));
		if (str != _timeText->GetLabel())
		{
			_timeText->SetLabel(str);
			_sizer->Layout();
		}
	}
}

void BigPauseWindow::ExecuteTask(float f, long /*time_went*/)
{
	if (!_showing && !_hiding)
	{
		_breakTimeLeft -= 0.1f * f;
		if (_breakTimeLeft < 0.0f)
			_breakTimeLeft = 0.0f;
		UpdateTimeLabel();
		
		g_TaskMgr->AddTask(GetName(), 100);
		/*if (_restoreFocus)
		{
			SetForegroundWindow(GetHWND());
			SetFocus();
			ShowWindow(GetHWND(), SW_RESTORE);
			_restoreFocus = false;
		}*/

		return;
	}

	if (_showing)
	{
		_alpha += 7.0f * f;
		if (_alpha >= 215.0f)
		{
			_alpha = 215.0f;
			_showing = false;
			UpdateTimeLabel();
			g_TaskMgr->AddTask(GetName(), 100);
		}
		else
		{
			g_TaskMgr->AddTask(GetName(), 20);
		}
		SetTransparent((int)_alpha);

		return;
	}
	
	if (_hiding)
	{
		_alpha -= 9.0f * f;
		if (_alpha <= 0.0f)
		{
			_alpha = 0.0f;
			_hiding = false;
			_preventClosing = false;
			Close();

			logging::msg(wxString::Format("BigPauseWindow (%s)::Update -> Close", GetName()));
		}
		else
		{
			SetTransparent((int)_alpha);
			g_TaskMgr->AddTask(GetName(), 20);
		}
	}
}


WXLRESULT BigPauseWindow::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{ 
	if (message == WM_WTSSESSION_CHANGE)
	{
		if (wParam == WTS_SESSION_LOGON)
		{
			Hide();
			getApp()->OnSessionUnlock();
			_alpha = 0;
		}
		else if (wParam == WTS_SESSION_UNLOCK)
		{
			Hide();
			getApp()->OnSessionUnlock();
			_alpha = 0;
		}
	}
	
	if (message == WM_PAINT)
	{
		wxPaintEvent e(0);
		//OnPaint(e);
	}

	return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void BigPauseWindow::Hide()
{
	logging::msg("BigPauseWindow::Hide");

	if ( _hiding )
		return;

	_hiding = true;
	_showing = false;
	_preventClosing = false;
	g_TaskMgr->AddTask(GetName(), 20);

	if (_breakTimeFull - _breakTimeLeft < 3.0f)
	{
		_alpha = 0.0f; // quick hide
	}
}

void BigPauseWindow::OnSkipClicked(wxCommandEvent &)
{
	logging::msg("BigPauseWindow::OnSkipClicked");

	getApp()->OnSkipBigPauseClicked();
}

void BigPauseWindow::OnKillFocus(wxFocusEvent &)
{
	//if (_primary)
	//	_restoreFocus = true;
}

void BigPauseWindow::OnClose(wxCloseEvent& event)
{
	if (_preventClosing)
	{
		//disable closing
	}
	else
	{
		event.Skip(true);
	}
}

void BigPauseWindow::OnPaint(wxPaintEvent& )
{
	wxPaintDC dc(this);
	wxBrush brush(wxColour(0, 0, 0, wxALPHA_OPAQUE), wxSOLID);
	dc.SetBrush(brush);
	dc.DrawRectangle(GetPosition(), GetSize());
}
