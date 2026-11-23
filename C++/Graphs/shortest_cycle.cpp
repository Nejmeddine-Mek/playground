
#include <climits>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef struct{
    int start_node;
    int u;          // u and v are the nnodes where the cycle closes
    int v;          //
    size_t length;
    std::vector<int> cycle_path;
}cycle_t;

cycle_t shortest_cycle_in_graph_helper(vector<vector<int>> &graph, int vertex);
cycle_t shortest_cycle_in_graph(vector<vector<int>> &graph);
void print_result(const string &name, cycle_t res);

int main([[maybe_unused]]int argc,[[maybe_unused]] char **argv){
    std::vector<std::vector<int>> no_cycle = {
        {0, 1, 0, 0},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {0, 0, 1, 0}
    };
    std::vector<std::vector<int>> triangle = {
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0}
    };
    std::vector<std::vector<int>> cycle_3_and_5 = {
        {0, 1, 1, 0, 1},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 1, 0, 1},
        {1, 0, 0, 1, 0}
    };
    std::vector<std::vector<int>> disconnected_c4 = {
        {0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 1},
        {0, 0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0, 1},
        {0, 0, 1, 0, 1, 0}
    };
    std::vector<std::vector<int>> empty_graph = {};

    cycle_t res1 = shortest_cycle_in_graph(no_cycle);
    cycle_t res2 = shortest_cycle_in_graph(triangle);
    cycle_t res3 = shortest_cycle_in_graph(cycle_3_and_5);
    cycle_t res4 = shortest_cycle_in_graph(disconnected_c4);
    cycle_t res5 = shortest_cycle_in_graph(empty_graph);
    
    print_result("Acyclic line (0-1-2-3)", res1);
    print_result("Single triangle (0-1-2-0)", res2);
    print_result("3-cycle and 5-cycle", res3);
    print_result("Disconnected + 4-cycle (2-3-4-5-2)", res4);
    print_result("Empty graph (N = 0)", res5);
    return 0; 
}

cycle_t shortest_cycle_in_graph(vector<vector<int>> &graph){
    cycle_t best_cycle = {.length = INT_MAX, .cycle_path = {}};
    
    for(size_t i = 0; i < graph.size(); ++i){
        cycle_t res = shortest_cycle_in_graph_helper(graph, i);
        //we found a better cycle
        if(best_cycle.length > res.length)
            best_cycle = res;
        
    }
    /*logging the winning cycle, or an emp*/

    return best_cycle;
}

cycle_t shortest_cycle_in_graph_helper(vector<vector<int>> &graph, int vertex){

    cycle_t res = {.start_node = vertex, .length = INT_MAX};
    vector<bool> visited(graph.size());
    vector<int> parent(graph.size(), -1);
    vector<size_t> distance(graph.size(), 0);
    queue<int> path_queue;

    int min_cycle = INT_MAX;
    
    
    visited[vertex] = true;
    path_queue.push(vertex);


    while(!path_queue.empty()){
        int current = path_queue.front();
        path_queue.pop();

        for(size_t child = 0; child < graph[current].size(); ++child){
            
            if(graph[current][child] != 0 && visited[child] && parent[current] != (int)child){
                int cycle_distance = distance[current] + distance[child] + 1;
                if(min_cycle > cycle_distance){
                    
                    min_cycle = cycle_distance;
                    res.length = cycle_distance;
                    res.u = current;
                    res.v = child;

                }

            } else if(graph[current][child] != 0 && !visited[child]){
                parent[child] = current;
                distance[child] = distance[current] + 1;
                visited[child] = !visited[child];
                path_queue.push(child);

            }
        }
    }
    if(res.length != INT_MAX){
        vector<int> u_to_start;
        for (int curr = res.u; curr != -1; curr = parent[curr]) 
            u_to_start.push_back(curr);

        vector<int> v_to_start;
        for (int curr = res.v; curr != -1; curr = parent[curr])
            v_to_start.push_back(curr);
        

        vector<int> cycle_q;
        cycle_q.reserve(u_to_start.size() + v_to_start.size());


        for (int i = static_cast<int>(u_to_start.size()) - 1; i >= 0; --i) {
            cycle_q.push_back(u_to_start[i]);
        }

        for (int node : v_to_start) {
            cycle_q.push_back(node);
        }
        res.cycle_path = cycle_q;
    }


    return res;
}

void print_result(const string &name, cycle_t res) {
    cout << "=== " << name << " ===\n";
    if (res.length == INT_MAX || res.length <= 0) {
        cout << "Shortest cycle: none\n\n";
        return;
    }

    cout << "Shortest cycle length: " << res.length;
    if (res.u != -1 && res.v != -1) {
        cout << " (closing edge: " << res.u << " - " << res.v << ")";
    }
    cout << "\n";

    if (!res.cycle_path.empty()) {
        cout << "Cycle path: ";
        for (size_t i = 0; i < res.cycle_path.size(); ++i) {
            if (i > 0) cout << " -> ";
            cout << res.cycle_path[i];
        }
        cout << "\n";
    }
    cout << "\n";
}