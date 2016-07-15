//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Pong
// Programmed by: Alvin Gonzales
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <exception>
using namespace std;

// The console coordinates start at (0, 0) and vectors start at (0, 0).
// Therefore, the (x, y) coordinates of the objects have to be adjusted
// with the conversion (x - 1, y - 1).

// move_cursor moves the console cursor to the (x, y) coordinate.
// clear_screen clears screen by filling it with blanks.
// cursor_off turns off blinking cursor.
// resize_console resizes the console window.
void move_cursor (int x, int y);
/*void clear_screen ();*/
void cursor_off ();
void resize_console (int table_length, int table_width);

class Ball;
class Table
{
	// Table matrix.
	vector <vector <string>> matrix;
	int table_length, table_width, p1_score, p2_score;
	bool var_game_over;
public:
	Table (int var_table_length, int var_table_width);
	void display ();
	void score (Ball& game_ball);
	void center_text (char* s);
	// Getter of var_game_over.
	bool game_over ();
	friend class Paddle;
	friend class Ball;
	friend class Comp_AI;
};

class Paddle
{
	// Coordinates (x, y) of paddle.
	int x, y;
public:
	Paddle (int x_coord, Table& game_table);
	void position (int input1, int input2, Ball& game_ball, Table& game_table);
	friend class Ball;
	friend class Comp_AI;
};

class Wait
{
protected:
	double clock1, clock2;
public:
	bool wait ();
};

class Ball: public Wait
{
	// For line formula: y = mx + b. seconds is for srand().
	// go_left is for ball direction. var_point is for scoring.
	int x, y, m, b;
	time_t seconds;
	bool go_left;
	bool var_point;
public:
	Ball (Table& game_table);
	void position (Paddle& p1_paddle, Paddle& p2_paddle, Table& game_table);
	void spin (int input2);
	// Getter for var_point.
	bool point ();
	friend class Table;
	friend class Paddle;
	friend class Comp_AI;
};

class Comp_AI: public Wait
{
	int y;
	time_t seconds;
public:
	Comp_AI ();
	void move (Paddle& p2_paddle, Ball& game_ball, Table& game_table);
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Start of main

int main ()
{
	// Table_length has to be odd and table_width even for
	// the relative positions of the objects to remain intact.
	// Two inputs are needed because getch returns two values for an arrow key press.
	// Up arrow = 224 72 
	// Down arrow = 224 80
	int table_length = 55, table_width = 22;
	int input1 = 0, input2 = 0;

	// Start of menu code.
	cursor_off ();
	resize_console (table_length - 1, table_width);
	cout << "Controls:\n";
	cout << "UP ARROW to go up\n";
	cout << "DOWN ARROW to go down\n";
	cout << "You are on the left\n";
	cout << "First to 11 wins";
	for (int n = 0; n < (table_width / 2 - 1) - 4; n++)
	{
		cout << "\n";
	}
	cout << setw (table_length / 2 + 2) << "Pong\n";
	cout << setw (table_length / 2 + 2) << "--> Play\n";
	cout << setw (table_length / 2 + 2) << "Quit\n";
	move_cursor ((table_length - 29) / 2, table_width - 1);
	cout << "Programmed by: Alvin Gonzales";
	
	bool new_game = true;
	// 13 is the hexcode for the enter key.
	while ((input1 = _getch()) != 13)
	{
		if (input1 == 224 && (input2 = _getch ()) == 72)
		{
			new_game = true;
			move_cursor (table_length / 2 - 7, table_width / 2 + 1);
			cout << "   ";
			move_cursor (table_length / 2 - 7, table_width / 2);
			cout << "-->";
		}
		else if (input1 == 224 && input2 == 80)
		{
			new_game = false;
			move_cursor (table_length / 2 - 7, table_width / 2);
			cout << "   ";
			move_cursor (table_length / 2 - 7, table_width / 2 + 1);
			cout << "-->";
		}
	}
	if (input1 == 13 && !new_game)
	{
		return 0;
	}
	// End of menu code.

	// Loop for start of new game.
	while (new_game)
	{
		move_cursor (0, 0);
		Table game_table (table_length, table_width);
		Paddle p1_paddle (1 + 4, game_table);
		Paddle p2_paddle (table_length - 4, game_table);
		Comp_AI comp_AI;
		game_table.display ();
	
		// Loop for game in progress. 0x71 and 0x51 are hexcode for the Q and q keys.
		while (input1 != 0x51 && input1 != 0x71 && !game_table.game_over ())
		{
			// Start of serve ball code (for a delay of 1 second between serves).
			double clock1 = clock ();
			Ball game_ball (game_table);
			while ((clock () - clock1)/CLOCKS_PER_SEC < 1 && input1 != 0x51 && input1 != 0x71)
			{
				// No key pressed.
				if (_kbhit () == 0)
				{
					comp_AI.move (p2_paddle, game_ball, game_table);
				}
				// Key pressed.
				else if ((input1 = _getch ()) == 224)
				{
					input2 = _getch ();
					p1_paddle.position (input1, input2, game_ball, game_table);
					comp_AI.move (p2_paddle, game_ball, game_table);
					_kbhit ();
				}
			}
			// End of serve ball code.

			// Loop for point in progress.
			while (input1 != 0x51 && input1 != 0x71 && !game_ball.point ())
			{
				// No key pressed.
				if (_kbhit () == 0)
				{
					comp_AI.move (p2_paddle, game_ball, game_table);
					game_ball.position (p1_paddle, p2_paddle, game_table);
				}
				// Key pressed.
				else if ((input1 = _getch ()) == 224)
				{
					input2 = _getch ();
					p1_paddle.position (input1, input2, game_ball, game_table);
					comp_AI.move (p2_paddle, game_ball, game_table);
					game_ball.position (p1_paddle, p2_paddle, game_table);
				}
			}

			// Code for if someone scores.
			if (game_ball.point ())
			{
				game_table.score (game_ball);
				// Destroys the ball.
				game_ball.~Ball ();
			}
		}

		// Prompts user for a new game.
		if (input1 != 0x51 && input1 != 0x71)
		{
			cout << "\n";
			char* text = "Press N for a new game or any other key to quit.";
			game_table.center_text (text);
			cout << "\n";
			// 0x6E and 0x4E are hexcode for 'n' and 'N'.
			if ((input1 = _getch()) == 0x6E || input1 == 0x4E)
			{
				new_game = true;
			}
			else
			{
				new_game = false;
			}
		}
		// This lets the user quit if the user presses 'Q' or 'q' while
		// in the middle of a game.
		else
		{
			new_game = false;
		}
	}
	return 0;
}

// End of main
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void move_cursor (int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), coord);
}
/*void clear_screen ()
{
	// Gets console information.
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &csbi);
	
	// Puts cusor at the origin and fills the screen with blanks.
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), coord);
	for (int n = 0; n <= csbi.srWindow.Bottom; n++)
	{
		for (int m = 0; m <= csbi.srWindow.Right; m++)
		{
			cout << " ";
		}
	}
	// Puts cursor back at the origin.
	SetConsoleCursorPosition (GetStdHandle (STD_OUTPUT_HANDLE), coord);
}*/
void cursor_off ()
{
	CONSOLE_CURSOR_INFO console_cursor;
	console_cursor.bVisible = 0;
	console_cursor.dwSize = 1;
	SetConsoleCursorInfo (GetStdHandle (STD_OUTPUT_HANDLE), &console_cursor);
}
void resize_console (int table_length, int table_width)
{
	SMALL_RECT r;
	r.Top = 0;
	r.Bottom = table_width;
	r.Left = 0;
	r.Right = table_length;
	SetConsoleWindowInfo (GetStdHandle (STD_OUTPUT_HANDLE), true, &r);
}

Table::Table (int var_table_length, int var_table_width)
{
	table_length = var_table_length;
	table_width = var_table_width;
	var_game_over = false;
	p1_score = 0;
	p2_score = 0;

	// Fills table matrix with blanks.
	matrix.assign (table_width, vector <string> (table_length, " "));
	// Top side of the table.
	for (int n = 1; n < (table_length - 1); n++)
	{
		matrix.at (0).at (n) = "-";
	}
	// Bottom side of the table.
	for (int n = 1; n < (table_length - 1); n++)
	{
		matrix.at (table_width - 1).at (n) = "-";
	}
	// Left side of the table.
	for (int n = 0; n < table_width; n++)
	{
		matrix.at (n).at (0) = (char) 0xB3;
	}
	// Right side of the table.
	for (int n = 0; n < table_width; n++)
	{
		matrix.at (n).at (table_length - 1) = (char) 0xB3;
	}
	// Line in the middle of the table.
	for (int n = 1; n < (table_width - 1); n++)
	{
		matrix.at (n).at ((table_length - 1) / 2) = "|";
	}
	// Scoreboard: Player 1.
	matrix.at (3).at ((table_length / 2) - 3) = "0";
	matrix.at (3).at ((table_length / 2) - 2) = "0";
	// Scoreboard: Player 2.
	matrix.at (3).at ((table_length / 2) + 2) = "0";
	matrix.at (3).at ((table_length / 2) + 3) = "0";
}
void Table::display ()
{
	for (int n = 0; n < table_width; n++)
	{
		for (int m = 0; m < table_length; m++)
		{
			try
			{
				cout << matrix.at (n). at (m);
			}
			catch (exception& e)
			{
				cout << "Element is outside the vector. Error: " << e.what ();
			}
		}
		cout << "\n";
	}
}
void Table::score (Ball& game_ball)
{
	// First part is needed to reprint the scoreboard if the ball
	// passes through the scoreboard.
	// Prints player 1's score.
	if (p1_score <= 9)
	{
		move_cursor ((table_length / 2) - 3, 3);
		cout << "0";
		cout << p1_score;
	}
	if (p1_score > 9 && p1_score < 11)
	{
		move_cursor ((table_length / 2) - 3, 3);
		cout << p1_score;
	}
	// Prints computer's score.
	if (p2_score <= 9)
	{
		move_cursor ((table_length / 2 + 2), 3);
		cout << "0";
		cout << p2_score;
	}
	if (p2_score > 9 && p2_score < 11)
	{
		move_cursor ((table_length / 2 + 2), 3);
		cout << p2_score;
	}

	// If player 1 scores.
	if (game_ball.point () &&
		game_ball.x == table_length - 1)
	{
		p1_score++;
		// Prints player 1's score.
		if (p1_score <= 9)
		{
			move_cursor ((table_length / 2) - 3, 3);
			cout << "0";
			cout << p1_score;
		}
		if (p1_score > 9 && p1_score <= 11)
		{
			move_cursor ((table_length / 2) - 3, 3);
			cout << p1_score;
		}
		// Player 1 wins.
		if (p1_score == 11)
		{
			cout << "\n";
			char* text = "You win!";
			center_text (text);
			var_game_over = true;
		}
	}
	// If the computer scores.
	if (game_ball.point () &&
	    game_ball.x == 2)
	{
		p2_score++;
		// Prints computer's score.
		if (p2_score <= 9)
		{
			move_cursor ((table_length / 2 + 2), 3);
			cout << "0";
			cout << p2_score;
		}
		if (p2_score > 9 && p2_score <= 11)
		{
			move_cursor ((table_length / 2 + 2), 3);
			cout << p2_score;
		}
		// Computer wins.
		if (p2_score == 11)
		{
			cout << "\n";
			char* text = "Computer wins!";
			center_text (text);
			var_game_over = true;
		}
	}
}
void Table::center_text (char* text)
{
	try
	{
		int l = strlen (text);
		// Redraws the left edge of the table.
		cout << (char) 0xB3;
		for (int n = 1; n < ((table_length - l) / 2); n++)
		{
			cout << " ";
		}
		cout << text;
	}
	catch (exception& e)
	{
		cout << "Error: " << e.what ();
	}
}
bool Table::game_over ()
{
	return var_game_over;
}

Paddle::Paddle (int x_coord, Table& game_table)
{
	x = x_coord;
	y = game_table.table_width / 2 + 1;
	// Top section of paddle.
	game_table.matrix.at (y - 2).at (x - 1) = (char) 0xB3;
	// Bottom section of paddle. The coordinates of the bottom part is used
	// for calculations.
	game_table.matrix.at (y - 1).at (x - 1) = (char) 0xB3;
}
void Paddle::position (int input1, int input2, Ball& game_ball, Table& game_table)
{
	// Erases previous spot.
	if (!(x == 2 || x == game_table.table_width - 1))
	{
		move_cursor (x - 1, y - 2);
		cout << " ";
		move_cursor (x - 1, y - 1);
		cout << " ";
		// Paddle movement tracking is needed in the matrix so that the ball
		// doesn't erase the paddle if the paddle moves into the ball's spot.
		game_table.matrix.at (y - 2).at (x - 1) = " ";
		game_table.matrix.at (y - 1).at (x - 1) = " ";
	}
	
	// Up arrow input.
	if (input1 == 224 && input2 == 72 && (y - 1) != 2)
	{
		--y;
	}
	// Down arrow input.
	else if (input1 == 224 && input2 == 80 && (y) != (game_table.table_width - 1))
	{
		++y;
	}
	// Displays the paddle.
	move_cursor (x - 1, y - 1);
	cout << (char) 0xB3;
	move_cursor (x - 1, y - 2);
	cout << (char) 0xB3;

	// Paddle tracking in the matrix is needed.
	game_table.matrix.at (y - 2).at (x - 1) = (char) 0xB3;
	game_table.matrix.at (y - 1).at (x - 1) = (char) 0xB3;
	
	// Determines if the paddle puts a spin a on the ball.
	if ((input1 == 224 && (input2 == 72 || input2 == 80)) &&
	    // Ball hits p1_paddle.
		((x == game_ball.x - 1 && game_ball.go_left) ||
		// Ball hits p2_paddle.
		(x == game_ball.x + 1 && !game_ball.go_left)) && 
		// The 'hit' face of the paddle.
		(y - 2 == game_ball.y || y - 1 == game_ball.y ||
		y == game_ball.y || y + 1 == game_ball.y))
	{
		game_ball.spin (input2);
	}
}

bool Wait::wait ()
{
	if (((clock () - clock1) * 1000 / CLOCKS_PER_SEC) >= clock2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Ball::Ball (Table& game_table)
{
	clock1 = clock ();
	clock2 = 80;
	var_point = false;

	// Gets system time and seeds rand ().
	time (&seconds);
	srand ((unsigned int) seconds);

	// Generates random values for the ball. Slope m is either +1 or -1.
	y = rand () % (game_table.table_width - 4) + 3;
	m = ((rand () % 2) * 2) - 1;
	x = (game_table.table_length - 1) / 2 + 1;
	b = y - (m * x);

	// Generates random direction for the ball.
	if ((rand () % 2) == 0)
	{
		go_left = true;
	}
	else
	{
		go_left = false;
	}

	// Displays the ball.
	move_cursor (x - 1, y - 1);
	cout << "O";
}
void Ball::position (Paddle& p1_paddle, Paddle& p2_paddle, Table& game_table)
{
	if (!wait ())
	{
		// Erases previous spot. The inequalities are needed so that the matrix vector
		// doesn't get checked outside its scope.
		if (y > 1 && y < game_table.table_width){
			if (game_table.matrix.at (y - 1).at (x - 1) != "\xB3")
			{
				move_cursor (x - 1, y - 1);
				cout << " ";
			}
			// Redraws scoreboard.
			if (game_table.matrix.at (y - 1).at (x - 1) == "0")
			{
				game_table.score (*this);
			}
			// Redraws the center line.
			if (x == (game_table.table_length - 1) / 2 + 1)
			{
				move_cursor (x - 1, y - 1);
				cout << "|";
			}
		}
		// If either player scores.
		if (x == 2 || x == game_table.table_length - 1)
		{
			var_point = true;
		}

		else
		{
			// Ball hits a table border. Collision detector.
			if (y <= 2 || y >= game_table.table_width - 1)
			{
				// m ranges from -2 to 2.
				if (m == -1 || m == 1)
				{
					// Formula for perpendicular lines.
					m = -1 / m;
					b = y - m * x;
				}
				else
				{
					m = -m;
					b = y - m * x;
				}
			}
			// Ball hits a paddle. Collision detector.
			if (// Ball hits p1_paddle.
				(p1_paddle.x == x - 1 && 
				(p1_paddle.y - 2 == y || p1_paddle.y - 1 == y ||
				 p1_paddle.y == y || p1_paddle.y + 1 == y)) ||
				// Ball hits top or bottom of p1_paddle.
				(p1_paddle.x == x && (p1_paddle.y + 1 == y ||
				 p1_paddle.y - 2 == y)) ||
				// Ball hits p2_paddle.
				(p2_paddle.x == x + 1 && 
				(p2_paddle.y - 2 == y || p2_paddle.y - 1 == y ||
				 p2_paddle.y == y || p2_paddle.y + 1 == y)) ||
				// Ball hits top or bottom of p2_paddle.
				(p2_paddle.x == x && (p2_paddle.y + 1 == y ||
				 p2_paddle.y - 2 == y)))
			{
				if (m == -1 || m == 1)
				{
					m = -1 / m;
				}
				else
				{
					m = -m;
				}
				b = y - m * x;
				// Determines direction after collision.
				if (p1_paddle.x == (x - 1))
				{
					go_left = false;
				}
				else if (p2_paddle.x == (x + 1))
				{
					go_left = true;
				}
				// Speeds up the ball.
				if (clock2 > 40)
				{
					clock2 -= 2;
				}
			}
			go_left ? x-- : x++;
			y = (m * x + b);
			// Displays the ball. B3 is the hexcode for the paddles.
			if ((y > 1 && y < game_table.table_width) &&
				game_table.matrix.at (y - 1) .at (x - 1) != "\xB3")
			{
				move_cursor (x - 1, y - 1);
				cout << "O";
			}
		}
		// Resets timer.
		clock1 = clock ();
	}
}
void Ball::spin (int input2)
{
	// Spin up. 72 is the hexcode for the up arrow.
	if (m >= -2 && m <= 2 &&
		input2 == 72)
	{
		switch (m)
		{
		case -2:
			go_left ? (m += 1) : (m = m);
			break;
		case -1:
			go_left ? (m += 1) : (m -= 1);
			break;
		case 0:
			m = 1;
			break;
		case 1:
			go_left ? (m += 1) : (m -= 1);
			break;
		case 2:
			go_left ? (m = m) : (m -= 1);
			break;
		}
	}
	// Spin down. 80 is the hexcode for the down arrow.
	if (m >= -2 && m <= 2 &&
		input2 == 80)
	{
		switch (m)
		{
		case -2:
			go_left ? (m = m) : (m += 1);
			break;
		case -1:
			go_left ? (m -= 1) : (m += 1);
			break;
		case 0:
			m = -1;
			break;
		case 1:
			go_left ? (m -= 1) : (m += 1);
			break;
		case 2:
			go_left ? (m -= 1) : (m = m);
			break;
		}
	}
}
bool Ball::point ()
{
	return var_point;
}

Comp_AI::Comp_AI ()
{
	// y is the calculated position of the ball when it reaches the computer paddle's
	// x coordinate.
	y = 0;
	clock1 = clock ();
	clock2 = 70;
	// For determining spins put by the AI on the ball and the AI's accuracy.
	time (&seconds);
	srand ((unsigned int) seconds);
}
void Comp_AI::move (Paddle& p2_paddle, Ball& game_ball, Table& game_table)
{
	if (!wait ())
	{
		// AI only recalculates when the calculated y value is outside
		// the playable area.
		// AI miscalculates.
		if ((y <= 1 || y >= game_table.table_width) &&
			(rand () % 100 + 1) > 80 && !game_ball.go_left)
		{
			// Direction of inaccuracy.
			if (rand () % 2 == 1)
			{
				if (game_ball.m == -1 || game_ball.m == -2)
				{
					y = game_ball.m * p2_paddle.x + game_ball.b + 4;
				}
				else if (game_ball.m == 1 || game_ball.m == 2)
				{
					y = game_ball.m * p2_paddle.x + game_ball.b + 3;
				}
			}
			else
			{
				if (game_ball.m == -1 || game_ball.m == -2)
				{
					y = game_ball.m * p2_paddle.x + game_ball.b - 2;
				}
				else if (game_ball.m == 1 || game_ball.m == 2)
				{
					y = game_ball.m * p2_paddle.x + game_ball.b - 3;
				}
			}
			// Correct calc.
			if (game_ball.m == 0)
			{
				y = game_ball.m * p2_paddle.x + game_ball.b;
			}
		}
		// Correct calculation.
		else if ((y <= 1 || y >= game_table.table_width) &&
				 !game_ball.go_left)
		{
			y = game_ball.m * p2_paddle.x + game_ball.b;
		}

		// Positions the paddle. arrow codes: 72 is up. 80 is down.
		if (y < game_table.table_width && y > 1)
		{
			// Positions paddle in the claculated y spot.
			if(!game_ball.go_left)
			{
				if (y < p2_paddle.y)
				{
					p2_paddle.position (224, 72, game_ball, game_table);
				}
				else if (y > p2_paddle.y)
				{
					p2_paddle.position (224, 80, game_ball, game_table);
				}
			}
			// Positions the paddle at the middle.
			else if (p2_paddle.y > (game_table.table_width / 2 + 1))
			{
				p2_paddle.position (224, 72, game_ball, game_table);
			}
			else if (p2_paddle.y < (game_table.table_width / 2 + 1))
			{
				p2_paddle.position (224, 80, game_ball, game_table);
			}
		}
		// Positions paddle in a good strategic position.
		else if (y >= game_table.table_width || y <= 1)
		{
			// Positions the paddle at 1 / 6 the table_width.
			if ((game_ball.x > (game_table.table_length * 2 / 3)) && 
				 !game_ball.go_left && y <= 1)
			{
				if (p2_paddle.y > game_table.table_width * 1 / 6)
				{
					p2_paddle.position (224, 72, game_ball, game_table);
				}
				else if (p2_paddle.y < game_table.table_width * 1 / 6)
				{
					p2_paddle.position (224, 80, game_ball, game_table);
				}
			}
			// Positions the paddle at 5 / 6 + 3 the table_width (the +3 is due
			// to the coordinates being the bottom part of the paddle 
			// and truncation error).
			else if ((game_ball.x > (game_table.table_length * 2 / 3)) && 
				      !game_ball.go_left && y >= game_table.table_width)
			{
				if (p2_paddle.y < game_table.table_width * 5 / 6 + 3)
				{
					p2_paddle.position (224, 80, game_ball, game_table);
				}
				else if (p2_paddle.y > game_table.table_width * 5 / 6 + 3)
				{
					p2_paddle.position (224, 72, game_ball, game_table);
				}
			}
			// Positions the paddle at the middle.
			else if (p2_paddle.y > (game_table.table_width / 2 + 1))
			{
				p2_paddle.position (224, 72, game_ball, game_table);
			}
			else if (p2_paddle.y < (game_table.table_width / 2 + 1))
			{
				p2_paddle.position (224, 80, game_ball, game_table);
			}
		}

		// Has the AI put a spin on a m = 0 ball.
		if (game_ball.m == 0 && !game_ball.go_left &&
			p2_paddle.x == game_ball.x + 1)
		{
			// Up spin.
			if (rand () % 2 == 1)
			{
				p2_paddle.position (224, 72, game_ball, game_table);
			}
			// Down spin.
			else
			{
				p2_paddle.position (224, 80, game_ball, game_table);
			}
		}
		// Has the AI potentially put a spin on a m!=0 ball.
		else if (!game_ball.go_left && p2_paddle.x == game_ball.x + 1 &&
				 rand () % 10 >= 8)
		{
			if (rand () % 2 == 1)
			{
				p2_paddle.position (224, 72, game_ball, game_table);
			}
			else
			{
				p2_paddle.position (224, 80, game_ball, game_table);
			}
		}
		// Resets the calculated y value to zero so that the AI centers the paddle.
		if (game_ball.go_left || game_ball.x == game_table.table_length - 1)
		{
			y = 0;
		}
		clock1 = clock ();
	}
}