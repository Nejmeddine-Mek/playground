#include <iostream>
#include <vector>

using namespace std;

void DFS(int vertex, vector<vector<int>> &adj_matrix, vector<bool> &visited){
    /**
     * we go simple, at each vertex, we just print it
     */
    cout << "vertex " << vertex << " is being visited now" << '\n';
    visited[vertex] = true;

    for(size_t i = 0; i < adj_matrix[vertex].size(); ++i){
        if(adj_matrix[vertex][i] == 1 && !visited[i]){
            visited[i] = true;
            DFS(i, adj_matrix, visited);
        }
            
    }

}

int main(int argc, [[maybe_unused]]char **argv){
// 6-vertex undirected graph (0 through 5)
    // Structure:
    //     0 --- 1 --- 3
    //     |     |
    //     2 --- 4 --- 5
    vector<vector<int>> adj_matrix = {
        // 0  1  2  3  4  5
        {  0, 1, 1, 0, 0, 0 }, // 0 connects to 1, 2
        {  1, 0, 0, 1, 1, 0 }, // 1 connects to 0, 3, 4
        {  1, 0, 0, 0, 1, 0 }, // 2 connects to 0, 4
        {  0, 1, 0, 0, 0, 0 }, // 3 connects to 1
        {  0, 1, 1, 0, 0, 1 }, // 4 connects to 1, 2, 5
        {  0, 0, 0, 0, 1, 0 }  // 5 connects to 4
    };

    vector<bool> visited(adj_matrix.size(), false);

    DFS(0, adj_matrix, visited);

    return 0;
}