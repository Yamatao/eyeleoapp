#include "minipause_wnd.h"
#include "main.h"
#include "oscapabilities.h"
#include "wx/button.h"
#include "image_resources.h"
#include "language_set.h"
#include "excercises.h"
#include <wx/event.h>

BEGIN_EVENT_TABLE(MiniPauseWindow, wxFrame)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MiniPauseControls, wxFrame)
	EVT_CLOSE(MiniPauseControls::OnClose)
END_EVENT_TABLE()

////////////////////////////////////////////////////////////////////////
struct SpecialMessage
{
	int show_count;
	const wchar_t * message;
	wxColour color;
};

static SpecialMessage specialMessages[] =
{
	{30, L"mini_pause_text_settings", wxColour(160, 255, 100)},
	{80, L"mini_pause_text_donate", wxColour(160, 255, 100)},
	{120, L"mini_pause_text_tell_friend", wxColour(160, 255, 100)},
	{150, L"mini_pause_text_posture", wxColour(160, 255, 100)},
	{400, L"mini_pause_text_check_eyeleo", wxColour(160, 255, 100)},
	{600, L"mini_pause_text_donate", wxColour(160, 255, 100)},
	{1000, L"mini_pause_text_donate", wxColour(160, 255, 100)},
	{1400, L"mini_pause_text_donate", wxColour(160, 255, 100)}
};

////////////////////////////////////////////////////////////////////////
int MiniPauseControls::_excercise = 0;
int MiniPauseControls::_line = 0;
int MiniPauseControls::_lastExcercise = 0;

MiniPauseWindow::MiniPauseWindow(const DisplayData& displayData, unsigned int showCount) :
	wxFrame(NULL, -1, L"", wxDefaultPosition, wxDefaultSize, wxFRAME_TOOL_WINDOW | wxFRAME_SHAPED | wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP),
	_preventClosing(true),
	_controlsWnd(0),
	_showCount(showCount),
	_displayData(displayData),
	_state(STATE_SHOWING),
	_alpha(0.0f)
{
	SetName(std::string("MiniPauseWindow") + (char)('0' + displayData.ind));
}

void MiniPauseWindow::Init()
{
	refillResolutionParams();

	wxRegion region(*_backBitmap, *wxWHITE);
	SetShape(region);

	SetBackgroundColour(wxColour(0, 0, 0));
	SetTransparent(0);

	SetSize(_backBitmap->GetSize());
	wxRect displayRect = _displayData.clientArea;
	SetPosition(wxPoint(displayRect.GetX() + displayRect.GetWidth() / 2 - GetSize().GetX() / 2, displayRect.GetY() + displayRect.GetHeight() / 2 - GetSize().GetY() / 2));

	_state = MiniPauseWindow::STATE_SHOWING;
	g_TaskMgr->AddTask(GetName(), 20);
	_alpha = 0.0f;

	Show(true);

	_controlsWnd = new MiniPauseControls(this);
	_controlsWnd->Init(_displayData, _showCount);
	_controlsWnd->Show(true);
}

MiniPauseWindow::~MiniPauseWindow()
{
	if (!getApp()->isFinished())
		g_TaskMgr->RemoveTasks(GetName());
	_controlsWnd = 0;

	getApp()->OnMiniPauseWindowClosed(this);
	assert(!getApp()->getWindow(GetName()));
}

void MiniPauseWindow::ExecuteTask(float f, long time_went)
{
	(void)time_went;

	switch (_state)
	{
		case MiniPauseWindow::STATE_SHOWING:
		{
			_alpha += 11.0f * f;
			if (_alpha >= 210.0f)
			{
				_alpha = 210.0f;
				_state = MiniPauseWindow::STATE_ACTIVE;
				
				int base_duration = getApp()->GetMiniPauseDuration();

				_timeLeft = 1000 * base_duration;
				
				_controlsWnd->UpdateTimeLabel(_timeLeft);
				
				g_TaskMgr->AddTask(GetName(), 100);
			}
			else
			{
				g_TaskMgr->AddTask(GetName(), 20);
			}
			SetTransparent((int)_alpha);
			break;
		}

		case MiniPauseWindow::STATE_ACTIVE:
		{
			_timeLeft -= 100.0f * f;
			
			_controlsWnd->Update();
			
			if (_timeLeft <= 0)
			{
				Hide();
			}
			else
			{
				g_TaskMgr->AddTask(GetName(), 100);
			}

			_controlsWnd->UpdateTimeLabel(_timeLeft);
			break;
		}

		case MiniPauseWindow::STATE_HIDING:
		{
			_alpha -= 15.0f * f;
			if (_alpha <= 0.0f)
			{
				_alpha = 0.0f;
				_state = MiniPauseWindow::STATE_DONE;
				_preventClosing = false;
				SetTransparent(0);
				
				Close();
			}
			else
			{
				SetTransparent((int)_alpha);
				g_TaskMgr->AddTask(GetName(), 20);
			}
			break;
		}
	}
}

void MiniPauseWindow::HideQuick()
{
	_controlsWnd->Show(false);
	_controlsWnd->Destroy();

	wxFrame::Close();

	g_TaskMgr->RemoveTasks(GetName());
}

void MiniPauseWindow::Hide()
{
	if (_state == STATE_HIDING)
		return;

	_controlsWnd->Show(false);
	_controlsWnd->Destroy();

	_state = STATE_HIDING;
	g_TaskMgr->AddTask(GetName(), 20);
}

void MiniPauseWindow::OnClose(wxCloseEvent& event)
{
	if (!_preventClosing)
	{
		event.Skip(true);
	}
}

////////////////////////////////////////////////////////////////////////

MiniPauseControls::MiniPauseControls(wxWindow * parent) :
	wxFrame(parent, -1, L"", wxDefaultPosition, wxDefaultSize, wxFRAME_TOOL_WINDOW | wxFRAME_SHAPED | wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP),
	_preventClosing(true),
	_excerciseAnim(0),
	_excerciseImg(0)
{
	
}

void MiniPauseControls::Init(const DisplayData& displayData, unsigned int showCount)
{
	if ((GetExtraStyle() & WS_EX_LAYERED) == 0 )
		SetWindowLong(GetHWND(), GWL_EXSTYLE, GetExtraStyle() | WS_EX_LAYERED);
	SetLayeredWindowAttributes(GetHWND(), RGB(32, 33, 34), 0, LWA_COLORKEY);
	SetBackgroundColour(wxColour(32, 33, 34));

	// Set position to center of the screen
	SetSize(wxSize(_backBitmap->GetWidth(), _backBitmap->GetHeight()));
	wxRect displayRect = displayData.clientArea;
	SetPosition(wxPoint(displayRect.GetX() + displayRect.GetWidth() / 2 - GetSize().GetX() / 2, displayRect.GetY() + displayRect.GetHeight() / 2 - GetSize().GetY() / 2));

	wxStaticText * txt = new wxStaticText(this, ID_MINIPAUSE_TEXT, L"", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	txt->SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
	txt->SetForegroundColour(wxColour(255, 255, 255, 255));
	txt->SetPosition(wxPoint(183, 52));

	_txtTime = new wxStaticText(this, ID_MINIPAUSE_TIMER, L"", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	_txtTime->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
	_txtTime->SetForegroundColour(wxColour(255, 200, 70, 255));
	_txtTime->SetPosition(wxPoint(420, 119));
	_txtTime->SetSize(wxSize(30, 30));

	_excerciseImg = new wxStaticBitmap(this, ID_MINIPAUSE_PORTRAIT, *_bmpWindow);
	_excerciseImg->SetPosition(wxPoint(18, 38));

	wxStaticBitmap * title = new wxStaticBitmap(this, ID_MINIPAUSE_LOGO, *_bmpTitle);
	title->SetPosition(wxPoint(40, 12));

	if (MiniPauseControls::_excercise == 0)
	{
		for (int tries = 0; tries < 8; ++tries)
		{
			MiniPauseControls::_excercise = rand() % NUM_EXCERCISES + 1;
			if (getApp()->GetWindowNearbySetting() == false && MiniPauseControls::_excercise == EXCERCISE_WINDOW)
				continue;
			if (MiniPauseControls::_excercise != MiniPauseControls::_lastExcercise)
				break;
		}

		MiniPauseControls::_lastExcercise = MiniPauseControls::_excercise;

		int ind = 1;
		while (ind < 20)
		{
			wxString id = wxString::Format(L"mini_pause_text_%d_%d", _excercise, ind);
			if (!langPack->Has(id))
			{
				ind--;
				break;
			}
			++ind;
		}

		if (ind > 0)
			MiniPauseControls::_line = rand() % ind + 1;
	}

	if (_excercise > 0 && _line > 0)
	{
		wxString text;
		int numSpecMsgs = sizeof(specialMessages) / sizeof(specialMessages[0]);
		for (int i = 0; i < numSpecMsgs; i++)
		{
			if ( (int)showCount == specialMessages[i].show_count)
			{
				text = langPack->Get(specialMessages[i].message);
				txt->SetForegroundColour(specialMessages[i].color);
				break;
			}
		}

		if ( text.empty() )
		{
			wxString id = wxString::Format(L"mini_pause_text_%d_%d", MiniPauseControls::_excercise, MiniPauseControls::_line);
			text = langPack->Get(id);
			txt->SetForegroundColour(wxColour(255, 255, 255));
		}

		txt->SetLabel(text);
	}

	_excerciseAnim = new ExcerciseAnim(_excerciseImg, _excercise);

	txt->SetSize(wxSize(250, 80));

	///
	_excerciseImg->Bind(wxEVT_RIGHT_UP, &MiniPauseControls::OnMouseTap, this);
	_txtTime->Bind(wxEVT_RIGHT_UP, &MiniPauseControls::OnMouseTap, this);
	txt->Bind(wxEVT_RIGHT_UP, &MiniPauseControls::OnMouseTap, this);
	title->Bind(wxEVT_RIGHT_UP, &MiniPauseControls::OnMouseTap, this);
	Bind(wxEVT_RIGHT_UP, &MiniPauseControls::OnMouseTap, this);
}

MiniPauseControls::~MiniPauseControls()
{
	delete _excerciseAnim;
	MiniPauseControls::_excercise = 0;
	MiniPauseControls::_line = 0;
}

void MiniPauseControls::UpdateTimeLabel(int msLeft)
{
	int secsLeft = (msLeft - 1) / 1000 + 1;
	if (secsLeft < 0)
		secsLeft = 0;
	wxString newStr = wxString::Format(L"%d", secsLeft);
	if (_txtTime->GetLabel() != newStr)
		_txtTime->SetLabel(newStr);
}

void MiniPauseControls::Update()
{
	_excerciseAnim->Update();
}

void MiniPauseControls::OnMouseTap(wxMouseEvent&)
{
	if (getApp()->GetCanCloseNotificationsSetting())
	{
		MiniPauseWindow* wnd = dynamic_cast<MiniPauseWindow *>(GetParent());
		if (wnd)
			wnd->HideQuick();
	}
}

void MiniPauseControls::OnClose(wxCloseEvent& event)
{
	if (!_preventClosing)
	{
		event.Skip(true);
	}
}
