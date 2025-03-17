#include<iostream>
#include<bits/stdc++.h>
using namespace std;
unordered_set<int> Q;
unordered_map<int,unordered_set<int>> Graph;
int numCliques = 0; // Variable to count the number of cliques
int maxCliqueSize = 0; // Variable to track the length of the maximum clique
//as the algorithm involves finding intersections and difference we write the functios for them
unordered_set<int> Intersection(const unordered_set<int> &s1, const unordered_set<int> &s2){
    unordered_set<int> ans;
    for(auto x:s1){
        if(s2.find(x)!=s2.end()){
            ans.insert(x);
        }
    }
    return ans;
}

unordered_set<int> Difference(const unordered_set<int> &s1, const unordered_set<int> &s2){
    unordered_set<int> ans;
    for(auto x :s1){
        if(s2.find(x)==s2.end()){
            ans.insert(x);
        }

    }
    return ans;
}
void expand(unordered_set<int> subg,unordered_set <int> cand){
    if(subg.empty()){
        // cout<<"CLIQUE ";
        cout<<endl; // Add newline after printing "CLIQUE"
        numCliques++; // Increment the number of cliques
        maxCliqueSize = max(maxCliqueSize, (int)Q.size()); // Update the maximum clique size
        cout<<endl;
    }
    else{
        // to find a vertex u that maximizies cand U T(u)
        int u=-1;
        int maxx=-1;
        for(auto x:subg){
            unordered_set<int> temp(Graph[x].begin(),Graph[x].end());
            //we need to maximize the intersection between temp and cand
            int temp_int = Intersection(temp,cand).size();
            if(temp_int>maxx){
                maxx=temp_int;
                u=x;
            } 
        }
        
        unordered_set<int> Ext=Difference(cand,Graph[u]);
        unordered_set<int> Fini;
        while(!Ext.empty()){
            //choose any vertex from Ext
            
            int q=*Ext.begin();
            Ext.erase(q);
            // cout<<q<<" ";
            Q.insert(q);
            
            unordered_set<int> subq_q=Intersection(subg,Graph[q]);
            unordered_set<int> cand_q=Intersection(cand,Graph[q]);
            expand(subq_q,cand_q);
            
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
    ifstream infile("as-skitter.txt");
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
    cout << "Number of cliques: " << numCliques << endl;
    cout << "Length of maximum clique: " << maxCliqueSize << endl;
    return 0;
}