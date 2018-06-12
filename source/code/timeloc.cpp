#include "timeloc.h"
#include "wx/intl.h"
#include "language_set.h"

wxString getSecondStr(int seconds, wxString const & lang)
{
	if (lang == L"ru")
	{
		int seconds_mod_10 = seconds % 10;
		if (seconds_mod_10 >= 2 && seconds_mod_10 <= 4 && seconds / 10 != 1) // 2-4, 22-24, 32-34 секунды
		{
			return wxString::Format(L"%d %s", seconds, langPack->Get("seconds_"));
		}
		else if (seconds_mod_10 == 1 && seconds != 11) // 1, 21, 31 секунда
		{
			return wxString::Format(L"%d %s", seconds, langPack->Get("second"));
		}
		else if (seconds_mod_10 == 0 || (seconds >= 5 && seconds <= 19) || (seconds_mod_10 >= 5 && seconds_mod_10 <= 9)) // 0, 5-19, 20, 25-29, 30, 35-39 секунд
		{
			return wxString::Format("%d %s", seconds, langPack->Get("seconds"));
		}
	}

	// lang "en" and others
	if (seconds > 1 || seconds == 0)
	{
		return wxString::Format(L"%d %s", seconds, langPack->Get("seconds"));
	}
	else if (seconds == 1)
	{
		return wxString::Format(L"1 %s", langPack->Get("second"));
	}

	return wxString();
}

wxString getTimeStr(int value, ETimeUnit unit, wxString const & lang)
{
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	int milliseconds = 0;
	splitTime(value, unit, &hours, &minutes, &seconds, &milliseconds);
	
	if (lang == L"en")
	{
		if (hours > 1)
		{
			if (minutes != 0)
				return wxString::Format(L"%d %s %d %s", hours, langPack->Get("hours"), minutes, langPack->Get("minutes"));
			else
				return wxString::Format(L"%d %s", hours, langPack->Get("hours"));
		}
		else if (hours == 1)
		{
			if (minutes != 0)
				return wxString::Format(L"1 %s %d %s", langPack->Get("hour"), minutes, langPack->Get("minutes"));
			else
				return wxString::Format(L"1 %s", langPack->Get("hour"));
		}
		else if (minutes > 1)
		{
			if (minutes > 3)
			{
				return wxString::Format(L"%d %s", minutes, langPack->Get("minutes"));
			}
			else // show seconds too
			{
				return wxString::Format(L"%d %s %s", minutes, langPack->Get("minutes"), getSecondStr(seconds, lang).c_str());
			}
		}
		else if (minutes == 1)
		{
			return wxString::Format(L"1 %s %s", langPack->Get("minute"), getSecondStr(seconds, langPack->Get("seconds")));
		}
		else
			return getSecondStr(seconds, lang);
	}
	else if (lang == L"ru")
	{
		//int seconds_mod_10 = seconds % 10;
		int minutes_mod_10 = minutes % 10;
		if (hours >= 2)
		{
			return langPack->Get("too_many_hours");
		}
		else if (hours == 1)
		{
			if (minutes_mod_10 >= 2 && minutes_mod_10 <= 4 && minutes / 10 != 1) // 2-4, 22-24, 32-34 минуты
			{
				return wxString::Format(L"1 %s %d %s", langPack->Get("hour"), minutes, langPack->Get("minutes_"));
			}
			else if (minutes_mod_10 == 1 && minutes != 11) // 1, 21, 31 минута
			{
				return wxString::Format(L"1 %s %d %s", langPack->Get("hour"), minutes, langPack->Get("minute"));
			}
			else if (minutes_mod_10 == 0 || (minutes >= 5 && minutes <= 19) || (minutes_mod_10 >= 5 && minutes_mod_10 <= 9)) // 0, 5-19, 20, 25-29, 30, 35-39 минут
			{
				if (minutes != 0)
					return wxString::Format("1 %s %d %s", langPack->Get("hour"), minutes, langPack->Get("minutes"));
				else
					return wxString::Format("1 %s", langPack->Get("hour"));
			}
		}
		////////////////////////////////////////////////////////////////////////
		else if (minutes_mod_10 >= 2 && minutes_mod_10 <= 4 && minutes / 10 != 1) // 2-4, 22-24, 32-34 минуты
		{
			if (minutes > 3)
				return wxString::Format(L"%d %s", minutes, langPack->Get("minutes_"));
			else
				return wxString::Format(L"%d %s %s", minutes, langPack->Get("minutes_"), getSecondStr(seconds, lang).c_str()); // 2 минуты 35 секунд
		}
		else if (minutes_mod_10 == 1 && minutes != 11) // 1, 21, 31 минута
		{
			if (minutes != 1)
				return wxString::Format(L"%d %s", minutes, langPack->Get("minute"));
			else
				return wxString::Format(L"%d %s %s", minutes, langPack->Get("minute"), getSecondStr(seconds, lang).c_str());
		}
		else if ((minutes_mod_10 == 0 && minutes != 0) || (minutes >= 5 && minutes <= 19) || (minutes_mod_10 >= 5 && minutes_mod_10 <= 9)) // 5-19, 20, 25-29, 30, 35-39 минут
		{
			return wxString::Format("%d %s", minutes, langPack->Get("minutes"));
		}
		////////////////////////////////////////////////////////////////////////
		else
			return getSecondStr(seconds, lang);
	}
	return wxString(L"");
}

void splitTime(int value, ETimeUnit unit, int * pHours, int * pMinutes, int * pSeconds, int * pMilliseconds)
{
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	int milliseconds = 0;
	
	if (unit == MILLISECONDS)
	{
		milliseconds = value;
		if (milliseconds >= 1000)
		{
			seconds = milliseconds / 1000;
			milliseconds %= 1000;
		}
		if (seconds >= 60)
		{
			minutes = seconds / 60;
			seconds %= 60;
		}
		if (minutes >= 60)
		{
			hours = minutes / 60;
			minutes %= 60;
		}
	}
	else if (unit == SECONDS)
	{
		seconds = value;
		if (seconds >= 60)
		{
			minutes = seconds / 60;
			seconds %= 60;
		}
		if (minutes >= 60)
		{
			hours = minutes / 60;
			minutes %= 60;
		}
		milliseconds = seconds * 1000;
	}
	else if (unit == MINUTES)
	{
		minutes = value;
		if (minutes >= 60)
		{
			hours = minutes / 60;
			minutes %= 60;
		}
		seconds = minutes * 60;
		milliseconds = seconds * 1000;
	}
	else if (unit == HOURS)
	{
		hours = value;
		minutes = hours * 60;
		seconds = minutes * 60;
		milliseconds = seconds * 1000;
	}

	if (pHours)
		*pHours = hours;
	if (pMinutes)
		*pMinutes = minutes;
	if (pSeconds)
		*pSeconds = seconds;
	if (pMilliseconds)
		*pMilliseconds = milliseconds;
}