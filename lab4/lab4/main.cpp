/// <summary>
/// Empty Project for SFML[2.5.0]
/// 
/// 
/// Author: Michal Krzyzanowski
/// @date May 2018
/// </summary>

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include "game.h"



/// <summary>
/// main entry point
/// </summary>
/// <returns>zero</returns>
int main()
{
	srand(static_cast<unsigned>(time(NULL))); // seed of rand() function, casted into an unsigned int

	Game game;
	game.run();
	return 0;
}