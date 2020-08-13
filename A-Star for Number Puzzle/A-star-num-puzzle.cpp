#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Node{
    int puzzle[9] = {0,0,0,0,0,0,0,0,0};
    double f = 999;
    double g = 999;
    double h = 999;
    int parent = 0;
};

void display(int[]);
bool moveTile(int[], int);
bool checkGoal(int[]);
void aStar(int[]);
int smallestF(Node[], int, double);
bool genSuccessors(Node[], Node[], int);
bool checkLowestF(Node&, Node[], Node[]);
double calcH(Node&);
int calcDistance(int, int, int);
int searchList(Node&, Node[]);

int heuristic = 0;
int nodesGenerated = 0;
int nodesExpanded = 0;

int main()
{
    // 0 = free space
    int puzzle1[9] = {2, 8, 3, 1, 6, 4, 0, 7, 5}; //initial state 1
    int puzzle2[9] = {2, 1, 6, 4, 0, 8, 7, 5, 3}; // initial state 2

    int sel = 0;
    bool complete = 0;
    bool valid = 0;

    bool validInput = 0;
    while (!validInput){
        cout << "1) h1\n" << "2) h2\n" << "3) Euclidean Distance\n" << "Select Heuristic Function: ";
        cin >> heuristic;
        if (heuristic > 0 && heuristic < 4){
            validInput = 1;
        }
        else{
            cout << "Invalid Input\n\n";
        }
    }

    int input;
    validInput = 0;
    while (!validInput){
        cout << "\n1) Initial Position 1\n" << "2) Initial Position 2\n" << "Select Initial Position: ";
        cin >> input;
        if (input == 1){
            validInput = 1;
            aStar(puzzle1);
        }
        if (input == 2){
            validInput = 1;
            aStar(puzzle2);
        }
        if (input != 1 && input != 2){
            cout << "Invalid Input\n\n";
        }
    }
}

// Displays puzzle
void display(int puzzle[]){

    cout << "\n------------------------------------------\n"
       << "Node selected from open list: \n\n" << puzzle[0] << " " << puzzle[1] << " " << puzzle[2] << "\n"
       << puzzle[3] << " " << puzzle[4] << " " << puzzle[5] << "\n"
       << puzzle[6] << " " << puzzle[7] << " " << puzzle[8] << "\n\n";
}

// Moves free space tile in given direction and determines if move is valid
bool moveTile(int puzzle[], int direction){

    // Get position of free tile
    int pos = 0;
    while (puzzle[pos] != 0){
        pos += 1;
    }

    bool valid = 0;
    int nextTile = 0;

    switch(direction){
        // UP
    case 0:
        nextTile = pos - 3;
        if (nextTile >= 0){
            int num = puzzle[nextTile];
            puzzle[nextTile] = 0;
            puzzle[pos] = num;
            valid = 1;
        }
        break;
        // DOWN
    case 1:
        nextTile = pos + 3;
        if (nextTile <= 8){
            int num = puzzle[nextTile];
            puzzle[nextTile] = 0;
            puzzle[pos] = num;
            valid = 1;
        }
        break;
        // LEFT
    case 2:
        if (pos != 0 && pos != 3 && pos != 6){
            nextTile = pos - 1;
            int num = puzzle[nextTile];
            puzzle[nextTile] = 0;
            puzzle[pos] = num;
             valid = 1;
        }
        break;
        // RIGHT
    case 3:
        if (pos != 2 && pos != 5 && pos != 8){
            nextTile = pos + 1;
            int num = puzzle[nextTile];
            puzzle[nextTile] = 0;
            puzzle[pos] = num;
            valid = 1;
        }
        break;
    }
    return valid;
}

// Check if given configuration is the goal state
bool checkGoal(int puzzle[]){

    bool flag = 1;

    int goalState[9] = {1, 2, 3, 8, 0, 4, 7, 6 ,5};

    for (int i = 0; i < 9; i++){
        if (puzzle[i] != goalState[i]){
            flag = 0;
        }
    }
    if (flag){
        cout << "\n**Goal State Found**";
        display(puzzle);
        cout << "********************\n";
    }
    return flag;
}

// Driver function for A* search
void aStar(int initPuzzle[]){

    // initialize open/close lists and first node with initial configuration
    int listSize = 400;
    Node open[listSize];
    Node close[listSize];
    int depth = 0;

    for (int i = 0; i < 9; i++){
        open[0].puzzle[i] = initPuzzle[i];
    }
    open[0].parent = -1;
    open[0].f = 0;
    open[0].g = 0;

    int x = 0;
    double prev = 999;
    Node emptyNode;
    bool goalFound = 0;

    auto start = high_resolution_clock::now();

    // Main loop, exits when a goal state is found
    while(!goalFound){

        int nodeVal = smallestF(open, listSize, prev);
        // If open list is empty, search failed, break loop
        if (nodeVal == 0 && open[nodeVal].f != 0){
            cout << "\n--Open list empty, search failed--\n";
            break;
        }

        goalFound = genSuccessors(open, close, nodeVal);
        nodesExpanded += 1;

        // Records max depth
        if (open[nodeVal].g >= depth)
            depth = open[nodeVal].g;

        // Moves selected node to close list
        if (!goalFound){
            prev = open[nodeVal].f;
            close[x] = open[nodeVal];
            open[nodeVal] = emptyNode;
            x++;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    if (heuristic == 1)
        cout << "\nHeuristic Function: " << "h1\n";
    if (heuristic == 2)
        cout << "\nHeuristic Function: " << "h2\n";
    if (heuristic == 3)
        cout << "\nHeuristic Function: " << "Euclidean\n";
    cout << setprecision(4);
    cout << "Execution Time: " << duration.count() << " ms" << endl;
    cout << "Nodes Generated: " << nodesGenerated << endl;
    cout << "Nodes Expanded: " << nodesExpanded << endl;
    cout << "Depth of Tree: " << depth << endl;
    cout << "Effective Branching Factor: " << (nodesGenerated / depth) << endl;

}
// Finds next node
int smallestF(Node open[], int listSize, double prev){

    double smallest = prev;
    int num = 0;
    bool flag = 0;

    // First searches for f value smaller than previous
    for (int i = 0; i < listSize; i++){
        if (open[i].f < smallest){
            smallest = open[i].f;
            num = i;
            flag = 1;
        }
        // In case of same f value, choose one with smaller h
        if (open[i].f == smallest && open[i].h < open[num].h){
            smallest = open[i].f;
            num = i;
            flag = 1;
        }
        // If the 2 best nodes have the same h and f, restart search just looking for smallest h
        if (open[i].f == smallest && open[i].h == open[num].h){
            flag = 0;
        }
    }
    if (flag == 0){
        smallest = 900;
        for (int i = 0; i < listSize; i++){
            if (open[i].h <= smallest){
                smallest = open[i].h;
                num = i;
            }
        }
    }
    return num;
}

// Generates successors for a given node
bool genSuccessors(Node open[], Node close[], int parentVal){

    display(open[parentVal].puzzle);
    cout << setprecision(4);
    cout << "Values for this node: \n";
    cout << "F: " << open[parentVal].f << "\n";
    cout << "G: " << open[parentVal].g << "\n";
    cout << "H: " << open[parentVal].h << "\n";
    bool goalFound = 0;
    int newVal = parentVal;

    // For each generated successor:
    for (int i = 0; i < 4; i++){
        newVal += 1;
        Node newNode;

        for (int j = 0; j < 9; j++){
            newNode.puzzle[j] = open[parentVal].puzzle[j];
        }
        //Check for goal state
        bool valid = moveTile(newNode.puzzle, i);
        if (valid){
            nodesGenerated += 1;
            goalFound = checkGoal(newNode.puzzle);
                if (goalFound){
                    break;
                }
            // Set values
            newNode.parent = parentVal;
            newNode.g = open[parentVal].g + 1;
            newNode.h = calcH(newNode);
            newNode.f = newNode.g + newNode.h;

            // Check if successor has lowest f val of all successors and if it is a duplicate of an existing node
            bool isLowest = checkLowestF(newNode, open, close);
            int openDuplicate = searchList(newNode, open);
            int closeDuplicate = searchList(newNode, close);

            // if lowest f and not a duplicate, add to open list
            if (isLowest && openDuplicate == -1 && closeDuplicate == -1){
                open[newVal] = newNode;
            }
            else
                newVal -= 1;
        }
        if (!valid)
            newVal -= 1;
    }

    return goalFound;
}

// Check if given node has the lowest f value of all nodes with the same parent
bool checkLowestF(Node &newNode, Node open[], Node close[]){

    bool isLowest = 1;

    for (int i = 0; i < 400; i++){
        if ((newNode.parent == open[i].parent) && newNode.f > open[i].f){
            isLowest = 0;
        }
        if ((newNode.parent == open[i].parent) && newNode.f == open[i].f && newNode.h > open[i].h){
            isLowest = 0;
        }
    }
    for (int i = 0; i < 400; i++){
        if ((newNode.parent == close[i].parent) && newNode.f > close[i].f){
            isLowest = 0;
        }
        if ((newNode.parent == close[i].parent) && newNode.f == close[i].f && newNode.h > close[i].h){
            isLowest = 0;
        }
    }
    return isLowest;
}

// Calculates h value for given node
double calcH(Node &newNode){

    int goalState[9] = {1, 2, 3, 8, 0, 4, 7, 6 ,5};
    double h = 0;

    // h1
    if (heuristic == 1){
        for (int i = 0; i < 9; i++){
            if (newNode.puzzle[i] != goalState[i])
                h += 1;
        }
    }

    // h2
    if (heuristic == 2){
        for (int i = 0; i < 9; i++){
            if (newNode.puzzle[i] != goalState[i]){
                int distance = calcDistance(newNode.puzzle[i], i, 0);
                h += distance;
            }
        }
    }

    // Euclidean distance
    if (heuristic == 3){
        for (int i = 0; i < 9; i++){
            if (newNode.puzzle[i] != goalState[i]){
                int distance = calcDistance(newNode.puzzle[i], i, 1);
                h += distance;
            }
        }
    }

    // Multiplying h by 1.01 gives h more weight which can help resolve ties in f value
    return (h *= 1.01);
}

// Calculates distance of a given tile from its goal state
int calcDistance(int num, int loc, int sel){

    int goalState[9] = {1, 2, 3, 8, 0, 4, 7, 6 ,5};
    double distance = 0;
    double x1, x2, y1, y2;

    // Gets coordinates of a given tile
    if (loc == 0 || loc == 3 || loc == 6)
        x1 = 1;
    if (loc == 1 || loc == 4 || loc == 7)
        x1 = 2;
    if (loc == 2 || loc == 5 || loc == 8)
        x1 = 3;

    if (loc == 6 || loc == 7 || loc == 8)
        y1 = 1;
    if (loc == 3 || loc == 4 || loc == 5)
        y1 = 2;
    if (loc == 0 || loc == 1 || loc == 2)
        y1 = 3;

    for (int i = 0; i < 9; i++){
        if (num == goalState[i])
           loc = i;
    }

    if (loc == 0 || loc == 3 || loc == 6)
        x2 = 1;
    if (loc == 1 || loc == 4 || loc == 7)
        x2 = 2;
    if (loc == 2 || loc == 5 || loc == 8)
        x2 = 3;

    if (loc == 6 || loc == 7 || loc == 8)
        y2 = 1;
    if (loc == 3 || loc == 4 || loc == 5)
        y2 = 2;
    if (loc == 0 || loc == 1 || loc == 2)
        y2 = 3;

    // For h2
    if (sel == 0)
        distance = abs(x1 - x2) + abs(y1 - y2);
    // For Euclidean
    if (sel == 1)
        distance = sqrt(pow((x1 - x2), 2.0) + pow((y1 - y2), 2.0));

    return distance;
}

// Search for duplicate of given node
int searchList(Node &newNode, Node lst[]){

    for (int i = 0; i < 400; i++){
        int dup = 0;
        for (int j = 0; j < 9; j++){
            if (newNode.puzzle[j] == lst[i].puzzle[j]){
                dup += 1;
            }
        }
        if (dup == 9 &&(newNode.g > lst[i].g)){
            return i;
        }
        if (dup == 9 &&(newNode.g < lst[i].g)){
            return -2;
        }
    }

    return -1;
}



