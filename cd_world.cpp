#include "cd_game.h"
#include "cd_world.h"
#include "utility_func.h"
//include logstream.h

#include <ctime>
#include <fstream>
#include <string>
#include <array>

DungeonWorld::DungeonWorld()
	: m_ihandler(),
	  m_xBoard(),
	  m_pCharacterObject(NULL),
	  m_pGUIFrame(NULL)
{
	m_pCharacterObject = new CharacterState();
}
DungeonWorld::~DungeonWorld()
{
	SAFE_DELETE(m_pGUIFrame);
}


void DungeonWorld::OnReset()
{
	InitBoard(32, 32);
	InitGUI(); 
}

void DungeonWorld::InitGUI()
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut != INVALID_HANDLE_VALUE)
	{
		ClearConsole(hStdOut);
		SetConsoleTitle(TEXT("Clone Dungeon"));
		CONSOLE_CURSOR_INFO	ciCursor = { 1, FALSE };
		SetConsoleCursorInfo(hStdOut, &ciCursor);
		SMALL_RECT srWin;

		short r0 = 1, c0 = 1, dr = 2, dc1 = 3, dc2 = 30, sc = 5;

		short sGUIRows = 2 * r0 + 2 * dr + m_xBoard.rows();
		short sGUICols = 2 * c0 + dc1 + dc2 + m_xBoard.cols();

		short sWinRows = sGUIRows;
		short sWinCols = sGUICols;

		COORD2 pos0(r0, c0),
			pos1(r0 + dr, c0 + dc1),
			pos2(r0 + dr, dc1 + m_xBoard.cols() + sc),
			pos3(r0 + dr + m_xBoard.rows() / 3, dc1 + m_xBoard.cols() + sc);

		srWin.Left = 0;
		srWin.Top = 0;
		srWin.Bottom = srWin.Top + sWinRows - 1;
		srWin.Right = srWin.Left + sWinCols - 1;
		SetConsoleWindowInfo(hStdOut, true, &srWin);
		COORD cWinSize = { sWinCols, sWinRows };
		SetConsoleScreenBufferSize(hStdOut, cWinSize);

		typedef Adaptor1<DungeonWorld, GUIBUFFER&, void>	GUIFUNCTOR;
		// Create a panel for the GUI frame
		SAFE_DELETE(m_pGUIFrame);
		m_pGUIFrame = new GUIPanel(hStdOut, pos0, sGUIRows - 2 * r0, sGUICols - 2 * c0);
		m_pGUIFrame->SetCallback(new GUIFUNCTOR(this, &DungeonWorld::DrawFrame));

		// Force the frame to draw
		m_pGUIFrame->notify();

		// Create a Panel for the board view
		SAFE_DELETE(m_pGUIBoard);
		m_pGUIBoard = new GUIPanel(hStdOut, pos1, m_xBoard.rows(), m_xBoard.cols());
		m_pGUIBoard->SetCallback(new GUIFUNCTOR(this, &DungeonWorld::DrawBoard));

		// Create a Panel for the Monster state view
		typedef Adaptor1<CharacterState, GUIBUFFER&, void>	GUISTATEFUNCTOR;
		SAFE_DELETE(m_pGUIState);
		m_pGUIState = new GUIPanel(hStdOut, pos2, 6, 20);
		m_pGUIState->SetCallback(new GUISTATEFUNCTOR(m_pCharacterObject, &CharacterState::Draw));
		m_pCharacterObject->attach(m_pGUIState);

		m_pGUIState->notify();

	}
}

void DungeonWorld::InitBoard(size_t rows, size_t cols)
{
	m_xBoard.resize(rows, cols);
	m_xBigBoard.resize(rows * 2, cols);

	// bigBoard init
		// Empty everything
	for (unsigned int r = 63; r > 0; r--)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			m_xBigBoard(r, c).type = EMPTY;
		}
	}

		// Bottom Starting Area
	for (unsigned int r = 41; r >= 31; r--)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			if (c >= 15 && c <= 17) m_xBigBoard(r, c).type = FLOOR;
			else m_xBigBoard(r, c).type = EMPTY;
		}
	}
	m_xBigBoard(39, 16).type = HERO;
	m_xBigBoard(36, 17).type = ICE;
	m_xBigBoard(35, 17).type = ICE;
	m_xBigBoard(35, 16).type = COIN;
	m_xBigBoard(36, 15).type = FIRE;
	m_xBigBoard(35, 15).type = FIRE;

	// Start generating paths.
	COORD2 pathEnd(32,16);
	COORD2 northOne(-1, 0);
	do
	{
		pathEnd = GeneratePath(pathEnd + northOne);
	} while (pathEnd.row > 15);

	//Send middle section of BigBoard to xBoard
	CopyBoard();
}

void DungeonWorld::DrawBoard(GUIBUFFER& rBuf)
{
	// if the board and the buffer have different sizes, we cannot draw
	if (m_xBoard.length() != rBuf.length())
		return;

	// otherwise, iterate over board and set buffer characters as appropriate

	GAMEBOARD::const_iterator	boardIter = m_xBoard.begin();
	GUIBUFFER::iterator			bufferIter = rBuf.begin();

	for (;
		boardIter != m_xBoard.end(),
		bufferIter != rBuf.end();
		++boardIter, ++bufferIter)
	{
		bufferIter->Char.AsciiChar = (char)TOKEN[boardIter->type];

		switch (boardIter->type)
		{
		case EMPTY:
			bufferIter->Attributes = 0;
			break;
		case FLOOR:
		case START:
		case END:
			bufferIter->Attributes = FOREGROUND_GREY | BACKGROUND_INTENSITY;
			break;
		case WALL:
			bufferIter->Attributes = FOREGROUND_WHITE | FOREGROUND_INTENSITY;
			break;
		case ICE:
			bufferIter->Attributes = FOREGROUND_CYAN | FOREGROUND_INTENSITY;
			break;
		case FIRE:
			bufferIter->Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		case HERO:
			bufferIter->Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case COIN:
			bufferIter->Attributes = FOREGROUND_YELLOW | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY;
		};
	}
}

void DungeonWorld::DrawFrame(GUIBUFFER& rBuf)
{
	if (rBuf.length() == 0)
		return;

	COORD2 size = rBuf.size();
	// set corners
	rBuf(0, 0).Char.AsciiChar = (char)201; // top-left
	rBuf(0, size.col - 1).Char.AsciiChar = (char)187; // top-right
	rBuf(size.row - 1, 0).Char.AsciiChar = (char)200; // bottom-left
	rBuf(size.row - 1, size.col - 1).Char.AsciiChar = (char)188; // bottom-right
	rBuf(0, 0).Attributes = FOREGROUND_WHITE;
	rBuf(0, size.col - 1).Attributes = FOREGROUND_WHITE;
	rBuf(size.row - 1, 0).Attributes = FOREGROUND_WHITE;
	rBuf(size.row - 1, size.col - 1).Attributes = FOREGROUND_WHITE;

	// fill in top and bottom border
	for (short c = 1; c < size.col - 1; ++c)
	{
		rBuf(0, c).Char.AsciiChar = (char)205; // top row
		rBuf(0, c).Attributes = FOREGROUND_WHITE;
		rBuf(size.row - 1, c).Char.AsciiChar = (char)205; // bottom row
		rBuf(size.row - 1, c).Attributes = FOREGROUND_WHITE;
	}
	// fill in left and right border
	for (short r = 1; r < size.row - 1; ++r)
	{
		rBuf(r, 0).Char.AsciiChar = (char)186; // top row
		rBuf(r, size.col - 1).Char.AsciiChar = (char)186; // bottom row
		rBuf(r, 0).Attributes = FOREGROUND_WHITE;
		rBuf(r, size.col - 1).Attributes = FOREGROUND_WHITE;
	}
}

void DungeonWorld::MoveCharacter(DIRECTION d)
{
	COORD2 dp, p = m_pCharacterObject->GetPosition();

	switch (d)
	{
	case NORTH: // W
		dp.col = 0;
		dp.row = -1;
		m_pCharacterObject->SetDirection(NORTH);
		break;
	case WEST: // A
		dp.col = -1;
		dp.row = 0;
		m_pCharacterObject->SetDirection(WEST);
		break;
	case SOUTH: // S
		dp.col = 0;
		dp.row = 1;
		m_pCharacterObject->SetDirection(SOUTH);
		break;
	case EAST: // D
		dp.col = 1;
		dp.row = 0;
		m_pCharacterObject->SetDirection(EAST);
		break;
	}
	OnStep(p, dp);
}

void DungeonWorld::OnStep(COORD2 p, COORD2 dp)
{

	ENTITY nextTile = m_xBigBoard(p + dp).type;

	switch (nextTile)
	{
	case FLOOR :
	case END   :
	case START :
		m_pCharacterObject->SetSliding(false);
		break;

	case   ICE :
		m_pCharacterObject->SetSliding(true);
		break;

	case COIN  :
		m_pCharacterObject->SetSliding(false);
		m_pCharacterObject->UpdateCoins(1);
		nextTile = FLOOR;
		break;

	case  WALL :
		m_pCharacterObject->SetSliding(false);
		return;

	case EMPTY :
	case FIRE  :
		m_eGameState = LOSE;
		return;
	default:
		break;
	}


	m_pCharacterObject->UpdateDepth(dp.row);

	m_xBigBoard(p + dp).type = HERO;
	m_xBigBoard(p).type = m_pCharacterObject->GetStanding();
	m_pCharacterObject->SetStanding(nextTile);
	COORD2 newPos = p + dp;
	newPos.row = 39; // Player ROW position should never change.
	m_pCharacterObject->SetPosition(newPos);

	//Shift the board in the opposite direction
	ShiftBoard(m_pCharacterObject->GetDirection());
	CopyBoard();

	

}

void DungeonWorld::CopyBoard()
{
	// Copy bigBoard to xBoard
	for (unsigned int c = 0; c < m_xBoard.cols(); c++)
	{
		for (unsigned int r = 0; r < m_xBoard.rows(); r++)
		{
			m_xBoard(r, c).type = m_xBigBoard(r + 16, c).type;
		}
	}
}

void DungeonWorld::ShiftBoard(DIRECTION d)
{
	switch (d)
	{
		//Player goes north, board goes DOWN the screen.
	case NORTH :

		for (unsigned int i = m_xBigBoard.rows() - 1; i > 0; i--)
		{
			for (unsigned int j = m_xBigBoard.cols() - 1; j > 0; j--)
			{
				if (i > 0)
					m_xBigBoard(i, j).type = m_xBigBoard(i - 1, j).type;
				else
					m_xBigBoard(i, j).type = EMPTY;
			}
		}

		//Check for end of path
		for (unsigned int i = 0; i < m_xBigBoard.cols(); i++)
		{
			if (m_xBigBoard(16, i).type == END)
			{
				if (m_xBigBoard(15, i).type != START)
				{
					COORD2 p;
					p.row = 15;
					p.col = i;
					
					GeneratePath(p);
				}
			}
		}
		return;

		//Player goes south, board goes UP the screen.
	case SOUTH :

		for (unsigned int i = 0; i < m_xBigBoard.rows(); i++)
		{
			for (unsigned int j = 0; j < m_xBigBoard.cols(); j++)
			{
				if (i < m_xBigBoard.rows() - 1)
					m_xBigBoard(i, j).type = m_xBigBoard(i + 1, j).type;
				else
					m_xBigBoard(i, j).type = EMPTY;
			}
		}
		return;

	default:
		break;
	}
}

COORD2 DungeonWorld::PathVarient_DoubleIceLoop(COORD2 p)
{
	/*		  |
		  +++ |
		  | | |
		  +++++++
			| | |
			| +++
			p
	*/

	COORD2 dp;

	if (p.col >= 16) // INVERTED (end up to the left)
	{
		m_xBigBoard(p.row, p.col).type = START;

		short i = 0;
		// If we are too close to the edge, we move 2 tiles toward the
		// centre, and two tiles up. Set 'i' = 2. This is our offset.
		if (p.col >= 29)
		{
			m_xBigBoard(p.row - 1, p.col).type = FLOOR;
			m_xBigBoard(p.row - 1, p.col - 1).type = FLOOR;
			m_xBigBoard(p.row - 1, p.col - 2).type = FLOOR;
			m_xBigBoard(p.row - 2, p.col - 2).type = FLOOR;
			i = 2;
		}

		//LEFT LOOP
		m_xBigBoard(p.row - 1 - i, p.col - i).type = ICE;
		m_xBigBoard(p.row - 2 - i, p.col - i).type = ICE;
		m_xBigBoard(p.row - 3 - i, p.col - i).type = ICE;
		m_xBigBoard(p.row - 4 - i, p.col - i).type = ICE;
		m_xBigBoard(p.row - 5 - i, p.col - i).type = FLOOR;
		m_xBigBoard(p.row - 5 - i, p.col + 1 - i).type = ICE;
		m_xBigBoard(p.row - 5 - i, p.col + 2 - i).type = FLOOR;
		m_xBigBoard(p.row - 4 - i, p.col + 2 - i).type = ICE;
		m_xBigBoard(p.row - 3 - i, p.col + 2 - i).type = FLOOR;
		m_xBigBoard(p.row - 3 - i, p.col + 1 - i).type = ICE;
		//MIDDLE
		m_xBigBoard(p.row - 3 - i, p.col - 1 - i).type = ICE;
		//RIGHT LOOP
		m_xBigBoard(p.row - 3 - i, p.col - 2 - i).type = ICE;
		m_xBigBoard(p.row - 3 - i, p.col - 3 - i).type = ICE;
		m_xBigBoard(p.row - 3 - i, p.col - 4 - i).type = FLOOR;
		m_xBigBoard(p.row - 2 - i, p.col - 4 - i).type = ICE;
		m_xBigBoard(p.row - 1 - i, p.col - 4 - i).type = FLOOR;
		m_xBigBoard(p.row - 1 - i, p.col - 3 - i).type = ICE;
		m_xBigBoard(p.row - 1 - i, p.col - 2 - i).type = FLOOR;
		m_xBigBoard(p.row - 2 - i, p.col - 2 - i).type = ICE;
		m_xBigBoard(p.row - 3 - i, p.col - 2 - i).type = ICE;
		m_xBigBoard(p.row - 4 - i, p.col - 2 - i).type = ICE;
		m_xBigBoard(p.row - 5 - i, p.col - 2 - i).type = ICE;
		//END
		m_xBigBoard(p.row - 6 - i, p.col - 2 - i).type = END;

		dp.row = -6 - i;
		dp.col = -2 - i;
	}
	else // AS PICTURED
	{
		m_xBigBoard(p.row, p.col).type = START;

		short i = 0;
		// If we are too close to the edge, we move 2 tiles toward the
		// centre, and two tiles up. Set 'i' = 2. This is our offset.
		if (p.col <= 2)
		{
			m_xBigBoard(p.row - 1, p.col).type = FLOOR;
			m_xBigBoard(p.row - 1, p.col + 1).type = FLOOR;
			m_xBigBoard(p.row - 1, p.col + 2).type = FLOOR;
			m_xBigBoard(p.row - 2, p.col + 2).type = FLOOR;
			i = 2;
		}


		//LEFT LOOP
		m_xBigBoard(p.row - 1 + i, p.col + i).type = ICE;
		m_xBigBoard(p.row - 2 + i, p.col + i).type = ICE;
		m_xBigBoard(p.row - 3 + i, p.col + i).type = ICE;
		m_xBigBoard(p.row - 4 + i, p.col + i).type = ICE;
		m_xBigBoard(p.row - 5 + i, p.col + i).type = FLOOR;
		m_xBigBoard(p.row - 5 + i, p.col - 1 + i).type = ICE;
		m_xBigBoard(p.row - 5 + i, p.col - 2 + i).type = FLOOR;
		m_xBigBoard(p.row - 4 + i, p.col - 2 + i).type = ICE;
		m_xBigBoard(p.row - 3 + i, p.col - 2 + i).type = FLOOR;
		m_xBigBoard(p.row - 3 + i, p.col - 1 + i).type = ICE;
		//MIDDLE
		m_xBigBoard(p.row - 3 + i, p.col + 1 + i).type = ICE;
		//RIGHT LOOP
		m_xBigBoard(p.row - 3 + i, p.col + 2 + i).type = ICE;
		m_xBigBoard(p.row - 3 + i, p.col + 3 + i).type = ICE;
		m_xBigBoard(p.row - 3 + i, p.col + 4 + i).type = FLOOR;
		m_xBigBoard(p.row - 2 + i, p.col + 4 + i).type = ICE;
		m_xBigBoard(p.row - 1 + i, p.col + 4 + i).type = FLOOR;
		m_xBigBoard(p.row - 1 + i, p.col + 3 + i).type = ICE;
		m_xBigBoard(p.row - 1 + i, p.col + 2 + i).type = FLOOR;
		m_xBigBoard(p.row - 2 + i, p.col + 2 + i).type = ICE;
		m_xBigBoard(p.row - 3 + i, p.col + 2 + i).type = ICE;
		m_xBigBoard(p.row - 4 + i, p.col + 2 + i).type = ICE;
		m_xBigBoard(p.row - 5 + i, p.col + 2 + i).type = ICE;
		//END
		m_xBigBoard(p.row - 6 + i, p.col + 2 + i).type = END;

		dp.row = -6 + i;
		dp.col = 2 + i;
	}
	
	return p + dp;
}

COORD2 DungeonWorld::PathVarient_TwoCorner(COORD2 p)
{
	/*		|
			|
			|
		+++++
		|
		|
		|
	*/	

	int v1 = rand() % 6 + 2;
	COORD2 dp(-v1 * 2, -v1);

	if (p.col >= 16) // Turn to the LEFT if COL is GREATER than or eq to 16;
	{
		for (int i = 0; i < v1; i++)
		{
			m_xBigBoard(p.row - i, p.col).type = FLOOR;
			m_xBigBoard(p.row - v1, p.col - i).type = COIN;
			m_xBigBoard(p.row - v1 - i, p.col - v1).type = FLOOR;
		}
		m_xBigBoard(p.row - v1, p.col).type = FLOOR;
	}
	else // Turn to the RIGHT if COL is LESS than 16;
	{
		for (int i = 0; i < v1; i++)
		{
			m_xBigBoard(p.row - i, p.col).type = FLOOR;
			m_xBigBoard(p.row - v1, p.col + i).type = COIN;
			m_xBigBoard(p.row - v1 - i, p.col + v1).type = FLOOR;
		}
		m_xBigBoard(p.row - v1, p.col).type = FLOOR;
		dp.col *= -1;
	}
	
	m_xBigBoard(p).type = START;
	m_xBigBoard(p + dp).type = END;

	return p + dp;
}

COORD2 DungeonWorld::PathVarient_FreeCoin(COORD2 p)
{
	/*	|
		c
		|
	*/

	COORD2 dp(-2, 0);

	m_xBigBoard(p).type = START;
	m_xBigBoard(p.row - 1, p.col).type = COIN;
	m_xBigBoard(p + dp).type = END;

	return p + dp;
}

COORD2 DungeonWorld::GeneratePath(COORD2 p)
{
	COORD2 r;

	//Varient Selector
	switch (rand() % 10)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		r = PathVarient_FreeCoin(p);
		break;
	case 5:
	case 6:
	case 7:
		r = PathVarient_DoubleIceLoop(p);
		break;
	case 8:
	case 9:
		//r = PathVarient_DoubleIceLoop(p);
		r = PathVarient_TwoCorner(p);
		break;
	default:
		break;
	}

	return r;
}

short DungeonWorld::OnPlay()
{
	m_eGameState = RUNNING;

	if (m_pGUIBoard)
		m_pGUIBoard->notify();

	// Gameloop
	do
	{
		// TIMING =========================


		// INPUT ==========================

		m_ihandler.UpdateKeyboardInput();

		// LOGIC ==========================

		if (m_pCharacterObject->GetSliding())
			MoveCharacter(m_pCharacterObject->GetDirection());
		else
		{
			if (m_ihandler.GetKey(0).bPressed)		MoveCharacter(NORTH); // W
			else if (m_ihandler.GetKey(1).bPressed)	MoveCharacter(WEST);  // A
			else if (m_ihandler.GetKey(2).bPressed)	MoveCharacter(SOUTH); // S
			else if (m_ihandler.GetKey(3).bPressed)	MoveCharacter(EAST);  // D
		}

		// RENDER =========================

		if (m_pGUIBoard)
			m_pGUIBoard->notify();

	} while (m_eGameState == RUNNING);

	return m_pCharacterObject->GetDepth();
}

void DungeonWorld::OnLose()
{

}