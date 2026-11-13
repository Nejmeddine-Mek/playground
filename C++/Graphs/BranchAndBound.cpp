#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

struct PathState {
    int f;                  // f = g + T*(last_node)
    int g;                  // Total edge weight accumulated so far
    vector<int> nodes;      // The partial path, e.g., {0, 1, 4}

    // Min-heap comparator: smallest 'f' gets popped first
    bool operator>(const PathState &other) const {
        return f > other.f;
    }
};


vector<int> BranchAndBound(int start, int end, vector<vector<int>> &adj_matrix, vector<int> &estimationFunction){
    priority_queue<PathState, vector<PathState>, greater<PathState>> pq;
    pq.push({estimationFunction[start], 0, {start}});
    int best_cost = INT_MAX;

    vector<int> best_path;

    bool found = false;

    while(!pq.empty() && !found){
        PathState current = pq.top();
        pq.pop();
        if(current.f >= best_cost) continue;
        
        int last_node = current.nodes.back();
        if(last_node == end){
            best_cost = current.g;
            best_path = current.nodes;
            continue; // Keep checking remaining queue items just in case T* wasn't admissible, or break if admissible
        }
        for (int next = 0; next < (int)adj_matrix[last_node].size(); ++next) {
            int weight = adj_matrix[last_node][next];
            if(weight > 0){
                bool already_in_path = find(current.nodes.begin(), current.nodes.end(), next) != current.nodes.end();
                if(!already_in_path){
                    int new_g = current.g + weight;
                    int new_f = new_g + estimationFunction[next];

                    if (new_f < best_cost) {
                        vector<int> new_path = current.nodes;
                        new_path.push_back(next);
                        pq.push({new_f, new_g, new_path});
                    }
                }
            }
        }

    }



    return best_path;
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
    vector<int> resultPath = BranchAndBound(0, 5, adj_matrix, estimationFunction);

    cout << "Traversal order: ";
    for(size_t i = 0; i < resultPath.size();++i){
        cout << resultPath.at(i);
        if(!(i + 1 == resultPath.size())) cout << " -> ";
    }
    cout << '\n';
    return 0;
}