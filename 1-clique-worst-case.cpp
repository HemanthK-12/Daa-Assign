#include<iostream>
#include<bits/stdc++.h>
#include<chrono>
using namespace std;
unordered_set<int> Q;
unordered_map<int,unordered_set<int>> Graph;
int numCliques = 0; // Variable to count the number of cliques
int maxCliqueSize = 0; // Variable to track the length of the maximum clique
map<int, int> cliqueSizeDistribution; // Track distribution of clique sizes
//as the algorithm involves finding intersections and difference we write the functios for them
unordered_set<int> Intersection(const unordered_set<int> &s1, const unordered_set<int> &s2){
    if(s1.size() > s2.size()) {
        return Intersection(s2, s1);  // Ensure we iterate over smaller set
    }
    unordered_set<int> ans;
    ans.reserve(s1.size());  // Pre-allocate space
    for(const auto &x : s1) {
        if(s2.count(x)) {
            ans.insert(x);
        }
    }
    return ans;
}

unordered_set<int> Difference(const unordered_set<int> &s1, const unordered_set<int> &s2){
    unordered_set<int> ans;
    ans.reserve(s1.size());  // Pre-allocate space
    for(const auto &x : s1) {
        if(!s2.count(x)) {
            ans.insert(x);
        }
    }
    return ans;
}
void expand(unordered_set<int> subg,unordered_set <int> cand){
    if(subg.empty()){
        cliqueSizeDistribution[Q.size()]++; // Track the size of this clique
        numCliques++; // Increment the number of cliques
        maxCliqueSize = max(maxCliqueSize, (int)Q.size()); // Update the maximum clique size
    }
    else{
        // to find a vertex u that maximizies cand U T(u)
        int u=-1;
        int maxx=-1;
        for(auto x:subg){
            //unordered_set<int> temp(Graph[x].begin(),Graph[x].end());
            //we need to maximize the intersection between temp and cand
            int temp_int = Intersection(Graph[x],cand).size();
            if(temp_int>maxx){
                maxx=temp_int;
                u=x;
            } 
        }
        
        unordered_set<int> Ext=Difference(cand,Graph[u]);
        while(!Ext.empty()){
            //choose any vertex from Ext
            
            int q=*Ext.begin();
            Ext.erase(q);
            // cout<<q<<" ";
            Q.insert(q);
            
            // unordered_set<int> subq_q=Intersection(subg,Graph[q]);
            // unordered_set<int> cand_q=Intersection(cand,Graph[q]);
            expand(Intersection(subg,Graph[q]),Intersection(cand,Graph[q]));

            Q.erase(q);
            cand.erase(q);
            // cout<<"back ";
        }
        //cout<<endl; // Add newline after printing "back" operations
    }
   
}
void CLIQUES(){
    Q.clear(); // Clear global variable Q
    unordered_set<int> V; // Set of all vertices
    for (const auto &pair : Graph) {
        V.insert(pair.first);
    }
    expand(V, V); 

}
int main(){
    auto start = chrono::high_resolution_clock::now(); // Start timing

    ifstream infile("./preprocessed-datasets/wiki-Vote.txt");
    string line;
    int u,v;
    unordered_set<int> nodes;
    if (infile.is_open()) {
        // Skip the first 3 lines of comments
        // for (int i=0;i<4;i++) {
        //     getline(infile,line);
        // }
        // Read the edges from the fourth line until the end of the file
        while (infile>>u>>v) {
            
            Graph[u].insert(v);  
            Graph[v].insert(u);   //undirected graph
            nodes.insert(u);
            nodes.insert(v);
        }
        infile.close();
    } 
    CLIQUES();

    auto end=chrono::high_resolution_clock::now();
    chrono::duration<double> duration=end - start;

    // Print results
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