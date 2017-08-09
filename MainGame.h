#pragma once

#include "SDL.h"
#include <stdio.h>
#include <iostream>
#include "glew.h"
#include <string>
#include "Sprite.h"
#include "GLSLProgram.h"



enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	SDL_Window* _window;
	int _screenHeight;
	int _screenWidth;
	GameState _gamestate;

	Sprite _sprite;

	GLSLProgram _colorProgram;

	float _time;
};

