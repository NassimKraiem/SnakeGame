#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "gameengine.h"
#include "input.h"

#define WIDTH 30
#define HEIGHT 30

//snake length
#define LENGTH 3

//directions
#define right newVector2(1, 0)
#define left newVector2(-1, 0)
#define up newVector2(0, -1)
#define down newVector2(0, 1)

//keys
#define w 119
#define s 115
#define d 100
#define a 97

//empty pixel
#define emp '.'

//snake body-part element
#define snakePart 'O'

//fruit sprite
#define fruitSprite 'X'

char screen[HEIGHT][WIDTH];
enum GameState state;

int length = LENGTH;
Vector2 fruitPos;

/***************************input**************************/

int anyKeyDown()
{
	set_conio_terminal_mode();
	if (kbhit())
	{
		reset_terminal_mode();
		return 1;
	}
	else
	{
		reset_terminal_mode();
		return 0;
	}
}

int getKeyDown()
{
	return getch();
}

/***************************endInput***************************/

void delay(int number_of_seconds)
{
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	printf("\n");
	while (clock() < (start_time + milli_seconds))
		;
}

int inBounds(int x, int maxX)
{
	return (x >= 0) ? x % maxX : maxX + (x % maxX);
}

void NdrawAt(char e, int x, int y)
{
	screen[y][x] = e;
}

void drawAt(char e, int x, int y)
{
	void println(char msg[]);
	void clearPrint();

	x = inBounds(x, WIDTH);
	y = inBounds(y, HEIGHT);

	if (screen[y][x] != emp && state != lost)
	{
		if (screen[y][x] == 'X')
		{
			length++;
			fruitPos = newVector2(rand() % WIDTH, rand() % HEIGHT);
		}
		else
		{
			clearPrint();
			println("GAME OVER!");
			state = lost;
		}
	}
	screen[y][x] = e;
}
/*void drawAtVect(char e, Vector2 position)
{
	void print(char msg[]);
	int x, y;
	x = inBounds(position.x, WIDTH);
	y = inBounds(position.y, HEIGHT);

	if (screen[y][x] != emp && screen[y][x] != e)
	{
		print("GAME OVER!");
		state = lost;
	}
	screen[y][x] = e;
}*/
void resetPoint(int x, int y)
{
	screen[y][x] = emp;
}
void resetPointVect(Vector2 position)
{
	screen[position.y][position.x] = emp;
}
char getPoint(int x, int y)
{
	return screen[y][x];
}
char getPointFromVect(Vector2 position)
{
	return screen[position.y][position.x];
}

void move(Object *obj, Vector2 direction)
{
	int newx, newy;

	newx = (obj->transform.position.x) + direction.x;
	obj->transform.position.x = inBounds(newx, WIDTH);

	newy = (obj->transform.position.y) + direction.y;
	obj->transform.position.y = inBounds(newy, HEIGHT);
}

void initScreen()
{
	int y, x;
	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			resetPoint(x, y);
		}
	}
}

void refreshScreen()
{
	int y, x;
	//system("clear");
	printf("\033[2J\033[1;1H");
	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			printf("%c ", screen[y][x]);
		}
		printf("\n");
	}
}

int len(char str[])
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}
void print(char msg[])
{
	int msgWidth = len(msg) + 2;

	int y, x, startX, endX;

	startX = (WIDTH - msgWidth) / 2;
	endX = (WIDTH + msgWidth) / 2;

	y = 6;
	for (x = startX; x < endX; x++)
	{
		NdrawAt('-', x, y);
	}

	y = 7;
	NdrawAt('|', startX, y);
	for (x = startX + 1; x < endX - 1; x++)
	{
		NdrawAt(msg[x - startX - 1], x, y);
	}
	NdrawAt('|', endX - 1, y);

	y = 8;
	for (x = startX; x < endX; x++)
	{
		NdrawAt('-', x, y);
	}

	/*printf("%*s\n", x+2, " ");
	printf(" %s ", msg);
	printf("%*s\n", x+2, " ");*/
}

void println(char msg[])
{
	int msgWidth = len(msg);

	int y, x, startX, endX;

	startX = (WIDTH - msgWidth) / 2;
	endX = (WIDTH + msgWidth) / 2;

	y = 6;
	for (x = 0; x < WIDTH; x++)
	{
		NdrawAt('-', x, y);
	}

	y = 7;
	for(x = 0; x < startX; x++)
	{
		NdrawAt(' ', x, y);
	}
	for (x = startX; x < endX; x++)
	{
		NdrawAt(msg[x - startX], x, y);
	}
	for(x = endX; x < WIDTH; x++)
	{
		NdrawAt(' ', x, y);
	}

	y = 8;
	for (x = 0; x < WIDTH; x++)
	{
		NdrawAt('-', x, y);
	}
}

void clearPrint()
{
	int y, x, startX, endX;

	startX = 0;
	endX = WIDTH;

	for (y = 6; y < 9; y++)
		for (x = startX; x < endX; x++)
			NdrawAt(emp, x, y);
}

Vector2 sn[WIDTH * HEIGHT];

int main()
{
	int i = 0, ji;
	char input[20];
	int restart = 1;
	Vector2 direction;

	//snake
	Object snake;
	snake.transform.position = newVector2(15, 15);

	state = play;

	initScreen();

	do
	{
		refreshScreen();
		printf("Start? y/n\n");
		scanf("%c", &input[0]);
	} while (input[0] != 'y' && input[0] != 'Y');

	do
	{
		i = 0;
		direction = right;
		length = LENGTH;

		for (ji = 0; ji < length; ji++)
		{
			sn[ji] = newVector2(snake.transform.position.x - ji, snake.transform.position.y);
		}

		fruitPos = newVector2(rand() % WIDTH, rand() % HEIGHT);

		do
		{
			initScreen();

			//draw static objects

			NdrawAt(fruitSprite, fruitPos.x, fruitPos.y);

			//try scrolling screen here

			fflush(stdin);
			if (anyKeyDown())
			{
				int keyCode = getKeyDown();
				switch (keyCode)
				{
				case w:
					//move up
					if (direction.y != down.y)
						direction = up;
					break;
				case s:
					//move down
					if (direction.y != up.y)
						direction = down;
					break;
				case d:
					//move right
					if (direction.x != left.x)
						direction = right;
					break;
				case a:
					//move left
					if (direction.x != right.x)
						direction = left;
					break;
				}
			}

			//move snake here
			move(&snake, direction);

			for (ji = 1; ji < length; ji++)
			{
				sn[length - ji] = sn[length - ji - 1];
				drawAt(snakePart, sn[length - ji].x, sn[length - ji].y);
			}
			sn[0] = snake.transform.position;
			drawAt(snakePart, sn[0].x, sn[0].y);

			if (i < 15)
				println("GAME START!");
			else if(i==15)
				clearPrint();

			//draw foreground here

			//draw screen here
			refreshScreen();
			printf("score: %d", length);
			i++;
			delay(150);
			//delay(150-i/100); //use this instead if you want that speed keeps increasing
		} while (state != lost && input[0] != 'q');
		do
		{
			fflush(stdin);
			printf("Restart? y/n\n");
			scanf("%c", &input[0]);
		} while (input[0] != 'y' && input[0] != 'Y' && input[0] != 'n' && input[0] != 'N');
		if (input[0] == 'y' || input[0] == 'Y')
			state = play;
		else
			restart = 0;
	} while (restart);

	return 0;
}