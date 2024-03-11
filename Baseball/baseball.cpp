// Name: Matt Galvan
// Date: 5/09/2016
// Lab: Final
// Summary: Baseball game

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

WORD Attributes = 0;
HANDLE hConsole;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // used for goto
COORD CursorPosition; // used for goto


//***********************
//*     Prototypes		*
//***********************
void reset();
void color(int);
void dCol();
void calc();
void wl();
void gotoxy(int, int);
void validate(string);

#pragma region VARS

#pragma region IO_FILE_VARS
//***************************************************
//*				  I/O File variables				*
//*-------------------------------------------------*
//*fc = game counter file							*
//*ofs = report file								*
//*sstats = stats file								*
//*file1 = file to read in player names and stats,	*
//*			used in players().						*
//*flie2 = game 2 report file name					*
//*file3 = game 2 stats csv file name				*
//*file4 = game 1 report file name					*
//*file5 = game 1 stats csv file name				*
//*file6 = copy of game 1 stats file name			*
//*file7 = game counter file name					*
//*drFile = Dr. Who player stats					*
//*game2 = false: it's game 1, true: it's game 2	*
//*game = holds game number							*
//***************************************************
ifstream fc("filecounter.txt");
ofstream ofs;
fstream sstats;
string file1;
string file2 = "Galvan_Game2_Lab23.txt";
string file3 = "Galvan_Game2_Stats.csv";
string file4 = "Galvan_lab23.txt";
string file5 = "Galvan_Stats.csv";
string file6 = "Galvan_Game2_Stats_Copy.csv";
string file7 = "filecounter.txt";
string drFile = "DrWho.csv";
string fin1 = "1", fin2 = "2";
boolean game2 = false;
int game;
#pragma endregion IO_FILE_VARS

#pragma region OUTPUT_FILE_VARS
//***************************************
//*			Output File variables		*
//*-------------------------------------*
//*no = eof message						*
//*hard = student name					*
//*coding = date and course number		*
//*of = what was printed in the report	*
//*anything = final message				*
//*labn = lab number					*
//*hdr1 = header text					*
//*ftr1 = footer text					*
//*g1 = game 1 text						*
//*g2 = game 2 text						*
//***************************************
string no = "EoF Message ";
string hard = " Matt Galvan ";
string coding = "5/09/2016, CIS 2541-003 ";
string of = "Baseball stats and scoreboard ";
string anything = "Goodbye. ";
string labn = "Lab27";
string hdr1 = "Player stats and scoreboard for Baseball";
string ftr1 = "Printed the player stats and scoreboard";
string g1 = "Game 1";
string g2 = "Game 2";
#pragma endregion OUTPUT_FILE_VARS

#pragma region BOXES_VARS
//***************************
//*		  Box variables		*
//*-------------------------*
//*box1 = the - for boxes	*
//*box2 = the | for boxes	*
//***************************
string box1 = "-";
string box2 = "|";
#pragma endregion BOXES_VARS

#pragma region CHAR_VARS
//***********************
//* Character variables	*
//*---------------------*
//*col = a :			*
//*com = a ,			*
//***********************
string col = ":";
string com = ",";
#pragma endregion CHAR_VARS

#pragma region PLAYER_STRUCTURE
//***************************
//*	    Player Structure	*
//*-------------------------*
//*Creates the player		*
//*structure				*
//*							*
//*name = player name		*
//*hr = home runs			*
//*so = strike outs			*
//*s = singles				*
//*d = doubles				*
//*t = triples				*
//*ab = at bats				*
//*oba = on base average	*
//*bavg = batting average	*
//*onBase = true if player	*
//*			is on base		*
//*bp = base position		*
//*		0 - not on base		*
//*		1 - first base		*
//*		2 - second base		*
//*		3 - third base		*
//*		4+- home base		*
//***************************
struct plr
{
	string name;
	int hr;
	int so;
	int s;
	int d;
	int t;
	int ab;
	double oba;
	double bavg;
	bool onBase;
	int bp;
};
#pragma endregion PLAYER_STRUCTURE

//************************
//*Lab Specific Variables*
//************************
#pragma region PLAYER_VARS
//***************************
//*    Player Variables		*
//*-------------------------*
//*p = player structure		*
//*plrs = player info vector*
//***************************
plr p;
vector<plr> plrs(18);
#pragma endregion PLAYER_VARS

#pragma region STATS_VARS
//*******************************************
//*				Stats Variables				*
//*-----------------------------------------*
//*stats = stats heading					*
//*ps = Player Stats text					*
//*temp = holds temp info when reading in	*
//*			player info. Used in players()	*
//*avg = holds oba and bavg for a player	*
//*******************************************
string stats[9] = { "Player", "HR", "SO", "S", "D", "T", "AB", "OBA", "BAVG" };
string ps = "Player Stats";
string temp;
double avg;
#pragma endregion STATS_VARS

#pragma region TEAM_VARS
//***************************
//*		 Team Variables		*
//*-------------------------*
//*tA, tB = team names		*
//*batA, batB = who's up to	*
//*				bat			*
//*bat = who's up to bat,	*
//*		passed into hit()	*
//***************************
string tA = "White Sox";
string tB = "Cubs";
int batA = 0, batB = 9, bat;
#pragma endregion TEAM_VARS

#pragma region COLOR_VARS
//***************************
//*	    Color Variables		*
//*-------------------------*
//*k = used to select color	*
//*		number for color()	*
//*sc = teamA color number	*
//*		for scoreboard		*
//*cc = teamB color number	*
//*		for scoreboard		*
//***************************
int k, sc = 8, cc = 9;
#pragma endregion COLOR_VARS

#pragma region MENU_VARS
//*******************************
//*		  Menu Variables		*
//*-----------------------------*
//*one, two, three, four, five	*
//*six = menu choices text		*
//*q = quit text				*
//*sel = Selection text			*
//*inv = invalid choice text	*
//*in = inning text				*
//*ng = game in progress text	*
//*curInn = current inning text	*
//*load = cheat option			*
//*warn = used in newgame()		*
//*opt = string selection option*
//*ecode = enter code string	*
//*menu_item = menu item number	*
//*x2 = default x for menu items*
//*running = used to quit		*
//*option = opt converted to int*
//*******************************
string one = "1. Play 1 inning (Sloooooow)";
string two = "2. Play the rest of the game";
string three = "3. Play full game (Fast!)";
string four = "4. View Player Stats";
string five = "5. View Scoreboard";
string six = "6. Rules";
string seven = "7. Enter Code";
string q = "8. Quit";
string sel = "SELECT: ";
string inv = "Invalid selection. Try again.";
string in = "Inning";
string ng = "There is no game currently in progress.";
string curInn = "Current Inning: ";
string load = "Loading new players...";
string warn = "Starting a new game will clear the previous scores and will not save them to the .txt file. Would you like to continue (y or n)? ";
string ecode = "Enter code: ";
string opt;
int menu_item = 0, x2 = 13, option;
bool running = true;
#pragma endregion MENU_VARS

#pragma region GAME_PLAY_VARS
//*******************************
//*		Game Play Variables		*
//*-----------------------------*
//*String variables that are	*
//*used during game play. If you*
//*can't figure out what they're*
//*used for, you need to go back*
//*to 1400.						*
//*up = is true while the batter*
//*		is up to bat			*
//*isTie = if it's a tie game	*
//*isOver = is the game over?	*
//*aUp, bUp = team up to bat	*
//*num = holds random number	*
//*strike, ball, outs = holds	*
//*		those numbers for each	*
//*		at bat					*
//*inNum = holds inning number	*
//*n = number of innings to play*
//*wb = used to determine if	*
//*		someone is on the next	*
//*		base during a walk		*
//*x = default x coordinate		*
//*		position				*
//*slp1, slp2 = holds values for*
//*				sleep commands	*
//*******************************
string tau = "White Sox are up to bat\n";
string tbu = "Cubs are up to bat\n";
string strikeStr = "Strikes : ";
string ballStr = "Balls   : ";
string outStr = "Outs    : ";
string abStr = "At Bat  : ";
string pscore = " has scored!";
string ha = " hit a ";
string ga = " got a ";
string ghr = " got a hit, resulting in a ";
string ge = " got to first on an ";
string hs = "SINGLE!";
string hd = "DOUBLE!";
string ht = "TRIPLE!";
string homer = "HOME RUN!";
string hw = "WALK!";
string he = "ERROR.";
string hso = " STRUCK OUT.";
string fo = "FLY OUT.";
string ins = "Inning: ";
string w = " won the game!";
string tie = "Due to some shady practices, the White Sox have been awarded an extra point and have won the game!";
bool up = true, isTie = false, isOver = false, aUp = false, bUp = false;
int num, strike = 0, ball = 0, outs = 0, inNum = 1, n = 0, wb = 1, x = 14, slp1, slp2;
#pragma endregion GAME_PLAY_VARS

#pragma region SCOREBOARD_VARS
//***************************
//*	 Scoreboard Variables	*
//*-------------------------*
//*sb = scoreboard text		*
//*inning = holds scoreboard*
//*			header text		*
//*scoreA, scoreB = holds	*
//*		game scoring and	*
//*		stats for each team	*
//***************************
string sb = "Scoreboard";
string inning[12] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "R", "H", "E" };
int scoreA[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int scoreB[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
#pragma endregion SCOREBOARD_VARS

#pragma region RULES_VARS
//***************************
//*		Rules Variables		*
//*-------------------------*
//*Variables for printing	*
//*the rules screen.		*
//***************************
string key = "Stats Key:\nHR   = Home Runs\nSO   = Strike Outs\nS    = Single\nD    = Double\nT    = Triple\nAB   = At Bats\nOBA  = On Base Average\nBAVG = Batting Average";
string rnd1 = "|-------------------------------|\n";
string rnd2 = "|  1 |Foul\t|  2 |Single\t|\n";
string rnd3 = "|  3 |Foul\t|  4 |Double\t|\n";
string rnd4 = "|  5 |Foul\t|  6 |Triple\t|\n";
string rnd5 = "|  7 |Foul\t|  8 |Home Run\t|\n";
string rnd6 = "|  9 |Strike\t| 10 |Ball\t|\n";
string rnd7 = "| 11 |Strike\t| 12 |Ball\t|\n";
string rnd8 = "| 13 |Strike\t| 14 |Ball\t|\n";
string rnd9 = "| 15 |Strike\t| 16 |Ball\t|\n";
string rnd10 = "| 17 |Strike\t| 18 |Ball\t|\n";
string rnd11 = "| 19 |Fly Out\t| 20 |Error\t|\n";
#pragma endregion RULES_VARS
#pragma endregion VARS

#pragma region BIG_LETTERS
//***************************
//*	 Big Letter Variables	*
//***************************

//***********************
//*	    Dalek Vars		*
//***********************
string d1 = "                    Exterminate!\n";
string d2 = "                   /\n";
string d3 = "              ___\n";
string d4 = "      D>=G==='   '.\n";
string d5 = "            |======|\n";
string d6 = "            |======|\n";
string d7 = "        )--/]IIIIII]\n";
string d8 = "           |_______|\n";
string d9 = "           C O O O D\n";
string d10 = "          C O  O  O D\n";
string d11 = "         C  O  O  O  D\n";
string d12 = "         C__O__O__O__D\n";
string d13 = "        [_____________]\n";

//***********************
//*	  Police Box Vars	*
//***********************
string pb[30] = { "                 _.--._\n",
	"                 _|__|_\n",
	"     _____________|__|_____________\n",
	"  .-'______________________________'-.\n",
	"  | |________POLICE___BOX__________| |\n",
	"  |  |============================|  |\n",
	"  |  | .-----------..-----------. |  |\n",
	"  |  | |  _  _  _  ||  _  _  _  | |  |\n",
	"  |  | | | || || | || | || || | | |  |\n",
	"  |  | | |_||_||_| || |_||_||_| | |  |\n",
	"  |  | | | || || | || | || || | | |  |\n",
	"  |  | | |_||_||_| || |_||_||_| | |  |\n",
	"  |  | |  _______  ||  _______  | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |_______| || |_______| | |  |\n",
	"  |  | |  _______ @||@ _______  | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |_______| || |_______| | |  |\n",
	"  |  | |  _______  ||  _______  | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |       | || |       | | |  |\n",
	"  |  | | |_______| || |_______| | |  |\n",
	"  |  | '-----------''-----------' |  |\n",
	" _|__|/__________________________\\|__|_ \n",
	"'----'----------------------------'----'\n" };

//***********************
//*	  Baseball Vars		*
//***********************
string b1 = " ____                 _           _ _ \n";
string b2 = "|  _ \\               | |         | | |\n";
string b3 = "| |_) | __ _ ___  ___| |__   __ _| | |\n";
string b4 = "|  _ < / _` / __|/ _ \\ '_ \\ / _` | | |\n";
string b5 = "| |_) | (_| \\__ \\  __/ |_) | (_| | | |\n";
string b6 = "|____/ \\__,_|___/\\___|_.__/ \\__,_|_|_|\n";

//***********************
//*		Menu Vars		*
//***********************
string mg1 = " __  __                  \n";
string mg2 = "|  \\/  |                 \n";
string mg3 = "| \\  / | ___ _ __  _   _ \n";
string mg4 = "| |\\/| |/ _ \\ '_ \\| | | |\n";
string mg5 = "| |  | |  __/ | | | |_| |\n";
string mg6 = "|_|  |_|\\___|_| |_|\\__,_|\n";

//***********************
//*		VS. Vars		*
//***********************
string v1 = "\t\t____    ____      _______.    \n";
string v2 = "\t\t\\   \\  /   /     /       |    \n";
string v3 = "\t\t \\   \\/   /     |   (----`    \n";
string v4 = "\t\t  \\      /       \\   \\        \n";
string v5 = "\t\t   \\    /    .----)   |    __ \n";
string v6 = "\t\t    \\__/     |_______/    (__)\n";

//***********************
//*		Rules Vars		*
//***********************
string ru1 = "      ___           ___           ___       ___           ___     \n";
string ru2 = "     /  /\\         /  /\\         /  /\\     /  /\\         /  /\\    \n";
string ru3 = "    /  /::\\       /  /:/        /  /:/    /  /::\\       /  /::\\   \n";
string ru4 = "   /  /:/\\:\\     /  /:/        /  /:/    /  /:/\\:\\     /__/:/\\:\\  \n";
string ru5 = "  /  /::\\ \\:\\   /  /:/        /  /:/    /  /::\\ \\:\\   _\\_ \\:\\ \\:\\ \n";
string ru6 = " /__/:/\\:\\_\\:\\ /__/:/     /\\ /__/:/    /__/:/\\:\\ \\:\\ /__/\\ \\:\\ \\:\\\n";
string ru7 = " \\__\\/~|::\\/:/ \\  \\:\\    /:/ \\  \\:\\    \\  \\:\\ \\:\\_\\/ \\  \\:\\ \\:\\_\\/\n";
string ru8 = "    |  |:|::/   \\  \\:\\  /:/   \\  \\:\\    \\  \\:\\ \\:\\    \\  \\:\\_\\:\\  \n";
string ru9 = "    |  |:|\\/     \\  \\:\\/:/     \\  \\:\\    \\  \\:\\_\\/     \\  \\:\\/:/  \n";
string ru10 = "    |__|:|~       \\  \\::/       \\  \\:\\    \\  \\:\\        \\  \\::/   \n";
string ru11 = "     \\__\\|         \\__\\/         \\__\\/     \\__\\/         \\__\\/    \n";

#pragma region CUBS_VARS
//***********************
//*   Cubs Variables	*
//*---------------------*
//*All variables needed	*
//*to print the Cubs	*
//*logo					*
//***********************
string c7 = "              ";
string c8 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n";
string c9 = "            ";
string c10 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n";
string c11 = "          ";
string c12 = "BBBBBBBBBBBBB";
string c14 = "BBBBBBBBBBBBBB\n";
string c15 = "        ";
string c16 = "BBBBBBBBBBBB";
string c17 = "     ";
string c18 = "RRRRRRRRRR";
string c20 = "BBBBBBBBBBB\n";
string c21 = "       ";
string c22 = "BBBBBBBBBBB";
string c23 = "   ";
string c24 = "RRRRRRRRRRRRRRRRR";
string c25 = "    ";
string c26 = "BBBBBBBBBB\n";
string c27 = "      ";
string c30 = "RRRRRRRRRRRRRRRRRRRRR";
string c32 = "BBBBBBBBB\n";
string c34 = "BBBBBBBBB";
string c36 = "RRRRRRRRRRRRRRRRRRRRRRRRR";
string c42 = "RRRRRRRRRRRRRRRRRRRRRRRRRRRR";
string c43 = "  ";
string c46 = "BBBBBBBB";
string c49 = "         ";
string c58 = "RRRRR";
string c60 = "BBBBBBBB\n";
string c65 = "                ";
string c66 = "R";
string c72 = "RRRRRRR";
string c74 = "RRR";
string c77 = " ";
string c112 = "BBBBBBB\n";
string c158 = "BBBBBBB";
string c161 = "                            ";
string c168 = "RR";
string c175 = "             ";
string c208 = "RRRRRRRRRRRRRRRR";
string c211 = "        ";
string c217 = "          ";
string c220 = "BBBBBBBBBBBBB\n";
string c223 = "              ";
#pragma endregion CUBS_VARS

#pragma region SOX_VARS
//***********************
//*    Sox Variables	*
//*---------------------*
//*All variables needed	*
//*to print the Sox	logo*
//***********************
string ws1 = "                                                        ";
string ws2 = "WWWW\n";
string ws3 = "                                                    ";
string ws4 = "WWWWWWWWWW\n";
string ws5 = "                                                ";
string ws6 = "WWWWWWWWWWWWWW\n";
string ws7 = "                                           ";
string ws8 = "WWWWWWWWWWWWWWWWWW\n";
string ws9 = "                                       ";
string ws10 = "WWWW";
string ws11 = "       ";
string ws13 = "                                    ";
string ws14 = "WWWWWW";
string ws15 = " ";
string ws17 = "WWWWWWW\n";
string ws18 = "                               ";
string ws19 = "WWWWWWWW";
string ws20 = "WWWWWWWWWWWW";
string ws21 = "WW\n";
string ws22 = "                           ";
string ws23 = "WWWWWWWWWW";
string ws24 = "WWWWWWWWWWWWWWWWWWWWW\n";
string ws25 = "                       ";
string ws26 = "WWWWWWWWWWWWW";
string ws27 = "WWWWWWWWWWWWWWWWWWWWWWWW\n";
string ws28 = "                   ";
string ws29 = "WWWWWWWWWWWWWWWW";
string ws31 = "               ";
string ws32 = "WWWWWWWWWWWWWWWWWW";
string ws33 = "WWWWWWWWWWWWWWWWWWWW\n";
string ws34 = "            ";
string ws35 = "WWWWWWWWWWWWWW";
string ws36 = "           ";
string ws38 = "     ";
string ws39 = "RRRRRR";
string ws40 = "WWWWWWWWW";
string ws41 = "                ";
string ws42 = "WWWWWWWWWWWWWWWW\n";
string ws43 = "WW";
string ws44 = "RRRRRRRR";
string ws45 = "WWWWWWWWWWWWWWWWWWWWWW";
string ws49 = "WWWWWWWWWWWWWWWWWWWWWW\n";
string ws50 = "WWWWWWWWWWWWWWWWWWWWW";
string ws51 = "WWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
string ws52 = "          ";
string ws54 = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
string ws55 = "W";
string ws56 = "    ";
string ws57 = "WWWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
string ws58 = "                                          ";
string ws60 = "                                         ";
string ws61 = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
string ws66 = "WWW";
string ws67 = "WWWWWWWWW\n";
string ws68 = "                                                       ";
string ws69 = "WWWWWWWWWWWWWWWWWWWWWWW";
string ws70 = "WWWWWWWWWWW\n";
string ws71 = "                 ";
string ws73 = "      ";
string ws75 = "WWWWWWWWWWWW\n";
string ws76 = "        ";
string ws78 = "WWWWWWWWWWW";
#pragma endregion SOX_VARS

#pragma region MLB_VARS
//***********************
//*   MLB Variables		*
//*---------------------*
//*All variables needed	*
//*to print the MLB	logo*
//***********************
string m1 = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n";
string m2 = "W";
string m3 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m4 = "     ";
string m5 = "RRRRRRRRRRRRRRRRRRRRRRR";
string m6 = "W\n";
string m7 = "W";
string m8 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m9 = "      ";
string m10 = "RRRRRRRRRRRRRRRRRRRRR";
string m11 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m12 = "         ";
string m13 = "BBBBBBB";
string m14 = "      ";
string m15 = "RRRRRRRRRRRRRRRRRRRR";
string m16 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m17 = "              ";
string m18 = "BBBBB";
string m19 = "      ";
string m20 = "RRRRRRRRRRRRRRRRRRR";
string m21 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m22 = "                ";
string m23 = "BBBBBB";
string m24 = "     ";
string m25 = "RRRRRRRRRRRRRRRRRR";
string m26 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m27 = "                 ";
string m28 = "BBBBBBB";
string m29 = "     ";
string m30 = "RRRRRRRRRRRRRRRR";
string m31 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m32 = "                    ";
string m33 = "BBBBBBB";
string m34 = "     ";
string m35 = "RRRRRRRRRRRRRRR";
string m36 = "BBBBBBBBBBBBBBBBBBBBBBBBBB";
string m37 = "                      ";
string m38 = "BBBBBBBBBB";
string m39 = "    ";
string m40 = "RRRRRRRRRRRRRR";
string m41 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m42 = "                ";
string m43 = "BBBBBBBBBBBB";
string m44 = "    ";
string m45 = "RRRRRRRRRRRRR";
string m46 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m47 = "                  ";
string m48 = "BBBBBBBBBBB";
string m49 = "      ";
string m50 = "RRRRRRRRRRR";
string m51 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m52 = "                                  ";
string m53 = "RRRRRRRRR";
string m54 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m55 = "                                     ";
string m56 = "RRRRRRRR";
string m57 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m58 = "                                        ";
string m59 = "RRRRRRR";
string m60 = "BBBBBBB";
string m61 = "    ";
string m62 = "BBBBBBBBBBBBBBBBB";
string m63 = "                                            ";
string m64 = "RRRR";
string m65 = "BBBBBB";
string m66 = "      ";
string m67 = "BBBBBBBBBBBBBBB";
string m68 = "                                              ";
string m69 = "RRR";
string m70 = "BBBBBBB";
string m71 = "    ";
string m72 = "BBBBBBBBBBBBBBBB";
string m73 = "                                               ";
string m74 = "RR";
string m75 = "BBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m76 = "                                              ";
string m77 = "RRR";
string m78 = "BBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m79 = "                                            ";
string m80 = "RRRRR";
string m81 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
string m82 = "                                ";
string m83 = "RRRRRRRRRRRRR";
#pragma endregion MLB_VARS

//***********************
//*	    Print "Menu"	*
//*---------------------*
//*Prints the menu big	*
//*letters graphic		*
//***********************
void menG() {

	gotoxy(0, 6);
	color(7);
	cout << mg1 << mg2 << mg3 << mg4 << mg5 << mg6 << endl;
	dCol();

}

//***********************
//*	    Print Rules		*
//*---------------------*
//*Prints the rules big	*
//*letters graphic		*
//***********************
void rus() {

	color(11);
	cout << ru1 << ru2 << ru3 << ru4 << ru5
		<< ru6 << ru7 << ru8 << ru9 << ru10
		<< ru11 << endl << endl;
	dCol();

}

//***********************
//*	   Print Baseball	*
//*---------------------*
//*Prints the baseball	*
//*big letters graphic	*
//***********************
void bbG() {

	gotoxy(0, 0);
	color(14);
	cout << b1 << b2 << b3 << b4 << b5 << b6;

}

//***********************
//*	     Print VS. 		*
//*---------------------*
//*Prints the VS. big	*
//*letters graphic		*
//***********************
void vsG() {

	color(12);
	cout << endl << endl << endl << endl << endl << endl;
	cout << v1 << v2 << v3 << v4 << v5 << v6 << endl;
	dCol();

}

#pragma region SOX_LOGO
//***********************
//*  Sox Logo Function	*
//*---------------------*
//*Prints the Sox logo	*
//***********************
void soxG() {

	int k = 255;
	dCol();
	cout << ws1;
	color(k);
	cout << ws2;
	dCol();
	cout << ws3;
	color(k);
	cout << ws4;
	dCol();
	cout << ws5;
	color(k);
	cout << ws6;
	dCol();
	cout << ws7;
	color(k);
	cout << ws8;
	dCol();
	cout << ws9;
	color(k);
	cout << ws10;
	dCol();
	cout << ws11;
	color(k);
	cout << ws4;
	dCol();
	cout << ws13;
	color(k);
	cout << ws14;
	dCol();
	cout << ws15;
	color(k);
	cout << ws14;
	dCol();
	cout << ws15;
	color(k);
	cout << ws17;
	dCol();
	cout << ws18;
	color(k);
	cout << ws19;
	dCol();
	cout << ws15;
	color(k);
	cout << ws20;
	dCol();
	cout << ws15;
	color(k);
	cout << ws21;
	dCol();
	cout << ws22;
	color(k);
	cout << ws23;
	dCol();
	cout << ws15;
	color(k);
	cout << ws24;
	dCol();
	cout << ws25;
	color(k);
	cout << ws26;
	dCol();
	cout << ws15;
	color(k);
	cout << ws27;
	dCol();
	cout << ws28;
	color(k);
	cout << ws29;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws8;
	dCol();
	cout << ws31;
	color(k);
	cout << ws32;
	dCol();
	cout << ws15 << ws15 << ws15;
	color(k);
	cout << ws33;
	dCol();
	cout << ws34;
	color(k);
	cout << ws35;
	dCol();
	cout << ws36;
	color(k);
	cout << ws8;
	dCol();
	cout << ws38;
	color(204);
	cout << ws39;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws40;
	dCol();
	cout << ws41;
	color(k);
	cout << ws42 << ws43;
	dCol();
	cout << ws15 << ws15;
	color(204);
	cout << ws44;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws45;
	dCol();
	cout << ws15;
	color(k);
	cout << ws42 << ws43;
	dCol();
	cout << ws15 << ws15;
	color(204);
	cout << ws44;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws45;
	dCol();
	cout << ws15;
	color(k);
	cout << ws49 << ws43;
	dCol();
	cout << ws15 << ws15;
	color(204);
	cout << ws44;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws50;
	dCol();
	cout << ws15;
	color(k);
	cout << ws51;
	dCol();
	cout << ws38;
	color(204);
	cout << ws39;
	dCol();
	cout << ws52;
	color(k);
	cout << ws35;
	dCol();
	cout << ws15;
	color(k);
	cout << ws54;
	dCol();
	cout << ws13;
	color(k);
	cout << ws55;
	dCol();
	cout << ws56;
	color(k);
	cout << ws57;
	dCol();
	cout << ws58;
	color(k);
	cout << ws57;
	dCol();
	cout << ws60;
	color(k);
	cout << ws61;
	dCol();
	cout << ws68;
	color(k);
	cout << ws17;
	dCol();
	cout << ws15;
	color(k);
	cout << ws50;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws50;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws19;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws66;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws67;
	color(k);
	cout << ws45;
	dCol();
	cout << ws15;
	color(k);
	cout << ws69;
	dCol();
	cout << ws15;
	color(k);
	cout << ws23;
	dCol();
	cout << ws15 << ws15 << ws15;
	color(k);
	cout << ws70;
	color(k);
	cout << ws14;
	dCol();
	cout << ws71;
	color(k);
	cout << ws14;
	dCol();
	cout << ws36;
	color(k);
	cout << ws14;
	dCol();
	cout << ws73;
	color(k);
	cout << ws6;
	color(k);
	cout << ws50;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws14;
	dCol();
	cout << ws36;
	color(k);
	cout << ws14;
	dCol();
	cout << ws11;
	color(k);
	cout << ws75;
	dCol();
	cout << ws15;
	color(k);
	cout << ws50;
	dCol();
	cout << ws15;
	color(k);
	cout << ws14;
	dCol();
	cout << ws36;
	color(k);
	cout << ws14;
	dCol();
	cout << ws76;
	color(k);
	cout << ws4;
	dCol();
	cout << ws41;
	color(k);
	cout << ws14;
	dCol();
	cout << ws15;
	color(k);
	cout << ws14;
	dCol();
	cout << ws36;
	color(k);
	cout << ws14;
	dCol();
	cout << ws73;
	color(k);
	cout << ws6;
	color(k);
	cout << ws45;
	dCol();
	cout << ws15;
	color(k);
	cout << ws69;
	dCol();
	cout << ws15;
	color(k);
	cout << ws78;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws70;
	color(k);
	cout << ws50;
	dCol();
	cout << ws15 << ws15 << ws15;
	color(k);
	cout << ws50;
	dCol();
	cout << ws15 << ws15;
	color(k);
	cout << ws23;
	dCol();
	cout << ws15 << ws15 << ws15 << ws15;
	color(k);
	cout << ws23;
	dCol();

}
#pragma endregion SOX_LOGO

#pragma region CUBS_LOGO
//***********************
//*  Cubs Logo Function	*
//*---------------------*
//*Prints the Cubs logo	*
//***********************
void cubG() {

	dCol();
	cout << c7;
	color(153);
	cout << c8;
	dCol();
	cout << c9;
	color(153);
	cout << c10;
	dCol();
	cout << c11;
	color(153);
	cout << c12;
	color(255);
	cout << c7;
	color(153);
	cout << c14;
	dCol();
	cout << c15;
	color(153);
	cout << c16;
	color(255);
	cout << c17;
	color(204);
	cout << c18;
	color(255);
	cout << c17;
	color(153);
	cout << c20;
	dCol();
	cout << c21;
	color(153);
	cout << c22;
	color(255);
	cout << c23;
	color(204);
	cout << c24;
	color(255);
	cout << c25;
	color(153);
	cout << c26;
	dCol();
	cout << c27;
	color(153);
	cout << c18;
	color(255);
	cout << c23;
	color(204);
	cout << c30;
	color(255);
	cout << c25;
	color(153);
	cout << c32;
	dCol();
	cout << c17;
	color(153);
	cout << c34;
	color(255);
	cout << c23;
	color(204);
	cout << c36;
	color(255);
	cout << c23;
	color(153);
	cout << c32;
	dCol();
	cout << c25;
	color(153);
	cout << c34;
	color(255);
	cout << c23;
	color(204);
	cout << c42;
	color(255);
	cout << c43;
	color(153);
	cout << c32;
	dCol();
	cout << c25;
	color(153);
	cout << c46;
	color(255);
	cout << c23;
	color(204);
	cout << c18;
	color(255);
	cout << c49;
	color(204);
	cout << c34;
	color(255);
	cout << c25;
	color(153);
	cout << c32;
	dCol();
	cout << c23;
	color(153);
	cout << c46;
	color(255);
	cout << c23;
	color(204);
	cout << c34;
	color(255);
	cout << c7;
	color(204);
	cout << c58;
	color(255);
	cout << c21;
	color(153);
	cout << c60;
	dCol();
	cout << c23;
	color(153);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c34;
	color(255);
	cout << c65;
	color(204);
	cout << c66;
	color(255);
	cout << c11;
	color(153);
	cout << c60;
	dCol();
	cout << c43;
	color(153);
	cout << c46;
	color(255);
	cout << c23;
	color(204);
	cout << c72;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c77;
	color(204);
	cout << c72;
	color(255);
	cout << c43;
	color(204);
	cout << c72;
	color(255);
	cout << c43;
	color(153);
	cout << c60;
	dCol();
	cout << c43;
	color(153);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c77;
	color(204);
	cout << c46;
	color(255);
	cout << c77;
	color(204);
	cout << c72;
	color(255);
	cout << c43;
	color(153);
	cout << c60;
	dCol();
	cout << c43;
	color(153);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c77;
	color(204);
	cout << c74;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c77;
	color(204);
	cout << c74;
	color(255);
	cout << c21;
	color(153);
	cout << c112;
	dCol();
	cout << c43;
	color(153);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c77;
	color(204);
	cout << c72;
	color(255);
	cout << c43;
	color(204);
	cout << c72;
	color(255);
	cout << c23;
	color(153);
	cout << c112;
	dCol();
	cout << c43;
	color(153);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c77;
	color(204);
	cout << c74;
	color(255);
	cout << c43;
	color(204);
	cout << c74;
	color(255);
	cout << c77;
	color(255);
	cout << c74;
	color(255);
	cout << c77;
	color(204);
	cout << c74;
	color(255);
	cout << c23;
	color(153);
	cout << c112;
	dCol();
	cout << c43;
	color(153);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c46;
	color(255);
	cout << c77;
	color(204);
	cout << c72;
	color(255);
	cout << c43;
	color(204);
	cout << c72;
	color(255);
	cout << c43;
	color(153);
	cout << c60;
	dCol();
	cout << c23;
	color(153);
	cout << c158;
	color(255);
	cout << c23;
	color(204);
	cout << c46;
	color(255);
	cout << c161;
	color(153);
	cout << c112;
	dCol();
	cout << c23;
	color(153);
	cout << c46;
	color(255);
	cout << c43;
	color(204);
	cout << c34;
	color(255);
	cout << c65;
	color(204);
	cout << c168;
	color(255);
	cout << c49;
	color(153);
	cout << c112;
	dCol();
	cout << c23;
	color(153);
	cout << c46;
	color(255);
	cout << c23;
	color(204);
	cout << c34;
	color(255);
	cout << c175;
	color(204);
	cout << c58;
	color(255);
	cout << c21;
	color(153);
	cout << c60;
	dCol();
	cout << c25;
	color(153);
	cout << c46;
	color(255);
	cout << c23;
	color(204);
	cout << c22;
	color(255);
	cout << c15;
	color(204);
	cout << c34;
	color(255);
	cout << c25;
	color(153);
	cout << c60;
	dCol();
	cout << c17;
	color(153);
	cout << c46;
	color(255);
	cout << c23;
	color(204);
	cout << c42;
	color(255);
	cout << c43;
	color(153);
	cout << c32;
	dCol();
	cout << c17;
	color(153);
	cout << c34;
	color(255);
	cout << c23;
	color(204);
	cout << c36;
	color(255);
	cout << c23;
	color(153);
	cout << c32;
	dCol();
	cout << c27;
	color(153);
	cout << c18;
	color(255);
	cout << c23;
	color(204);
	cout << c30;
	color(255);
	cout << c25;
	color(153);
	cout << c32;
	dCol();
	cout << c21;
	color(153);
	cout << c18;
	color(255);
	cout << c17;
	color(204);
	cout << c208;
	color(255);
	cout << c25;
	color(153);
	cout << c26;
	dCol();
	cout << c211;
	color(153);
	cout << c16;
	color(255);
	cout << c27;
	color(204);
	cout << c46;
	color(255);
	cout << c27;
	color(153);
	cout << c20;
	dCol();
	cout << c217;
	color(153);
	cout << c12;
	color(255);
	cout << c175;
	color(153);
	cout << c220;
	dCol();
	cout << c9;
	color(153);
	cout << c10;
	dCol();
	cout << c223;
	color(153);
	cout << c8;
	dCol();

}
#pragma endregion CUBS_LOGO

#pragma region MLB_LOGO
//***********************
//*  MLB Logo Function	*
//*---------------------*
//*Prints the MLB logo	*
//***********************
void mlb() {
	cout << endl;
	color(255);			//White
	cout << m1 << m2;
	color(153);			//Blue
	cout << m3;
	color(255);
	cout << m4;
	color(204);			//Red
	cout << m5;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m8;
	color(255);
	cout << m9;
	color(204);
	cout << m10;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m11;
	color(255);
	cout << m12;
	color(153);
	cout << m13;
	color(255);
	cout << m14;
	color(204);
	cout << m15;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m16;
	color(255);
	cout << m17;
	color(153);
	cout << m18;
	color(255);
	cout << m19;
	color(204);
	cout << m20;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m21;
	color(255);
	cout << m22;
	color(153);
	cout << m23;
	color(255);
	cout << m24;
	color(204);
	cout << m25;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m26;
	color(255);
	cout << m27;
	color(153);
	cout << m28;
	color(255);
	cout << m29;
	color(204);
	cout << m30;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m31;
	color(255);
	cout << m32;
	color(153);
	cout << m33;
	color(255);
	cout << m34;
	color(204);
	cout << m35;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m36;
	color(255);
	cout << m37;
	color(153);
	cout << m38;
	color(255);
	cout << m39;
	color(204);
	cout << m40;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m41;
	color(255);
	cout << m42;
	color(153);
	cout << m43;
	color(255);
	cout << m44;
	color(204);
	cout << m45;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m46;
	color(255);
	cout << m47;
	color(153);
	cout << m48;
	color(255);
	cout << m49;
	color(204);
	cout << m50;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m51;
	color(255);
	cout << m52;
	color(204);
	cout << m53;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m54;
	color(255);
	cout << m55;
	color(204);
	cout << m56;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m57;
	color(255);
	cout << m58;
	color(204);
	cout << m59;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m60;
	color(255);
	cout << m61;
	color(153);
	cout << m62;
	color(255);
	cout << m63;
	color(204);
	cout << m64;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m65;
	color(255);
	cout << m66;
	color(153);
	cout << m67;
	color(255);
	cout << m68;
	color(204);
	cout << m69;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m70;
	color(255);
	cout << m71;
	color(153);
	cout << m72;
	color(255);
	cout << m73;
	color(204);
	cout << m74;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m75;
	color(255);
	cout << m76;
	color(204);
	cout << m77;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m78;
	color(255);
	cout << m79;
	color(204);
	cout << m80;
	color(255);
	cout << m6 << m7;
	color(153);
	cout << m81;
	color(255);
	cout << m82;
	color(204);
	cout << m83;
	color(255);
	cout << m6 << m1;
	dCol();
}
#pragma endregion MLB_LOGO

//***********************
//*	   Dalek Graphic 	*
//*---------------------*
//*Prints the Dalek pic	*
//***********************
void dalek() {

	color(6);
	cout << d1 << d2 << d3 << d4 << d5
		<< d6 << d7 << d8 << d9 <<
		d10 << d11 << d12 << d13;
	dCol();

}

//***********************
//*	Police Box Graphic 	*
//*---------------------*
//*Prints the police box*
//*pic					*
//***********************
void policeBox() {

	color(9);
	for (int i = 0; i < 30; i++)
	{
		gotoxy(39, i);
		cout << pb[i];
	}
	dCol();

}

#pragma endregion BIG_LETTERS

//***********************
//*    Pause Function	*
//*---------------------*
//*Creates 2 blank lines*
//*then pauses the game *
//***********************
void pause() {
	cout << endl << endl;
	system("PAUSE");
}

#pragma region COLORS
//***********************
//*	   Change Colors	*
//***********************
void color(int k) {

	SetConsoleTextAttribute(hConsole, k);

}

//***********************
//*	   Default Color	*
//*---------------------*
//*Sets the text color	*
//*light green			*
//***********************
void dCol() {

	SetConsoleTextAttribute(hConsole, 10);

}
#pragma endregion COLORS

#pragma region Cursor_Position
//***********************
//*	  Cursor Position	*
//*		 Function		*
//*---------------------*
//*Changes the cursor	*
//*position to print to	*
//*the console screen in*
//*a specific coordinate*
//***********************
void gotoxy(int column, int line) {
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
		);
}

//*******************************
//*wherex(), wherey() return	*
//*the current position of the	*
//*cursor						*
//*******************************
int wherex() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD                      result;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	result = csbi.dwCursorPosition;
	return result.X;
}

int wherey() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD                      result;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	result = csbi.dwCursorPosition;
	return result.Y;
}

//Menu cursor position
void gotoXY(int x2, int y)
{
	CursorPosition.X = x2;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
#pragma endregion Cursor_Position

//************************
//* Open Files Function  *
//*----------------------*
//*Opens correct files	 *
//************************
void oFiles() {

	fc >> game;
	if (game == 2) {
		file1 = "Galvan_Stats.csv";
		ofs.open(file2, ios::app);
		sstats.open(file3);
		fc.close();
		game2 = true;
	}
	else {
		file1 = "PlayerInfo.csv";
		fc.close();
		ofs.open(file4);
		sstats.open(file5);
	}

}

//***********************
//*Player Stats Function*
//*---------------------*
//*Reads in player stats*
//***********************
void players() {

	fstream csv(file1);

	for (int i = 0; i < (int)plrs.size(); i++)
	{
		getline(csv, temp, ',');
		plrs[i].name = temp;
		getline(csv, temp, ',');
		plrs[i].hr = atoi(temp.c_str());
		getline(csv, temp, ',');
		plrs[i].so = atoi(temp.c_str());
		getline(csv, temp, ',');
		plrs[i].s = atoi(temp.c_str());
		getline(csv, temp, ',');
		plrs[i].d = atoi(temp.c_str());
		getline(csv, temp, ',');
		plrs[i].t = atoi(temp.c_str());
		getline(csv, temp, ',');
		plrs[i].ab = atoi(temp.c_str());
		getline(csv, temp, ',');
		plrs[i].oba = atof(temp.c_str());
		getline(csv, temp, '\n');
		plrs[i].bavg = atof(temp.c_str());
		p.onBase = false;
		p.bp = 0;
	}
	csv.close();

}

#pragma region TXT_FILE_FUNCTIONS
//***********************
//*	  .txt file Box		*
//*		 Function		*
//*---------------------*
//*Creates the - part of*
//*the boxes			*
//***********************
void box() {

	ofs << box2;

	for (int i = 1; i < 83; i++)
	{
		ofs << box1;
	}

	ofs << box2 << endl;

}

//***********************
//*	  .txt blank Box	*
//*		 Function		*
//*---------------------*
//*Creates blank boxes	*
//***********************
void blankBox() {

	ofs << box2 << right << setw(83) <<
		box2 << endl;

}


//***********************
//*		Console Box		*
//*		 Function		*
//*---------------------*
//*Creates the boxes for*
//*the console			*
//***********************
void cbox(int z) {

	cout << box2;

	for (int i = 1; i < z; i++)
	{
		cout << box1;
	}

	cout << box2 << endl;

}


//***********************
//*	     EOF Message	*
//***********************
void eof() {

	box();

	ofs << box2 << no << hard << coding
		<< of << setw(4) << box2 << endl
		<< box2 << anything << labn <<
		setw(69) << box2 << endl;

	box();

}


//***********************
//*	  Header Message	*
//***********************
void hdr() {

	box();

	ofs << box2 << hdr1 << hard <<
		coding << labn <<
		box2 << endl;

	box();

	blankBox();

}


//***********************
//*    Footer Message	*
//***********************
void ftr() {

	box();

	ofs << box2 << left << setw(82) << ftr1 <<
		box2 << endl;

	box();

	blankBox();

}
#pragma endregion TXT_FILE_FUNCTIONS

//***********************
//*	Console Stats Header*
//*   Output Function 	*
//*---------------------*
//*Prints out the header*
//*for the player stats *
//***********************
void hout() {

	cout << box2 << left << setw(20) << stats[0] <<
		box2 << setw(5) << stats[1] <<
		box2 << setw(5) << stats[2] <<
		box2 << setw(5) << stats[3] <<
		box2 << setw(5) << stats[4] <<
		box2 << setw(5) << stats[5] <<
		box2 << setw(5) << stats[6] <<
		box2 << setw(5) << stats[7] <<
		box2 << setw(5) << stats[8] <<
		box2 << endl;

}

//***********************
//*	   Console Stats	*
//*   Output Function 	*
//*---------------------*
//*Prints out the player*
//*stats on the console *
//***********************
void psprint(int i) {

	cout << box2 << left << setw(20) << plrs[i].name << box2;
	cout << setw(5) << right << plrs[i].hr << box2
		<< setw(5) << plrs[i].so << box2
		<< setw(5) << plrs[i].s << box2
		<< setw(5) << plrs[i].d << box2
		<< setw(5) << plrs[i].t << box2
		<< setw(5) << plrs[i].ab << box2
		<< setw(5) << plrs[i].oba << box2
		<< setw(5) << plrs[i].bavg << box2
		<< endl;

}

//***********************
//*	  Text File Stats	*
//*   Output Function 	*
//*---------------------*
//*Prints out the player*
//*stats to the output  *
//*file					*
//***********************
void psoprint(int i) {

	ofs << box2 << left << setw(26) << plrs[i].name << box2;
	ofs << setw(6) << right << plrs[i].hr << box2
		<< setw(6) << plrs[i].so << box2
		<< setw(6) << plrs[i].s << box2
		<< setw(6) << plrs[i].d << box2
		<< setw(6) << plrs[i].t << box2
		<< setw(6) << plrs[i].ab << box2
		<< setw(6) << plrs[i].oba << box2
		<< setw(6) << plrs[i].bavg << box2
		<< endl;

}

//***********************
//*	   Stats Console	*
//*   Output Function 	*
//*---------------------*
//*Prints the stats to  *
//*the console			*
//***********************
void sout() {

	system("CLS");

	cbox(69);
	cout << box2 << left << setw(68) << tA << box2 << endl;
	cbox(69);
	hout();
	cbox(69);
	for (int i = 0; i < 9; i++)
	{
		psprint(i);
	}
	cbox(69);
	cout << box2 << left << setw(68) << tB << box2 << endl;
	cbox(69);
	hout();
	cbox(69);
	for (int i = 9; i < (int)plrs.size(); i++)
	{
		psprint(i);
	}
	cbox(69);

	pause();

}

//***********************
//*	   Player Stats		*
//*   Output Function 	*
//*---------------------*
//*Prints the stats to  *
//*the output file		*
//***********************
void pout() {

	box();
	ofs << box2 << left << setw(82) << ps << box2 << endl;
	box();
	ofs << box2 << left << setw(82) << tA << box2 << endl;
	box();
	ofs << box2 << left << setw(26) << stats[0] <<
		box2 << setw(6) << stats[1] <<
		box2 << setw(6) << stats[2] <<
		box2 << setw(6) << stats[3] <<
		box2 << setw(6) << stats[4] <<
		box2 << setw(6) << stats[5] <<
		box2 << setw(6) << stats[6] <<
		box2 << setw(6) << stats[7] <<
		box2 << setw(6) << stats[8] <<
		box2 << endl;
	box();
	for (int i = 0; i < 9; i++)
	{
		psoprint(i);
	}
	box();
	box();
	ofs << box2 << left << setw(82) << tB << box2 << endl;
	box();
	ofs << box2 << left << setw(26) << stats[0] <<
		box2 << setw(6) << stats[1] <<
		box2 << setw(6) << stats[2] <<
		box2 << setw(6) << stats[3] <<
		box2 << setw(6) << stats[4] <<
		box2 << setw(6) << stats[5] <<
		box2 << setw(6) << stats[6] <<
		box2 << setw(6) << stats[7] <<
		box2 << setw(6) << stats[8] <<
		box2 << endl;
	box();
	for (int i = 9; i < (int)plrs.size(); i++)
	{
		psoprint(i);
	}
	box();

}

//***********************
//*	   Score Console	*
//*   Output Function 	*
//*---------------------*
//*Prints the scoreboard*
//*to the console		*
//***********************
void sboard() {

	gotoxy(0, 0);

	cbox(69);
	cout << box2 << left << setw(20) << in << box2;
	//*******************************************************
	//*Prints and creates the inning number blinking effect	*
	//*******************************************************
	for (int i = 0; i < 12; i++)
	{
		if ((i + 1) == inNum)
		{
			if (k == 6)
			{
				k = 14;
			}
			else {
				k = 6;
			}
			color(k);
		}
		cout << setw(3) << right << inning[i];
		dCol();
		cout << box2;
	}
	cout << endl;
	cbox(69);
	cout << box2;
	//***************************************************************
	//*If team A is up, creates the inning number blinking effect	*
	//***************************************************************
	if (aUp) {
		if (sc == 23)
		{
			sc = 7;
		}
		else {
			sc = 23;
		}
	}
	else {
		sc = 7;
	}
	color(sc);
	cout << left << setw(20) << tA;
	dCol();
	cout << box2;
	//***************************************************************
	//*Prints the inning score. Changes color to black if it is not *
	//*the current or past inning									*
	//***************************************************************
	for (int i = 0; i < 12; i++)
	{
		if (scoreA[i] == 0 && i < 9 && i >(inNum - 1))
		{
			color(0);
		}
		cout << right << setw(3) << scoreA[i];
		dCol();
		cout << box2;
	}
	cout << endl;
	cbox(69);
	cout << box2;
	//***************************************************************
	//*If team B is up, creates the inning number blinking effect	*
	//***************************************************************
	if (bUp) {
		if (cc == 23)
		{
			cc = 11;
		}
		else {
			cc = 23;
		}
	}
	else {
		cc = 11;
	}
	color(cc);
	cout << left << setw(20) << tB;
	dCol();
	cout << box2;
	//***************************************************************
	//*Prints the inning score. Changes color to black if it is not *
	//*the current or past inning									*
	//***************************************************************
	for (int i = 0; i < 12; i++)
	{
		if (scoreB[i] == 0 && i < 9 && i >(inNum - 1))
		{
			color(0);
		}
		cout << right << setw(3) << scoreB[i];
		dCol();
		cout << box2;
	}
	cout << endl;
	cbox(69);

}

//***********************
//*		Scoreboard		*
//*   Output Function 	*
//*Prints the scoreboard*
//*to the output file	*
//***********************
void sbout() {

	box();
	ofs << box2 << left << setw(82) << sb << box2 << endl;
	box();
	ofs << box2 << left << setw(22) << in << box2;
	//***********************
	//*Scoreboard header	*
	//***********************
	for (int i = 0; i < 12; i++)
	{
		ofs << setw(4) << right << inning[i] << box2;
	}
	ofs << endl;
	box();
	ofs << box2 << left << setw(22) << tA << box2;
	//***********************
	//*Team A score			*
	//***********************
	for (int i = 0; i < 12; i++)
	{
		ofs << right << setw(4) << scoreA[i] << box2;
	}
	ofs << endl;
	box();
	ofs << box2 << left << setw(22) << tB << box2;
	//***********************
	//*Team B score			*
	//***********************
	for (int i = 0; i < 12; i++)
	{
		ofs << right << setw(4) << scoreB[i] << box2;
	}
	ofs << endl;
	box();
	blankBox();

}

#pragma region PLAY_BALL
//***********************
//*	  Score Function	*
//*---------------------*
//*Determines who scored*
//*for which team		*
//***********************
void score(int i) {

	gotoxy(0, (x + 2));

	if (i < 9)
	{
		scoreA[(inNum - 1)] += 1;
		scoreA[9] += 1;
	}
	else {
		scoreB[(inNum - 1)] += 1;
		scoreB[9] += 1;
	}
	plrs[i].bp = 0;
	plrs[i].onBase = false;
	cout << plrs[i].name << left << setw(50) << pscore;
	Sleep(slp1);

}

//***********************
//*	    Hit Function	*
//*---------------------*
//*Advances the players *
//*to the correct base	*
//*after each hit		*
//***********************
void hit(int bat, int addB) {

	up = false;
	plrs[bat].onBase = true;
	plrs[bat].bp += addB;

	if (bat < 9)
	{
		scoreA[10]++;
	}
	else {
		scoreB[10]++;
	}

	for (int i = 0; i < (int)plrs.size(); i++)
	{
		if (i != bat)
		{
			if (plrs[i].onBase) {
				plrs[i].bp += addB;
			}

		}
		if (plrs[i].bp >= 4)
		{
			score(i);
		}
	}

}

//***********************
//*	   Walk Function	*
//*---------------------*
//*Advances the players *
//*to the appropriate	*
//*base after a walk (a	*
//*player does not go to*
//*the next base unless *
//*they are occupying	*
//*the next base)		*
//***********************
void walk(int bat) {

	plrs[bat].s++;
	up = false;
	plrs[bat].onBase = true;
	plrs[bat].bp++;
	for (int i = 0; i < (int)plrs.size(); i++)
	{
		if (i == bat) continue;

		if (plrs[i].bp == wb)
		{
			plrs[i].bp++;
			wb++;
			for (int j = 0; j < (int)plrs.size(); j++)
			{
				if (j == bat || j == i) continue;
				if (plrs[j].bp == wb)
				{
					plrs[j].bp++;
					wb++;
					for (int l = 0; l < (int)plrs.size(); l++)
					{
						if (l == bat || l == j || l == i) continue;
						if (plrs[l].bp == wb)
						{
							plrs[l].bp++;
						}
					}
				}
			}
		}
	}
	wb = 1;
	for (int i = 0; i < (int)plrs.size(); i++)
	{
		if (plrs[i].bp >= 4)
		{
			score(i);
		}
	}

}

//***********************
//*Reset Inning Function*
//*---------------------*
//*Resets strike and	*
//*ball variables. Also *
//*resets if the playser*
//*is up for each inning*
//***********************
void reset() {

	strike = 0;
	ball = 0;
	up = true;

}

//***********************
//*Rand Number Function *
//*---------------------*
//*Determines outcome of*
//*each pitch/at bat	*
//***********************
void r(int bat) {

	num = 1 + (rand() % 20);

	switch (num)
	{
	case 1:
	case 3:
	case 5:
	case 7:
		//***************************************
		//*Foul ball							*
		//*If foul and not strike 2, increase	*
		//*number of strikes					*
		//***************************************
		if (strike < 2)
		{
			strike += 1;
		}
		break;
	case 2:
		//***************
		//*Single		*
		//***************
		gotoxy(0, x);
		cout << plrs[bat].name << ha << left << setw(70) << hs;
		plrs[bat].s++;
		hit(bat, 1);
		Sleep(slp2);
		break;
	case 4:
		//***************
		//*Double		*
		//***************
		gotoxy(0, x);
		cout << plrs[bat].name << ha << left << setw(70) << hd;
		plrs[bat].d++;
		hit(bat, 2);
		Sleep(slp2);
		break;
	case 6:
		//***************
		//*Triple		*
		//***************
		gotoxy(0, x);
		cout << plrs[bat].name << ha << left << setw(70) << ht;
		plrs[bat].t++;
		hit(bat, 3);
		Sleep(slp2);
		break;
	case 8:
		//***************
		//*Home Run		*
		//***************
		gotoxy(0, x);
		cout << plrs[bat].name << ha << left << setw(70) << homer;
		plrs[bat].hr++;
		hit(bat, 4);
		Sleep(slp2);
		break;
	case 9:
	case 11:
	case 13:
	case 15:
	case 17:
		//***************
		//*Strike		*
		//***************
		strike += 1;
		//cout << "Strike " << left << setw(70) << strike << endl;
		break;
	case 10:
	case 12:
	case 14:
	case 16:
	case 18:
		//***************
		//*Ball			*
		//***************
		ball += 1;
		//cout << "Ball " << left << setw(70) << ball << endl;
		//***************
		//*If ball 4, go*
		//*to walk()	*
		//***************
		if (ball >= 4)
		{
			gotoxy(0, x);
			cout << plrs[bat].name << ga << left << setw(70) << hw;
			walk(bat);
			Sleep(slp2);
		}
		break;
	case 19:
		//***************
		//*Fly Out		*
		//***************
		gotoxy(0, x);
		cout << plrs[bat].name << ghr << left << setw(70) << fo;
		up = false;
		outs++;
		Sleep(slp2);
		break;
	case 20:
		//***************
		//*Error		*
		//***************
		gotoxy(0, x);
		cout << plrs[bat].name << ge << left << setw(70) << he;
		if (bat < 9)
		{
			scoreB[11]++;
		}
		else {
			scoreA[11]++;
		}
		hit(bat, 1);
		Sleep(slp2);
		break;
	default:
		break;
	}

	if (strike >= 3)
	{
		outs++;
		up = false;
		plrs[bat].so++;
		gotoxy(0, x);
		cout << plrs[bat].name << left << setw(70) << hso;
		Sleep(slp2);
	}
	Sleep(slp1);

}

//***********************
//*	   Team A at bat	*
//*---------------------*
//*This is what happens	*
//*when team A is up to *
//*bat					*
//***********************
void abA() {

	reset();
	outs = 0;
	aUp = true;
	bUp = false;
	gotoxy(10, 9);
	cout << outs;
	//***************************************
	//*Reset every player's base position	*
	//***************************************
	for (int i = 0; i < (int)plrs.size(); i++)
	{
		plrs[i].onBase = false;
		plrs[i].bp = 0;
	}
	//***************************************
	//*While there are less than 3 outs...	*
	//***************************************
	do {
		plrs[batA].ab++;
		//***************************************
		//*While the batter is still up to bat..*
		//***************************************
		do {
			sboard();
			cout << strikeStr << strike << endl;
			cout << ballStr << ball << endl;
			cout << outStr << endl << endl;
			cout << abStr << left << setw(30) << plrs[batA].name << endl;
			Sleep(slp1);
			r(batA);
		} while (up);
		if (batA == 8)
		{
			batA = 0;
		}
		else {
			batA++;
		}
		reset();
		gotoxy(10, 9);
		cout << outs;
	} while (outs < 3);

}

//***********************
//*	   Team B at bat	*
//*---------------------*
//*This is what happens	*
//*when team B is up to *
//*bat					*
//***********************
void abB() {

	reset();
	outs = 0;
	aUp = false;
	bUp = true;
	gotoxy(10, 9);
	cout << outs;
	//***************************************
	//*Reset every player's base position	*
	//***************************************
	for (int i = 0; i < (int)plrs.size(); i++)
	{
		plrs[i].onBase = false;
		plrs[i].bp = 0;
	}
	//***************************************
	//*While there are less than 3 outs...	*
	//***************************************
	do {
		plrs[batB].ab++;
		//***************************************
		//*While the batter is still up to bat..*
		//***************************************
		do {
			sboard();
			cout << strikeStr << strike << endl;
			cout << ballStr << ball << endl;
			cout << outStr << endl << endl;
			cout << abStr << left << setw(30) << plrs[batB].name << endl;
			Sleep(slp1);
			r(batB);
		} while (up);
		if (batB == 17)
		{
			batB = 9;
		}
		else {
			batB++;
		}
		reset();
		gotoxy(10, 9);
		cout << outs;
	} while (outs < 3);

}

//***********************
//*		 Play Ball		*
//*---------------------*
//*Keeps track of which *
//*team is up to bat	*
//***********************
void play(int n) {

	for (int i = (inNum - 1); i < n; i++)
	{
		cout << ins << inNum << endl << endl;
		cout << left << setw(70) << tau << endl;
		Sleep(slp2);
		abA();
		Sleep(slp2);
		system("CLS");
		cout << left << setw(70) << tbu << endl;
		Sleep(slp2);
		abB();
		Sleep(slp2);
		system("CLS");
		inNum++;
	}
	calc();
	sboard();
	gotoxy(0, 20);
	pause();
	if (inNum > 9 && (scoreA[9] == scoreB[9])) {
		isTie = true;
		system("CLS");
		cout << tie;
		scoreA[9]++;
	}
	if (inNum > 9 && !isTie) wl();

}

#pragma endregion PLAY_BALL

//***********************
//*  Win/Loss Function  *
//*---------------------*
//*Shows who won/lost	*
//*with graphics and	*
//*sound				*
//***********************
void wl() {

	system("CLS");

	if (scoreA[9] > scoreB[9]) {
		PlaySound(TEXT("sox.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		soxG();
		Sleep(2000);
		cout << endl << endl << tA << w;
	}
	else {
		cubG();
		PlaySound(TEXT("boo.wav"), NULL, SND_FILENAME);
		PlaySound(TEXT("cubs.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Sleep(2000);
		cout << endl << endl << tB << w;
	}
	pause();
	PlaySound(TEXT("troy.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	isOver = true;
	inNum = 1;
}

//***********************
//*Calculations Function*
//*---------------------*
//*Calculates the on	*
//*base and batting avg	*
//***********************
void calc() {

	for (int i = 0; i < (int)plrs.size(); i++)
	{
		avg = (double)(plrs[i].s + plrs[i].d + plrs[i].t + plrs[i].hr) / plrs[i].ab;
		plrs[i].oba = avg;
		plrs[i].bavg = avg;
	}

}

//***********************
//*	   Rules Function	*
//*---------------------*
//*Prints the rules out	*
//***********************
void rules() {

	system("CLS");

	rus();
	color(269);
	cout << key << endl;
	pause();
	gotoxy(0, 13);
	color(14);
	cout << rnd1 << rnd2 << rnd1 <<
		rnd3 << rnd1 << rnd4 << rnd1 <<
		rnd5 << rnd1 << rnd6 << rnd1 <<
		rnd7 << rnd1 << rnd8 << rnd1 <<
		rnd9 << rnd1 << rnd10 << rnd1
		<< rnd11 << rnd1;
	dCol();
	pause();

}

//***********************
//*	  Sound Function 	*
//*---------------------*
//*Determines which WAV	*
//*file to play each	*
//*inning				*
//***********************
void pls() {

	switch (inNum)
	{
	case 1:
		PlaySound(TEXT("inn1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 2:
		PlaySound(TEXT("inn2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 3:
		PlaySound(TEXT("inn3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 4:
		PlaySound(TEXT("inn4.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 5:
		PlaySound(TEXT("inn5.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 6:
		PlaySound(TEXT("inn6.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 7:
		PlaySound(TEXT("inn7.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 8:
		PlaySound(TEXT("inn8.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case 9:
		PlaySound(TEXT("inn9.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	default:
		break;
	}

}

//***********************
//*	 New Game Function 	*
//*---------------------*
//*Checks if user wants *
//*to reset the score & *
//*start a new game.	*
//***********************
void newgame() {

	do
	{
		gotoxy(0, 21);
		cout << warn;
		cin >> opt;
		if (opt == "y" || opt == "Y" || opt == "n" || opt == "N") {
		}
		else {
			opt = "";
		}

	} while (opt == "");

	if (opt == "y" || opt == "Y") {
		for (int i = 0; i < 12; i++)
		{
			scoreA[i] = 0;
			scoreB[i] = 0;
		}
		reset();
		batA = 0;
		batB = 9;
		outs = 0;
		isOver = false;
	}
}

//***************************************
//*		    Selection Validation		*
//***************************************
//*Validate the user's selection by		*
//*turning opt into an int from a string*
//***************************************
void validate( string opt ) {

	if (opt != "12345") {
		option = 0;
	}
	else {
		option = atoi(opt.c_str());
	}

}

//***********************
//*		  Back To		*
//*	   Menu Function 	*
//*---------------------*
//*Clears screen, prints*
//*baseball and menu	*
//*graphics, resets the	*
//*default cursor pos.	*
//***********************
void b2menu() {

	system("CLS");
	bbG();
	menG();
	gotoXY(1, 13);
	cout << "->";
	x2 = 13;
	menu_item = 0;

}

//***********************
//*	   Menu Function 	*
//*---------------------*
//*Prints the menu onto	*
//*the screen. See 		*
//*variable information	*
//*on lines 198-225		*
//***********************
void menu() {

	system("CLS");

	bbG();
	menG();

	gotoXY(1, 13);
	cout << "->";

	while (running) {

		gotoxy(3, 13);
		cout << one;
		gotoxy(3, 14);
		cout << two;
		gotoxy(3, 15);
		cout << three;
		gotoxy(3, 16);
		cout << four;
		gotoxy(3, 17);
		cout << five;
		gotoxy(3, 18);
		cout << six;
		gotoxy(3, 19);
		cout << seven;
		gotoxy(3, 20);
		cout << q;

		system("pause>nul"); // the >nul bit causes it the print no message

		if (GetAsyncKeyState(VK_DOWN) && x2 != 20) //down button pressed
		{
			gotoXY(1, x2); cout << "  ";
			x2++;
			gotoXY(1, x2); cout << "->";
			menu_item++;
			continue;

		}

		if (GetAsyncKeyState(VK_UP) && x2 != 13) //up button pressed
		{
			gotoXY(1, x2); cout << "  ";
			x2--;
			gotoXY(1, x2); cout << "->";
			menu_item--;
			continue;
		}

		if (GetAsyncKeyState(VK_RETURN)) { // Enter key pressed

			switch (menu_item)
			{
			case 0:
				//*******************
				//*Play 1 inning	*
				//*******************
				if (isOver)
				{
					newgame();
				}
				else {
					slp1 = 300;
					slp2 = 2000;
					system("CLS");
					if (plrs[1].name != "K-9") pls();
					play(inNum);
				}
				b2menu();
				break;
			case 1:
				//***************************************
				//*Play to the end of the current game	*
				//***************************************
				if (isOver)
				{
					newgame();
				}
				else {
					system("CLS");
					if (plrs[1].name != "K-9") PlaySound(TEXT("fastinn.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					if (inNum == 1)
					{
						cout << ng;
					}
					else {
						slp1 = 0;
						slp2 = 0;
						play(9);
					}
				}
				b2menu();
				break;
			case 2:
				//*******************
				//*Play full game	*
				//*******************
				if (isOver)
				{
					newgame();
				}
				else {
					system("CLS");
					if (plrs[1].name != "K-9") PlaySound(TEXT("fastinn.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					slp1 = 0;
					slp2 = 0;
					play(9);
				}
				b2menu();
				break;
			case 3:
				//*******************
				//*Print stats		*
				//*******************
				sout();
				b2menu();
				break;
			case 4:
				//*******************
				//*Print scoreboard	*
				//*******************
				system("CLS");
				sboard();
				pause();
				b2menu();
				break;
			case 5:
				//*******************
				//*View the rules	*
				//*******************
				rules();
				b2menu();
				break;
			case 6:
				//*******************
				//*Cheat code		*
				//*******************
				gotoxy(3, 21);
				cout << ecode;
				cin >> opt;
				validate(opt);
				if (option == 12345)
				{
					system("CLS");
					PlaySound(TEXT("spaceballs.wav"), NULL, SND_FILENAME | SND_ASYNC);
					cout << load;
					file1 = drFile;
					players();
					Sleep(11000);
					gotoxy(0, 5);
					dalek();
					PlaySound(TEXT("drwhotheme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					pause();
				}
				else {
					cout << inv;
				}
				b2menu();
				gotoxy(20, 13);
				policeBox();
				break;
			case 7:
				//*******************
				//*Quit				*
				//*******************
				running = false;
				b2menu();
				break;
			}
		}
	}
}

//***********************
//*    Open Function	*
//*---------------------*
//*Plays the opening	*
//*sound and graphics	*
//***********************
void open() {

	PlaySound(TEXT("anykey.wav"), NULL, SND_FILENAME | SND_ASYNC);
	pause();
	PlaySound(TEXT("worldseries.wav"), NULL, SND_FILENAME | SND_ASYNC);
	system("CLS");
	mlb();
	Sleep(2000);
	system("CLS");
	cubG();
	Sleep(2000);
	system("CLS");
	vsG();
	Sleep(2000);
	system("CLS");
	soxG();
	pause();
	PlaySound(TEXT("troy.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	cout << setprecision(3) << fixed;
	ofs << setprecision(3) << fixed;
	sstats << setprecision(3) << fixed;

}

//***********************
//*	  Output File for	*
//*	   player stats		*
//***********************
void ostats() {

	ofstream statcopy(file6);

	for (int i = 0; i < (int)plrs.size(); i++)
	{
		sstats << plrs[i].name << com;
		sstats << plrs[i].hr << com;
		sstats << plrs[i].so << com;
		sstats << plrs[i].s << com;
		sstats << plrs[i].d << com;
		sstats << plrs[i].t << com;
		sstats << plrs[i].ab << com;
		sstats << plrs[i].oba << com;
		sstats << plrs[i].bavg << endl;
		statcopy << plrs[i].name << com;
		statcopy << plrs[i].hr << com;
		statcopy << plrs[i].so << com;
		statcopy << plrs[i].s << com;
		statcopy << plrs[i].d << com;
		statcopy << plrs[i].t << com;
		statcopy << plrs[i].ab << com;
		statcopy << plrs[i].oba << com;
		statcopy << plrs[i].bavg << endl;
	}

}

//***********************
//*	   Output File		*
//*---------------------*
//*Creates the output	*
//*files				*
//***********************
void output() {

	if (!game2) {
		hdr();
		box();
		ofs << box2 << left << setw(82) << g1 << box2 << endl;
		pout();
		sbout();
		box();
		ftr();
		eof();
		ofs.close();
		ofs.open(file2);
		hdr();
		box();
		ofs << box2 << left << setw(82) << g1 << box2 << endl;
		pout();
		sbout();
		box();
		box();
		ofstream fc(file7);
		fc << fin2;
	}
	else {
		ofs << box2 << left << setw(82) << g2 << box2 << endl;
		pout();
		sbout();
		ftr();
		box();
		eof();
		ofstream fc(file7);
		fc << fin1;
	}

}

//***********************
//*   Close Function	*
//*---------------------*
//*Closes all open files*
//***********************
void clFiles() {

	sstats.close();
	ofs.close();
	fc.close();

}

//*******************************
//*		   Main Function		*
//*-----------------------------*
//*sets hConsole for colors		*
//*initializes random numbers	*
//*initializes file names		*
//*creates player list			*
//*plays opening graphics		*
//*calls the menu function		*
//*calls the output function	*
//*calls ostats() function		*
//*pauses on exit				*
//*closes all files				*
//*******************************
int main()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand((unsigned)time(NULL));
	oFiles();
	players();
	open();
	menu();
	output();
	ostats();
	pause();
	clFiles();
	return 0;
}