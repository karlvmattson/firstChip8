 #include<conio.h>    
#include<stdio.h>    
#include<math.h>    
#include<string.h>    
#include<chip8.h>
#include<iostream>
#include<SDL.h>
#include<SDL_opengl.h>
#include <string>



int windowWidth;
int windowHeight;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_GLContext context;
chip8 myChip8;

using namespace std;


bool setupGraphics() {
	// Initialize SDL with video
	SDL_Init(SDL_INIT_VIDEO);

	windowWidth = 1280;
	windowHeight = 640;

	// Create an SDL window
	window = SDL_CreateWindow("Karl's Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	// if failed to create a window
	if (!window)
	{
		// we'll print an error message and exit
		std::cerr << "Error failed to create window!\n";
		return false;
	}

	// Set render color to black ( background will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear winow
	SDL_RenderClear(renderer);

	// Render the rect to the screen
	SDL_RenderPresent(renderer);


	//// Create an OpenGL context (so we can use OpenGL functions)
	//context = SDL_GL_CreateContext(window);

	//// if we failed to create a context
	//if (!context)
	//{
	//	// we'll print out an error message and exit
	//	std::cerr << "Error failed to create a context\n!";
	//	return false;
	//}
	//glClearColor(0, 0, 0, 0);


	return true;
}


bool setupInput() {
	// do any input setup here


	//Keypad                   Keyboard
	//	+ -+-+-+-++-+-+-+-+
	//	| 1 | 2 | 3 | C|                |1 | 2 | 3 | 4 |
	//	+-+-+-+-++-+-+-+-+
	//	| 4 | 5 | 6 | D|                |Q | W | E | R |
	//	+-+-+-+-+= > +-+-+-+-+
	//	| 7 | 8 | 9 | E|                |A | S | D | F |
	//	+-+-+-+-++-+-+-+-+
	//	| A | 0 | B | F|                |Z | X | C | V |
	//	+-+-+-+-++-+-+-+-+


	return true;
}

void drawGraphics(chip8 &thisChip) {
	// Set render color to black ( background will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Clear winow
	SDL_RenderClear(renderer);

	// Set render color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Loop through gfx array and draw each white rectangle
	// X = 64, Y = 32, draw to 1280 x 640 window
	for (int i = 0; i < 32 * 64; i++) {
		if (thisChip.gfx[i] != 0) {
			// draw white rectangle
			// rectangle should be 20 x 20 pixels
			SDL_Rect r;
			r.x = 20 * (i % 64);
			r.y = 620 - (20 * (i / 64));
			r.w = 20;
			r.h = 20;
			// Render rect
			SDL_RenderFillRect(renderer, &r);
		}
	}

	// Render the rect to the screen
	SDL_RenderPresent(renderer);

	// Set draw flag to false
	thisChip.drawFlag = false;
}

int main(int argc, char* argv[])
{

	// Set up graphics
	if (!setupGraphics()) return 1;

	// Set up input
	if (!setupInput()) return 1;

	// Initilize the system
	myChip8.initialize();

	// Load a program!!!
	myChip8.loadApplication("PONG");

	SDL_Event event;	 // used to store any events from the OS
	bool running = true; // used to determine if we're running the game

	while (running)
	{
		// poll for events from SDL
		while (SDL_PollEvent(&event))
		{
			// determine if the user still wants to have the window open
			// (this basically checks if the user has pressed 'X')
			running = event.type != SDL_QUIT;
		}

		// Emulate one cycle
		myChip8.emulateCycle();

		// If the draw flag is set, update the screen
		if (myChip8.drawFlag)
			drawGraphics(myChip8);

		// Store key press state (Press and Release)
		myChip8.setKeys();

	}


	// Destroy the context
	SDL_GL_DeleteContext(context);

	// Destroy the window
	SDL_DestroyWindow(window);

	// And quit SDL
	SDL_Quit();

	return 0;
}