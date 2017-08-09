
#include <stdint.h>
#include <cstddef>
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <chrono>
#include <vector>
#include <Windows.h>

#include <glm\glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "glm/ext.hpp"

#include "Entity.h"
#include "Object.h"
#include "OBJLoader.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"


using namespace std;


//NOTE: handling console output
//TODO: to delete when using debug on screen
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

//NOTE: circle collision detection between entities 
bool collision(Ship entity1, Ship entity2)
{
	
	GLfloat radiusSum = entity1.radius + entity2.radius;
	GLfloat distance = glm::pow2((entity2.pos.x - entity1.pos.x)) + glm::pow2((entity2.pos.y - entity1.pos.y));
	if (distance > radiusSum*radiusSum)
	{
		
		return 0;
	}
	else
	{
		return 1;
	}
}



GLfloat random(GLfloat min, GLfloat max) {
	GLfloat random = ((GLfloat)rand()) / (GLfloat)RAND_MAX;
	GLfloat diff = max - min;
	GLfloat r = random * diff;
	return min + r;
}

int main(int argc, char *argv[])
{
	//NOTE: Feeding the random number generator
	srand(static_cast <unsigned> (time(0)));
	
	//NOTE: setting resolution for timers
	timeBeginPeriod(1);


	bool fullHD = false;
	
	GLfloat screenwidth = 0.0f;
	GLfloat screenheight = 0.0f;

	if (fullHD)
	{
		screenwidth = 1920.0f;
		screenheight = 1080.0f;
		
	}
	else
	{
		screenwidth = 1280.0f;
		screenheight = 720.0f;

	}

	int FPS = 60;
	int currentFPS = 0;
	SDL_Window* _window = nullptr;
	const GLubyte* renderer;
	const GLubyte* version;
	bool isRunning = true;

	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	//NOTE: Antialiasing level
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	

	//_window = SDL_CreateWindow("SDL-OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenwidth, (int)screenheight, SDL_WINDOW_FULLSCREEN_DESKTOP| SDL_WINDOW_OPENGL);
	_window = SDL_CreateWindow("SDL-OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenwidth, (int)screenheight, SDL_WINDOW_OPENGL);
	
	if (!_window) {
		fprintf(stderr, "ERROR: could not open window via SDL\n");
		SDL_Quit();
		return 1;
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	
	//NOTE: retrieving screen info
	SDL_DisplayMode SDLVideoMode; 
	SDL_GetDisplayMode(0, 0, &SDLVideoMode);

	//NOTE: Turn on\off vsync
	SDL_GL_SetSwapInterval(1);
	
	//NOTE: initialising glew
	glewExperimental = GL_TRUE;
	
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		cout << "\nGlew failed to initialize.\n";
	}
	else
	{
		cout << "\nGlew OK.\n";
	}

	//NOTE: getting the driver details
	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	
	//------------------------------------------

	//NOTE: 
	Ship reactor;
	reactor.vel.x = 0.0f;
	reactor.vel.y = 0.0f;
	reactor.pos.x = screenwidth / 2;
	reactor.pos.y = screenheight / 2;
	reactor.angleVel = 5.0f;
	reactor.radius = 30.0f;
	reactor.scaleFactor = 10.0f;

	Ship ship;
	ship.vel.x = 0.0f;
	ship.vel.y = 0.0f;
	ship.pos.x = screenwidth / 2;
	ship.pos.y = screenheight / 2;
	ship.angleVel = 5.0f;
	ship.radius = 30.0f;
	ship.scaleFactor = 10.0f; //NOTE: set this to 10.0f for the ship



	GLfloat width = 50.0f;
	GLfloat height = 50.0f;
	GLfloat x = -width / 2;
	GLfloat y = -height / 2;

	Ship rock;
	Ship missile;
	Ship frame;
	Ship pebble;
	Ship plane;

	Ship planet;
	planet.vel = glm::vec3(0.0f, 0.0f, 0.0f);
	planet.pos = glm::vec3(1100.0f, 600.0f, -1200.f);
	planet.scaleFactor = 700.0f;

	Texture texture("f:/dev/randopengl/randopengl/randopengl/Resources/textures/shipTexture.png");
	Texture texture2("f:/dev/randopengl/randopengl/randopengl/Resources/textures/testtex.png");
	Texture rockTexture("f:/dev/randopengl/randopengl/randopengl/Resources/textures/rock02.png");
	//Texture planetTexture("f:/dev/randopengl/randopengl/randopengl/Resources/textures/rock03.png");
	Texture reactorTexture("f:/dev/randopengl/randopengl/randopengl/Resources/textures/shipreactor.png");
	Texture whiteTexture("f:/dev/randopengl/randopengl/randopengl/Resources/textures/white.png");

	
	Vertex missileVertices[] = { Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(50.0f/8.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(33.2f/8.0f, 33.2f / 8.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(0.0f, 50.0f / 8.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(-33.2f / 8.0f, 33.2f / 8.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(-50.0f / 8.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(-33.2f / 8.0f, -33.2f / 8.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(0.0f, -50.0f / 8.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(33.2f / 8.0f, -33.2f / 8.0f, 0.0f), glm::vec2(0.0, 0.0)),
									Vertex(glm::vec3(50.0f / 8.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0)) };
	

	
	unsigned int missileIndices[] = { 0,1,2,3,4,5,6,7,8,9 };
	
	

	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
	glDepthFunc(GL_LESS);/*depth-testing interprets a smaller value as "closer"*/
	glEnable(GL_MULTISAMPLE);
	

	Shader baseShader("f:/dev/randopengl/randopengl/randopengl/Shaders/vertexShader.hlsl", "f:/dev/randopengl/randopengl/randopengl/Shaders/fragmentShader.hlsl");
	Shader rockShader("f:/dev/randopengl/randopengl/randopengl/Shaders/rocksVert.hlsl", "f:/dev/randopengl/randopengl/randopengl/Shaders/rocksFrag.hlsl");
	
	
	Mesh shipMesh("f://dev/randopengl/randopengl/randopengl/Resources/models/ship12.obj");
	ship.mesh = &shipMesh;

	//Mesh planetMesh("f://dev/randopengl/randopengl/randopengl/planet04.obj");
	//planet.mesh = &planetMesh;

	Mesh rockMesh("f://dev/randopengl/randopengl/randopengl/Resources/models/rock01.obj");
	rock.mesh = &rockMesh;
	
	Mesh littleRockMesh("f://dev/randopengl/randopengl/randopengl/Resources/models/littlerock08.obj");

	Mesh reactorMesh("f://dev/randopengl/randopengl/randopengl/Resources/models/shipreactor02.obj");
	reactor.mesh = &reactorMesh;
	
	Mesh pebbleMesh("f://dev/randopengl/randopengl/randopengl/Resources/models/pebble01.obj");
	

	Mesh missileMesh(missileVertices, sizeof(missileVertices) /
		sizeof(missileVertices[0]), missileIndices, sizeof(missileIndices)/sizeof(missileIndices[0]));
	missile.mesh = &missileMesh;
	
	//NOTE: background test plane
	/*
	Mesh planeMesh("f://dev/randopengl/randopengl/randopengl/plane01.obj");
	plane.mesh = &planeMesh;
	plane.pos = glm::vec3(screenwidth / 2.0f, screenheight / 2.0f, -2.0f);
	plane.scaleFactor = 50.0f;
	*/

	std::vector<Ship> rockList;
	std::vector<Ship> littleRockList;
	std::vector<Ship> rockScreen;
	std::vector<Ship> missileList;
	std::vector<Ship> pebbleList;
	
	
	
	for (GLfloat i = 100.0f; i < 100.0f*7.0f; i+=100.0f)
	{
		rock.vel = glm::vec3(random(-4.0f, 4.0f), random(-4.0f, 4.0f), 0.0f);
		rock.pos = glm::vec3(random(0.0f, screenwidth), random(0.0f, screenheight), 0.0f);
		//rock.vel = glm::vec3(0.0f, 0.0f, 0.0f);
		//rock.pos = glm::vec3(i, screenheight / 2.0f, 0.0f);
		rock.angleVel = 0.5f;
		rock.radius =47.0f;
		rock.energy = 30;
		rock.scaleFactor = 50.0f;
		rockList.push_back(rock);
	}
	
	std::vector<Ship> RockListCopy;
	RockListCopy = rockList;

	//cout<< glm::to_string(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	

	
	rock.vel = glm::vec3(0.0f, 0.0f, 0.0f);
	rock.pos = glm::vec3(0.0f, 0.0f, 0.0f);
	rock.radius = 40.0f;
	rockScreen.push_back(rock);
	
	rock.vel = glm::vec3(0.0f, 0.0f, 0.0f);
	rock.pos = glm::vec3(screenwidth, 0.0f, 0.0f);
	rock.radius = 40.0f;
	rockScreen.push_back(rock);

	rock.vel = glm::vec3(0.0f, 0.0f, 0.0f);
	rock.pos = glm::vec3(screenwidth, screenheight, 0.0f);
	rock.radius = 40.0f;
	rockScreen.push_back(rock);

	rock.vel = glm::vec3(0.0f, 0.0f, 0.0f);
	rock.pos = glm::vec3(0.0f, screenheight, 0.0f);
	rock.radius = 40.0f;
	rockScreen.push_back(rock);
	

	/*
	for (int i = 0; i < 1; i++)
	{
		rock.vel = glm::vec3(0.0f, 0.0f, 0.0f);
		rock.pos = glm::vec3(ship.pos.x, ship.pos.y, 0.0f);
		rock.radius = 80.0f;
		rockList.push_back(rock);
	}
	*/

	//------------------------------------------

	GLfloat fov = 69.7399f;
	//NOTE: good camera
	//Camera camera(glm::vec3(600.0f, 300.0f, 750.0f), 70.0f, (GLfloat)screenwidth / (GLfloat)screenheight, 0.01f, 10000.0f);
	//Camera camera(glm::vec3(640, 360, 1315.0f), 100.0f, (GLfloat)screenwidth / (GLfloat)screenheight, 0.01f, 10000.0f);
	
	Camera camera(glm::vec3(640, 360, 1110.0f), fov, (GLfloat)screenwidth / (GLfloat)screenheight, 0.01f, 10000.0f);


	//NOTE: Setting up the orthocamera
	/*
	glm::mat4 cameraMatrix(1.0f);
	glm::mat4 orthoMatrix = glm::ortho(0.0f, screenwidth, 0.0f, screenheight);
	cameraMatrix = orthoMatrix;
	*/

	GLfloat xLight = -300.0f;


	glm::mat4 model;

	float time = 0.0f;

	Uint32 startingTicks = 0;
	Uint32 fpsUpdate = 0;
	
	


	while (isRunning) 
	{
		//NOTE: ticks to calculate the FPS.
		startingTicks = SDL_GetTicks();
		
	
	

		SDL_Event evnt;
		while (SDL_PollEvent(&evnt))
		{
			switch (evnt.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_MOUSEMOTION:
				//cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
				break;
			case SDL_MOUSEWHEEL:

			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				
				}
			}
		}


		//NOTE: Gameplay input
		const Uint8* keys = SDL_GetKeyboardState(NULL);



		if (keys[SDL_SCANCODE_UP])
		{
			ship.accelerating = true;
		}

		if (keys[SDL_SCANCODE_DOWN])
		{
			ship.decelerating = true;
		}

		if (keys[SDL_SCANCODE_LEFT])
		{
			ship.angle += ship.angleVel;
			ship.turningLeft = true;
		}


		if (keys[SDL_SCANCODE_RIGHT])
		{
			ship.angle -= ship.angleVel;
			ship.turningRight = true;
		}

		if (keys[SDL_SCANCODE_SPACE])
		{
			ship.fire = true;
			//cout << "\n" << missileList.size() << endl;
		}

		//NOTE: Camera control
		if (keys[SDL_SCANCODE_W])
		{
			camera.position.y += 1.0f;
			
		}
		if (keys[SDL_SCANCODE_S])
		{
			camera.position.y -= 1.0f;
			
		}
		if (keys[SDL_SCANCODE_A])
		{
			camera.position.x -= 3.0f;
		}
		if (keys[SDL_SCANCODE_D])
		{
			camera.position.x += 3.0f;
			
		}
		if (keys[SDL_SCANCODE_Q])
		{
			camera.position.z += 10.0f;
		}
		if (keys[SDL_SCANCODE_E])
		{
			camera.position.z -= 10.0f;

		}
		if (keys[SDL_SCANCODE_Z])
		{
			xLight -= 1.0f;

		}
		if (keys[SDL_SCANCODE_X])
		{
			xLight += 1.0f;

		}

		if (keys[SDL_SCANCODE_R])
		{
			ship.inerthia.x = 0.0f;
			ship.inerthia.y = 0.0f;

		}

		if (keys[SDL_SCANCODE_T])
		{
			ship.pos.x = screenwidth*0.5f;
			ship.pos.y = screenheight*0.5f;

		}

		if (keys[SDL_SCANCODE_F1])
		{
			camera.position.x = 640.0f;
			camera.position.y = 360.0f;
			camera.position.z = 750.0f;
		}
		
		if (keys[SDL_SCANCODE_C])
		{
			camera.fov += 0.01f;
		}


		if (keys[SDL_SCANCODE_V])
		{
			camera.fov -= 0.01f;
		}
		

		if (keys[SDL_SCANCODE_F5])
		{
			rockList = RockListCopy;
			pebbleList.clear();
		}


		
		camera.Update();

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//NOTE: model matrix for the ship
		GLfloat turning = 0.0f;
		if (ship.turningLeft || ship.turningRight)
		{
			if (ship.turningLeft)
			{
				turning = -30.0f;
			}
			else
			{
				turning = 30.0f;
			}
		}
		model = glm::rotate(glm::radians(ship.angle), glm::vec3(0.0f, 0.0f, 1.0f));
		model *= glm::rotate(glm::radians(turning), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 scale = glm::scale(glm::vec3(ship.scaleFactor, ship.scaleFactor, ship.scaleFactor));
		glm::vec4 rotatingShipVector = glm::rotate(glm::mat4(), glm::radians(ship.angle), glm::vec3(0.0, 0.0f, 1.0f))* glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		
		ship.direction.x = rotatingShipVector.x;
		ship.direction.y = rotatingShipVector.y;

		ship.vel = ship.inerthia;

		if (ship.accelerating)
		{
			ship.inerthia += (ship.direction * 0.2f);
		}

		if (ship.decelerating)
		{
			ship.inerthia -= (ship.direction * 0.1f);
		}
		ship.pos += ship.inerthia;

		//NOTE: Console debug ship position
		gotoxy(0, 19);
		cout << "                ";
		gotoxy(0, 19);
		cout << "X: "<<(int)ship.pos.x<<" Y: "<<(int)ship.pos.y;
		


		
		
		//NOTE: wrap the position of the ship around the screen
		if (ship.pos.x > screenwidth + +50.0f)
			ship.pos.x = 0.0f - 50.0f;
		if (ship.pos.x < 0.0f - 50.0f)
			ship.pos.x = screenwidth + 50.0f;
		if (ship.pos.y > screenheight + 50.0f)
			ship.pos.y = 0.0f - 50.0f;
		if (ship.pos.y < 0.0f - 50.0f)
			ship.pos.y = screenheight + 50.0f;
		

		//NOTE: spawning the missile if fire is pressed
		model = glm::translate(ship.pos) * scale * model;
		if (ship.fire)
		{
			missile.pos = ship.pos + ship.direction * 35.0f;

			glm::vec4 randFireDirection = glm::rotate(glm::mat4(), glm::radians(ship.angle + random(-2.5f, 2.5)), glm::vec3(0.0, 0.0f, 1.0f))* glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			missile.vel =glm::vec3(randFireDirection) *15.0f;
			missile.radius = 0.0f;
			missileList.push_back(missile);
		}
		
		
		//NOTE: verifying collision between ship and the asteroids on the screen
		ship.hit = 0;
		for (unsigned int i = 0; i < rockList.size(); i++)
		{
			if (collision(ship, rockList[i]))
			{
				ship.hit = 1;
				rockList[i].hit = 1;
			}
		}


		//NOTE: deleting missiles outside screen
		for (unsigned int i = 0; i < missileList.size(); i++)
		{
			missileList[i].pos += missileList[i].vel;
			if (missileList[i].pos.x > screenwidth + 50.0f ||
				missileList[i].pos.x < 0.0f - 50.0f ||
				missileList[i].pos.y > screenheight + 50.0f ||
				missileList[i].pos.y < 0.0f - 50.0f)
			{
				missileList.erase(missileList.begin() + i);

			}
		}

		//NOTE: Collision missile\rocks
		
		
		for (unsigned int i = 0; i < rockList.size(); i++)
		{
			for (unsigned int i2 = 0; i2 < missileList.size(); i2++)
			{
				
				int coll = collision(missileList[i2], rockList[i]);
				if (coll && (!missileList[i2].hit && !rockList[i].hit))
				{
					missileList[i2].hit = 1;
					rockList[i].hit = 1;
					rockList[i].energy -= 1;
					if (rockList[i].energy <= 0)
					{
						if (rockList[i].mesh == &rockMesh)
						{
							glm::vec3 normalized(glm::normalize(missileList[i].vel));
							glm::vec2 debug(normalized.x, normalized.y);
							GLfloat angleRockSpawned =  -glm::degrees(glm::orientedAngle(debug, glm::vec2(0.0f, 1.0f)));

							rockList[i].isAlive = false;

							//NOTE: initialising first little rock spawned 
							//TODO: sometimes the orientation of the spawned little rocks is wrong
							rock.speed = 4.5f;
							rock.angle = angleRockSpawned - 90.0f;
							rock.angleVel = 1.0f;

							glm::vec3 littleRockDir = glm::cross(glm::normalize(missileList[i].vel), glm::vec3(0.0f, 0.0f, 1.0f));

							rock.vel = rock.speed * littleRockDir;
							rock.pos = rockList[i].pos;
							rock.radius = 46.0f;
							rock.energy = 30;
							rock.scaleFactor = 50.0f;
							rock.isAlive = true;
							rock.mesh = &littleRockMesh;
							
							rockList.push_back(rock);
							
							//NOTE: initialising second little rock
							rock.speed = random(3.0f, 4.5f);
							rock.angle = angleRockSpawned + 90.0f;
							rock.angleVel = -1.0f;
							rock.vel = rock.speed * -littleRockDir;
							rock.pos = rockList[i].pos;
							rock.radius = 46.0f;
							rock.energy = 30;
							rock.scaleFactor = 50.0f;
							rock.isAlive = true;
							rock.mesh = &littleRockMesh;
							rockList.push_back(rock);
							
							//NOTE: spawning the pebbles
							rock.speed = rock.speed = random(3.0f, 4.5f);
							rock.pos = rockList[i].pos;
							GLfloat randPebble = random(0.1f, 0.2f);
							rock.vel = -(missileList[i].vel) * randPebble;
							rock.scaleFactor = 20.0f;
							rock.angleVel = random(2.0f, 8.0f);
							rock.isAlive = true;
							
							rock.mesh = &pebbleMesh;
							pebbleList.push_back(rock);

						}
						else
						{
							rockList[i].isAlive = false;
						}
					}
				}
			}
		}
		
	
		//NOTE: deleting the destroyed asteroids from the list
		for (unsigned int i = 0; i < rockList.size(); i++)
		{
			if (!rockList[i].isAlive)
			{
				rockList.erase(rockList.begin() + i);

			}
		}
		
		gotoxy(0, 15);
		cout << " " << rockList.size() << " ";

		baseShader.Use();
		texture.Bind(0);

		
		GLint xLightloc = baseShader.getUniformLoaction("xLight");
		glUniform1f(xLightloc, xLight);
		//---------------------------------------------------------
	

		//NOTE: cameraMatrix	
		GLint pLocation = baseShader.getUniformLoaction("P");
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(camera.viewProjection[0][0]));

		GLint collLocation = baseShader.getUniformLoaction("hit");
		glUniform1i(collLocation, ship.hit);
		(collLocation, 1, GL_FALSE, ship.hit);

	
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);

	
		

		ship.mesh->draw(model, &baseShader, GL_TRIANGLES, ship.x, ship.y, 10.0f, false);
		reactorTexture.Bind(0);
		if (ship.accelerating)
		{
			reactor.mesh->draw(model, &baseShader, GL_TRIANGLES, ship.x, ship.y, 10.0f, false);
		}
		

		baseShader.Unuse();

		rockShader.Use();
		xLightloc = rockShader.getUniformLoaction("xLight");
		glUniform1f(xLightloc, xLight);

		//NOTE: cameraMatrix
		//GLint pLocation = Shaders.getUniformLoaction("P");

		pLocation = rockShader.getUniformLoaction("P");
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(camera.viewProjection[0][0]));

		glm::mat4 modelRock;

	

		rockTexture.Bind(0);
		for (unsigned int i = 0; i < rockList.size(); i++)
		{
			rockList[i].pos += rockList[i].vel;
			  
			if (rockList[i].pos.x > screenwidth + 50.0f)
			{
				rockList[i].pos.x -= (screenwidth + 100.0f) ;
			}
			
			if (rockList[i].pos.x < 0.0f - 30.0f)
			{
				rockList[i].pos.x += (screenwidth + 100.0f);
			}

			if (rockList[i].pos.y > screenheight + 50.0f)
			{
				rockList[i].pos.y -= (screenheight + 100.0f);
			}

			if (rockList[i].pos.y < 0.0f - 50.0f)
			{
				rockList[i].pos.y += (screenheight + 100.0f);
			}

		
			collLocation = rockShader.getUniformLoaction("hit");
			glUniform1i(collLocation, rockList[i].hit);
			(collLocation, 1, GL_FALSE, rockList[i].hit);

			glm::mat4 rockScale = glm::scale(glm::vec3(rockList[i].scaleFactor, rockList[i].scaleFactor, rockList[i].scaleFactor));
			modelRock = glm::rotate(glm::radians(rockList[i].angle+=rockList[i].angleVel), glm::vec3(-0.5f, 0.0f, 1.0f));

			if (rockList[i].hit)
			{
				glm::vec3 postemp = rockList[i].pos + glm::vec3(random(-6.5f, 6.5f), random(-6.5f, 6.5f), 0.0f);
				modelRock = glm::translate(postemp) * rockScale * modelRock;
			}
			else
			{

				modelRock = glm::translate(rockList[i].pos) * rockScale * modelRock;
			}
			rockList[i].mesh->draw(modelRock, &rockShader, GL_TRIANGLES, 0, 0, 0,false);
			rockList[i].hit = 0;

		}


		//NOTE: Debug to centering the camera
		for (unsigned int i = 0; i < rockScreen.size(); i++)
		{
			collLocation = rockShader.getUniformLoaction("hit");
			glUniform1i(collLocation, rockScreen[i].hit);
			(collLocation, 1, GL_FALSE, rockScreen[i].hit);


			modelRock = glm::mat4();
			GLfloat rockScaleFactor = 50.0f;
			glm::mat4 rockScale = glm::scale(glm::vec3(rockScaleFactor, rockScaleFactor, rockScaleFactor));
			modelRock = glm::rotate(glm::radians(-time), glm::vec3(0.0f, 0.0f, 1.0f));
			modelRock = glm::translate(rockScreen[i].pos) * rockScale * modelRock;

			rockScreen[i].mesh->draw(modelRock, &rockShader, GL_TRIANGLES, 0, 0, 0, false);
		}
		
	//planetTexture.Bind(0);
		/*
		//NOTE: transforming & rendering the suzanne planet
		glm::mat4 planetModel = glm::mat4();
		glm::mat4 planetScale = glm::scale(glm::vec3(planet.scaleFactor, planet.scaleFactor, planet.scaleFactor));
		planetModel = glm::rotate(glm::radians(-time/30.0f), glm::vec3(0.0f, 1.0f, 0.3f));
		planetModel = glm::translate(planet.pos) * planetScale* planetModel;

		planet.mesh->draw(planetModel, &rockShader, GL_TRIANGLES, 0, 0, 0, false);
		*/
		//NOTE: trasforming & rendering all the pebbles
		for (unsigned int i = 0; i < pebbleList.size(); i++)
		{
			glm::mat4 pebbleModel = glm::mat4();
			glm::mat4 pebbleScale = glm::scale(glm::vec3(pebbleList[i].scaleFactor, pebbleList[i].scaleFactor, pebbleList[i].scaleFactor));
			pebbleModel = glm::rotate(glm::radians(pebbleList[i].angle += pebbleList[i].angleVel), glm::vec3(0.0f, 0.0f, 1.0f));
			pebbleModel = glm::translate(pebbleList[i].pos+= pebbleList[i].vel) * pebbleScale * pebbleModel;

			pebbleList[i].mesh->draw(pebbleModel, &rockShader, GL_TRIANGLES, 0, 0, 0, false);
		}
		rockShader.Unuse();
		
		

		baseShader.Use();

		//NOTE: render the background test plane
		//whiteTexture.Bind(0);
		//glm::mat4 planeModel = glm::mat4();
		//glm::mat4 planeScale = glm::scale(glm::vec3(plane.scaleFactor, plane.scaleFactor, plane.scaleFactor));
		//planeModel = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//planeModel = glm::translate(plane.pos) * planeScale * planeModel;
		//plane.mesh->draw(planeModel, &baseShader, GL_TRIANGLES, 0, 0, 0, false);
		

		//NOTE: trasforming & rendering all the pebbles
		glm::mat4 missileMatrix;
		for (unsigned int i = 0; i < missileList.size(); i++)
		{
			if (missileList.size() > 0)
			{
				missileMatrix = glm::mat4();
				missileMatrix = glm::rotate(glm::radians(ship.angle), glm::vec3(0.0f, 0.0f, 1.0f));
				missileMatrix = glm::translate(missileList[i].pos) * missileMatrix;
				
				missileList[i].mesh->draw(missileMatrix, &baseShader, GL_TRIANGLE_FAN, 0, 0, 0, false);
			}
			
		}
	
		baseShader.Unuse();		
		time++;

		ship.accelerating = false;
		ship.decelerating = false;
		ship.turningLeft = false;
		ship.turningRight = false;
		ship.fire = false;

		//NOTE: deleting all the missile that has collide with the rocks
		
		
		for (unsigned int i = 0; i < missileList.size(); i++)
		{
			if(missileList[i].hit)
			{
				missileList.erase(missileList.begin() + i);
				i = 0;
			}
		}
		
		gotoxy(19, 8);
		cout << "Camera X: " << camera.position.x << " CameraFOV: " << camera.fov<<"   ";
		
		
		
		SDL_GL_SwapWindow(_window);
		Uint32 currentTicks = SDL_GetTicks();
		
		currentFPS++;
		if ((1000.0f / FPS) > currentTicks - startingTicks)
		{
			//SDL_Delay(1000 / FPS - (currentTicks - startingTicks));
			//Sleep((1000 / FPS - (currentTicks - startingTicks)));
			currentTicks = SDL_GetTicks();
			

		}
		
		if (currentTicks - fpsUpdate >= 1000)
		{
				
			char title[80];
			//sprintf_s(title, "SDL-OpenGL Test - FPS: %d - millisecond per frame: %.2f", currentFPS, (float)frameTimer.getElapsedTimeInMilliSec());
			sprintf_s(title, "SDL-OpenGL Test - FPS: %d" , currentFPS);

			SDL_SetWindowTitle(_window, title);
			fpsUpdate = SDL_GetTicks();
			currentFPS = 0;

		}
		
	}
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(_window);

	SDL_Quit();
	return 0;
}
