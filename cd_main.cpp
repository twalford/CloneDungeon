// STL includes
#include <string>
#include <sstream>
#include <vector>

// Application includes
#include "cd_game.h"
#include "cd_world.h"
//#include "log_stream.h"
#include "utility_func.h"

// Application entry point
int main(int argc, char* argv[])
{
	DungeonWorld	game;

	SHORT	sGameScore = 0;

	game.OnReset();
	sGameScore = game.OnPlay();
	game.OnLose();
		
	std::cout << "Finished with score of " << sGameScore << "         ";
	system("pause");

	return 0;
}