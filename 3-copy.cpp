#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <fstream>
using namespace std;

// Graph representation using adjacency lists
vector<vector<int>> graph;
// Use a set of sets to automatically avoid duplicates
set<set<int>> uniqueCliques;
int iterationCount = 0,n;  // Add this counter

// Helper arrays
vector<int> T, S;
vector<int> degree;
map<int, int> old_to_new; // Maps original vertex numbers to 0...n-1
map<int, int> new_to_old; // Maps 0...n-1 to original vertex numbers

// Helper function to compute C ∩ N(i)
set<int> compute_C_intersect_Ni(const set<int> &C, int i)
{
    set<int> C_inter_Ni;
    for (int v : C)
    {
        for (int neighbor : graph[i])
        {
            if (neighbor == v)
            {
                C_inter_Ni.insert(v);
                break;
            }
        }
    }
    return C_inter_Ni;
}

// Helper function to compute C - N(i)
set<int> compute_C_minus_Ni(const set<int> &C, int i)
{
    set<int> C_minus_Ni;
    for (int v : C)
    {
        bool isNeighbor = false;
        for (int neighbor : graph[i])
        {
            if (neighbor == v)
            {
                isNeighbor = true;
                break;
            }
        }
        if (!isNeighbor)
        {
            C_minus_Ni.insert(v);
        }
    }
    return C_minus_Ni;
}

// Helper function to compute T[y] = |N(y) ∩ C ∩ N(i)| for y ∈ V - C - {i}
void compute_T(const set<int> &C_inter_Ni, const set<int> &C,int i)
{
    fill(T.begin(), T.end(), 0);
    for (int x : C_inter_Ni)
    {
        for (int y : graph[x])
        {
            if (C.find(y) == C.end() && y != i)
            {
                T[y]++;
            }
        }
    }

}

// Helper function to compute S[y] = |N(y) ∩ (C - N(i))| for y ∈ V - C
void compute_S(const set<int> &C_minus_Ni, const set<int> &C)
{
    fill(S.begin(), S.end(), 0);
    for (int x : C_minus_Ni)
    {
        for (int y : graph[x])
        {
            if (C.find(y) == C.end())
            {
                S[y]++;
            }
        }
    }
}

// Helper function to perform the maximality test (Step 4)
bool is_maximal(const set<int> &C_inter_Ni, const set<int> &C, int i)
{
    for (int y = 0; y < i; y++)
    {
        bool inNi = false;
        for (int neighbor : graph[i])
        {
            if (neighbor == y)
            {
                inNi = true;
                break;
            }
        }
        if (inNi && C.find(y) == C.end() && T[y] == C_inter_Ni.size())
        {
            return false; // (C ∩ N(i)) ∪ {i} is not a clique of G_i
        }
    }
    return true;
}

// Helper function to perform the lexicographical test (Steps 5-7)
bool is_lexicographically_largest(const set<int> &C_minus_Ni, const set<int> &C_inter_Ni,const set<int> &C, int i)
{
    vector<int> sorted_C_minus_Ni(C_minus_Ni.begin(), C_minus_Ni.end());
    sort(sorted_C_minus_Ni.begin(), sorted_C_minus_Ni.end());
    int p = sorted_C_minus_Ni.size();

    // Case S(y) >= 1
    for (int k = 0; k < p; k++)
    {
        int jk = sorted_C_minus_Ni[k];
        for (int y : graph[jk])
        {
            if (C.find(y) == C.end() && y < i && T[y] == C_inter_Ni.size())
            {
                if (y >= jk)
                {
                    S[y]--;
                }
                else
                {
                    bool isFirst = true;
                    for (int m = 0; m < k; m++)
                    {
                        if (y < sorted_C_minus_Ni[m])
                        {
                            isFirst = false;
                            break;
                        }
                    }
                    //SEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE, added -1 below
                    if (isFirst && S[y] + k - 1== p && y >= (k > 0 ? sorted_C_minus_Ni[k - 1] : 0))
                    {
                        return false; // C is not lexicographically largest
                    }
                }
            }
        }
    }

    // Case S(y) = 0
    if (!C_inter_Ni.empty())
    {
        for (int v : C_inter_Ni)
        {
            for (int y : graph[v])
            {
                if (C.find(y) == C.end() && y != i && y < i && T[y] == C_inter_Ni.size() && S[y] == 0)
                {
                    if (p > 0 && sorted_C_minus_Ni[p - 1] < y)
                    {
                        return false; // C is not lexicographically largest
                    }
                    
                }
            }
        }
    }
    else if (p > 0 && sorted_C_minus_Ni[p - 1] < i - 1)
    {
        return false; // C is not lexicographically largest
    }

    return true;
}

// Update procedure of the TIAS algorithm
void UPDATE(int i, set<int> &C)
{
    if (i == n)
    {
        // Found a new maximal clique
        uniqueCliques.insert(C);
        iterationCount++;
        cout  << iterationCount<<endl ;
        return;
        // set<int> C_inter_Ni = compute_C_intersect_Ni(C, i);
        // set<int> C_minus_Ni = compute_C_minus_Ni(C, i);
    }
    else
    {
    // Compute C ∩ N(i) and C - N(i)
        // set<int> C_inter_Ni = compute_C_intersect_Ni(C, i);
        // set<int> C_minus_Ni = compute_C_minus_Ni(C, i);

        // Step 1: If C - N(i) is not empty, call UPDATE(i+1, C)
        if (!compute_C_minus_Ni(C, i).empty())
        {
            UPDATE(i + 1, C);
        }
        set<int> C_inter_Ni = compute_C_intersect_Ni(C, i);
        set<int> C_minus_Ni = compute_C_minus_Ni(C, i);
        // Compute T and S arrays (i == n+1)
        compute_T(C_inter_Ni, C, i);
        compute_S(C_minus_Ni, C);

        // Perform maximality and lexicographical tests
        bool FLAG = is_maximal(C_inter_Ni, C, i) && is_lexicographically_largest(C_minus_Ni, C_inter_Ni,C, i);
        // Step 10: If FLAG is true, add i to C and recursively call UPDATE
        if (FLAG)
        {
            set<int> SAVE = C_minus_Ni;
            C = C_inter_Ni;
            C.insert(i);
            UPDATE(i + 1, C);
            C.erase(i);
            for (int v : SAVE)
            {
                C.insert(v);
            }
        }
    }
}

int main()
{
    ifstream infile("./input.txt");
    string line;
    set<int> unique_vertices;
    vector<pair<int, int>> edges;

    if (!infile.is_open())
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    int u, v;
    while (infile >> u >> v)
    {
        edges.push_back({u, v});
        unique_vertices.insert(u);
        unique_vertices.insert(v);
    }
    infile.close();

    n = unique_vertices.size();

    // Initialize graph with size n (for 0-based indexing)
    graph.resize(n);

    // Create mapping for vertices
    int new_id = 0;
    for (int v : unique_vertices)
    {
        old_to_new[v] = new_id;
        new_to_old[new_id] = v;
        new_id++;
    }

    // Build graph with remapped vertices
    for (const pair<int, int> &edge : edges)
    {
        graph[old_to_new[edge.first]].push_back(old_to_new[edge.second]);
        graph[old_to_new[edge.second]].push_back(old_to_new[edge.first]);
    }

    // Calculate degrees
    degree.resize(n, 0);
    for (int i = 0; i < n; i++)
    {
        degree[i] = graph[i].size();
    }

    // Reorder vertices by degree
    vector<pair<int, int>> vertices;
    for (int i = 0; i < n; i++)
    {
        vertices.push_back({degree[i], i});
    } 
    sort(vertices.begin(), vertices.end());

    // Create new mappings after degree-based reordering
    map<int, int> degree_old_to_new;
    map<int, int> degree_new_to_old;
    vector<vector<int>> new_graph(n);

    for (int i = 0; i < n; i++)
    {
        int old_id = vertices[i].second;
        int new_id = i;
        degree_old_to_new[old_id] = new_id;
        degree_new_to_old[new_id] = old_id;
    }

    // Renumber the graph based on degree ordering
    for (int i = 0; i < n; i++)
    {
        for (int neighbor : graph[i])
        {
            new_graph[degree_old_to_new[i]].push_back(degree_old_to_new[neighbor]);
        }
    }

    graph = new_graph;

    // Initialize arrays
    T.resize(n, 0);
    S.resize(n, 0);

    // Start with vertex 0
    set<int> C = {0};

    // Call UPDATE starting from vertex 1
    UPDATE(1, C);

    // cout << "Maximal Cliques:" << endl;
    // for (const auto &clique : uniqueCliques)
    // {
    //     for (int v : clique)
    //     {
    //         // Convert back to original vertex numbering (two-step conversion)
    //         cout << new_to_old[degree_new_to_old[v]] << " ";
    //     }
    //     cout << endl;
    // }

    cout << "Total number of maximal cliques: " << uniqueCliques.size() << endl;

    return 0;
}