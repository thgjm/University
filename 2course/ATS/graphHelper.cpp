// c++ program to identify Tree, Back, 
// Edge and Cross Edges in DFS of Graph
#include <iostream>
#include <vector>
using namespace std;

void dfs(int u, vector<vector<int>>& adj, vector<bool>& visited, 
    vector<bool>& inStack, vector<int>& discovery, 
    vector<int>& finish, int& timeStamp,
    vector<vector<vector<int>>>& edges) {
    
    visited[u] = true;
    inStack[u] = true;
    discovery[u] = ++timeStamp;
    
    for(int v : adj[u]) {
        if(!visited[v]) {
            
            // Tree Edge
            edges[0].push_back({u, v});
            dfs(v, adj, visited, inStack, 
            discovery, finish, timeStamp, edges);
        }
        else {
            if(inStack[v]) {
                
                // Back Edge
                edges[2].push_back({u, v});
            }
            else if(discovery[u] < discovery[v]) {
                
                // Forward Edge
                edges[1].push_back({u, v});
            }
            else {
                
                // Cross Edge
                edges[3].push_back({u, v});
            }
        }
    }
    
    inStack[u] = false;
    finish[u] = ++timeStamp;
}

vector<vector<vector<int>>> classifyEdges(vector<vector<int>>& adj) {
    int n = adj.size();
    
    // Array to store discovery timeStamp of node
    vector<int> discovery(n, 0);
    
    // Array to store finish timeStamp of node.
    vector<int> finish(n, 0);
    
    // Array to check if node is visited
    vector<bool> visited(n, false);
    
    // Array to check if node is in call stack.
    vector<bool> inStack(n, false);
    int timeStamp = 0;
    
    // Initialize result vector with 
    // 4 empty vectors for each edge type
    vector<vector<vector<int>>> edges(4);
    
    // Run DFS for each unvisited vertex
    for(int i = 0; i < n; i++) {
        if(!visited[i]) {
            dfs(i, adj, visited, inStack, 
            discovery, finish, timeStamp, edges);
        }
    }
    
    return edges;
}

int main() {
    
    vector<vector<int>> adj = {
        {1, 2, 4},
        {2, 4},
        {3},
        {0},
        {},
        {1}
    };
    
    vector<vector<vector<int>>> edges = classifyEdges(adj);
    
    vector<string> edgeNames = {"Tree Edges", "Forward Edges", 
    "Back Edges", "Cross Edges"};
    
    for(int i = 0; i < 4; i++) {
        cout << edgeNames[i] << ": ";
        for(auto& edge : edges[i]) {
            cout << edge[0] << "->" << edge[1] << " ";
        }
        cout << endl;
    }
    
    return 0;
}