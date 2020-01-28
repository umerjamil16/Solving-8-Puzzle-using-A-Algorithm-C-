// C++ code to demonstrate 2D vector 
#include <iostream> 
#include <vector> // for 2D vector 
#include <stdio.h> 
#include <algorithm>

using namespace std;

//For finding bar position in goal state
struct barPos {
	int x_pos, y_pos;
};

// A Node Struct to contains node child state and the net cost value
struct Node{
	//Initialize the node with the STATE, STEP_COST of the node and the calculated fvalue
		vector<vector<int> > childState;
		int fVal;
};

//Global Variables			
//Explored Set that contains all the explored states
vector<vector<vector<int>> > exploredSet;

// Heuristic based on manhattan distace metric
int heuristic1(vector<vector<int> > start, vector<vector<int> >  goal) {
	int cdist = 0;
		for (int i = 0; i < start.size(); i++) 
			for (int j = 0; j < start[i].size(); j++) 
				//now loop through goal state
				for (int k = 0; k < goal.size(); k++) 
					for (int l = 0; l < goal[k].size(); l++) 
						if (start[i][j] != 0) 
							if ((start[i][j] == goal[k][l]))
								cdist = (cdist + abs(i - k) + abs(j - l));
	return cdist;
}

// Heuristic based on the no of misplaced tiles in a given puzzle arrangements
int heuristic2(vector<vector<int> > start, vector<vector<int> >  goal) {
	//""" Calculates the different between the given puzzles """
		int temp = 0;
	for (int i = 0; i < start.size(); i++) {
		for (int j = 0; j < start[i].size(); j++) {
			if ((start[i][j] != goal[i][j]) && start[i][j] !=0){
				temp = temp + 1;
			}
		}
	}
	return temp;
}

//Check whether the chils state is equal to goal state
bool checkState(vector<vector<int> > child, vector<vector<int> >  goal) {
	//find the heuristc value
	int hu_val = heuristic1(child, goal);
	if (hu_val == 0)
		return 1;
	else
		return 0;
}

bool sortByCostVal(const Node &a, const Node &b)
{
	//sort the vectors based on fVal
	return a.fVal < b.fVal;
}

//Get the position of Bar in a given puzzle arrangement
barPos getBarPos(vector<vector<int>> vect)
{
	barPos s; // init. a struc for bar pos
	for (int i = 0; i < vect.size(); i++) {
		for (int j = 0; j < vect[i].size(); j++)
			if (vect[i][j] == 0){
				s.x_pos = i;
				s.y_pos = j;
				return s;
			}
	}

}

//To print a state in a matrix format
void printState(vector<vector<int>> vect)
{
	cout << "______"<<endl;
	for (int i = 0; i < vect.size(); i++) {
		for (int j = 0; j < vect[i].size(); j++)
			cout << vect[i][j] << " ";
		cout << endl;
	}
	cout << "______" << endl;
}

//Generate valid moves based on the position of bar in a given puzzle arrangement
vector<vector<int>> genPossibleMoves(int x, int y) {
	vector<vector<int> > possibleMoves{
										{ x, y - 1 },//left move
										{ x, y + 1 },//right move
										{ x - 1, y },//up move
										{ x + 1, y } //down move
									};
	//init a vector to contains the validMoves from the possible Moves
	vector<vector<int> > validMoves;
	for (int i = 0; i < possibleMoves.size(); i++) {
		if ((possibleMoves[i][0] <= 2 && possibleMoves[i][0] >= 0 && possibleMoves[i][1] <= 2 && possibleMoves[i][1] >= 0)) {
			validMoves.push_back({ possibleMoves[i][0], possibleMoves[i][1] });
		}
	}
	return validMoves;
}

//check if a given state of puzzle is already been explored
bool checkIfExplored(vector<vector<int> > child) {
	//loop through the explored set
	for (int i = 0; i < exploredSet.size();  i++) {
		if (child == exploredSet[i]){
			// if child is already in explored set, return 0
			return 0;
		}
	}
	//else return 1;
	return 1;
}

//Generate child nodes
vector<Node> genChildNodes(vector<vector<int>> startState, vector<vector<int>> goalState, int curSTEP_COST) {
	// init a Struct to get bar position
	barPos s;
	s = getBarPos(startState);

	int barX, barY;
	barX = s.x_pos;
	barY = s.y_pos;

	//generate valid moves based on the bar position
	vector<vector<int>> validMoves;
	validMoves = genPossibleMoves(barX, barY);

	//init a frontier
	vector<Node> frontier;

	//Child Struct, an instance of Node
	Node Child;

	//iterate through valid moves in a puzzle arrangement
	for (int i = 0; i < validMoves.size(); i++) {
		int move_x, move_y;
		move_x = validMoves[i][0];
		move_y = validMoves[i][1];

		vector<vector<int>> childSTATE = startState;

		//swap the bar position and the element on the valid move position
		int temp0, temp1;
		temp0 = childSTATE[move_x][move_y];
		temp1 = childSTATE[barX][barY];
		childSTATE[move_x][move_y] = temp1;
		childSTATE[barX][barY] = temp0;

		//Get the heuristic value of child genereated
		int hVal;
		hVal = heuristic1(childSTATE, goalState);

		//Calculate the net cost
		int fVal;
		fVal = hVal + curSTEP_COST;

		//Assign values to child stuct; an instance of Node
		Child.fVal = fVal;
		Child.childState = childSTATE;

		//Add the child in the previously init frontier
		frontier.push_back(Child);
	}

	//return the frontier
	return frontier;
}
//print arrow on console
void printArrow() {
	cout << endl;
	cout << "   |" << endl;
	cout << "   |" << endl;
	cout << "  \\|/" << endl;
}

//To get input initial state and goal state from the user
vector<vector<int> > getInput() {
	int a, b, c, d, e, f, g, h, i;

	cout << "Please the first row:";
	cin >> a >> b >> c;

	cout << "Please the second row:";
	cin >> d >> e >> f;

	cout << "Please the third row:";
	cin >> g >> h >> i;

	cout << endl;

	vector<vector<int> > inputState{ { a, b, c },
	{ d, e, f },
	{ g, h, i } };

	return inputState;
}

/*
Qazi Umer Jamil
RIME
NUST Regn No. 317920
*/
int main()
{
	// Init a frontier
	vector<Node> frontier;

	//Start State
	vector<vector<int> > startState;
		/*= { { 1, 8, 2 },
	{ 0, 4, 3 },
	{ 7, 6, 5 } };
	*/
	//Goal State
	vector<vector<int> > goalState = { { 1, 2, 3 },
	{ 4, 5, 6 },
	{ 7, 8, 0 } };
	
							
	//To track depth of tree
	int depth_count = 0;

	cout << "Welcome" << endl;
	cout << "8 Puzzle Solving using A* Algorithm" << endl<<endl;
	/*
	//to get goal state from user
	cout << "Please enter the Goal State of puzzle" << endl;
	goalState = getInput();
	cout << endl;
	*/
	//to get start state from user
	cout << "Please enter the Initial/Start State of puzzle" << endl;
	startState = getInput();
	cout << endl;
	
	cout <<"Goal State: "<< endl;
	printState(goalState);
	cout << "Start State: " << endl;
	printState(startState);
	exploredSet.push_back(startState);
	
	//Check if the entered state is a goal state
	if (checkState(startState, goalState)) {
		cout << "Entered Initial State and Goal states are same" << endl;
		cout << "Please enter a different state" << endl;
		cout << "Existing program..." << endl;
		return 0;
	}

	//Init a next start state vect
	vector<vector<int> > lastExploredState;
	lastExploredState = startState;

	while(1) {
		//increment the depth count
		depth_count = depth_count + 1;

		//Generate a frontier, based on the last explored state and the goal state
		frontier = genChildNodes(lastExploredState, goalState, depth_count);

		//sort the frontier based on the cost value
		sort(frontier.begin(), frontier.end(), sortByCostVal);

		//make a copy of lastExploreState
		bool stateFound = 0;
		for (int i = 0; i < frontier.size(); i++) {
			//cout << "HERE" << endl;
			if (checkIfExplored(frontier[i].childState)) {
				//if not, continue
				lastExploredState = frontier[i].childState;
				stateFound = 1;
				exploredSet.push_back(lastExploredState);
				break;
			}
		}

		if (stateFound == 0) {
			cout << "Puzzle not solveable" << endl;
			cout << "Odd numbers of Inversions are present in the puzzle" << endl;
			cout << "Please enter a different arrangement" << endl;
			break;
		}
		printArrow();
		printState(lastExploredState);
		cout << "Depth of tree is: " << depth_count << endl<<endl;

		if (checkState(lastExploredState, goalState)){
			cout << "Goal State Reached" << endl;
			break;
		}
	}
	return 0;
}