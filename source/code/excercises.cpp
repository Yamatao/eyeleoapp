#include "excercises.h"
#include "wx/bitmap.h"
#include "wx/statbmp.h"
#include "image_resources.h"

PersonageData * g_Personage = 0;

PersonageData::PersonageData(wxString const & name) :
	_default(0),
	_lookLeft(0),
	_lookRight(0),
	_lookUp(0),
	_lookDown(0),
	_blink(0),
	_closeTightly(0)
{
	_name = name;

	wxString path = wxString::Format(L"Personages/%s/%s_", name, name);
	
	_default = new wxBitmap(path + L"default.png", wxBITMAP_TYPE_PNG);
	_lookLeft = new wxBitmap(path + L"look_left.png", wxBITMAP_TYPE_PNG);
	_lookRight = new wxBitmap(path + L"look_right.png", wxBITMAP_TYPE_PNG);
	_lookUp = new wxBitmap(path + L"look_up.png", wxBITMAP_TYPE_PNG);
	_lookDown = new wxBitmap(path + L"look_down.png", wxBITMAP_TYPE_PNG);
	_blink = new wxBitmap(path + L"blink.png", wxBITMAP_TYPE_PNG);
	_closeTightly = new wxBitmap(path + L"close_tightly.png", wxBITMAP_TYPE_PNG);
}

PersonageData::~PersonageData()
{
	delete _default;
	delete _lookLeft;
	delete _lookRight;
	delete _lookUp;
	delete _lookDown;
	delete _blink;
	delete _closeTightly;
}

ExcerciseAnim::ExcerciseAnim(wxStaticBitmap * img, int excerciseNum) :
	_personageImg(img),
	_excercise(excerciseNum),
	_state(0)
{
	if (_excercise == EXERCISE_WINDOW)
	{
		_personageImg->SetBitmap(*_bmpWindow);
	}
	else
	{
		_personageImg->SetBitmap(*g_Personage->_default);
	}
	
	_time = 400;
}

void ExcerciseAnim::Update()
{
	if (_excercise == EXERCISE_WINDOW || _excercise == EXERCISE_LOOK_FAR)
		return;
	
	_time -= 100;

	if (_time <= 0)
	{
		switch(_excercise)
		{
		case EXERCISE_LOOK_HORZ:
			{
				if (_state)
					_personageImg->SetBitmap(*g_Personage->_lookLeft);
				else
					_personageImg->SetBitmap(*g_Personage->_lookRight);
				_state = _state == 1 ? 0 : 1;
				_time = 1200;
			}
			break;
		case EXERCISE_LOOK_VERT:
			{
				if (_state)
					_personageImg->SetBitmap(*g_Personage->_lookUp);
				else
					_personageImg->SetBitmap(*g_Personage->_lookDown);
				_state = _state == 1 ? 0 : 1;
				_time = 1200;
			}
			break;
		case EXERCISE_ROLL:
			{
				if (_state == 0)
					_personageImg->SetBitmap(*g_Personage->_lookLeft);
				else if (_state == 1)
					_personageImg->SetBitmap(*g_Personage->_lookUp);
				else if (_state == 2)
					_personageImg->SetBitmap(*g_Personage->_lookRight);
				else if (_state == 3)
					_personageImg->SetBitmap(*g_Personage->_lookDown);
				if (++_state == 4)
					_state = 0;
				_time = 1200;
			}
			break;
		case EXERCISE_BLINK:
			{
				if (_state)
					_personageImg->SetBitmap(*g_Personage->_blink);
				else
					_personageImg->SetBitmap(*g_Personage->_default);
				_state = _state == 1 ? 0 : 1;
				_time = 200;
			}
			break;
		case EXERCISE_CLOSE_TIGHTLY:
			{
				if (_state)
					_personageImg->SetBitmap(*g_Personage->_closeTightly);
				else
					_personageImg->SetBitmap(*g_Personage->_default);
				_state = _state == 1 ? 0 : 1;
				_time = 2000;
			}
			break;
		}
	}
}