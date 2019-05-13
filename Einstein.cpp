#include "Einstein.h"
#include<cmath>
#include<cstdlib>
#include<vector>
#define cof 1.38
using namespace std;
//a random number between 0-100
int random() {
	srand(time(0));
	rand();
	return (int)(100.0*rand() / RAND_MAX);
}


string Direction_blue[3] = { "leftup","up","left" };
string Direction_red[3] = { "rightdown","down","right" };
class Action {
public:
	int number;//chess number;
	string direction;
	Action() { number = 0; }
	Action(int my_number, string my_direction) { number = my_number; direction = my_direction; };
	//return "chess|direction"
	string back_action() {
		string back = to_string(number);
		back = back + '|' + direction;
		return back;
	};
};


//& for state because has int*!!!
class State {
public:
	//used for expand
	vector<Action> avail_act;//all actions can take in this state
	int chessboard[5][5];
	string color;//who is going to act
	string winer;//note the winer in one simulation
	State() {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				this->chessboard[i][j] = 0;
			}
		}
	}
	State(const State& state) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				this->chessboard[i][j] = state.chessboard[i][j];
			}
		}
		color = state.color;
	}
	State(int board[25], string Color) {
		winer = "null";
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				this->chessboard[i][j] = board[i * 5 + j];
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
		if (chessboard[4][4] != 0 && chessboard[4][4] <= 6) {
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
	//in this state act action x,and turn the color
	void act(Action x) {
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

		//turn color 
		if (color == "blue") {
			color = "red";
		}
		else {
			color = "blue";
		}
	}
	//new the avail_act
	void get_avail_act() {
		if (color == "blue") {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					if (chessboard[i][j] > 6) {
						for (int k = 0; k < 3; k++) {
							Action a(chessboard[i][j], Direction_blue[k]);
							if (act_right(a)) {
								avail_act.push_back(a);
							}
						}
					}
				}
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					if (chessboard[i][j] > 0 && chessboard[i][j] <= 6) {
						for (int k = 0; k < 3; k++) {
							Action a(chessboard[i][j], Direction_red[k]);
							if (act_right(a)) {
								avail_act.push_back(a);
							}
						}
					}
				}
			}
		}
	}
	void get_root_avail_act(int dice) {
		int my_chess[6] = { 0 };
		if (color == "blue") {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					if (chessboard[i][j] > 6) {
						my_chess[chessboard[i][j] - 7] = chessboard[i][j];
					}
				}
			}
			if (my_chess[dice - 7] != 0) {
				for (int k = 0; k < 3; k++) {
					Action a(my_chess[dice - 7], Direction_blue[k]);
					if (act_right(a)) {
						avail_act.push_back(a);
					}
				}
			}
			else {
				vector<int> right_chess;
				//front
				for (int i = dice - 7; i >= 0; i--) {
					if (my_chess[i] != 0) {
						right_chess.push_back(my_chess[i]);
						break;
					}
				}
				//back
				for (int i = dice - 7; i <= 5; i++) {
					if (my_chess[i] != 0) {
						right_chess.push_back(my_chess[i]);
						break;
					}
				}
				for (int i = 0; i < right_chess.size(); i++) {
					for (int k = 0; k < 3; k++) {
						Action a(right_chess[i], Direction_blue[k]);
						if (act_right(a)) {
							avail_act.push_back(a);
						}
					}
				}
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					if (chessboard[i][j] > 0 && chessboard[i][j] <= 6) {
						my_chess[chessboard[i][j] - 1] = chessboard[i][j];
					}
				}
			}
			if (my_chess[dice - 1] != 0) {
				for (int k = 0; k < 3; k++) {
					Action a(my_chess[dice - 1], Direction_red[k]);
					if (act_right(a)) {
						avail_act.push_back(a);
					}
				}
			}
			else {
				vector<int> right_chess;
				//front
				for (int i = dice - 1; i >= 0; i--) {
					if (my_chess[i] != 0) {
						right_chess.push_back(my_chess[i]);
						break;
					}
				}
				//back
				for (int i = dice - 1; i <= 5; i++) {
					if (my_chess[i] != 0) {
						right_chess.push_back(my_chess[i]);
						break;
					}
				}
				for (int i = 0; i < right_chess.size(); i++) {
					for (int k = 0; k < 3; k++) {
						Action a(right_chess[i], Direction_red[k]);
						if (act_right(a)) {
							avail_act.push_back(a);
						}
					}
				}
			}

		}
	}
	//in this state, blue or red choose one chess ' probility
	double get_one_chess_probility(int chessnumber) {
		int my_chess[6] = { 0 };
		vector<int> pro_chess;
		if (color == "blue") {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					if (chessboard[i][j] > 6) {
						my_chess[chessboard[i][j] - 7] = chessboard[i][j];
					}
				}
			}
			if (my_chess[chessnumber - 7] != chessnumber) {
				cout << "wrong" << endl;
				system("pause");
				exit(-1);
			}
			else {
				int back = 0;//back tian chong
				int front = 0;//front tian chong
				//back
				for (int i = chessnumber - 8; i >= 0; i--) {
					if (my_chess[i] == 0) {
						back++;
					}
					else {
						break;
					}
					if (i == 0) {//back double!
						back *= 2;
					}
				}
				//front
				for (int i = chessnumber - 6; i <= 5; i++) {
					if (my_chess[i] == 0) {
						front++;
					}
					else {
						break;
					}
					if (i == 5) {//back double!
						front *= 2;
					}
				}
				int all = back + front+2;
				return 1.0*all / 12;
			}
		}
		else {//for red
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					if (chessboard[i][j] > 0 && chessboard[i][j] <= 6) {
						my_chess[chessboard[i][j] - 1] = chessboard[i][j];
					}
				}
			}
			if (my_chess[chessnumber - 1] != chessnumber) {
				cout << "wrong" << endl;
				system("pause");
				exit(-1);
			}
			else {
				int back = 0;//back tian chong
				int front = 0;//front tian chong
				//back
				for (int i = chessnumber - 2; i >= 0; i--) {
					if (my_chess[i] == 0) {
						back++;
					}
					else {
						break;
					}
					if (i == 0) {//back double!
						back *= 2;
					}
				}
				//front
				for (int i = chessnumber; i <= 5; i++) {
					if (my_chess[i] == 0) {
						front++;
					}
					else {
						break;
					}
					if (i == 5) {//back double!
						front *= 2;
					}
				}
				int all = back + front + 2;
				return 1.0*all / 12;
			}
		}
	}
};

//my UCT node
class Node {
public:
	State state;//now state
	Action action;//how to get here
	int total;//total number get there
	int score;//win number
	Node* parent;
	vector<Node*> child;
	Node(State& my_state, Action get_here) :state(my_state) {
		action = get_here;
		total = 0;
		score = 0;
		parent = nullptr;
	}
	bool is_end() { return state.end(); }
	Node* expand() {
		Action a = state.avail_act[child.size()];

		State temp(state);
		temp.act(a);

		Node* p = new Node(temp, a);
		//!!!
		p->parent = this;
		//!!!
		child.push_back(p);
		p->state.get_avail_act();
		return p;
	}
	Node* bestchild() {
		Node* best = nullptr;
		if (!child.empty()) {
			double temp = 0;
			auto p = child.begin();
			//double max_ucb = 1.0*(1.0*(*p)->score / (*p)->total + cof * sqrt(2 * log((*p)->parent->total) / (*p)->total));
			double max_ucb = (*p)->compute_ucb();
			best = *p;
			p++;
			for (; p != child.end(); p++) {
				//temp = 1.0*(1.0*(*p)->score / (*p)->total + cof * sqrt(2 * log((*p)->parent->total) / (*p)->total) );
				temp = (*p)->compute_ucb();
				if (temp > max_ucb) {
					max_ucb = temp;
					best = *p;
				}
			}
			return best;
		}
		else {
			system("pause");
			exit(-1);
		}
	}
	Node* bestchoice() {
		Node* best = nullptr;
		if (!child.empty()) {
			double temp = 0;
			auto p = child.begin();
			//double max_ucb = 1.0*(1.0*(*p)->score / (*p)->total + cof * sqrt(2 * log((*p)->parent->total) / (*p)->total));
			double max_ucb = (*p)->best_choice();
			best = *p;
			p++;
			for (; p != child.end(); p++) {
				//temp = 1.0*(1.0*(*p)->score / (*p)->total + cof * sqrt(2 * log((*p)->parent->total) / (*p)->total) );
				temp = (*p)->best_choice();
				if (temp > max_ucb) {
					max_ucb = temp;
					best = *p;
				}
			}
			return best;
		}
		else {
			system("pause");
			exit(-1);
		}
	}
	double compute_ucb() {
		return 1.0*parent->state.get_one_chess_probility(action.number)*score / total + cof * sqrt(2 * log(parent->total) / total);
	}
	double best_choice() {
		return 1.0*score / total + cof * sqrt(2 * log(parent->total) / total);
	}
	//back this node action's probility of its parent's all right action
};

//my UCT tree
class Tree {
	string myColor;//"blue" or "red"
	Node* root;
public:
	Tree(State& origin,int dice) {
		myColor = origin.color;
		Action a;
		root = new Node(origin, a);
		root->state.get_root_avail_act(dice);
	}
	~Tree() {
		Delete_tree(root);
	}
	void Delete_tree(Node* p) {
		for (int i = 0; i < p->child.size(); i++) {
			Delete_tree(p->child[i]);
		}
		delete p;
	}
	Node* get_root() { return root; }
	//simulate in state, back the profit
	//save
	int simulate(State& state) {


		State temp(state);
		//use the temp -chessboard and color

		while (!temp.end()) {
			int my_chess[6] = { 0 };
			vector<int> gamble_number;
			if (temp.color == "blue") {
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						if (temp.chessboard[i][j] > 6) {
							my_chess[temp.chessboard[i][j] - 7] = temp.chessboard[i][j];
						}
					}
				}
			}
			else {
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						if (temp.chessboard[i][j] > 0 && temp.chessboard[i][j] <= 6) {
							my_chess[temp.chessboard[i][j] - 1] = temp.chessboard[i][j];
						}
					}
				}
			}
			//set the probility
			for (int i = 0; i < 6; i++) {
				if (my_chess[i] == 0) {
					//
					bool back = 0;
					bool front = 0;
					int index_back = -1;
					int index_front = -1;
					//back
					for (int j = i - 1; j >= 0; j--) {
						if (my_chess[j] != 0) {
							back = 1;
							index_back = j;
							break;
						}
					}
					//front
					for (int j = i + 1; j < 6; j++) {
						if (my_chess[j] != 0) {
							front = 1;
							index_front = j;
							break;
						}
					}

					if (back&&front) {
						gamble_number.push_back(my_chess[index_back]);
						gamble_number.push_back(my_chess[index_front]);
					}
					else if (back) {
						gamble_number.push_back(my_chess[index_back]);
						gamble_number.push_back(my_chess[index_back]);
					}
					else if (front) {
						gamble_number.push_back(my_chess[index_front]);
						gamble_number.push_back(my_chess[index_front]);
					}
				}
				else {
					gamble_number.push_back(my_chess[i]);
					gamble_number.push_back(my_chess[i]);
				}
			}
			if (temp.color == "blue") {
				int move_chess = gamble_number[random() % 12];
				vector<Action> random_act;
				for (int k = 0; k < 3; k++) {
					Action a(move_chess, Direction_blue[k]);
					if (temp.act_right(a)) {
						random_act.push_back(a);
					}
				}
				temp.act(random_act[random() % random_act.size()]);
			}
			else {
				int move_chess = gamble_number[random() % 12];
				vector<Action> random_act;
				for (int k = 0; k < 3; k++) {
					Action a(move_chess, Direction_red[k]);
					if (temp.act_right(a)) {
						random_act.push_back(a);
					}
				}
				temp.act(random_act[random() % random_act.size()]);
			}
		}

		if (temp.winer == myColor) {
			return 1;
		}
		else {
			return -1;
		}
	}
	Node* treePolicy() {
		Node* p=root;
		while (!p->is_end()) {
			if (p->state.avail_act.size() > p->child.size()) {
				return p->expand();
			}
			else {
				p = p->bestchild();
			}
		}
		return p;
	}
	void backup(Node* p, int profit);
};

Action UCTsearch(State origin,int dice) {
	Tree uct(origin,dice);
	time_t start, now;
	start = clock();
	now = clock();
	int count = 0;
	while ((int)(now - start) < 4600) {
		Node* p = uct.treePolicy();
		int profit = uct.simulate(p->state);
		uct.backup(p, profit);
		now = clock();
		count++;
	}
	cout << "have searched " << uct.get_root()->total << endl;
	cout << "win rate: " <<100.0* uct.get_root()->bestchild()->score /uct.get_root()->bestchild()->total << endl;
	cout << "other win rate: " << endl;
	for (int i = 0; i < uct.get_root()->child.size(); i++) {
		cout << (uct.get_root()->child[i]->score) <<"/"<< uct.get_root()->child[i]->total << "  ";
		cout << "ucb :" << uct.get_root()->child[i]->compute_ucb();
		cout << endl;
	}
	return uct.get_root()->bestchoice()->action;
}

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
	if (parse(s)) {
		//judge win or lose!!
		//##
		//##
		//##
		
		//suppose one game

		//double x = log(3);
		string color;
		if (dice <= 6) {
			//i am red
			color = "red";
		}
		else {
			//I am blue
			color = "blue";
		}
		State origin(chessboard, color);
		//cout << "1" << endl;
		Action my_action=UCTsearch(origin,dice);
		//cout << "2" << endl;
		string action = my_action.back_action();
		cout << action << endl<<endl;
		clientsocket.sendMsg(&action[0]);
		//cout << "4" << endl;
	}
	else {
		return 0;
	}
}

void Tree::backup(Node * p, int profit)
{
	Node* temp = p;//save
	while (p != nullptr) {
		p->total++;
		if (p->state.color == myColor) {//MIN
			if (profit != 1) {
				p->score++;
			}
		}
		else {//MAX
			if (profit == 1) {
				p->score++;
			}
		}
		p = p->parent;
	}
}
 