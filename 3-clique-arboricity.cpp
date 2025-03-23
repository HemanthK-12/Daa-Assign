#include <bits/stdc++.h>
#include <sys/resource.h>
#include <ranges>
#include <algorithm>
using namespace std;

int maximC = 0;
vector<set<int>> adj;      // Changed to vector
vector<set<int>> adjMat;   // Changed to vector
vector<int> S, T;
set<int> C;
int count_C = 0;
unordered_map<int, int> SizeDist;
int n, m;

vector<int> computeIntersection(const set<int>& curr_C, const set<int>& neighbors) {
    vector<int> intersection;
    vector<int> adj_vec(neighbors.begin(), neighbors.end());
    
    for (int x : curr_C) {
        if (binary_search(adj_vec.begin(), adj_vec.end(), x)) {
            intersection.push_back(x);
        }
    }
    return intersection;
}

vector<int> computeDifference(const set<int>& curr_C, const set<int>& neighbors) {
    vector<int> difference;
    vector<int> adj_vec(neighbors.begin(), neighbors.end());
    
    for (int x : curr_C) {
        if (!binary_search(adj_vec.begin(), adj_vec.end(), x)) {
            difference.push_back(x);
        }
    }
    return difference;
}

void UPDATE(int i)
{
    vector<int> index_vec,stateV;
    vector<set<int>> CliqueV,saveV;
    vector<vector<int>> CnN,indexV,CminN;
    vector<bool> flagV;

    index_vec.push_back(i);
    CliqueV.push_back(C);
    CnN.push_back(vector<int>());
    CminN.push_back(vector<int>());
    indexV.push_back(vector<int>());
    flagV.push_back(true);
    stateV.push_back(0);
    saveV.push_back(set<int>());

    while (!index_vec.empty()) {
        int I = index_vec.back();
        set<int>& curr_C = CliqueV.back();
        vector<int>& C_inter_N = CnN.back();
        vector<int>& C_minus_N = CminN.back();
        vector<int>& cur_dif = indexV.back();
        bool curr_flag = flagV.back();
        int& curr_state = stateV.back();
        set<int>& curr_save = saveV.back();
        if (I==n) {
            maximC=max(maximC,(int)curr_C.size());
            count_C++;
            SizeDist[curr_C.size()]++;
            if(count_C%2000==0) {
                cout<<"Count = "<<count_C<<", Maximal Clique Size = "<<maximC<<"\n";
            }
            
        }
        if(I>=n) {
            index_vec.pop_back();
            CliqueV.pop_back();
            CnN.pop_back();
            CminN.pop_back();
            indexV.pop_back();
            flagV.pop_back();
            stateV.pop_back();
            saveV.pop_back();
            continue;
        }

        if (curr_state==0) {
            curr_state=1;
            C_inter_N=computeIntersection(curr_C, adjMat[I]);
            C_minus_N=computeDifference(curr_C, adjMat[I]);

            
            if (!C_minus_N.empty()) {
                index_vec.push_back(I+1);
                CliqueV.push_back(curr_C);
                CnN.push_back(vector<int>());
                CminN.push_back(vector<int>());
                indexV.push_back(vector<int>());
                flagV.push_back(true);
                stateV.push_back(0);
                saveV.push_back(set<int>());
                continue;
            }
        }

        if (curr_state==1) {
            for (int x:C_inter_N) {
                for (int y:adjMat[x]) {
                    if (curr_C.find(y)==curr_C.end() && y!=I) {
                        T[y]++;
                    }
                }
            }

            for (int x:C_minus_N) {
                for (int y:adjMat[x]) {
                    if (curr_C.find(y) == curr_C.end()) {
                        S[y]++;
                    }
                }
            }

            curr_flag=true;

            for (int y : adjMat[I]) {
                if (curr_C.find(y) == curr_C.end() && y < I && T[y] == C_inter_N.size()) {
                    curr_flag = false;
                    break;
                }
            }

            if (curr_flag) {
                cur_dif=vector<int>(C_minus_N.begin(), C_minus_N.end());
                sort(cur_dif.begin(), cur_dif.end());

                int p = cur_dif.size();
                for (int k = 1; k <= p; k++) {
                    int jk = cur_dif[k-1];
                    bool first_handled = false;

                    for (int y : adjMat[jk]) {
                        if (curr_C.find(y) == curr_C.end() && y < I && T[y] == C_inter_N.size()) {
                            if (y >= jk) {
                                S[y]--;
                            } else {
                                if (!first_handled) {
                                    if (S[y] + k - 1 == p && ((k - 1 == 0 && y >= 0) || (y >= cur_dif[k-2]))) {
                                        curr_flag = false;
                                        first_handled = true;
                                    }
                                }
                            }
                        }
                    }
                }

                if (curr_flag) {
                    int jp = (!cur_dif.empty()) ? cur_dif.back() : 0;
                    if (!C_inter_N.empty()) {
                        for (int y = 0; y < I; y++) {
                            if (curr_C.find(y) == curr_C.end() && y != I &&
                            T[y] == C_inter_N.size() && S[y] == 0) {
                                if (jp < y) {
                                    curr_flag = false;
                                    break;
                                }
                            }
                        }
                    } else if (jp < I - 1) {
                        curr_flag = false;
                    }
                }
            }
            fill(S.begin(),S.end(),0);
            fill(T.begin(),T.end(),0);
            if (curr_flag) {
                curr_save = set<int>(C_minus_N.begin(), C_minus_N.end());
                curr_state=2;
                set<int> Updated;
                Updated.insert(C_inter_N.begin(), C_inter_N.end());
                Updated.insert(I);
                index_vec.push_back(I + 1);
                CliqueV.push_back(Updated);
                CnN.push_back(vector<int>());
                CminN.push_back(vector<int>());
                indexV.push_back(vector<int>());
                flagV.push_back(true);
                stateV.push_back(0);
                saveV.push_back(set<int>());
                continue;
            } else {
                index_vec.pop_back();
                CliqueV.pop_back();
                CnN.pop_back();
                CminN.pop_back();
                indexV.pop_back();
                flagV.pop_back();
                stateV.pop_back();
                saveV.pop_back();
                continue;
            }
        }

        if (curr_state==2) {
            if (!index_vec.empty()) {
                auto& prev_C = CliqueV.back();
                prev_C.erase(I);
                prev_C.insert(curr_save.begin(), curr_save.end());
            }

            index_vec.pop_back();
            CliqueV.pop_back();
            CnN.pop_back();
            CminN.pop_back();
            indexV.pop_back();
            flagV.pop_back();
            stateV.pop_back();
            saveV.pop_back();
        }
    }

    if (!CliqueV.empty()) {
        C=CliqueV.back();
    }
}
void CLIQUE() {
    int s;
    // Find first non-empty adjacency list
    for(s = 0; s < n && adjMat[s].empty(); s++);
    if (s == n) return;
    
    C = {s};
    S.resize(n, 0);
    T.resize(n, 0);
    UPDATE(s + 1);
}

int main() {
    ifstream infile("./datasets/email-Enron.txt");
    int vert = INT_MIN;
    vector<pair<int, int>> edges;
    int u, v;
    
    // Read edges and find maximum vertex
    while (infile >> u >> v) {
        edges.push_back({u, v});
        vert = max({vert, u, v});
    }
    
    // Initialize adjacency lists with correct size
    adj.resize(vert + 1);
    
    // Build initial adjacency list
    for (const auto& edge : edges) {
        int a = edge.first, b = edge.second;
        adj[a].insert(b);
        adj[b].insert(a);
    }
    
    // Create vertex ordering by degree
    vector<pair<int, int>> degree;
    for(int i = 0; i < vert + 1; i++) {
        degree.push_back({adj[i].size(), i});
    }
    sort(degree.begin(), degree.end());
    
    // Create mapping for new vertex ordering
    vector<int> oldToNew(vert + 1);
    for(int i = 0; i < vert + 1; i++) {
        oldToNew[degree[i].second] = i;
    }
    
    // Initialize final adjacency matrix with new ordering
    adjMat.resize(vert + 1);
    for(int i = 0; i < vert + 1; i++) {
        for (int x : adj[i]) {
            adjMat[oldToNew[i]].insert(oldToNew[x]);
        }
    }
    
    n = vert + 1;
    
    // Run clique finding algorithm and measure time
    auto start = chrono::high_resolution_clock::now();
    CLIQUE();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    
    // Output results
    cout << "\n=== Results ===\n";
    cout << "1. Largest clique size: " << maximC << endl;
    cout << "2. Total number of maximal cliques: " << count_C << endl;
    cout << "3. Execution time: " << duration.count() << " seconds\n";
    cout << "4. Clique size distribution:\n";
    for (const auto& [size, count] : SizeDist) {
        cout << "Size " << size << ": " << count << "\n";
    }
    
    return 0;
}