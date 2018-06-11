#ifndef EXCERCISES_H
#define EXCERCISES_H
#include "wx/string.h"

#define NUM_EXCERCISES 6

enum
{
	EXCERCISE_NONE,
	EXCERCISE_ROLL,
	EXCERCISE_LOOK_VERT,
	EXCERCISE_LOOK_HORZ,
	EXCERCISE_CLOSE_TIGHTLY,
	EXCERCISE_BLINK,
	EXCERCISE_WINDOW
};

class wxStaticBitmap;
class wxBitmap;

struct PersonageData
{
	wxString _name;
	wxBitmap * _default;
	wxBitmap * _lookLeft;
	wxBitmap * _lookRight;
	wxBitmap * _lookUp;
	wxBitmap * _lookDown;
	wxBitmap * _blink;
	wxBitmap * _closeTightly;
	
	PersonageData(wxString const & name);
	~PersonageData();
};

extern PersonageData * g_Personage;

class ExcerciseAnim
{
public:
	ExcerciseAnim(wxStaticBitmap * img, int excerciseNum);
	
	void Update();
	
private:
	wxStaticBitmap * _personageImg;
	int _excercise;
	int _time;
	int _state;
};

#endif