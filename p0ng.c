//
// A simple p0ng game single player implementation for the r0ket badge of the chaos communication camp 2011
// This program is free software, licensed under (cc) by-nc-sa by leonard, http://telelo.de
// version 0.2, 19:85 - 12.08.2011
//
// Known bugs:
// • The ball makes sometimes strange things if he's droven through an edge (in the simulat0r)
//
// Missing features:
// • Highscore in the mesh
// • Multiplayer game via RF
// • The bat of the virtual player

#include "basic/basic.h"

#include "usetable.h"

#define debug 0

int points = 1 - 1;
int b_x = 5;
int b_y = 68;
// the balls way, 0 = left top to right bottom, 1 = left bottom to right top, 2 = right top to left bottom, 3 = right bottom to left top
int b_way = 1;

static void bat();
static void ball();
static void intro();

// main function
void ram(void) {
	int high = 15;
	static int maxhigh = 61;
	static int minhigh = 0;
	static int length = 14;
	static int distance = 3;
	static int minpoints = -2;
	static int speed = 18;
	static int batsteps = 3;
	
	// clear the screen
	lcdClear();
	
	// draw the intro
	intro();
	
	// draw the bat
	bat(0, high, length, distance);
		
	while(1) {

		// check the points
		if(points < minpoints) {
			lcdClear();
			lcdNl();
			lcdNl();
			lcdNl();
			lcdPrint("  GAME OVER");
			lcdRefresh();
			delayms(1000);
			return;
		}
		
		// start the ball
		ball(distance, high, length);
		
		// points shower
		DoInt(0, 0, points);
			
		// the hardware triggers2
		switch(getInputRaw()){
			
			case BTN_RIGHT:
			
				// start the ball operation
				//ball(distance, high, length);
				
				break;
			
			case BTN_UP:
			
				// move the plaything up
				if(high != minhigh) {
					high = high - batsteps;
					
					bat(0, high, length, distance);
				}
				
				break;
				
			case BTN_DOWN:
			
				// move the plaything down
				if(high < maxhigh - batsteps) {
					high = high + batsteps;
					
					bat(0, high, length, distance);
				}
				
				break;
				
			case BTN_LEFT:
				return;
		};
		
		// wait for the right game speed
		delayms(speed);
		
		// refresh the screen
		lcdRefresh();
	}
}

// function to show the bat
void bat(int side, int high, int length, int distance) {
	int width = 96;
	int height = 68;
	
	// set the distance according to the side
	if(side == 1) {
		distance = width - distance - 1;
	}
	
	// clear the line
	for(int i = 0; i < height; i++) {
		lcdSetPixel(distance, i, 0);
	}
	
	// set the line
	for(int i = 0; i < length; i++) {
		lcdSetPixel(distance, i + high, 1);
	}
}

// function to show the ball and handle its collisions
void ball(int distance, int high, int length) {
	
	// destroy the old position
	lcdSetPixel(b_x, b_y, 0);
	lcdSetPixel(b_x + 1, b_y, 0);
	lcdSetPixel(b_x + 1, b_y + 1, 0);
	lcdSetPixel(b_x, b_y + 1, 0);
	
	// go to the next
	if(b_way == 0) {
		b_x++;
		b_y++;
	} else if(b_way == 1) {
		b_x++;
		b_y--;
	} else if(b_way == 2) {
		b_x--;
		b_y++;
	} else if(b_way == 3) {
		b_x--;
		b_y--;
	}
	
	// look for collisions
	// collision with the top
	if(b_y == 0) {
		// look for the old way
		if(b_way == 1) {
			b_way = 0;
		} else {
			b_way = 2;
		}
	// with the bottom
	} else if(b_y == 68) {
		// look for the old way
		if(b_way == 2) {
			b_way = 3;
		} else {
			b_way = 1;
		}
	// with the left wall
	} else if(b_x == 0) {
		if(b_way == 3) {
			b_way = 1;
		} else {
			b_way = 0;
		}
		// count the points down
		points--;
	// with the right wall
	} else if(b_x == 96) {
		if(b_way == 0) {
			b_way = 2;
		} else {
			b_way = 3;
		}
		// with the bat
	} else if(b_x == distance + 1 && b_y >= high && b_y <= high + length) {
		if(b_way == 3) {
			b_way = 1;
		} else {
			b_way = 0;
		}
		// count the points up
		points++;
	}
	
	// show the ball position, way and bat high if debugging is enabled
	#if debug == 1
	DoInt(20,20,b_way);
	DoInt(20,30,b_x);
	DoInt(20,40,b_y);
	DoInt(20,50,high);
	#endif
	
	// set the new position	
	lcdSetPixel(b_x, b_y, 1);
	lcdSetPixel(b_x + 1, b_y, 1);
	lcdSetPixel(b_x + 1, b_y + 1, 1);
	lcdSetPixel(b_x, b_y + 1, 1);
}

// function to show a little intro for 10 seconds
void intro(void) {
	lcdNl();
	lcdNl();
	lcdNl();
	lcdPrint("     p0ng");
	lcdRefresh();
	lcdClear();
	delayms(210);
}
