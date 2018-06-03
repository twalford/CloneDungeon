// * ************************************************************************ //
// * * Utility Source                                                     * * //
// * *   : defines utility function implementations                       * * //
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
// * * Utility.cpp                                                        * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 12/07/11                      * * //
// * ********************************************************************** * //

#include <stdlib.h>
#include <cmath>
#include "cd_game.h"
#include "utility_func.h"

COORD2 operator+(COORD2 const& lhs, COORD2 const& rhs)
{
	return lhs += rhs;
}

COORD2 operator-(COORD2 const& lhs, COORD2 const& rhs)
{
	return lhs -= rhs;
}

COORD2 operator+=(COORD2 const& lhs, COORD2 const& rhs)
{
	COORD2 c = lhs;
	c.col += rhs.col;
	c.row += rhs.row;
	return c;
}

COORD2 operator-=(COORD2 const& lhs, COORD2 const& rhs)
{
	COORD2 c = lhs;
	c.col -= rhs.col;
	c.row -= rhs.row;
	return c;
}

bool   operator==(COORD2 const& lhs, COORD2 const& rhs)
{
	return (lhs.col == rhs.col && lhs.row == rhs.row);
}

bool   operator!=(COORD2 const& lhs, COORD2 const& rhs)
{
	return !operator==(lhs, rhs);
}

COORD2 operator-(COORD2 const& c)
{
	COORD2 nc;
	nc.col = -c.col;
	nc.row = -c.row;
	return nc;
}

COORD2	RandomCoordinate(COORD2 const& lower, COORD2 const& upper)
{
	short dx = upper.col - lower.col;
	short dy = upper.row - lower.row;

	COORD2 p;

	p.col = lower.col + rand() % dx;
	p.row = lower.row + rand() % dy;

	return p;
}

float distance(COORD2 const &p1, COORD2 const &p2)
{
	short dx = p2.col - p1.col;
	short dy = p2.row - p1.row;

	return sqrt(float(dx*dx + dy*dy));
}

#if defined (_WIN32)
void ClearConsole(HANDLE hStdOut)
{
	CONSOLE_SCREEN_BUFFER_INFO csbiConsoleInfo;
	GetConsoleScreenBufferInfo(hStdOut, &csbiConsoleInfo);
	SMALL_RECT srcWindow = csbiConsoleInfo.srWindow;
	DWORD dNumChars = (srcWindow.Right - srcWindow.Left + 1)*(srcWindow.Bottom - srcWindow.Top + 1);
	COORD cStart;
	cStart.X = srcWindow.Left;
	cStart.Y = srcWindow.Top;
	FillConsoleOutputCharacter(hStdOut, ' ', dNumChars, cStart, &dNumChars);
}
#endif // _WIN32

std::string substring(std::string& str, std::string& delim) {
	size_t left = str.find_first_of(delim) + 1;
	size_t count = str.find_last_of(delim) - left;
	return str.substr(left, count);
}

void unquote(std::string& str) {
	std::string q("\"");
	str = substring(str, q);
}

size_t IndexOf(const char* str, const char *str_list[], size_t _siz) {
	for (size_t i = 0; i<_siz; i++)
		if (strcmp(str, str_list[i]) == 0)
			return i;
	return -1;
}