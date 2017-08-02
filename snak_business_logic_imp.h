#ifndef SNAK_H_INCLUDE
#define SNAK_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>

#include "stage.h"

#define MAX_LENGTH 100

enum e_dir {up, down, left, right};

/*const int block_length = 5;
int block_x[block_length] = {12, 13, 15, 17, 20};
int block_y[block_length] = {22, 22, 22, 25, 20};*/





struct snake_data
{
	int x[MAX_LENGTH];
	int y[MAX_LENGTH];
	int length;
	int size;
	int step_span;
	int old_x[MAX_LENGTH];
	int old_y[MAX_LENGTH];
	enum e_dir dir;
};
struct snake_data s;

struct game_data
{
	int grid_init_x;
	int grid_init_y;
	int food_x;
	int food_y;
	int stage;
	int num_eat;
};
struct game_data game;

void draw_food(HWND hWnd, HDC hdc)
{
	int food_x=game.food_x;
	int food_y=game.food_y;
	
	HPEN hpen_green = CreatePen(PS_SOLID, 2, RGB( 0x00, 0xFF,0x00));
	SelectObject(hdc, hpen_green);
	MoveToEx(hdc, food_x+0,food_y+0, NULL);
	LineTo(hdc, food_x+0,food_y+20);
	LineTo(hdc, food_x+20,food_y+20);
	LineTo(hdc, food_x+20,food_y+0);
	LineTo(hdc, food_x+0,food_y+0);
}

void draw_block(HWND hWnd, HDC hdc)
{
	HPEN hpen_blue = CreatePen(PS_SOLID, 2, RGB(0x00, 0x00, 0xff));
	SelectObject(hdc, hpen_blue);
	for(int i=0;i<block_length_stage[game.stage];i++)
	{
		MoveToEx(hdc, block_x_stage[game.stage][i]+0,block_y_stage[game.stage][i]+0, NULL);
		LineTo(hdc, block_x_stage[game.stage][i]+0,block_y_stage[game.stage][i]+20);
		LineTo(hdc, block_x_stage[game.stage][i]+20,block_y_stage[game.stage][i]+20);
		LineTo(hdc, block_x_stage[game.stage][i]+20,block_y_stage[game.stage][i]+0);
		LineTo(hdc, block_x_stage[game.stage][i]+0,block_y_stage[game.stage][i]+0);
	}
}

void next_stage()
{
	Beep(500, 50);
	Beep(1000, 50);
	Beep(2000, 50);
	Beep(4000, 50);
	Beep(8000, 50);
	game.num_eat = 0;
	game.stage++;
	for(int i=0;i<block_length_stage[game.stage];i++)
	{
		block_x_stage[game.stage][i]=game.grid_init_x+block_x_stage[game.stage][i]*s.step_span;
		block_y_stage[game.stage][i]=game.grid_init_x+block_y_stage[game.stage][i]*s.step_span;
	}
	s.length = 3;
}

void init_game()
{
   s.size = 20;
   s.step_span = 22;

	game.grid_init_x = 10;
	game.grid_init_y = 10;
	game.food_x = game.grid_init_x + 2*s.step_span;
	game.food_y = game.grid_init_y + 3*s.step_span;
	game.stage = 1;
	game.num_eat = 0;

	/*int x=game.grid_init_x;
	int y=game.grid_init_y;*/
	for(int i=0;i<block_length_stage[game.stage];i++)
	{
		block_x_stage[game.stage][i]=game.grid_init_x+block_x_stage[game.stage][i]*s.step_span;
		block_y_stage[game.stage][i]=game.grid_init_x+block_y_stage[game.stage][i]*s.step_span;
	}
}

void init_snake()
{
   s.length = 3;


   s.x[0] = game.grid_init_x+5*s.step_span;
   s.y[0] = game.grid_init_y;
   s.x[1] = game.grid_init_x+4*s.step_span;
   s.y[1] = game.grid_init_y;
   s.x[2] = game.grid_init_x+3*s.step_span;
   s.y[2] = game.grid_init_y;

   s.old_x[0] = game.grid_init_x+5*s.step_span;
   s.old_y[0] = game.grid_init_y;
   s.old_x[1] = game.grid_init_x+4*s.step_span;
   s.old_y[1] = game.grid_init_y;
   s.old_x[2] = game.grid_init_x+3*s.step_span;
   s.old_y[2] = game.grid_init_y;

   s.dir = right;
}

void start_snake_timer(HWND hWnd)
{
	int delay = 500;
	SetTimer(hWnd, 0x42, delay, NULL);
}

bool is_eat()
{
	return  (game.food_x == s.x[0] && game.food_y == s.y[0] );
}

bool hit()
{
	for(int i=1;i<s.length;i++)
	{
		if( s.x[0]== s.x[i] && s.y[0]== s.y[i] )
			return true;
	}
	for(int i=0;i<block_length_stage[game.stage];i++)
	{
		if( s.x[0]== block_x_stage[game.stage][i] && s.y[0]== block_y_stage[game.stage][i] )
			return true;
	}


	return false;
}

void new_food()
{
	int x;
	int y;

	int range_max = 30;
	int range_min = 0;
	x= (int)((double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min);
	y= (int)((double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min);
    //x=rand()%5;
	//y=rand()%5;
	game.food_x = game.grid_init_x + x*s.step_span;
	game.food_y = game.grid_init_y + y*s.step_span;
}

bool pass()
{
	return game.num_eat >= 3;
}


void run_snake(HWND hWnd)
{
	int step = s.step_span;
	int i=0;
	switch (s.dir)
	{
	case left:
		for(i=s.length-1;i>0;--i)
		{
			s.x[i] = s.x[i-1];
			s.y[i] = s.y[i-1];
		}
		s.x[0] = s.x[0] - step;
		s.y[0] = s.y[0];
		break;
	case right:
		for(i=s.length-1;i>0;--i)
		{
			s.x[i] = s.x[i-1];
			s.y[i] = s.y[i-1];
		}
		s.x[0] = s.x[0] + step;
		s.y[0] = s.y[0];
		break;
	case up:
		for(i=s.length-1;i>0;--i)
		{
			s.x[i] = s.x[i-1];
			s.y[i] = s.y[i-1];
		}
		s.x[0] = s.x[0];
		s.y[0] = s.y[0]-step;
		break;
	case down:
		for(i=s.length-1;i>0;--i)
		{
			s.x[i] = s.x[i-1];
			s.y[i] = s.y[i-1];
		}
		s.x[0] = s.x[0];
		s.y[0] = s.y[0]+step;
		break;
	}
	if(hit())
	{
		Beep(5000, 50);
		Beep(2500, 50);
		Beep(1000, 50);
		Beep(5000, 50);
		Beep(2500, 50);
		Beep(1000, 50);
		Beep(5000, 50);
		Beep(2500, 50);
		Beep(1000, 50);
		exit(0);
	}
	if(is_eat())
	{
		game.num_eat++;
		if(pass())
		{
			next_stage();
		}
		new_food();
		Beep(1000, 50);
		Beep(2000, 50);
		Beep(4000, 50);
		int tailend = s.length;
		s.x[tailend]=s.x[tailend-1];
		s.y[tailend]=s.y[tailend-1];
		s.length++;
	}

}


void draw_snake(HWND hWnd, HDC hdc)
{
	HPEN hpen_black = CreatePen(PS_SOLID, 2, RGB(0x00, 0x00, 0x00));
	HPEN hpen_white = CreatePen(PS_SOLID, 2, RGB(0xff, 0xff, 0xff));
	HPEN hpen_red = CreatePen(PS_SOLID, 2, RGB(0xff, 0x00, 0x00));

	int i=0;

	for(i=0;i<s.length;i++)
	{  
	// erase
/*		SelectObject(hdc, hpen_white);
		MoveToEx(hdc, s.old_x[i]+0,s.old_y[i]+0, NULL);
		LineTo(hdc, s.old_x[i]+0,s.old_y[i]+20);
		LineTo(hdc, s.old_x[i]+20,s.old_y[i]+20);
		LineTo(hdc, s.old_x[i]+20,s.old_y[i]+0);
		LineTo(hdc, s.old_x[i]+0,s.old_y[i]+0);*/

	// draw
		if(i==0)
		{
			SelectObject(hdc, hpen_red);
		}
		else
		{
			SelectObject(hdc, hpen_black);
		}
		//绘制正方形
		MoveToEx(hdc, s.x[i]+0,s.y[i]+0, NULL);
		LineTo(hdc, s.x[i]+0,s.y[i]+20);
		LineTo(hdc, s.x[i]+20,s.y[i]+20);
		LineTo(hdc, s.x[i]+20,s.y[i]+0);
		LineTo(hdc, s.x[i]+0,s.y[i]+0);

		s.old_x[i] = s.x[i];
		s.old_y[i] = s.y[i];
	}
}


#endif
