// * ************************************************************************ //
// * * pnn_gui header                                                     * * //
// * *   : defines game specific gui token values                         * * //
// * *                                                                    * * //
// * * Disclaimer: This document represents no significant intellectual   * * //
// * * property and my be used freely. The author(s) takes no             * * //
// * * responsibility for any loss or damage arising from the use of      * * //
// * * the computer code contained herein. Where this document is         * * //
// * * submitted as part of an assessment task this header must remain    * * //
// * * intact and the student must make clear indication which parts      * * //
// * * have been added by themselves.                                     * * //
// * *                                                                    * * //
// * ********************************************************************** * //
// * ********************************************************************** * //
// * * pnn_gui.h                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 26/03/11     Last Modified: 31/03/11                      * * //
// * ********************************************************************** * //

#include "wincon.h"

#ifndef CD_GUI_H
#define CD_GUI_H

#if defined(_WIN32)

enum COLOUR {
	BLACK = 0x0000,
	FOREGROUND_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	FOREGROUND_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	FOREGROUND_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	FOREGROUND_GREY = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN,
	FOREGROUND_WHITE = FOREGROUND_GREY | FOREGROUND_INTENSITY,
	BACKGROUND_CYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
	BACKGROUND_MAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
	BACKGROUND_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
	BACKGROUND_GREY = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN,
	BACKGROUND_WHITE = BACKGROUND_GREY | BACKGROUND_INTENSITY
};


const int TOKEN[9] = {		 32,   32,  219, 176,   35,    2,	32,	   32,  7};
//enum entity_types { EMPTY = 0, FLOOR, WALL, ICE, FIRE, HERO, END, START, COIN };

#endif // _WIN32

#endif // CD_GUI_H