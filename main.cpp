 #include<conio.h>    
#include<stdio.h>    
#include<math.h>    
#include<string.h>    
#include<chip8.h>
#include<iostream>
#include<SDL.h>
#include<SDL_opengl.h>



int windowWidth;
int windowHeight;
SDL_Window* window;
SDL_GLContext context;
chip8 myChip8;




bool setupGraphics() {
	// Initialize SDL with video
	SDL_Init(SDL_INIT_VIDEO);

	windowWidth = 640;
	windowHeight = 480;

	// Create an SDL window
	window = SDL_CreateWindow("Karl's Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	// if failed to create a window
	if (!window)
	{
		// we'll print an error message and exit
		std::cerr << "Error failed to create window!\n";
		return false;
	}

	// Create an OpenGL context (so we can use OpenGL functions)
	context = SDL_GL_CreateContext(window);

	// if we failed to create a context
	if (!context)
	{
		// we'll print out an error message and exit
		std::cerr << "Error failed to create a context\n!";
		return false;
	}

	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);

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

void drawGraphics() {
	// redraw screen

			// Swap OpenGL buffers
		//SDL_GL_SwapWindow(window);
}

int main(int argc, char* argv[])
{

	// Set up graphics
	if (!setupGraphics()) return 1;

	// Set up input
	if (!setupInput()) return 1;

	// Initilize the system
	myChip8.initialize();




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
			drawGraphics();

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