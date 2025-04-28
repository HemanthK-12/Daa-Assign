#include <bits/stdc++.h>

using namespace std;

struct ProgramResults {
    vector<int> densest_subgraph;
    double density;
    int num_vertices;
    int num_edges;
    int self_loops;
    int iterations;
    double execution_time;
};



struct Edge_flow {
    int from, to;
    double capacity, flow;
    size_t rev_idx;
};

vector<vector<Edge_flow>> network;
vector<int> level;
vector<size_t> edge_ptr;
struct Equal {
    bool operator()(const unordered_set<int>& first, const unordered_set<int>& second) const {
        if (first.size() != second.size()) {
            return false;
        }
        return all_of(first.begin(), first.end(), 
                    [&second](int val) { return second.count(val) > 0; });
    }
};

void init_flow_network(int num_vertices) {
    network.clear();
    network.resize(num_vertices);
    level.resize(num_vertices);
    edge_ptr.resize(num_vertices);
}

void add_flow_edge(int from, int to, double capacity) {
    size_t idx1 = network[from].size();
    size_t idx2 = network[to].size();
    network[from].push_back({from, to, capacity, 0, idx2});
    network[to].push_back({to, from, 0, 0, idx1});
}

bool build_level_graph(int source, int sink) {
    fill(level.begin(), level.end(), -1);
    level[source] = 0;
    
    deque<int> queue = {source};
    
    while (!queue.empty() && level[sink] == -1) {
        int current = queue.front();
        queue.pop_front();
        
        for (const auto& edge : network[current]) {
            if (level[edge.to] == -1 && edge.capacity > edge.flow) {
                level[edge.to] = level[current] + 1;
                queue.push_back(edge.to);
            }
        }
    }
    
    return level[sink] != -1;
}

double push_flow(int vertex, int sink, double flow_cap) {
    if (vertex == sink) return flow_cap;
    
    for (; edge_ptr[vertex] < network[vertex].size(); ++edge_ptr[vertex]) {
        auto& edge = network[vertex][edge_ptr[vertex]];
        
        if (level[edge.to] == level[vertex] + 1 && edge.capacity > edge.flow) {
            double pushed = push_flow(edge.to, sink, 
                min(flow_cap, edge.capacity - edge.flow));
                
            if (pushed > 0) {
                edge.flow += pushed;
                network[edge.to][edge.rev_idx].flow -= pushed;
                return pushed;
            }
        }
    }
    
    return 0;
}
struct Hashing {
    template <class T>
    inline void hash_combine(size_t& seed, const T& v) const {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    size_t operator()(const unordered_set<int>& s) const {
        size_t seed = 0;
        for (const auto& i : s) {
            hash_combine(seed, i);
        }
        return seed;
    }
};
double compute_max_flow(int source, int sink) {
    double total_flow = 0;
    
    while (build_level_graph(source, sink)) {
        fill(edge_ptr.begin(), edge_ptr.end(), 0);
        
        while (double pushed = push_flow(source, sink, numeric_limits<double>::infinity())) {
            total_flow += pushed;
        }
    }
    
    return total_flow;
}

vector<bool> get_min_cut_set(int source) {
    vector<bool> reachable(network.size(), false);
    vector<bool> visited(network.size(), false);
    deque<int> queue = {source};
    
    reachable[source] = visited[source] = true;
    
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop_front();
        
        for (const auto& edge : network[current]) {
            if (!visited[edge.to] && edge.capacity > edge.flow) {
                reachable[edge.to] = visited[edge.to] = true;
                queue.push_back(edge.to);
            }
        }
    }
    
    return reachable;
}

vector<unordered_set<int>> find_all_cliques(const vector<vector<int>>& adj_list, int size) {
    vector<unordered_set<int>> all_cliques;
    vector<bool> in_clique(adj_list.size(), false);
    
    function<void(int, vector<int>&)> build_clique = [&](int start, vector<int>& current) {
        if (current.size() == size - 1) {
            all_cliques.emplace_back(current.begin(), current.end());
            return;
        }
        
        for (int v = start; v < adj_list.size(); v++) {
            if (in_clique[v]) continue;
            
            bool can_add = true;
            for (int u : current) {
                if (find(adj_list[v].begin(), adj_list[v].end(), u) == adj_list[v].end()) {
                    can_add = false;
                    break;
                }
            }
            
            if (can_add) {
                current.push_back(v);
                in_clique[v] = true;
                build_clique(v + 1, current);
                in_clique[v] = false;
                current.pop_back();
            }
        }
    };
    
    vector<int> current;
    for (int i = 0; i < adj_list.size(); i++) {
        current.push_back(i);
        in_clique[i] = true;
        build_clique(i + 1, current);
        in_clique[i] = false;
        current.pop_back();
    }
    
    return all_cliques;
}

struct GraphData {
    int n;  
    vector<vector<int>> adj; 
    
    GraphData(int vertices) : n(vertices), adj(vertices) {}
};

GraphData read_graph_from_file(const string& filename) {
    ifstream file(filename);
    set<pair<int,int>> edges; 
    int max_vertex = -1;
    int u, v;
    while (file >> u >> v) {
        if (u == v) continue;
        if (u > v) swap(u, v);
        max_vertex = max(max_vertex, max(u, v));
        edges.insert({u, v});
    }
    
    GraphData G(max_vertex + 1);
    for (const auto& edge : edges) {
        G.adj[edge.first].push_back(edge.second);
        G.adj[edge.second].push_back(edge.first);
    }
    
    return G;
}

GraphData load_graph(const string& filename) {
    return read_graph_from_file(filename);
}

void print_graph_info(const GraphData& G) {
    cout << "Graph loaded successfully:" << endl;
    cout << "Number of vertices: " << G.n << endl;
    
    int edge_count = 0;
    for (const auto& edges : G.adj) {
        edge_count += edges.size();
    }
    cout << "Number of edges: " << edge_count/2 << endl << endl;
}

int get_clique_size(int max_vertices) {
    int h;
    cout << "Enter the value of h (clique size): ";
    while (!(cin >> h) || h <= 0 || h > max_vertices) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return h;
}

vector<unordered_set<int>> process_cliques(const vector<vector<int>>& graph, int h, vector<int>& deg_Psi) {
    vector<unordered_set<int>> Lambda = find_all_cliques(graph, h);
    
    for (const auto& sigma:Lambda) {
        if (sigma.empty()) continue;
        
        unordered_set<int> common_neighbors;
        bool first = true;
        
        for (int v : sigma) {
            if (first) {
                for (int neighbor : graph[v]) {
                    if (sigma.count(neighbor) == 0) {
                        common_neighbors.insert(neighbor);
                    }
                }
                first = false;
            } else {
                unordered_set<int> new_common;
                for (int neighbor : graph[v]) {
                    if (sigma.count(neighbor) == 0 && 
                        common_neighbors.count(neighbor) != 0) {
                        new_common.insert(neighbor);
                    }
                }
                common_neighbors = move(new_common);
            }
        }
        
        for (int v : common_neighbors) {
            deg_Psi[v]++;
        }
    }
    return Lambda;
}

ProgramResults find_densest_subgraph(const vector<vector<int>>& graph, const vector<unordered_set<int>>& Lambda, 
                                   const vector<int>& deg_Psi, int h, int n) {
    auto start_time = chrono::high_resolution_clock::now();
    
    int max_deg = *max_element(deg_Psi.begin(), deg_Psi.end());
    double l = 0.0, u = static_cast<double>(max_deg);
    double eps= (n > 1) ? 1.0 / (n * (n - 1)) : 1e-9;
    
    vector<int> D;
    int iteration = 0;
    
    while (u - l > eps) {
        iteration++;
        double lambda = (l + u) / 2.0;
        int num_nodes =  3+n+Lambda.size()-1; 
        
        int s = 0;
        int t = 1;  
        int v_offset = 2;
        int clique_offset = v_offset + n;
        init_flow_network(num_nodes);
        int source_edges = 0;
        for (int v = 0; v < n; v++) {
            if (deg_Psi[v] > 0) {
                add_flow_edge(s, v_offset + v, deg_Psi[v]);
                source_edges++;
            }
        }
        for (int v = 0; v < n; ++v) {
            add_flow_edge(v_offset + v, t, h * lambda);
        }
        int clique_vertex_edges = 0;
        int vertex_clique_edges = 0;
        
        for (size_t i = 0; i < Lambda.size(); ++i) {
            const auto& sigma = Lambda[i];
            int clique_node = clique_offset + i;
            
            for (int v : sigma) {
                add_flow_edge(clique_node, v_offset + v, numeric_limits<double>::infinity());
                clique_vertex_edges++;
            }
            unordered_set<int> neighbors_c;
            bool first = true;
            
            for (int v : sigma) {
                if (first) {
                    for (int neighbor : graph[v]) {
                        if (sigma.count(neighbor)==0) {
                            neighbors_c.insert(neighbor);
                        }
                    }
                    first = false;
                } else {
                    unordered_set<int> new_common;
                    for (int neighbor : graph[v]) {
                        if (sigma.count(neighbor)==0 && 
                            neighbors_c.count(neighbor) != 0) {
                            new_common.insert(neighbor);
                        }
                    }
                    neighbors_c = move(new_common);
                }
            }
            
            for (int v : neighbors_c) {
                add_flow_edge(v_offset + v, clique_node, 1.0);
                vertex_clique_edges++;
            }
            
        }
        
        
        double cut_value = compute_max_flow(s, t);
        vector<bool> min_cut_set = get_min_cut_set(s);
        
        bool only_source_in_S = true;
        for (int i = 1; i < num_nodes; ++i) {
            if (min_cut_set[i]) {
                only_source_in_S = false;
                break;
            }
        }
        
        if (only_source_in_S) {
            u = lambda;
        } else {
            l = lambda;
            D.clear();
            for (int v = 0; v < n; ++v) {
                if (min_cut_set[v_offset + v]) {
                    D.push_back(v);
                }
            }
        }
        
        if (abs(u-l)<1e-12) {
            break;
        }
    }
    int num_edges = 0;
    int self = 0;
    int num_vertices = D.size();
    
        
    auto end_time = chrono::high_resolution_clock::now();
    double elapsed_time = chrono::duration<double>(end_time - start_time).count();
  
    for (int i = 0; i < D.size(); ++i) {
        for (int j = i; j < D.size(); ++j) {
            
            bool edge=false;
            for (int neighbor : graph[D[i]]) {
            if (neighbor-D[j] == 0) {
                edge = true;
                break;
                }
            }
                
            if (edge==1) {
                    num_edges++;
                    if (D[i] -D[j] == 0) {
                        self++;
                    }
                }
            }
        }
        
    return {D, l, num_vertices, num_edges, self, iteration, elapsed_time};
}

void print_results(const ProgramResults& results, int h) {
    
    cout << "\nThe densest subgraph is: ";
    vector<int> orD = results.densest_subgraph;
    sort(orD.begin(), orD.end());
    
    for (int i = 0; i < orD.size(); i++) {
        cout << orD[i];

        if (i < orD.size()-1) cout << ", ";
    }
    cout << endl;
    cout << "Number of vertices: " << results.num_vertices << endl;
    cout << "Number of edges: " << results.num_edges <<  endl;
    cout << (h-1) << "-clique Density: " << round(results.density * 10000) / 10000 << endl;
    
    cout << "Execution Time: " << results.execution_time << " s" << endl;
}

int main() {
    GraphData Gr = load_graph("as20000102.txt");
     
    vector<int> deg_Psi(Gr.n, 0);
    print_graph_info(Gr);
    
    int h = get_clique_size(Gr.n);
   
    vector<unordered_set<int>> lm = process_cliques(Gr.adj, h, deg_Psi);
    
    ProgramResults results = find_densest_subgraph(Gr.adj, lm, deg_Psi, h, Gr.n);
    print_results(results, h);
    
    return 0;
}
