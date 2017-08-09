#include "Error.h"
#include <iostream>
#include "SDL.h"


extern void fatalError(std::string string)
{
	std::cout << string << std::endl;
	std::cout << "Enter to continue...\n";
	std::getchar();
	//SDL_Quit();
}

extern void message(std::string string)
{
	std::cout << string << std::endl;
}