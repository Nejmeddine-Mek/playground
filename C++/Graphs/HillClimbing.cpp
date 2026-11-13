#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

queue<int> HillClimbing(int start, int end, vector<vector<int>> &adj_matrix, vector<bool> &visited, vector<int> &estimationFunction){
    queue<int> path;
    if(start == end)
    return path;
    stack<int> nextNeighbors;
    bool found = false;
    visited[start] = true;
    nextNeighbors.push(start);
    while(!nextNeighbors.empty() && !found){
        int current = nextNeighbors.top();
        nextNeighbors.pop();
        /*add to the current path*/
        path.push(current);
        if(current != end){
            if(!visited[current])
                visited[current] = true;
                
            vector<int> neighbors;
            for (int i = 0; i < (int)adj_matrix[current].size(); ++i) {
                if (adj_matrix[current][i] > 0 && !visited[i]) {
                    neighbors.push_back(i);
                }
            }

            // 2. Sort in DECREASING order of heuristic estimation
            // (Worst heuristic goes in first, BEST/lowest heuristic goes on TOP of stack)
            sort(neighbors.begin(), neighbors.end(), [&](int a, int b) {
                return estimationFunction[a] > estimationFunction[b];
            });

            // 3. Push onto stack
            for (int nb : neighbors) {
                visited[nb] = true; // Mark visited on push so duplicates don't pile up in stack
                nextNeighbors.push(nb);
            }
        } else {
            found = true;
        }
    }

    return path;
}

int main(int argc, [[maybe_unused]]char **argv){
// 6-vertex weighted undirected graph (0 = no edge)
    // Start = 0, Goal = 5
    //
    //        (4)       (10)
    //    0 ------- 1 -------- 3   <-- Dead end with tempting heuristic!
    //    |         |
    // (2)|      (3)|
    //    |         |    (5)
    //    2 ------- 4 -------- 5 (Goal)
    //        (6)
    vector<vector<int>> adj_matrix = {
        // 0   1   2   3   4   5
        {  0,  4,  2,  0,  0,  0 }, // 0: edges to 1 (w=4), 2 (w=2)
        {  4,  0,  0, 10,  3,  0 }, // 1: edges to 0 (w=4), 3 (w=10), 4 (w=3)
        {  2,  0,  0,  0,  6,  0 }, // 2: edges to 0 (w=2), 4 (w=6)
        {  0, 10,  0,  0,  0,  0 }, // 3: dead end connected only to 1 (w=10)
        {  0,  3,  6,  0,  0,  5 }, // 4: edges to 1 (w=3), 2 (w=6), 5 (w=5)
        {  0,  0,  0,  0,  5,  0 }  // 5: Goal, connected to 4 (w=5)
    };

    // Heuristic h(n): estimated distance from node n to Goal (5)
    // Node 3 looks super promising (h=2), tricking Hill Climbing into a dead end!
    vector<int> estimationFunction = {
        10, // node 0
        6,  // node 1
        8,  // node 2
        2,  // node 3 (DECOY: closer heuristic than node 4, but dead end!)
        4,  // node 4
        0   // node 5 (Goal)
    };

    vector<bool> visited(adj_matrix.size(), false);
    queue<int> resultPath = HillClimbing(0, 5, adj_matrix, visited, estimationFunction);

    cout << "Traversal order: ";
    while (!resultPath.empty()) {
        cout << resultPath.front();
        resultPath.pop();
        if (!resultPath.empty()) cout << " -> ";
    }
    cout << '\n';
    return 0;
}