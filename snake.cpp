#include<iostream>
#include<conio.h>
using namespace std;
bool gameOver;
int tailX[100];
int tailY[100];
int ntail=1;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
enum eDirection { STOP = 0,LEFT,RIGHT,UP,DOWN };
eDirection dir;

void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand()%width;
	fruitY = rand()%height;
	score = 0;
}
void Draw()
{
	system("cls");
	//Upper wall
	for(int i=0;i<width;i++)
		cout << "#";
	cout << endl;
	for(int i=0;i<height;i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0  or j == (width - 1))
				cout << "#";
			else 
			{
				if (j == x and i == y)
					cout << "O";
				else if (j == fruitX and i == fruitY)
					cout << "F";
				else
				{
					bool print = false;
					for (int k = 0; k < ntail; k++)
					{
						if (tailX[k] == j and tailY[k] == i)
						{
							cout << 'o';
							print = true;
						}
					}
					if(!print)
						cout << " ";
				}
			}
				
		}
		cout << endl;
	}
	//lower wall
	for (int i = 0; i < width; i++)
		cout << "#";
	cout << endl;
	cout << "Score: " << score << endl;
}
void Input()
{
	if (_kbhit) 
	{
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}

void Logic()
{
	switch (dir)
	{
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	}
	if (x==0 or x==(width-1) or y<0 or y==height)
		gameOver = true;
	for (int n = 0; n < ntail; n++)
	{
		if (tailX[n] == x and tailY[n] == y)
			gameOver = true;
	}
	if (x == fruitX and y == fruitY)
	{
		score += 10;
		bool already = true;
		while(already)
		{
			fruitX = rand() % width;
			fruitY = rand() % height;
			for (int k = 0; k < ntail; k++)
				if (fruitX == tailX[k] and fruitY == tailY[k])
				{
					already = true;
					continue;
				}
				else
					already = false;	
		}
		
		ntail++;
	}
	int prevX, prevY;
	int prevX2, prevY2;
	prevX = tailX[0];
	prevY = tailY[0];
	for (int k = 1; k < ntail; k++)
	{
		prevX2 = tailX[k];
		prevY2 = tailY[k];
		tailX[k] = prevX;
		tailY[k] = prevY;
		prevX = prevX2;
		prevY = prevY2;
	}
	tailX[0] = x;
	tailY[0] = y;
}


int main() {
	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
	}
}