#ifndef _GLOB_
#define _GLOB_
#include "ANDROID"
// Notes:
//		- Program currently uses Vsync
//		- Each 10 px = 1 meter
//		- If the program get memroy leak check out for static variables.

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
//#include <thread>

#include "SDLs\Image.h"
#include "SDLs\SDLHelper.h"

#define		Pi				3.14159265

#define		WIN_HEIGHT		720		// 6  Blocks
#define		WIN_WIDTH		1200	// 10 Blocks 

#define		MAX_SAMPLECARS	25

enum BlockType {
	BLK_NOTSET,

	BLK_BACKGROUND,
	BLK_TEXTURE,

	BLK_VROAD, /* Vertical Road */
	BLK_HROAD, /* Horizontal Road */

	BLK_2SQUARE0, /* L shape square */
	BLK_2SQUARE1, /* 90deg clockwise rotate of 2square */
	BLK_2SQUARE2, /* 180deg clockwise rotate of 2square */
	BLK_2SQUARE3, /* 270deg clockwise rotate of 2square */

	BLK_3SQUARE0, /*   |
					 ----- shape square */
	BLK_3SQUARE1, /* 90deg clockwise rotate of 3square */
	BLK_3SQUARE2, /* 180deg clockwise rotate of 3square */
	BLK_3SQUARE3, /* 270deg clockwise rotate of 3square */

	BLK_4SQUARE /* + shape square */
};
enum Direction { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT }; // Clockwise
enum SquareTLStatus { SQR_VER_GREEN, SQR_HOR_GREEN };

static void InitializeProgram()
{

}


#endif