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
void bronKerboschPivot(set<int> R, set<int> P, set<int> X,int& count/*reference here coz we are making changes in the count variable in bronkerboschdegeneracy method*/) {
    //if there are no common elements and we are only left with all vertices with degree>=d, we say that is maximal clique
    if (P.empty() && X.empty())
    {
        int cliqueSize = R.size();
        maxCliqueSize = max(maxCliqueSize, cliqueSize);
        cliqueSizeDistribution[cliqueSize]++;
        numCliques++;
        count++;
        return;
    }
    //we use pivoting to randomly pick the top element of the set so that we stop recursive calls when we reach there
    int pivot=*P.begin();
    for (int v : P)
        if (graph.at(v).size()>graph.at(pivot).size())
            pivot=v;
    set<int> non_neighbors;
    for (int v : P)
        if (graph.find(pivot) != graph.end() && find(graph.at(pivot).begin(), graph.at(pivot).end(), v) == graph.at(pivot).end())
            non_neighbors.insert(v);
    //below is same as bronKerboschDegeneracy ka recursive step
    for (int v : non_neighbors) {
        set<int> newR=R;
        set<int> newP,newX;
        newR.insert(v);
        for (int u : graph.at(v))
        {
            if (P.count(u))
                newP.insert(u);
            if (X.count(u))
                newX.insert(u);
        }
        bronKerboschPivot(newR,newP,newX,count);
        P.erase(v);
        X.insert(v);
    }
}

int bronKerboschDegeneracy(int vertices)//& only because giving pointer deferenceing exception
{

    //first we find degeneracy ordering which will be passed as input to bronKerboschpivoting function
    vector<int> ordering=findDegeneracyOrdering(vertices);
    set<int> P,R,X; //used vector first but should eliminate duplicates so set best option
    for (int v : ordering) {
        P.insert(v);
    }
    int count=0;//only prints total number of cliques

    for (int i = 0; i < ordering.size(); i++)
    {
        set<int> newP, newX;
        if (graph.find(ordering[i]) != graph.end())
        {
            for (int u : graph.at(ordering[i]))
            {
                if (P.count(u))
                    newP.insert(u);
                if (X.count(u))
                    newX.insert(u);
            }
        }
        bronKerboschPivot({ordering[i]}, newP, newX,count);
        P.erase(ordering[i]);
        X.insert(ordering[i]);
    }
    return count;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    int choice;
    cout<<"Enter the dataset number : \n1 for Enron email network\n2 for Wiki-Vote network \n3 for Skitter\n";
    cin>>choice;
    ifstream inputFile;
    if(choice==1)
    {
        inputFile.open("./datasets/wiki-Vote.txt");
        string line;
        for(int i = 0; i < 4; i++)
            getline(inputFile, line);
    }
    else if(choice==2)
    {
        inputFile.open("./datasets/email-Enron.txt");
        string line;
        for(int i = 0; i < 4; i++)
            getline(inputFile, line);
    }
    else
    {
        inputFile.open("./datasets/as-skitter.txt");
        string line;
        for(int i = 0; i < 5; i++)
            getline(inputFile, line);
    }
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
    cout << "Nodes: " << nodes.size() << endl;
    cout << "Edges: " << graph.size() << endl;
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
