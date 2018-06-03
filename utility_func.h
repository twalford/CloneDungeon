// * ************************************************************************ //
// * * Utility Header                                                     * * //
// * *   : defines utility functions interface                            * * //
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
// * * Utility.h                                                          * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 12/07/11                      * * //
// * ********************************************************************** * //

#ifndef UTILITY_H
#define UTILITY_H

#include "TArray.h"
#include "cd_game.h"

COORD2 operator+(COORD2 const& lhs, COORD2 const& rhs);
COORD2 operator-(COORD2 const& lhs, COORD2 const& rhs);

COORD2 operator+=(COORD2 const& lhs, COORD2 const& rhs);
COORD2 operator-=(COORD2 const& lhs, COORD2 const& rhs);

bool   operator==(COORD2 const& lhs, COORD2 const& rhs);
bool   operator!=(COORD2 const& lhs, COORD2 const& rhs);

COORD2 operator-(COORD2 const& c);

COORD2	RandomCoordinate(COORD2 const& lower, COORD2 const& upper);

float distance(COORD2 const &p1, COORD2 const &p2);

#if defined (_WIN32)
void ClearConsole(HANDLE hStdOut);
#endif // _WIN32

std::string substring(std::string& str, std::string& delim);
void unquote(std::string& str);
size_t IndexOf(const char* str, const char *str_list[], size_t _siz);

#endif