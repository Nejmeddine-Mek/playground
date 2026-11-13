#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

// Min-heap storing {f_cost, vertex_id}
using MinHeap = priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>;

vector<int> A_star(int start, int end, vector<vector<int>> &adj_matrix, vector<int> &T_star){
    MinHeap pq;
    int n = adj_matrix.size();
    vector<int> best_g(n, INT_MAX); // Cheapest actual cost g(v) found so far for each vertex
    vector<int> parent(n, -1);      // Tracks the optimal predecessor for path reconstruction
    int best_cost = INT_MAX;

    best_g[start] = 0;
    pq.push({T_star[start], start}); // f = 0 + T*(start)
    while (!pq.empty()) {
        int current_f = pq.top().first;
        int current = pq.top().second;
        pq.pop();
        // Goal reached: first pop of 'end' is guaranteed optimal when T* is admissible
        if (current == end) {
            vector<int> path;
            for (int v = end; v != -1; v = parent[v]) {
                path.push_back(v);
            }
            reverse(path.begin(), path.end());
            return path;
        }
        // Lazy deletion check: skip stale heap entries if a cheaper route to 'current' already ran
        if (current_f > best_g[current] + T_star[current]) continue;
        for (int next = 0; next < n; ++next) {
            int weight = adj_matrix[current][next];
            if (weight <= 0) continue;

            int new_g = best_g[current] + weight;
            // A* PRUNING: Only accept if this path is strictly cheaper to reach 'next'
            if (new_g < best_g[next]) {
                best_g[next] = new_g;
                parent[next] = current;
                int new_f = new_g + T_star[next];
                pq.push({new_f, next});
            }
        }
    }

    return {};
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
    vector<int> resultPath = A_star(0, 5, adj_matrix, estimationFunction);

    cout << "Traversal order: ";
    for(size_t i = 0; i < resultPath.size();++i){
        cout << resultPath.at(i);
        if(!(i + 1 == resultPath.size())) cout << " -> ";
    }
    cout << '\n';
    return 0;
}