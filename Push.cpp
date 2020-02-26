#include<cstdio>
#include<cstring>
#include<fstream>
#include<iostream>
#include<windows.h>
using namespace std;
int person_x, person_y, total, size_x, size_y, box_map[40][40];
char No_num;
bool goal_map[40][40];
enum KEY{ESC,SHIFT,UP,DOWN,LEFT,RIGHT,Up,Down,Left,Right};
void cls(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}
KEY get_keypress()
{
	bool c_esc, c_up, c_down, c_left, c_right, c_shift, att_up, att_down, att_left, att_right;
	while (true)
	{
		Sleep(200);
		c_esc = c_up = c_down = c_left = c_right = c_shift = att_up = att_down = att_left = att_right = false;
		c_up = GetAsyncKeyState(VK_UP);
		c_down = GetAsyncKeyState(VK_DOWN);
		c_left = GetAsyncKeyState(VK_LEFT);
		c_right = GetAsyncKeyState(VK_RIGHT);
		c_shift = GetAsyncKeyState(VK_SHIFT);
		c_esc = GetAsyncKeyState(VK_ESCAPE);
		att_up = GetAsyncKeyState(VK_NUMPAD8);
		att_down = GetAsyncKeyState(VK_NUMPAD2);
		att_left = GetAsyncKeyState(VK_NUMPAD4);
		att_right = GetAsyncKeyState(VK_NUMPAD6);
		if (c_up)
			return UP;
		if (c_down)
			return DOWN;
		if (c_left)
			return LEFT;
		if (c_right)
			return RIGHT;
		if (att_up)
			return Up;
		if (att_down)
			return Down;
		if (att_left)
			return Left;
		if (att_right)
			return Right;
		if (c_shift)
		{
			Sleep(200);
			return SHIFT;
		}
		if (c_esc)
		{
			Sleep(200);
			return ESC;
		}
	}
}
void read_file(char num)
{
	string name = "map_*.push";
	name[4] = num;
	ifstream fin;
	fin.open(name.c_str());
	if (!fin.is_open())
		cout << "fail to find this map file!" << endl;
	fin >> size_y >> size_x >> total;
	for (int i = 1; i <= size_x; i++)
		for (int j = 1; j <= size_y; j++)
		{
			fin >> box_map[i][j];
			if (box_map[i][j] == 3)
			{
				box_map[i][j] = 0;
				goal_map[i][j] = true;
			}
			if (box_map[i][j] == 4)
			{
				person_x = j;
				person_y = i;
				box_map[i][j] = 0;
			}
		}
	fin.close();
}
void print(bool x)
{
	if (x)
		cout << "\n\n    No." << No_num - 'a' + 1 << "level\n\n\n";
	for (int i = 1; i <= size_x; i++)
	{
		cout << "    ";
		for (int j = 1; j <= size_y; j++)
		{
			if (i == person_y && j == person_x)
				cout << "Ю";
			else if (box_map[i][j] == 1)
				cout << "█";
			else if (box_map[i][j] == 2)
				cout << "▓";
			else if (goal_map[i][j])
				cout << "◎";
			else
				cout << "  ";
		}
		cout << endl;
	}
}
void clean_map()
{
	memset(box_map, 0, 1600 * sizeof(int));
	memset(goal_map, 0, 1600 * sizeof(bool));
}
char choose()
{
	int x = 1, y = 1;
	while (true)
	{
		system("cls");
		cout << endl << "   Please choose the level(press the \"shift\" to enter):" << endl << endl << "   01~05  06~10  11~15  16~20" << endl;
		for (int i = 1; i <= 5 + 7 * (x - 1); i++)
			cout << " ";
		cout << "↑\n\n";
		for (int i = 1; i <= 5; i++)
		{
			cout << "  ";
			if (i == y)
				cout << "=> ";
			else
				cout << "   ";
			cout << i + 5 * (x - 1) << endl;
		}
		cout << endl << endl;
		clean_map();
		read_file(char(y + 5 * (x - 1) + 'a' - 1));
		print(false);
		switch (get_keypress())
		{
		case UP:
			if (y - 1 >= 1)
				y--;
			else
				if (x - 1 >= 1)
				{
					y = 5;
					x--;
				}
			break;
		case DOWN:
			y + 1 <= 5 ? y++ : (y = 1, x++);
			break;
		case LEFT:
			if (x - 1 >= 1)
				x--;
			break;
		case RIGHT:
			if (x + 1 <= 4)
				x++;
			break;
		case SHIFT:
			return char(y + 5 * (x - 1) + 'a' - 1);
		}
	}
}
bool win()
{
	int ttotal = 0;
	for (int i = 1; i <= size_x; i++)
		for (int j = 1; j <= size_y; j++)
			if (goal_map[i][j] && box_map[i][j] == 2)
				ttotal++;
	if (ttotal == total)
		return true;
	return false;
}
void print_win()
{
	system("cls");
	system("Color 5F");
	cout << "\n                Complete!\n\n    Wait to go on the next level...";
}
void move(KEY x)
{
	int mx = 0, my = 0;
	switch (x)
	{
	case UP:
	case Up:
		mx = 0; my = -1; break;
	case DOWN:
	case Down:
		mx = 0; my = 1; break;
	case LEFT:
	case Left:
		mx = -1; my = 0; break;
	case RIGHT:
	case Right:
		mx = 1; my = 0; break;
	}
	if (box_map[person_y - my][person_x - mx] == 2 && (x == Up || x == Down || x == Left || x == Right))
	{
		if (box_map[person_y + my][person_x + mx] == 0)
		{
			box_map[person_y - my][person_x - mx] = 0;
			box_map[person_y][person_x] = 2;
			person_x += mx;
			person_y += my;
		}
		return;
	}
	if (box_map[person_y + my][person_x + mx] == 0)
	{
		person_x += mx;
		person_y += my;
		return;
	}
	if (box_map[person_y + my][person_x + mx] == 2 && (x == UP || x == DOWN || x == LEFT || x == RIGHT))
	{
		if (box_map[person_y + my + my][person_x + mx + mx] == 0)
		{
			box_map[person_y + my][person_x + mx] = 0;
			box_map[person_y + my + my][person_x + mx + mx] = 2;
			person_x += mx;
			person_y += my;
		}
		return;
	}
}
int main()
{
	system("title =  Push_Case by White Monday");
	system("mode con: cols=80 lines=30");
	menu:
	system("Color 1F");			//change the color
	No_num = choose();			//choose
	start:
	system("Color 3F");			//change the color
	system("cls");				//clean the screen
	clean_map();				//reset the map
	read_file(No_num);
	while (true)
	{
		cls(0, 0);				//reset the handle
		print(true);			//print the map
		if (win())				//if all the cases are on the goals
		{
			print_win();
			No_num++;
			Sleep(1500);
			goto start;
		}
		KEY key_press = get_keypress();
		if (key_press == -1)	//Esc: return to "menu" mark
			goto menu;
		if (key_press == 0)		//Shift: return to "start" mark
			goto start;
		else
			move(key_press);	//move the person
	}
	return 0;
}