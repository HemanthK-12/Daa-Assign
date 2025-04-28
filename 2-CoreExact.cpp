#include <bits/stdc++.h>
using namespace std;

struct GraphData {
    int n;
    vector<vector<int>> adj;
};

bool isClique(const GraphData& G, const vector<int>& vertices) {
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = i + 1; j < vertices.size(); j++) {
            if (find(G.adj[vertices[i]].begin(), G.adj[vertices[i]].end(), vertices[j]) == G.adj[vertices[i]].end()) {
                return false;
            }
        }
    }
    return true;
}

GraphData createGraph(int vertices) {
    GraphData G;
    G.n = vertices;
    G.adj.resize(vertices);
    return G;
}

void addEdge(GraphData& G, int u, int v) {
    G.adj[u].push_back(v);
    G.adj[v].push_back(u);
}

int getVertexCount(const GraphData& G) {
    return G.n;
}

void findCliques(const GraphData& G, int h, vector<int>& current, int start, vector<vector<int>>& cliques) {
    if (current.size() == h) {
        if (isClique(G, current)) {
            cliques.push_back(current);
        }
        return;
    }

    for (int i = start; i < G.n; i++) {
        current.push_back(i);
        findCliques(G, h, current, i + 1, cliques);
        current.pop_back();
    }
}

int cliqueDegree(const GraphData& G, int v, int h, const vector<vector<int>>& hCliques) {
    int degree = 0;
    for (const auto& clique : hCliques) {
        if (find(clique.begin(), clique.end(), v) != clique.end()) {
            degree++;
        }
    }
    return degree;
}

GraphData getInducedSubgraph(const GraphData& G, const vector<int>& vertices) {
    GraphData subgraph = createGraph(vertices.size());
    unordered_map<int, int> indexMap;

    for (size_t i = 0; i < vertices.size(); i++) {
        indexMap[vertices[i]] = i;
    }

    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = i + 1; j < vertices.size(); j++) {
            int u = vertices[i];
            int v = vertices[j];
            if (find(G.adj[u].begin(), G.adj[u].end(), v) != G.adj[u].end()) {
                addEdge(subgraph, indexMap[u], indexMap[v]);
            }
        }
    }

    return subgraph;
}

double cliqueDensity(const GraphData& G, int h) {
    vector<vector<int>> cliques;
    vector<int> temp;
    findCliques(G, h, temp, 0, cliques);

    if (G.n == 0) return 0.0;
    return static_cast<double>(cliques.size()) / G.n;
}

void printGraph(const GraphData& G) {
    cout << "Graph structure:" << endl;
    for (int i = 0; i < G.n; i++) {
        cout << "Vertex " << i << " connected to: ";
        for (int j : G.adj[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
}

vector<int> coreDecomposition(const GraphData& G, int h) {
    int n = G.n;
    vector<int> core(n, 0);
    vector<vector<int>> hCliques;
    vector<int> tmp;
    findCliques(G, h, tmp, 0, hCliques);
    vector<vector<int>> vertexToCliques(n);
    for (int i = 0; i < (int)hCliques.size(); i++)
        for (int v : hCliques[i])
            vertexToCliques[v].push_back(i);

    vector<int> degrees(n);
    for (int v = 0; v < n; v++)
        degrees[v] = vertexToCliques[v].size();

    vector<int> cliqueSize(hCliques.size(), h);
    vector<bool> cliqueActive(hCliques.size(), true);
    vector<pair<int,int>> vertexQueue;
    vertexQueue.reserve(n);
    for (int v = 0; v < n; v++)
        vertexQueue.emplace_back(degrees[v], v);

    vector<bool> removed(n,false);

    while (!vertexQueue.empty())
    {
        auto to_be_removed_node=min_element(vertexQueue.begin(), vertexQueue.end());
        int first = to_be_removed_node->first,second = to_be_removed_node->second;
        removed[second] = true;core[second] = first;
        vertexQueue.erase(to_be_removed_node);

        for (int i=0;i<vertexToCliques[second].size();i++)
        {
            if (!cliqueActive[vertexToCliques[second][i]])
                continue;
            if (cliqueSize[vertexToCliques[second][i]]-1 < h)
            {
                cliqueActive[vertexToCliques[second][i]] = false;
                for (int u : hCliques[vertexToCliques[second][i]])
                {
                    if (u == second || removed[u]) continue;
                    if (degrees[u] > first)
                    {
                        degrees[u]--;
                        for (auto& vd : vertexQueue)
                        {
                            if (vd.second == u)
                            {
                                vd.first = degrees[u];
                                break;
                            }
                        }
                    }
                }
            }
            cliqueSize[vertexToCliques[second][i]]=cliqueSize[vertexToCliques[second][i]]-1;
        }
    }

    return core;
}

GraphData extractKCore(const GraphData& G, int k, const vector<int>& coreNumbers) {
    int n = G.n;
    vector<int> kCoreVertices;

    for (int v = 0; v < n; v++) {
        if (coreNumbers[v] >= k) {
            kCoreVertices.push_back(v);
        }
    }

    cout << "Extracting " << k << "-core with " << kCoreVertices.size() << " vertices: ";
    for (int v : kCoreVertices) {
        cout << v << " ";
    }
    cout << endl;

    return getInducedSubgraph(G, kCoreVertices);
}

vector<GraphData> getConnectedComponents(const GraphData& G) {
    int n = G.n;
    vector<bool> visited(n, false);
    vector<GraphData> components;

    for (int v = 0; v < n; v++) {
        if (!visited[v]) {
            vector<int> componentVertices;
            queue<int> q;
            q.push(v);
            visited[v] = true;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                componentVertices.push_back(u);

                for (int w : G.adj[u]) {
                    if (!visited[w]) {
                        visited[w] = true;
                        q.push(w);
                    }
                }
            }

            cout << "Component " << components.size() + 1 << " vertices: ";
            for (int u : componentVertices) {
                cout << u << " ";
            }
            cout << endl;

            components.push_back(getInducedSubgraph(G, componentVertices));
        }
    }

    return components;
}

vector<vector<int>> buildFlowNetwork(const GraphData& G, int h, double alpha) {
    int n = G.n;
    vector<vector<int>> hCliques;
    vector<vector<int>> hMinus1Cliques;
    vector<int> temp;
    findCliques(G, h, temp, 0, hCliques);
    findCliques(G, h-1, temp, 0, hMinus1Cliques);

    cout << "Flow network: Found " << hCliques.size() << " " << h << "-cliques and " 
         << hMinus1Cliques.size() << " " << (h-1) << "-cliques" << endl;

    vector<int> cliqueDegrees(n, 0);
    for (int v = 0; v < n; v++) {
        cliqueDegrees[v] = cliqueDegree(G, v, h, hCliques);
    }

    int numNodes = 1 + n + hMinus1Cliques.size() + 1;
    vector<vector<int>> capacity(numNodes, vector<int>(numNodes, 0));

    int s = 0;
    int t = numNodes - 1;

    for (int v = 0; v < n; v++) {
        capacity[s][v + 1] = cliqueDegrees[v];
        if (cliqueDegrees[v] > 0) {
            cout << "Edge s -> " << v << " with capacity " << cliqueDegrees[v] << endl;
        }
    }

    for (int v = 0; v < n; v++) {
        capacity[v + 1][t] = alpha * h;
        cout << "Edge " << v << " -> t with capacity " << (alpha * h) << endl;
    }

    for (size_t i = 0; i < hMinus1Cliques.size(); i++) {
        const auto& clique = hMinus1Cliques[i];

        for (int v : clique) {
            capacity[n + 1 + i][v + 1] = INT_MAX;
            cout << "Edge clique" << i << " -> " << v << " with capacity INF" << endl;
        }

        for (int v = 0; v < n; v++) {
            if (find(clique.begin(), clique.end(), v) != clique.end()) continue;
            bool canFormClique = true;
            for (int u : clique) {
                if (find(G.adj[v].begin(), G.adj[v].end(), u) == G.adj[v].end()) {
                    canFormClique = false;
                    break;
                }
            }
            if (canFormClique) {
                capacity[v + 1][n + 1 + i] = 1;
                cout << "Edge " << v << " -> clique" << i << " with capacity 1" << endl;
            }
        }
    }

    return capacity;
}

int fordFulkerson(const vector<vector<int>>& capacity, int s, int t, vector<int>& minCut) {
    int n = capacity.size();
    vector<vector<int>> residual = capacity;
    vector<int> parent(n);
    int maxFlow = 0;
    
    auto bfs = [&](vector<int>& parent) -> bool {
        vector<bool> visited(n, false);
        queue<int> q; 
        q.push(s);
        visited[s] = true;
        parent[s] = -1;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v = 0; v < n; v++)
            {
                if (!visited[v])
                {
                  if(residual[u][v] > 0)
                  {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                    if (v == t) return true;
                  }
                }
            }
        }
        if(visited[t]!=0)
            return true;
        else {
          return false;
        }
    };
    
    while (bfs(parent)) {
        int pathFlow = INT_MAX;
        
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residual[u][v]);
        }
        
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }
        
        maxFlow += pathFlow;
    }
    
    cout << "Max flow: " << maxFlow << endl;
    
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v = 0; v < n; v++) {
            if (!visited[v] && residual[u][v] > 0) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    
    minCut.clear();
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            minCut.push_back(i);
        }
    }
    
    cout << "Min-cut vertices: ";
    for (int v : minCut) {
        cout << v << " ";
    }
    cout << endl;
    
    return maxFlow;
}

GraphData coreExact(const GraphData& G, int h) {
    int n = G.n;
    cout << "Running CoreExact algorithm for " << h << "-clique densest subgraph" << endl;
    printGraph(G);
    cout << "Performing core decomposition..." << endl;
    vector<int> coreNumbers = coreDecomposition(G, h);
    
    int kMax = 0;
    for (int k : coreNumbers) {
        kMax = max(kMax, k);
    }
    cout << "Maximum core number: " << kMax << endl;
    double rho = 0.0;
    vector<vector<int>> hCliques;
    vector<int> temp;
    findCliques(G, h, temp, 0, hCliques);
    
    if (!hCliques.empty()) {
        rho = static_cast<double>(hCliques.size()) / n;
    }
    
    int kPrime = ceil(rho);
    cout << "Initial lower bound: " << rho << ", k': " << kPrime << endl;
    
    GraphData kPrimeCore = extractKCore(G, kPrime, coreNumbers);
    vector<GraphData> components = getConnectedComponents(kPrimeCore);
    cout << "Number of connected components: " << components.size() << endl;
    
    GraphData bestSubgraph = createGraph(0);
    double bestDensity = 0.0;
    
    for (size_t i = 0; i < components.size(); i++) {
        GraphData component = components[i];
        cout << "Processing component " << i+1 << " with " << component.n << " vertices" << endl;
    
        if (component.n < h) {
            cout << "Component too small, skipping" << endl;
            continue;
        }
        double componentDensity = cliqueDensity(component, h);
        cout << "Component density: " << componentDensity << endl;
        
        if (componentDensity < rho) {
            cout << "Component density " << componentDensity << " < lower bound " << rho << ", skipping" << endl;
            continue;
        }
        double l = 0;
        double u = kMax > 0 ? kMax : 1.0;
        vector<int> bestCut;
        
        cout << "Starting binary search with bounds [" << l << ", " << u << "]" << endl;
        
        while (u - l >= 1.0 / (component.n * (component.n - 1))) {
            double alpha = (l + u) / 2.0;
            cout << "Trying α = " << alpha << endl;
            vector<vector<int>> flowNetwork = buildFlowNetwork(component, h, alpha);
            vector<int> minCut;
            fordFulkerson(flowNetwork, 0, flowNetwork.size()-1, minCut);
            
            if (minCut.size() <= 1) {
                u = alpha;
                cout << "Cut contains only source, reducing upper bound to " << u << endl;
            } else {
                vector<int> cutVertices;
                for (int node : minCut) {
                    if (node != 0 && node < component.n + 1) {
                        cutVertices.push_back(node - 1);
                    }
                }
                
                l = alpha;
                bestCut = cutVertices;
                cout << "Cut contains " << cutVertices.size() << " vertices, increasing lower bound to " << l << endl;
            }
        }
        
        if (!bestCut.empty()) {
            GraphData candidateSubgraph = getInducedSubgraph(component, bestCut);
            double candidateDensity = cliqueDensity(candidateSubgraph, h);
            
            cout << "Candidate subgraph has " << candidateSubgraph.n << " vertices and density " << candidateDensity << endl;
            
            if (candidateDensity > bestDensity) {
                bestDensity = candidateDensity;
                bestSubgraph = candidateSubgraph;
                cout << "Found better subgraph with density " << bestDensity << endl;
            }
        }
    }
    
    cout << "CoreExact completed. Best subgraph has " << bestSubgraph.n << " vertices and density " << bestDensity << endl;
    
    return bestSubgraph;
}

GraphData read_graph_from_file(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        throw runtime_error("Error: Cannot open " + filename);
    }

    string line;
    vector<pair<int, int>> edges;
    unordered_map<int, int> vertex_map;
    int next_id = 0;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        istringstream iss(line);
        int v1, v2;
        if (!(iss >> v1 >> v2)) continue;
        if (vertex_map.find(v1) == vertex_map.end()) {
            vertex_map[v1] = next_id++;
        }
        if (vertex_map.find(v2) == vertex_map.end()) {
            vertex_map[v2] = next_id++;
        }

        edges.emplace_back(vertex_map[v1], vertex_map[v2]);
    }
    fin.close();

    GraphData G = createGraph(vertex_map.size());
    for (const auto& edge : edges) {
        addEdge(G, edge.first, edge.second);
    }

    return G;
}

int main() {
    GraphData G = read_graph_from_file("./as20000102.txt");
    
    cout << "Graph loaded successfully:" << endl;
    cout << "Number of vertices: " << G.n << endl;
    
    int edge_count = 0;
    for (const auto& edges : G.adj) {
        edge_count += edges.size();
    }
    cout << "Number of edges: " << edge_count/2 << endl << endl;
    
    int h;
    cout << "Enter the value of h (clique size): ";
    while (!(cin >> h) || h <= 0 || h > G.n) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a positive integer less than " 
             << G.n + 1 << ": ";
    }

    auto startTime = chrono::high_resolution_clock::now();
    GraphData densestSubgraph = coreExact(G, h);
    auto endTime = chrono::high_resolution_clock::now();
    
    double executionTime = chrono::duration<double>(endTime - startTime).count();
    cout << "Execution time: " << executionTime << " seconds" << endl;
    
    return 0;
}
