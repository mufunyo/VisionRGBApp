#include "stdafx.h"

HFONT GetWindowsDefaultFont()
{
	// Magical GDI thing to get Windows' default GUI font.
	return (HFONT)GetStockObject(DEFAULT_GUI_FONT);
}
