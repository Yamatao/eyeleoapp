#ifndef EXCERCISES_H
#define EXCERCISES_H
#include "wx/string.h"

enum EExercise
{
    EXERCISE_NONE = 0,
    EXERCISE_ROLL,
    EXERCISE_LOOK_VERT,
    EXERCISE_LOOK_HORZ,
    EXERCISE_CLOSE_TIGHTLY,
    EXERCISE_BLINK,
    EXERCISE_WINDOW,
    EXERCISE_STRETCH,

    NUM_EXERCISES = EXERCISE_STRETCH
};

class wxStaticBitmap;
class wxBitmap;

struct PersonageData {
    wxString _name;
    wxBitmap *_default;
    wxBitmap *_lookLeft;
    wxBitmap *_lookRight;
    wxBitmap *_lookUp;
    wxBitmap *_lookDown;
    wxBitmap *_blink;
    wxBitmap *_closeTightly;

    PersonageData(wxString const &name);
    ~PersonageData();
};

extern PersonageData *g_Personage;

class ExcerciseAnim {
public:
    ExcerciseAnim(wxStaticBitmap *img, int excerciseNum);

    void Update();

private:
    wxStaticBitmap *_personageImg;
    int _excercise;
    int _time;
    int _state;
};

#endif