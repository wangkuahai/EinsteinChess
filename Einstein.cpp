#include "Einstein.h"
#include<cmath>
#include<vector>
using namespace std;

//0 up  1 left  2 leftup
string right_action_for_blue[3][4][4];
string right_action_for_red[3][4][4];

string Direction_blue[3] = { "up","left","leftup" };
string Direction_red[3] = { "down","right","rightdown" };
class Action {
public:
	int number;//chess number;
	string direction;
	Action() { number = 0; }
	Action(int my_number, string my_direction) { number = my_number; direction = my_direction; };
	//return "chess|direction"
	string back_action() {
		string back = to_string(number);
		back = back + "|" + direction;
		return back;
	};
};

class State {
public:
	vector<Action> avail_act;//all actions can take in this state
	string winer;
	int chessboard[5][5];
	string color;//who is going to act   0 red 1 blue
	
	State(int *board) {
		winer = "null";
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				chessboard[i][j] = board[i * 5 + j];
			}
		}
		
	}

	//game over ; set the winer
	bool end() {
		if (chessboard[0][0] >= 7) {
			winer = "blue";
			return true;
		}
		if (chessboard[4][4] != 0&&chessboard[4][4]<=6) {
			winer = "red";
			return true;
		}
		bool red_lose = 1;
		bool blue_lose = 1;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				//have blue
				if (chessboard[i][j] >= 7) {
					blue_lose = 0;
				}//have red
				else if (chessboard[i][j] != 0) {
					red_lose = 0;
				}

				if (!red_lose && !blue_lose) {
					return false;
				}
			}
		}
		if (blue_lose) {
			winer = "red";
			return true;
		}
		if (red_lose) {
			winer = "blue";
			return true;
		}
	}
	//weather action x is right in this state
	bool act_right(Action x) {
		int c_x, c_y;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (chessboard[i][j] == x.number) {
					c_x = j;
					c_y = i;
					break;
				}
			}
		}
		if (x.direction == "up") {
			if (c_y >= 1) {
				return true;
			}
		}
		else if (x.direction == "left") {
			if (c_x >= 1) {
				return true;
			}
		}
		else if (x.direction == "leftup") {
			if (c_x >= 1 && c_y >= 1) {
				return true;
			}
		}
		else if (x.direction == "down") {
			if (c_y <= 3) {
				return true;
			}
		}
		else if (x.direction == "right") {
			if (c_x <= 3) {
				return true;
			}
		}
		else if (x.direction == "rightdown") {
			if (c_x <= 3 && c_y <= 3) {
				return true;
			}
		}
		return false;
	}
};

Action UCTsearch();

//my UCT node
class Node {
public:
	Action action;//how to get here
	int total;//total number get there
	int score;//win number
	Node* parent;
	vector<Node*> child;

};

//my UCT tree
class Tree {
	string myColor;//"blue" or "red"
	Node* root;
public:
	Tree(string color, State origin) {
		myColor = color;

	}
	Node* expand(State s) {

	}
	void nextnode();
};



Einstein::Einstein()
{
	this->clientsocket.connectServer();
	clientsocket.sendMsg(ID);
}

Einstein::~Einstein()
{
	this->clientsocket.close();
}

int Einstein::parse(std::string s)
{
	if (s == "close") {
		return 0;
	}
	else {
		char temp[3] = { '\0' };
		int index = 0;
		for (int i = 0; i < 26;) {//for 5
			if (i == 25) {
				int y = 0;
			}
			if (s[index] == '0') {
				chessboard[i] = 0;
				i++;
			}
			else if (s[index] > '0'&&s[index] <= '9') {
				temp[0] = s[index];
				index++;
				if (s[index] >= '0'&&s[index] <= '9') {//** the number >= 10
					temp[1] = s[index];
					index++;

					if (i < 25) {
						chessboard[i] = (temp[0] - '0') * 10 + temp[1] - '0';
					}
					else {
						dice = (temp[0] - '0') * 10 + temp[1] - '0';
					}
				}
				else {//number <= 9
					temp[1] = '\0';

					if (i < 25) {
						chessboard[i] = temp[0] - '0';
					}
					else {
						dice = temp[0] - '0';
					}
				}

				i++;//one number
			}
			index++;
		}
		return 1;
	}
}

int Einstein::handle()
{
	while (clientsocket.recvMsg());
	string s(clientsocket.getRecvMsg());
	if (parse) {
		//judge win or lose!!
		//##
		//##
		//##
		
		//suppose one game
		
		//double x = log(3);
		if (dice <= 6) {
			//i am red
			
		}
		else {
			//I am blue

		}
	}
	else {
		return 0;
	}
}
