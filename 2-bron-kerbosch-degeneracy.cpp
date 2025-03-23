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
- Here, the whole clique and the list of vertices is directly given in R, which is made to be empty after printing the maximal clique
*/

#include <bits/stdc++.h>
using namespace std;
map<int,set<int>> graph;
set<int> nodes;
int maxCliqueSize = 0;
int numCliques = 0;
map<int, int> cliqueSizeDistribution;
vector<int> findDegeneracyOrdering(int vertices) {
    // Check if nodes is empty
    if (nodes.empty()) {
        return vector<int>();
    }

    map<int,int> degree;
    int maxDeg = 0;
    
    // Calculate degrees and find max degree
    for (int v : nodes) {
        if (graph.find(v) != graph.end()) {
            degree[v] = graph[v].size();
            maxDeg = max(maxDeg, degree[v]);
        }
    }

    // Initialize buckets with proper size
    vector<list<int>> buckets(maxDeg + 1);
    
    // Fill buckets safely
    for (int v : nodes) {
        if (graph.find(v) != graph.end()) {
            buckets[degree[v]].push_back(v);
        }
    }

    // Use nodes.size() instead of vertices parameter
    vector<bool> removed(nodes.size() * 2, false);  // Make it bigger to be safe
    vector<int> ordering;
    ordering.reserve(nodes.size());

    // Main degeneracy loop
    while (ordering.size() < nodes.size()) {
        int d = 0;
        while (d <= maxDeg && buckets[d].empty()) {
            d++;
        }
        
        if (d > maxDeg) break;  // Safety check
        
        int v = buckets[d].front();
        buckets[d].pop_front();
        removed[v] = true;
        ordering.push_back(v);

        // Update neighbors
        if (graph.find(v) != graph.end()) {
            for (int u : graph[v]) {
                if (!removed[u] && graph.find(u) != graph.end()) {
                    buckets[degree[u]].remove(u);
                    degree[u]--;
                    if (degree[u] >= 0) {  // Safety check
                        buckets[degree[u]].push_back(u);
                    }
                }
            }
        }
    }

    return ordering;
}

void bronKerboschPivot(set<int> R, set<int>& P, set<int>& X, int& count) {
    if (P.empty() && X.empty()) {
        int cliqueSize = R.size();
        maxCliqueSize = max(maxCliqueSize, cliqueSize);
        cliqueSizeDistribution[cliqueSize]++;
        numCliques++;
        count++;
        return;
    }

    // Find pivot with maximum neighbors
    int pivot = -1;
    int maxNeighbors = -1;
    for (int v : P) {
        int neighborCount = graph[v].size();
        if (neighborCount > maxNeighbors) {
            maxNeighbors = neighborCount;
            pivot = v;
        }
    }

    // Use iterator-based traversal
    for (auto it = P.begin(); it != P.end();) {
        int v = *it;
        
        // Skip if v is connected to pivot
        if (pivot != -1 && graph[pivot].find(v) != graph[pivot].end()) {
            ++it;
            continue;
        }
        
        // Create new R with v
        set<int> newR = R;
        newR.insert(v);
        
        // Create new P and X sets
        set<int> newP, newX;
        const auto& neighbors = graph[v];
        
        // Find neighbors in P and X using set operations
        for (const int& u : P) {
            if (u != v && neighbors.find(u) != neighbors.end()) {
                newP.insert(u);
            }
        }
        
        for (const int& u : X) {
            if (neighbors.find(u) != neighbors.end()) {
                newX.insert(u);
            }
        }
        
        bronKerboschPivot(newR, newP, newX, count);
        
        // Move v from P to X using iterators
        it = P.erase(it);
        X.insert(v);
    }
}

int bronKerboschDegeneracy(int vertices) {
    vector<int> ordering = findDegeneracyOrdering(vertices);
    set<int> P, R, X;
    for (int v : ordering) {
        P.insert(v);
    }
    int count = 0;

    for (int i = 0; i < ordering.size(); i++) {
        set<int> newP, newX;
        int current = ordering[i];
        
        // Skip redundant graph.find() check since we know the vertex exists
        for (int u : graph[current]) {
            if (P.count(u)) newP.insert(u);
            if (X.count(u)) newX.insert(u);
        }
        
        bronKerboschPivot({current}, newP, newX, count);
        P.erase(current);
        X.insert(current);
    }
    return count;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    ifstream inputFile;
    inputFile.open("./datasets/email-Enron.txt");
    int u, v;maxCliqueSize = 0;numCliques = 0;cliqueSizeDistribution.clear();
    if (inputFile.is_open()) {
        while (inputFile >> u >> v) {
            graph[u].insert(v);  
            graph[v].insert(u);
            nodes.insert(u);
            nodes.insert(v);
        }
        inputFile.close();
    }
    int totalCliques = bronKerboschDegeneracy(nodes.size());
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "\n=== Results ===\n";
    cout << "1. Largest clique size: " << maxCliqueSize << endl;
    cout << "2. Total number of maximal cliques: " << numCliques << endl;
    cout << "3. Execution time: " << duration.count() << " seconds\n";
    cout << "4. Clique size distribution:\n";
    for(const auto& pair : cliqueSizeDistribution) {
        cout << "   Size " << pair.first << ": " << pair.second << " cliques\n";
    }
    return 0;
}