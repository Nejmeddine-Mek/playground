#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void BFS(int vertex, vector<vector<int>> &adj_matrix, vector<bool> &visited){
    queue<int> VerticesQueue;
    /**
     * we go simple, at each vertex, we just print it
     */
    
    visited[vertex] = true;
    cout << "Vertex " << vertex << " is now visited" << '\n';
    VerticesQueue.push(vertex);
    while(!VerticesQueue.empty()){
        vertex = VerticesQueue.front();
        VerticesQueue.pop();
        for(size_t i = 0; i < adj_matrix[vertex].size(); ++i){
            if(adj_matrix[vertex][i] == 1 && !visited[i]){
                VerticesQueue.push(i);
                visited[i] = true;
                cout << "Vertex " << i << " is now visited" << '\n';
            }
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

    BFS(0, adj_matrix, visited);

    return 0;
}