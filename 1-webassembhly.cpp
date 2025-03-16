#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <string>
using namespace std;
using namespace emscripten;

// Global variables (try to minimize these in WebAssembly)
vector<int> Graph[1000];
set<int> Q;
string output_result; // Store the output result

// Helper functions
set<int> Intersection(const set<int>& s1, const set<int>& s2) {
    set<int> ans;
    for(auto x:s1)
        if(s2.find(x)!=s2.end())
            ans.insert(x);
    return ans;
}

set<int> Difference(const set<int>& s1, const set<int>& s2) {
    set<int> ans;
    for(auto x:s1)
        if(s2.find(x)==s2.end())
            ans.insert(x);
    return ans;
}

void expand(set<int> subg, set<int> cand) {
    if (subg.empty()) {
        output_result += "CLIQUE\n";
        return;
    } else {
        int u = -1;
        int maxx = -1;
        for (auto x : subg) {
            set<int> temp(Graph[x].begin(), Graph[x].end());
            int temp_int = Intersection(temp, cand).size();
            if (temp_int > maxx) {
                maxx = temp_int;
                u = x;
            }
        }
        if (u == -1) return;

        set<int> temp2(Graph[u].begin(), Graph[u].end());
        set<int> Ext = Difference(cand, temp2);
        while (!Ext.empty()) {
            int q = *Ext.begin();
            Ext.erase(q);
            output_result += to_string(q) + " ";
            Q.insert(q);
            set<int> temp3(Graph[q].begin(), Graph[q].end());
            set<int> subq_q = Intersection(subg, temp3);
            set<int> cand_q = Intersection(cand, temp3);
            expand(subq_q, cand_q);
            cand.erase(q);
            Q.erase(q);
            output_result += "back ";
        }
    }
}
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    const char* CLIQUES(int n, const int* edges, int edgeCount) {
        // Input validation
        if (n <= 0 || n > 1000 || edgeCount <= 0 || edges == nullptr) {
            return "Invalid input parameters";
        }

        // Clear global state
        for (int i = 0; i < 1000; ++i) {
            Graph[i].clear();
        }
        Q.clear();
        output_result = "";

        // Build the graph from flat array with bounds checking
        try {
            for (int i = 0; i < edgeCount; i += 2) {
                int u = edges[i];
                int v = edges[i + 1];
                
                // Validate vertex indices
                // if (u <= 0 || u > n || v <= 0 || v > n) {
                //     printf("Invalid vertex indices: %d %d n=%d\n", u, v, n);
                //     return "Invalid vertex indices";
                // }
                
                Graph[u].push_back(v);
                Graph[v].push_back(u);
            }

            set<int> V;
            for (int i = 1; i <= n; i++) {
                V.insert(i);
            }
            expand(V, V);

            // Return the result string
            static string result = output_result;
            return result.c_str();
        } catch (...) {
            return "Error processing graph";
        }
    }
}
// Use a wrapper function for binding
string CLIQUESWrapper(int n, const vector<int>& edges) {
    return CLIQUES(n, edges.data(), edges.size());
}
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::register_vector<int>("VectorInt");
    emscripten::function("CLIQUES", &CLIQUESWrapper);
}