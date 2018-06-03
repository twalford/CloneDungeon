#include <iostream>
#include "cd_character.h"
#include "utility_func.h"
#include "cd_world.h"
//#include "log_stream.h"

#include <algorithm>
using std::max;

using std::cerr;
using std::endl;

CharacterState::CharacterState(short const &x, short const &y)
	: m_pObs(0)
{
	m_sPosition.col = x;
	m_sPosition.row = y;
	m_eStanding = FLOOR;
	m_eDirection = NORTH;
	m_bSliding = false;
	m_sDepth = 0;
	m_sCoins = 0;
}

void CharacterState::SetPosition(COORD2 const& c)
{
	m_sPosition = c;
	if (m_pObs)
		m_pObs->notify();
}

COORD2 const& CharacterState::GetPosition() const
{
	return m_sPosition;
}

void CharacterState::attach(Observer *pObs)
{
	m_pObs = pObs;
}

void CharacterState::Draw(GUIBUFFER& rBuf)
{

	SHORT rows = rBuf.size().row;
	SHORT cols = rBuf.size().col;

	COORD pos = { 0, 0 };


	char szWords[6][20] = {
		"Character STATE",
		"Position :",
		"Standing :",
		"Direction:",
		"Depth    :",
		"Coins    :"
	};
	sprintf_s(szWords[1] + 10, 8 * sizeof(char), " %2d,%2d", m_sPosition.row, m_sPosition.col);

	switch (m_eStanding)
	{
	case EMPTY:
		sprintf_s(szWords[2] + 10, 7, " %s", "EMPTY");
		break;
	case FLOOR:
		sprintf_s(szWords[2] + 10, 7, " %s", "FLOOR");
		break;
	case WALL:
		sprintf_s(szWords[2] + 10, 7, "  %s", "WALL");
		break;
	case FIRE:
		sprintf_s(szWords[2] + 10, 7, "  %s", "FIRE");
		break;
	case HERO:
		sprintf_s(szWords[2] + 10, 7, "  %s", "HERO");
		break;
	case END:
		sprintf_s(szWords[2] + 10, 7, "   %s", "END");
		break;
	case START:
		sprintf_s(szWords[2] + 10, 7, " %s", "START");
		break;
	default:
		break;
	};

	switch (m_eDirection)
	{
	case NORTH:
		sprintf_s(szWords[3] + 10, 7, " %s", "NORTH");
		break;
	case EAST:
		sprintf_s(szWords[3] + 10, 7, "  %s", "EAST");
		break;
	case SOUTH:
		sprintf_s(szWords[3] + 10, 7, " %s", "SOUTH");
		break;
	case WEST:
		sprintf_s(szWords[3] + 10, 7, "  %s", "WEST");
		break;
	default:
		break;
	};

	sprintf_s(szWords[4] + 10, 8 * sizeof(char), " %2d", m_sDepth);
	sprintf_s(szWords[5] + 10, 8 * sizeof(char), " %2d", m_sCoins);
	
	for (SHORT r = 0; r < 6; ++r)
	{
		for (SHORT c = 0; c < 20; ++c)
		{
			char ch = szWords[r][c];
			if (ch == '\0')
				break;
			rBuf(r, c).Char.AsciiChar = ch;
			if (r == 0)
				rBuf(r, c).Attributes = COMMON_LVB_UNDERSCORE | FOREGROUND_WHITE;
			else
				rBuf(r, c).Attributes = FOREGROUND_GREY;

		}

	}

	

}

