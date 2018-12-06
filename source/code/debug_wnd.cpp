#include "debug_wnd.h"
#include "main.h"

BEGIN_EVENT_TABLE(DebugWindow, wxFrame)
EVT_CLOSE(DebugWindow::OnClose)
END_EVENT_TABLE()


DebugWindow::DebugWindow() :
	wxFrame(NULL, -1, L"DebugWindow", wxDefaultPosition, wxDefaultSize, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	wxBoxSizer * valuesSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer * valueSizer1 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText * txt1 = new wxStaticText(this, wxID_ANY, "_timeLeftToBigPause");
	_timeLeftToBigPause = new wxStaticText(this, wxID_ANY, "");
	valueSizer1->Add(txt1);
	valueSizer1->AddSpacer(15);
	valueSizer1->Add(_timeLeftToBigPause);

	wxBoxSizer * valueSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText * txt2 = new wxStaticText(this, wxID_ANY, "_timeLeftToMiniPause");
	_timeLeftToMiniPause = new wxStaticText(this, wxID_ANY, "");
	valueSizer2->Add(txt2);
	valueSizer2->AddSpacer(15);
	valueSizer2->Add(_timeLeftToMiniPause);

	wxBoxSizer * valueSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText * txt3 = new wxStaticText(this, wxID_ANY, "_inactivityTime");
	_inactivityTime = new wxStaticText(this, wxID_ANY, "");
	valueSizer3->Add(txt3);
	valueSizer3->AddSpacer(15);
	valueSizer3->Add(_inactivityTime);

	wxBoxSizer * valueSizer4 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText * txt4 = new wxStaticText(this, wxID_ANY, "_relaxingTimeLeft");
	_relaxingTimeLeft = new wxStaticText(this, wxID_ANY, "");
	valueSizer4->Add(txt4);
	valueSizer4->AddSpacer(15);
	valueSizer4->Add(_relaxingTimeLeft);

	valuesSizer->Add(valueSizer1);
	valuesSizer->Add(valueSizer2);
	valuesSizer->Add(valueSizer3);
	valuesSizer->Add(valueSizer4);

	SetSizerAndFit(valuesSizer);
	SetSize(GetSize().x + 60, GetSize().y);
}

void DebugWindow::OnClose(wxCloseEvent& event)
{
	event.Skip(true);
	getApp()->OnDebugWindowClosed();
}