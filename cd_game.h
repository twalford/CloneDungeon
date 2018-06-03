// * ************************************************************************ //
// * * Application header                                                 * * //
// * *   : defines application macros and includes                        * * //
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
// * * pnn_game.cpp                                                         * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 05/03/11     Last Modified: 12/07/11                      * * //
// * ********************************************************************** * //

#ifndef CLONE_H
#define CLONE_H


#ifdef __GNUC__
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define __INT32 __uint32_t
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define __INT32 unsigned __int32
#endif

#ifndef SAFE_DELETE_DEF
#define SAFE_DELETE_DEF
template <typename T> void SAFE_DELETE(T* &p) { if (p) { delete p; p = NULL; } }
#endif

#define MAXBUFLEN	1024

// application includes
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>


// Error code enumeration
enum	ERROR_CODE {
	SUCCESS = 0,
	FAILED = 1,
	USAGE_FAILURE,
	BOT_ALLOC_FAILURE,
	EXCEED_TIME_LIMIT,
	EXCEED_TURN_LIMIT,
	INVALID_ACTION
};

#endif // CLONE_H