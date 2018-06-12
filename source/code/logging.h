#pragma once

#include "wx/string.h"

namespace logging
{
	void Init();
	void msg(wxString const & msg);
}