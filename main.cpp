#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

bool assign(map<string, vector<int> > &values, string square, int digit);

bool eliminate(map<string, vector<int> > &values, string square, int digit);

string findMinRemainValues(map<string, vector<int> > values, vector<string> squares);

bool checkIfSolved(map<string, vector<int> > values, vector<string> squares);

bool search(map<string, vector<int> > values, vector<string> squares);

map<string, set<string> > peers_global;
map<string, vector<vector<string> > > units_global;

vector<string> cross(vector<string> A, vector<string> B) {
    vector<string> cross;
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B.size(); j++) {
            string temp = A[i] + B[j];
            cross.push_back(temp);
        }
    }
    return cross;
}

vector<vector<string> > prepareUnitList(vector<string> rows, vector<string> cols) {
    vector<vector<string> > unitList;
    //Get all columns
    for (int i = 0; i < cols.size(); i++) {
        vector<string> tempVec;
        for (int j = 0; j < rows.size(); j++) {
            string temp = rows[j] + cols[i];
            tempVec.push_back(temp);
        }

        unitList.push_back(tempVec);
    }
    //Get all rows
    for (int i = 0; i < rows.size(); i++) {
        vector<string> tempVec;
        for (int j = 0; j < cols.size(); j++) {
            string temp = rows[i] + cols[j];
            tempVec.push_back(temp);
        }
        unitList.push_back(tempVec);
    }
    //Get all 3x3 boxes
    vector<vector<string> > rowSets = {{"A", "B", "C"},
                                       {"D", "E", "F"},
                                       {"G", "H", "I"}};
    vector<vector<string> > colSets = {{"1", "2", "3"},
                                       {"4", "5", "6"},
                                       {"7", "8", "9"}};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vector<string> tempBox = cross(rowSets[i], colSets[j]);
            unitList.push_back(tempBox);
        }

    }

    return unitList;
}

map<string, vector<vector<string> > > prepareUnits(vector<string> squares,
                                                   vector<vector<string> > unitList) {
    map<string, vector<vector<string> > > unit;
    int count = 0;
    while (count < squares.size()) {
        for (int i = 0; i < unitList.size(); i++) {
            for (int j = 0; j < unitList[i].size(); j++) {
                if (squares[count] == unitList[i][j]) {
                    unit[squares[count]].push_back(unitList[i]);
                    break;
                }
            }
        }
        count++;
    }
    return unit;
};

map<string, set<string> > preparePeers(vector<string> squares,
                                       map<string, vector<vector<string> > > units) {
    map<string, set<string> > peers;
    for (int i = 0; i < squares.size(); i++) {
        vector<vector<string> > peerUnits = units[squares[i]];
        //        set tempSet = peers[squares[i]];
        for (int j = 0; j < peerUnits.size(); j++) {
            for (int k = 0; k < peerUnits[j].size(); k++) {
                if (squares[i] != peerUnits[j][k]) {
                    peers[squares[i]].insert(peerUnits[j][k]);
                }
            }
        }
    }
    return peers;
};


void setGridValues(map<string, vector<int> > &values, string inputGrid,
                   vector<string> squares) {
    for (int i = 0; i < squares.size(); i++) {
        if ((inputGrid[i] == '.') || (inputGrid[i] == '0')) {
            for (int j = 1; j < 10; j++) {
                values[squares[i]].push_back(j);
            }
        } else {
            int temp = 999;
            temp = inputGrid[i] - '0';
            values[squares[i]].push_back(temp);
        }
    }
}

void displayGrid(map<string, vector<int> > values, vector<string> squares) {
    int count = 0;
    for (int i = 0; i < squares.size(); i++) {
        vector<int> temp;
        temp = values[squares[i]];
        for (int j = 0; j < temp.size(); j++) {
            cout << temp[j];
        }
        count++;
        cout << " | ";
        if (count % 9 == 0) {
            cout << "\n";
            cout << "-------------------------------------------\n";
        }
    }
}

void print_unitlist(string square) {
    cout << "Printing units of square " << square << endl;
    for (int i = 0; i < units_global[square].size(); i++) {
        for (int j = 0; j < units_global[square][i].size(); j++)
            cout << units_global[square][i][j] << " ";
    }
    cout << endl;
}

void print_peers(string square) {
    cout << "Printing peers of square " << square << endl;
    for (set<string>::iterator it = peers_global[square].begin(); it != peers_global[square].end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

}

int main() {
    vector<string> digits = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    vector<string> rows = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
    vector<string> cols = digits;
    vector<string> squares = cross(rows, cols);
    vector<vector<string> > unitList = prepareUnitList(rows, cols);
    //map <string, vector< vector<string> > > units;
    //map <string, set<string> > peers;

    units_global = prepareUnits(squares, unitList);
    peers_global = preparePeers(squares, units_global);

    string inputGrid;

    string empty_grid = ".................................................................................";


    int totalproblems = 0, solvedproblems = 0;

    while (getline(cin, inputGrid)) {
        map<string, vector<int> > values;
        map<string, vector<int> > empty_map;

        setGridValues(values, inputGrid, squares);
        setGridValues(empty_map, empty_grid, squares);

        totalproblems++;

        char temp;
        bool result = true;

        //for each square which has 1 fixed value
        for (int i = 0; i < squares.size(); i++) {
            if (values[squares[i]].size() == 1) {
                // attempt to assign that value to that square
                bool x = assign(empty_map, squares[i], values[squares[i]][0]);
                //cout << "Value of assign for square " << squares[i] << " is: " << x << endl;
                if (x == false) {
                    // if false, problem is incorrect
                    cout << "No Solution" << endl;
                }
            }
        }

        for (int i = 0; i < squares.size(); i++) {
            if (empty_map[squares[i]].size() != 1) {
                result = false;
                break;
            }
        }

        displayGrid(empty_map, squares);

        if (result) {
            solvedproblems++;
            cout << "Problem Solved" << endl;
            cout << "===================================================\n\n" << endl;
        } else {
            cout << "Problem Unsolved" << endl;
            cout << "===================================================\n\n" << endl;
        }

    }
    if (cin.eof()) {
        cout << "\n\n=====================================================" << endl;
        cout << "Solved Problems " << solvedproblems << endl;
        cout << "Total Problems " << totalproblems << endl;
    }
    return 0;
}

void print_vec(vector<int> x) {
    cout << "Size of vector is " << x.size() << endl;
    for (int i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }
    cout << endl;
}

bool assign(map<string, vector<int> > &values, string square, int digit) {
    //find required digit and exclude from search but do not delete from map yet
    vector<int> other_values;
    //create a copy first
    for (int i = 0; i < values[square].size(); i++)
        other_values.push_back(values[square][i]);

    other_values.erase(remove(other_values.begin(), other_values.end(), digit), other_values.end());

    for (int i = 0; i < other_values.size(); i++) {
        // if at least one eliminate for all i in other_values[i] is false, then return false
        if (eliminate(values, square, other_values[i]) == false) {
            //values = altered_values;
            return false;
        }

    }
    //else if all are true return true;
    return true;

}

bool eliminate(map<string, vector<int> > &values, string square, int digit) {
    vector<string> digit_places;

    // if digit is not in values[square]
    if (find(values[square].begin(), values[square].end(), digit) == values[square].end()) {
        //already eliminated
        return true;
    }

    // else: now perform actual deletion of digit from map
    values[square].erase(remove(values[square].begin(), values[square].end(), digit), values[square].end());

    //if values[square] is empty
    if (values[square].size() == 0)
        return false;
        // else if values[square] has exactly 1 element
    else if (values[square].size() == 1) {
        for (set<string>::iterator it = peers_global[square].begin(); it != peers_global[square].end(); it++) {
            // if even one eliminate(peers(s)) is false, then return false
            if (eliminate(values, *it, values[square][0]) == false)
                return false;
        }
    }

    // else proceed with elimination
    // check this part:

    for (int i = 0; i < units_global[square].size(); i++) {
        vector<string> temp;
        vector<string> array = units_global[square][i];
        for (int j = 0; j < array.size(); j++) {
            if (find(values[array[j]].begin(), values[array[j]].end(), digit) != values[array[j]].end())
                temp.push_back(array[j]);
        }
        digit_places = temp;
        if (digit_places.size() == 0)
            return false;
        else if (digit_places.size() == 1) {
            if (assign(values, digit_places[0], digit) == false)
                return false;
        }

    }

    return true;

}

string findMinRemainValues(map<string, vector<int> > values, vector<string> squares) {
    int minVectorSize = 100;
    int minVec;
    for (int i = 0; i < squares.size(); i++) {
        if (minVectorSize > values[squares[i]].size()) {
            minVectorSize = values[squares[i]].size();
            minVec = i;
        } else {
            continue;
        }
    }
    return squares[minVec];
}

bool checkIfSolved(map<string, vector<int> > values, vector<string> squares) {
    for (int i = 0; i < squares.size(); i++) {
        if (values[squares[i]].size() == 1) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

bool search(map<string, vector<int> > values, vector<string> squares) {
    if (checkIfSolved(values, squares)) {
        return true;
    }
}
