#include<iostream>
#include<ctime>
#include<math.h>
#include<conio.h>
#include<random>
#include<windows.h>

using namespace std;


//function that shows starting captions
void startingCaptions();

//this function makes choosing a level possible
void choosingLevel(int number, int board[20][20]);

//two functions that are made for changing coords symbol
//this makes field generation a lot faster
void writeOnCoords(int y, int x, char letter);
void writeOnCoords(int y, int x, int number);

//function that is checking which key have the player clicked
char keyCheck(char sign);

//this function generates a random number (used for randomly generated holes in obstacles)
void randomNumberGenerator(int* wsk, int rozmiar, int a, int b);

//this function is responsible for averything conected with obstacles - like moving or generating
void obstacle(int board[20][20], int* obstacleColumn, int obstacleFalling, int holeBeginning[20], int whichObstacle, int speed, int* playerY, int* jump);

//this function connects everything into one field to play on
void field(int board[20][20], int difficulty);


void startingCaptions()
{
	cout << "   III        CCCCC   YYYY     YYYY       TTTTTTTTTTTTTTTT    OOOO       WWW                 WWW  EEEEEEEEEEEE    RRRRRRR" << endl;
	cout << "   III      CCC         YYY   YYY               TTT         OOO  OOO      WWW               WWW   EEE             RR     RR" << endl;
	cout << "   III     CCC            YY YY                 TTT        OOO    OOO      WWW             WWW    EEE             RR     RR" << endl;
	cout << "   III    CCC              YYY                  TTT       OOO      OOO      WWW           WWW     EEEEEEEE        RR    RR" << endl;
	cout << "   III    CCC              YYY                  TTT       OOO      OOO       WWW         WWW      EEEEEEEE        RRRRRR" << endl;
	cout << "   III     CCC             YYY                  TTT        OOO    OOO         WWW  WWW  WWW       EEE             RR   RR" << endl;
	cout << "   III      CCC            YYY                  TTT         OOO  OOO           WW WW WW WW        EEE             RR    RR" << endl;
	cout << "   III        CCCCC        YYY                  TTT           OOOO              WWW   WWW         EEEEEEEEEEEE    RR     RR" << endl;
	cout << "Witam w grze Icy Tower! " << endl;
	cout << "Gra polega na przetrwaniu. Za kazdą przetwaną sekundę gracz zdobywa jeden punkt, więc im dłuzej przeżyjesz, tym więcej punktów zdobędziesz!" << endl;
	cout << "Przegrana następuje w momencie, w którym postać spada do przepaści, czyli na sam dół planszy." << endl;
	cout << "Aby tego uniknąć, musisz przeskakiwać przez luki w przeszkodach znajdujących się nad tobą.\n\n" << endl;
	cout << "Sterowanie: " << endl;
	cout << "-Przesunięcie w lewo - 'a'," << endl;
	cout << "-Przesunięcie w prawo - 'd'," << endl;
	cout << "-Skok do góry - spacja.\n" << endl;
	cout << "Udanej zabawy!!" << endl;
	cout << endl;
}

void choosingLevel(int number, int board[20][20])
{
	cout << "\nPodaj poziom  trudności:" << endl;
	cout << "1. łatwy" << endl;
	cout << "2. normalny" << endl;
	cout << "3. trudny" << endl;

	cin >> number;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
	}
	if (number == 1)
	{
		field(board, 1);
	}
	else if (number == 2)
	{
		field(board, 2);
	}
	else if (number == 3)
	{
		field(board, 4);
	}
	else
	{
		cout << endl;
		cout << "Podałeś zły klawisz" << endl;
		choosingLevel(number, board);
	}
}

void writeOnCoords(int y, int x, char letter) {
	//set cursor
	COORD c;
	c.X = x + 1;
	c.Y = y + 1;

	//write on cursor
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

	cout << letter;

	//hide cursor
	::HANDLE hConsoleOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::CONSOLE_CURSOR_INFO hCCI;
	::GetConsoleCursorInfo(hConsoleOut, &hCCI);
	hCCI.bVisible = FALSE;
	::SetConsoleCursorInfo(hConsoleOut, &hCCI);
}

void writeOnCoords(int y, int x, int number) {
	//set cursor
	COORD c;
	c.X = x;
	c.Y = y;

	//write on cursor
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

	cout << number;

	//hide cursor
	::HANDLE hConsoleOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::CONSOLE_CURSOR_INFO hCCI;
	::GetConsoleCursorInfo(hConsoleOut, &hCCI);
	hCCI.bVisible = FALSE;
	::SetConsoleCursorInfo(hConsoleOut, &hCCI);
}

char keyCheck(char sign)
{
	char newSign = sign;
	if (_kbhit())
	{
		while (_kbhit() == 1)
		{
			switch (_getch())
			{
			case 'a': newSign = 'a'; break;
			case 'd': newSign = 'd'; break;
			case ' ': newSign = ' '; break;
			case 'A': newSign = 'a'; break;
			case 'D': newSign = 'd'; break;
			case 'w': newSign = ' '; break;
			case 'W': newSign = ' '; break;
			}
		}
	}
	return newSign;
}

void randomNumberGenerator(int* wsk, int size, int a, int b)
{
	mt19937 generator(time(nullptr));
	uniform_int_distribution<int>dist(a, b);
	for (int i = 0; i < size; i++)
	{
		wsk[i] = dist(generator);
	}
}

void obstacle(int board[20][20], int* obstacleColumn, int obstacleFalling, int holeBeginning[20], int whichObstacle, int speed,int* playerY,int* jump)
{
	if (obstacleFalling % ((20 / speed)) == 0)
	{
		for (int j = 0; j < 20; j++)
		{
			if (j != holeBeginning[whichObstacle] && j != holeBeginning[whichObstacle] + 1 && j != holeBeginning[whichObstacle] + 2 && board[obstacleColumn[whichObstacle]][j]==0)
				board[obstacleColumn[whichObstacle]][j] = 1;
			else if (board[obstacleColumn[whichObstacle]][j] == 2 && j != holeBeginning[whichObstacle] && j != holeBeginning[whichObstacle] + 1 && j != holeBeginning[whichObstacle] + 2 && *playerY<19)
			{
				(*playerY)++;
				board[*playerY][j] = 2;
				board[obstacleColumn[whichObstacle]][j] = 1;
				jump=0;
			}
			if (obstacleColumn[whichObstacle] > 0)
			{
				board[obstacleColumn[whichObstacle] - 1][j] = 0;
			}
		}
		obstacleColumn[whichObstacle]++;
	}
}

void field(int board[20][20], int difficulty)
{
		system("CLS");
		cout << "__________________________________________ Punkty: " << endl;
		for (int i = 0; i < 20; i++)
		{
			cout<<"|                                        |"<<endl;
			for (int j = 0; j < 20; j++)
			{
				board[i][j] = 0;
			}
		}
		cout << "------------------------------------------";
		for (int j = 0; j < 20; j++)
		{
			board[19][j] = 1;
		}

		//variables needed for player generation
		board[18][10] = 2;
		int playerX = 10;
		int playerY = 18;
		char sign = '*';
		int jump = 0;
		int jumpStrength = 0;
		int playerFalling = 0;
		//end of variables needed for player generation
		
		//variables needed for obstacle generation
		int obstacleColumn[20]{};
		int holeBeginning[20]{};
		randomNumberGenerator(holeBeginning, 20, 0, 17);
		int obstacleFalling = 0;
		int speed = difficulty;
		//end of variables needed for obstacle generation
		
		//variables needed for time and generation purpuses
		int startTime = time(NULL);
		int currentTime = time(NULL);
		int lastLineClear = 0;
		//end of variables needed for time and generation purpuses

		//field generation
		while (true)
		{
			currentTime = time(NULL);
			//checking if the player lost the game
			if (playerY >= 19)
			{
				cout << "\n\n\n";
				cout << "Przegrałeś!" << endl;
				cout << "Liczba uzyskanych przez ciebie punktów to " << currentTime - startTime <<"." << endl;
				cout << endl;
				system("PAUSE");
				system("CLS");
				break;
			}

			//start of the segment responsible for creating obstacles
			if (obstacleFalling % 400 == 0 && obstacleFalling != 0 && speed<9)
			{
				speed += 2;
			}

			if (obstacleColumn[0] < 20)
				obstacle(board, obstacleColumn, obstacleFalling, holeBeginning, 0, speed,&playerY,&jump);
			else if (obstacleFalling % (20 / speed) == 0)
				obstacleColumn[0]++;

			if (obstacleColumn[0] == 21)
			{
				for (int j = 0; j < 20; j++)
				{
					board[19][j] = 0;
				}
				obstacleColumn[0] = 1;
				randomNumberGenerator(&holeBeginning[0], 1, 0, 17);
			}
			if (obstacleFalling >= (4 * 20) / difficulty)
			{
				if (obstacleColumn[1] < 20)
					obstacle(board, obstacleColumn, obstacleFalling, holeBeginning, 1, speed,&playerY,&jump);
				else if (obstacleFalling % (20 / speed) == 0)
					obstacleColumn[1]++;
				if (obstacleColumn[1] == 21)
				{
					for (int j = 0; j < 20; j++)
					{
						board[19][j] = 0;
					}
					obstacleColumn[1] = 1;
					randomNumberGenerator(&holeBeginning[1], 1, 0, 17);
				}
			}
			if (obstacleFalling >= (8 * 20) / difficulty)
			{
				if (obstacleColumn[2] < 20)
					obstacle(board, obstacleColumn, obstacleFalling, holeBeginning, 2, speed,&playerY,&jump);
				else if (obstacleFalling % (20 / speed) == 0)
					obstacleColumn[2]++;
				if (obstacleColumn[2] == 21)
				{
					for (int j = 0; j < 20; j++)
					{
						board[19][j] = 0;
					}
					obstacleColumn[2] = 1;
					randomNumberGenerator(&holeBeginning[2], 1, 0, 17);
				}
			}
			if (obstacleFalling >= (12 * 20) / difficulty)
			{
				if (obstacleColumn[3] < 20)
					obstacle(board, obstacleColumn, obstacleFalling, holeBeginning, 3, speed,&playerY,&jump);
				else if (obstacleFalling % (20 / speed) == 0)
					obstacleColumn[3]++;
				if (obstacleColumn[3] == 21)
				{
					for (int j = 0; j < 20; j++)
					{
						board[19][j] = 0;
					}
					obstacleColumn[3] = 1;
					randomNumberGenerator(&holeBeginning[3], 1, 0, 17);
				}
			}
			if (obstacleFalling >= (16 * 20) / difficulty)
			{
				if (obstacleColumn[4] < 20)
					obstacle(board, obstacleColumn, obstacleFalling, holeBeginning, 4, speed,&playerY,&jump);
				else if (obstacleFalling % (20 / speed) == 0)
					obstacleColumn[4]++;
				if (obstacleColumn[4] == 21)
				{
					for (int j = 0; j < 20; j++)
					{
						board[19][j] = 0;
					}
					obstacleColumn[4] = 1;
					randomNumberGenerator(&holeBeginning[4], 1, 0, 17);
				}
			}
			//end of the segment responsible for creating obstacles

			sign = keyCheck(sign);

			//start of the segment responsible for moving left
			if (sign == 'a' && playerX != 0 && board[playerY][playerX - 1] != 1)
			{
				writeOnCoords(playerY, playerX * 2, ' ');
				writeOnCoords(playerY, playerX * 2 + 1, ' ');
				playerX--;
				board[playerY][playerX + 1] = 0;
				board[playerY][playerX] = 2;
				sign = '*';
			}
			//end of the segment responsible for moving left

			//start of the segment responsible for moving right
			if (sign == 'd' && playerX != 19 && board[playerY][playerX + 1] != 1)
			{
				writeOnCoords(playerY, playerX * 2, ' ');
				writeOnCoords(playerY, playerX * 2 + 1, ' ');
				playerX++;
				board[playerY][playerX - 1] = 0;
				board[playerY][playerX] = 2;
				sign = '*';
			}
			//end of the segment responsible for moving right

			//start of the segment responsible for jumping
			if (sign == ' ' && jump == 0 && board[playerY + 1][playerX] == 1 && playerY != 19)
			{
				jump = 1;
				sign = '*';
			}
			if (jump == 1)
			{
				if (board[playerY - 1][playerX] != 1 && playerY > 1)
				{
					writeOnCoords(playerY, playerX * 2, ' ');
					writeOnCoords(playerY, playerX * 2 + 1, ' ');
					playerY--;
					board[playerY][playerX] = 2;
					board[playerY + 1][playerX] = 0;
					jumpStrength++;
					if (jumpStrength == 6)
					{
						jumpStrength = 0;
						jump = 0;
					}
				}
				else
				{
					jumpStrength = 0;
					jump = 0;
				}
			}
			//end of the segment responsible for jumping

			//start of the segment responsible for falling
			if (board[playerY + 1][playerX] == 0 && playerFalling % 1 == 0 && jumpStrength == 0)
			{
				writeOnCoords(playerY, playerX * 2, ' ');
				writeOnCoords(playerY, playerX * 2 + 1, ' ');
				playerY++;
				board[playerY][playerX] = 2;
				if (board[playerY - 1][playerX] != 1)
					board[playerY - 1][playerX] = 0;
			}
			//ent of the segment responsible for falling

			//start of the segment responsible for field generation
			writeOnCoords(0, 50, currentTime - startTime);
			if (lastLineClear == 2)
			{
				lastLineClear = 0;
			}
			else if (lastLineClear == 1)
			{
				lastLineClear = 2;
			}
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (lastLineClear != 0 && i == 0)
					{
						writeOnCoords(19, j * 2, ' ');
						writeOnCoords(19, j * 2 + 1, ' ');
					}
					if (board[i][j] == 1)
					{
						writeOnCoords(i, j * 2, '[');
						writeOnCoords(i, j * 2 + 1, ']');
						if (board[i - 1][j] != 2 && i != 0)
						{
							writeOnCoords(i - 1, j * 2, ' ');
							writeOnCoords(i - 1, j * 2 + 1, ' ');
						}
					}
					else if (board[i][j] == 2)
					{
						writeOnCoords(i, j * 2, '(');
						writeOnCoords(i, j * 2 + 1, ')');
					}
				}
				if (i == 19)
					lastLineClear = 1;
			}
			//end of the segment responsible for field generation
			Sleep(40);
			playerFalling++;
			obstacleFalling++;
		}
}

void main()
{
	setlocale(LC_CTYPE, "Polish");
	int board[20][20]{};
	int difficulty = 0;
	int whatgame = 0;
	bool loop = true;
	int number = 0;
	startingCaptions();
	while (loop)
	{
		cout << "1. Rozpocznij grę \n2. Zakończ działanie programu" << endl;
		if (whatgame != 0)
			cout << "3. Przypomnij sobie zasady gry" << endl;
		cin >> (number);
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Podałeś zły klawisz" << endl;
			cout << "1. Rozpocznij grę \n2. Zakończ działanie programu" << endl;
			if (whatgame != 0)
				cout << "3. Przypomnij sobie zasady gry" << endl;
			cin >> number;
		}
		if (number == 1)
		{
			choosingLevel(number, board);
			whatgame += 1;
			loop = true;
		}
		else if (number == 2)
			loop = false;
		else if (number == 3 && whatgame != 0)
		{
			cout << "Gra polega na przetrwaniu. Za każdą przetwaną sekundę gracz zdobywa jeden punkt, więc im dłuzej przeżyjesz, tym więcej punktów zdobędziesz!" << endl;
			cout << "Przegrana następuje w momencie, w którym postać spada do przepaści, czyli na sam dół planszy." << endl;
			cout << "Aby tego uniknąć, musisz przeskakiwać przez luki w przeszkodach znajdujących się nad tobą." << endl;
			cout << "Sterowanie: " << endl;
			cout << "-Przesunięcie w lewo - 'a'" << endl;
			cout << "-Przesunięcie w prawo - 'd'" << endl;
			cout << "-Skok do góry - spacja" << endl;
			cout << endl;
		}
		else
		{
			cout << "Podałeś zły klawisz" << endl;
		}
	}
}
