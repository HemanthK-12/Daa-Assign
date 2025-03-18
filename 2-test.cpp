/*
- Bron Kerbosch Algorithm
- Finds all maximal cliques in graph.
- Recursive backtracking
- Has 3 parameters :
    - R = clique (initially empty)
    - P = All vertices adjacent to all vertices in R(initially all vertices)
        = vertices which should be considered next to check if those are also included in the clique
    - X = all vertices which SHOULD be excluded from the clique (initially empty)
- FIRST, we make a degeneracy ordering of the graph g=(V,E)
    - Degeneracy of a graph G=(V,E) = the largest k for which the k-core exists. (k-core of a graph=subgraph where every vertex has minimum degree k;see this to understand what a k core is - https://www.youtube.com/watch?v=rHVrgbc_3JA)
    - For graph with degeneracy d:
        - maximum clique size=d+1 vertices
        - Max edges= nd-(d(d+1)/2)

    - After this , find a degeneracy ordering,i.e :
        - Find the vertex with minimum degree in the graph
        - Remove this vertex from the graph and all of it's edges too. This'll be the first element in the ordering.
        - Repeat the above 2 steps until all vertices are removed
        - The order in which the vertices are removed is the degeneracy ordering
- O(3^(n/3)) --> O(nd3^(d/3)) where n=number of vertices, d=degeneracy of the graph
- First=Bron-Kerbosch Algorithm
- Second=Bron-Kerbosch with pivoting and degeneracy ordering
*/

#include <bits/stdc++.h>

using namespace std;

vector<int> findDegeneracyOrdering(int n, const vector<pair<int, int>>& edges) {
    // Initialize adjacency list
    vector<vector<int>> adj(n + 1);
    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }

    // Calculate initial degrees
    vector<int> degree(n + 1);
    int maxDeg = 0;
    for (int i = 1; i <= n; i++) {
        degree[i] = adj[i].size();
        maxDeg = max(maxDeg, degree[i]);
    }

    // Create degree buckets
    vector<list<int>> buckets(maxDeg + 1);
    for (int i = 1; i <= n; i++) {
        buckets[degree[i]].push_back(i);
    }

    // Track which vertices have been processed
    vector<bool> removed(n + 1, false);
    vector<int> ordering;
    ordering.reserve(n);

    // Find vertex ordering
    for (int i = 0; i < n; i++) {
        // Find smallest non-empty bucket
        int d = 0;
        while (d <= maxDeg && buckets[d].empty()) d++;
        
        // Get vertex with minimum degree
        int v = buckets[d].front();
        buckets[d].pop_front();
        removed[v] = true;
        ordering.push_back(v);

        // Update neighbors
        for (int u : adj[v]) {
            if (!removed[u]) {
                buckets[degree[u]].remove(u);
                degree[u]--;
                buckets[degree[u]].push_back(u);
            }
        }
    }

    return ordering;
}

int main()
{
    int v,e;
    cout<<"Enter number of vertices and edges: \n";
    cin>>v>>e;
    vector<pair<int,int>> edges;
    for(int i=0;i<e;i++)
    {
        int a,b;
        cin>>a>>b;
        edges.push_back({a,b});
    }
    vector<int> ordering=findDegeneracyOrdering(v,edges);
    cout<<"Degeneracy ordering: \n";
    for(int i=0;i<ordering.size();i++)
    {
        cout<<ordering[i]<<" ";
    }
    return 0;
}
