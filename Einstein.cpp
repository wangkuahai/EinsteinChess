#include "Einstein.h"
#include<ctime>
#include<cmath>
#include<cstdlib>
#include<vector>
using namespace std;
//a random number between 0-100
int random(){
	srand(time(0));
	rand();
	return (int)(100.0*rand() / RAND_MAX);
}


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

int get_score(State &state, Tree &tree){

}

//& for state because has int*!!!
class State {
public:
	//used for expand
	vector<Action> avail_act;//all actions can take in this state
	int chessboard[5][5];
	string color;//who is going to act
	string winer;//note the winer in one simulation
	
	State(const State& state){
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				chessboard[i][j] = state.chessboard[i][j];
			}
		}
		color = state.color;
	}
	State(int *board,string Color) {
		winer = "null";
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				chessboard[i][j] = board[i * 5 + j];
			}
		}
		color = Color;
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
	//in this state act action x
	void act(Action x){
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

		chessboard[c_y][c_x] = 0;

		if (x.direction == "up") {
			c_y--;
		}
		else if (x.direction == "left") {
			c_x--;
		}
		else if (x.direction == "leftup") {
			c_x--;
			c_y--;
		}
		else if (x.direction == "down") {
			c_y++;
		}
		else if (x.direction == "right") {
			c_x++;
		}
		else if (x.direction == "rightdown") {
			c_x++;
			c_y++;
		}

		chessboard[c_y][c_x] = x.number;
	}
};

Action UCTsearch();

//my UCT node
class Node {
public:
	State state;//now state
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
	Tree(string color, State& origin) {
		myColor = color;

	}
	Node* expand(State& s) {

	}
	int simulate(State& state){//simulate in state, back the profit
		//save
		State temp(state);
		//use the temp -chessboard and color
		while (!temp.end()){
			int my_chess[6] = { 0 };
			vector<int> gamble_number;
			if (temp.color == "blue"){
				for (int i = 0; i < 5; i++){
					for (int j = 0; j < 5; j++){
						if (temp.chessboard[i][j]>6){
							my_chess[temp.chessboard[i][j] - 7] = temp.chessboard[i][j];
						}
					}
				}
				int pre = -1;
				int cur = 0;
				for (int i = 0; i < 6; i++){
					if (my_chess[i] == 0){
						//if (cur != -1&&pre==-1){
						//	for (; pre < cur; pre++){
						//		gamble_number.push_back(my_chess[cur]);
						//		gamble_number.push_back(my_chess[cur]);
						//	}
						//}
						//else if (cur != -1){
						//	for (; pre < cur; pre++){
						//		gamble_number.push_back(my_chess[cur]);
						//		gamble_number.push_back(my_chess[cur]);
						//	}
						//}

					}
					else{
						//
						gamble_number.push_back(my_chess[i]);
						gamble_number.push_back(my_chess[i]);
						//
						for (; pre < i; pre++){

						}
					}
				}
			}
			else{
				for (int i = 0; i < 5; i++){
					for (int j = 0; j < 5; j++){
						if (temp.chessboard[i][j]>0 && temp.chessboard[i][j] <= 6){
							my_chess[temp.chessboard[i][j] - 1] = temp.chessboard[i][j];
						}
					}
				}

			}
			
		}
	}
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
