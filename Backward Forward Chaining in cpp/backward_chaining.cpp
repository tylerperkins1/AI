#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

void initVar(string[][10]);
void initConc(string[][10]);
void initClause(int clause[], string conc[][10]);
bool checkConclusion(stack<int> concStack, int clause[], string var[][10], string conc[][10]);
bool checkVariable(int num, string var[][10]);
void initVarFW(string[][5], string filename);
void initConcFW(string[][5], string filename);
void initClauseFW(int clause[], string conc[][5]);
bool checkVariableFW(int num, string var[][5]);

int main(){

    string var[3][10];
    initVar(var);

    string conc[2][10];
    initConc(conc);

    int clause[100] = {0};
    initClause(clause, conc);

    // Without this line, program will output a blank line instead of the word MEDICAL
    //      even though all other conclusion names output correctly.
    conc[0][0] = "MEDICAL";

    // create conclusion stack
    stack <int> concStack;
    for (int i = 10; i > 0; i--){
        concStack.push(i);
    }

    bool success = checkConclusion(concStack, clause, var, conc);

    /************************************************************
                        Forward Chaining
    ************************************************************/

    string varFW[3][5];
    string concFW[2][5];
    int clauseFW[50] = {0};
    stack <int> concStackFW;
    string fileVar;
    string fileConc;
    int a = 0;
    switch(a){
    case 0:
    fileVar = "med_vars.txt";
    fileConc = "med_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 1:
    fileVar = "sci_vars.txt";
    fileConc = "sci_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 2:
    fileVar = "eng_vars.txt";
    fileConc = "eng_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 3:
    fileVar = "ed_vars.txt";
    fileConc = "ed_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 4:
    fileVar = "fine_art_vars.txt";
    fileConc = "fine_art_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 5:
    fileVar = "pol_vars.txt";
    fileConc = "pol_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 6:
    fileVar = "law_vars.txt";
    fileConc = "law_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    };

    break;
    case 7:
    fileVar = "comm_vars.txt";
    fileConc = "comm_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 8:
    fileVar = "bus_vars.txt";
    fileConc = "bus_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    case 9:
    fileVar = "lib_art_vars.txt";
    fileConc = "lib_art_conc.txt";
    initVarFW(varFW, fileVar);
    initConcFW(concFW, fileConc);
    for (int i = 5; i > 0; i--){
    concStack.push(i);
    }

    break;
    }

/**output job to console **/

    return 0;
}

void initVar(string var[][10]){

    string varName;
    string node;
    ifstream file ("variables.txt");

    for (int i = 0; i < 10; i++){

        getline(file, varName);
        var[0][i] = varName;

        getline(file, node);
        var[1][i] = node;

        var[2][i] = "NI";
    }
    file.close();
}

void initConc(string conc[][10]){

    string concName;
    string path;
    ifstream file ("conclusions.txt");

    for (int i = 0; i < 10; i++){

        getline(file, concName);
        conc[0][i] = concName;

        getline(file, path);
        conc[1][i] = path;
    }
    file.close();
}

void initClause(int clause[], string conc[][10]){

    for (int i = 0; i < 10; i++){

        // Get location of each rule in clause list
        int location = (i * 10) + 1;


        // Convert path string from conclusion list to array of ints
        string pathString = conc[1][i];
        int length = pathString.length();
        int path[length] = { 0 };

        int x = 0;

        for (int y = 0; pathString[y] != '\0'; y++) {

            if (pathString[y] == ',') {
                x++;
            }
            else {
                path[x] = path[x] * 10 + (pathString[y] - 48);
                //cout << path[x];
            }
        }
        // Assign rules to clause list
        for (int j = 0; j <= length; j++){

            int ruleNum = path[j];
            clause[(j + location)] = ruleNum;
        }
    }
}

bool checkConclusion(stack<int> concStack, int clause[], string var[][10], string conc[][10]){

    int ruleNum = concStack.top();
    int varNum;
    bool success = false;

    // Get the location in clause list for the rule
    ruleNum = ((ruleNum - 1) * 10) + 1;
    int ruleLen = 0;

    // Go to end of rule to work backwards
    while (clause[ruleNum] != 0){
        ruleLen++;
        ruleNum++;
    }

    // Check each variable of rule
    for (int i = (ruleLen - 1); i > 0; i--){

        varNum = clause[i];

        // Find associated variable
        for (int i = 0; i < 10; i++){

            string node = var[1][i];
            int nodeNum = stoi(node);
            if (varNum == nodeNum){
                // Check if node is initialized and value
                success = checkVariable(i, var);
                break;
            }
        }
    }

    return true;
}

bool checkVariable(int num, string var[][10]){

    if (var[2][num] == "NI"){
        // Initialize Variable
    }
    // If variable has already been initialized
    else if(var[2][num] == "YES"){
        return true;
    }
    else if(var[2][num] == "NO"){
        return false;
    }
    return true;
}

/**************************************************
          Functions for forward chaining
**************************************************/

void initVarFW(string varFW[][5], string filename){

    string varName;
    string node;
    ifstream file (filename);

    for (int i = 0; i < 5; i++){

        getline(file, varName);
        varFW[0][i] = varName;

        getline(file, node);
        varFW[1][i] = node;

        varFW[2][i] = "NI";
    }
    file.close();
}

void initConcFW(string concFW[][5], string filename){

    string concName;
    string path;
    ifstream file (filename);

    for (int i = 0; i < 5; i++){

        getline(file, concName);
        concFW[0][i] = concName;

        getline(file, path);
        concFW[1][i] = path;
    }
    file.close();
}

void initClauseFW(int clauseFW[], string concFW[][5]){

    for (int i = 0; i < 5; i++){

        // Get location of each rule in clause list
        int location = (i * 10) + 1;


        // Convert path string from conclusion list to array of ints
        string pathString = concFW[1][i];
        int length = pathString.length();
        int path[length] = { 0 };

        int x = 0;

        for (int y = 0; pathString[y] != '\0'; y++) {

            if (pathString[y] == ',') {
                x++;
            }
            else {
                path[x] = path[x] * 10 + (pathString[y] - 48);/**change formula**/
                //cout << path[x];
            }
        }
        // Assign rules to clause list
        for (int j = 0; j <= length; j++){

            int ruleNum = path[j];
            clauseFW[(j + location)] = ruleNum;
        }
    }
}

