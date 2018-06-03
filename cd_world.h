#ifndef CAVE_WORLD_H
#define CAVE_WORLD_H

#include <map>
#include <vector>
#include "GameTypes.h"
#include "cd_character.h"
#include "Observer.h"
#include "cd_gui.h"
#include "console_gui.h"
#include "input_handler.h"

class DungeonWorld
{
	private:

		InputHandler	m_ihandler;

		GAMEBOARD		m_xBoard;
		GAMEBOARD		m_xBigBoard;

		CharacterState	*m_pCharacterObject;

		GAMESTATE		m_eGameState;

		GUIPanel		*m_pGUIFrame,
						*m_pGUIBoard,
						*m_pGUIState;

		void DungeonWorld::DrawBoard(GUIBUFFER& rBuf);
		void DungeonWorld::DrawFrame(GUIBUFFER& rBuf);

		void InitGUI();

		void InitBoard(size_t rows, size_t cols);

		void MoveCharacter(DIRECTION d);
		void OnStep(COORD2 p, COORD2 dp);
		void ShiftBoard(DIRECTION d);
		void CopyBoard();

		COORD2 GeneratePath(COORD2 p);
		COORD2 PathVarient_DoubleIceLoop(COORD2 p);
		COORD2 PathVarient_TwoCorner(COORD2 p);
		COORD2 PathVarient_FreeCoin(COORD2 p);

	public:
		DungeonWorld();
		~DungeonWorld();
		void OnReset();
		short OnPlay();
		void OnLose();

};

#endif