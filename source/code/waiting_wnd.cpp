#include "waiting_wnd.h"
#include "oscapabilities.h"
#include "main.h"
#include "image_resources.h"
#include "language_set.h"
#include "logging.h"

BEGIN_EVENT_TABLE(WaitingFullscreenWindow, wxFrame)
	EVT_PAINT(WaitingFullscreenWindow::OnPaint)
	EVT_CLOSE(WaitingFullscreenWindow::OnClose)
END_EVENT_TABLE()


WaitingFullscreenWindow::WaitingFullscreenWindow() :
	wxFrame(NULL, -1, L"", wxDefaultPosition, wxDefaultSize, wxFRAME_TOOL_WINDOW | wxFRAME_SHAPED | wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP),
	_preventClosing(true)
{
}

void WaitingFullscreenWindow::Init(int displayInd)
{
	SetName(wxString("WaitingFullscreenWindow") + (char)('0' + displayInd));

	refillResolutionParams();

	wxRegion region(*_backBitmap_long, *wxWHITE);
	SetShape(region);

	_showing = true;
	_hiding = false;
	_alpha = 0.0f;

	SetTransparent((int)_alpha);

	// Set position to center of the screen
	SetSize(wxSize(_backBitmap_long->GetWidth(), _backBitmap_long->GetHeight()));
	assert(displayInd < osCaps.numDisplays);
	wxRect displayRect = osCaps.displays[displayInd].clientArea;
	SetPosition(wxPoint(displayRect.GetX() + displayRect.GetWidth() / 2 - GetSize().GetX() / 2, displayRect.GetY() + displayRect.GetHeight() / 2 - GetSize().GetY() / 2));

	wxStaticText * txt = new wxStaticText(this, wxID_ANY, langPack->Get("waiting_text"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	wxFont font(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	txt->SetFont(font);
	txt->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	txt->SetBackgroundColour(wxColour(0, 0, 0, 0));
	txt->SetForegroundColour(wxColour(255, 255, 255, 255));
	txt->SetPosition(wxPoint(0, 25));
	txt->SetSize(wxSize(550, 30));

	g_TaskMgr->AddTask(GetName(), 20);
}

WaitingFullscreenWindow::~WaitingFullscreenWindow()
{
	logging::msg("WaitingFullscreenWindow::~WaitingFullscreenWindow");

	if (!getApp()->isFinished())
		g_TaskMgr->RemoveTasks(GetName());

	getApp()->OnCloseWaitingWnd(this);
	assert(!getApp()->getWindow(GetName()));
}

void WaitingFullscreenWindow::ExecuteTask(float f, long time_went)
{
	(void)time_went;

	if (!_showing && !_hiding)
	{
		g_TaskMgr->AddTask(GetName(), 20);
		_hiding = true;
	}
	else if (_showing)
	{
		_alpha += 11.0f * f;
		if (_alpha >= 220.0f)
		{
			_alpha = 220.0f;
			_showing = false;
			
			g_TaskMgr->AddTask(GetName(), 60 * 1000); // 1 min
		}
		else
		{
			g_TaskMgr->AddTask(GetName(), 20);
		}
		SetTransparent(_alpha);
	}
	else if (_hiding)
	{
		_alpha -= 15.0f * f;
		if (_alpha <= 0.0f)
		{
			_alpha = 0.0f;
			_hiding = false;
			_preventClosing = false;
			
			Close();
		}
		else
		{
			SetTransparent(_alpha);
			g_TaskMgr->AddTask(GetName(), 20);
		}
	}
}

void WaitingFullscreenWindow::Hide()
{
	if ( _hiding )
		return;

	logging::msg("WaitingFullscreenWindow::Hide");

	_hiding = true;
	_showing = false;
	_preventClosing = false;
	
	g_TaskMgr->AddTask(GetName(), 20);
}

void WaitingFullscreenWindow::OnClose(wxCloseEvent& event)
{
	if (!_preventClosing)
		event.Skip(true);
}

void WaitingFullscreenWindow::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*_backBitmap_long, 0, 0, true);
}