#include<iostream>
#include<bits/stdc++.h>
#include<chrono>
#include<fstream>
#include<string>
using namespace std;

set<int> Q;
map<int,set<int>> Graph;
int numCliques = 0;
int maxCliqueSize = 0;
map<int, int> cliqueSizeDistribution;

set<int> Intersection(const set<int> &s1, const set<int> &s2){
    set<int> ans;
    set_intersection(s1.begin(), s1.end(), 
                    s2.begin(), s2.end(),
                    inserter(ans, ans.begin()));
    return ans;
}

set<int> Difference(const set<int> &s1, const set<int> &s2){
    set<int> ans;
    set_difference(s1.begin(), s1.end(),
                  s2.begin(), s2.end(),
                  inserter(ans, ans.begin()));
    return ans;
}

void expand(set<int> subg, set<int> cand){
    if(subg.empty()){
        cliqueSizeDistribution[Q.size()]++; // Track the size of this clique
        numCliques++; // Increment the number of cliques
        maxCliqueSize = max(maxCliqueSize, (int)Q.size()); // Update the maximum clique size
    }
    else{
        int u = -1;
        int maxx = -1;
        // Modified pivot selection with tie-breaking
        for(auto x : subg){
            set<int> temp(Graph[x].begin(), Graph[x].end());
            int temp_int = Intersection(temp,cand).size();
            if(temp_int > maxx || (temp_int == maxx && x < u)){  // Added vertex ID as tie-breaker
                maxx = temp_int;
                u = x;
            } 
        }
        
        set<int> Ext=Difference(cand,Graph[u]);
        while(!Ext.empty()){
            int q=*Ext.begin();
            Ext.erase(q);
            Q.insert(q);
            
            set<int> subq_q=Intersection(subg,Graph[q]);
            set<int> cand_q=Intersection(cand,Graph[q]);
            expand(subq_q,cand_q);
            
            Q.erase(q);
            cand.erase(q);
        }
    }
}

void CLIQUES(){
    Q.clear();
    set<int> V;
    for (const auto &pair : Graph) {
        V.insert(pair.first);
    }
    expand(V, V); 
}

int main(){
    auto start = chrono::high_resolution_clock::now(); // Start timing
    int choice;
    cout<<"Enter the dataset number : \n1 for Enron email network\n2 for Wiki-Vote network \n3 for Skitter\n";
    cin>>choice;
    ifstream infile;
    if(choice==1)
    {
        infile.open("./datasets/wiki-Vote.txt");
        string line;
        for(int i = 0; i < 4; i++)
            getline(infile, line);
    }
    else if(choice==2)
    {
        infile.open("./datasets/email-Enron.txt");
        string line;
        for(int i = 0; i < 4; i++)
            getline(infile, line);
    }
    else
    {
        infile.open("./datasets/as-skitter.txt");
        string line;
        for(int i = 0; i < 5; i++)
            getline(infile, line);
    }
    int u,v;
    set<int> nodes;
    if (infile.is_open()) {
        while (infile>>u>>v) {
            Graph[u].insert(v);  
            Graph[v].insert(u);
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