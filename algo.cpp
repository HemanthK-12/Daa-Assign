#include<iostream>
#include<bits/stdc++.h>
using namespace std;
set<int> Q;
vector<int> Graph[1000];

//as the algorithm involves finding intersections and difference we write the functios for them
set<int> Intersection(const set<int> &s1, const set<int> &s2){
    set<int> ans;
    for(auto x:s1){
        if(s2.find(x)!=s2.end()){
            ans.insert(x);
        }
    }
    return ans;
}

set<int> Difference(const set<int> &s1, const set<int> &s2){
    set<int> ans;
    for(auto x :s1){
        if(s2.find(x)==s2.end()){
            ans.insert(x);
        }

    }
    return ans;
}
void expand(set<int> subg,set <int> cand){
    if(subg.empty()){
        cout<<"CLIQUE ";
        cout<<endl; // Add newline after printing "CLIQUE"
        return;
    }
    else{
        // to find a vertex u that maximizies cand U T(u)
        int u=-1;
        int maxx=-1;
        for(auto x:subg){
            set<int> temp(Graph[x].begin(),Graph[x].end());
            //we need to maximize the intersection between temp and cand
            int temp_int = Intersection(temp,cand).size();
            if(temp_int>maxx){
                maxx=temp_int;
                u=x;
            } 
        }
        set<int> temp2(Graph[u].begin(),Graph[u].end());
        set<int> Ext=Difference(cand,temp2);
        set<int> Fini;
        while(!Ext.empty()){
            //choose any vertex from Ext
            
            int q=*Ext.begin();
            Ext.erase(q);
            cout<<q<<" ";
            Q.insert(q);
            set<int> temp3(Graph[q].begin(),Graph[q].end());
            set<int> subq_q=Intersection(subg,temp3);
            set<int> cand_q=Intersection(cand,temp3);
            expand(subq_q,cand_q);
            cand.erase(q);
            Q.erase(q);
            cout<<"back ";

        }
        //cout<<endl; // Add newline after printing "back" operations
    }
   
}
void CLIQUES(vector<int> Graph[],int n){
Q.clear(); // Clear global variable Q
    set<int> V; // Set of all vertices
    for (int i = 1; i <= n; i++) { // Assuming vertices are 1-indexed
        V.insert(i);
    }
    expand(V, V); 

}
int main(){
    //u is the Number of vertices and v is the numer of edges
    int u,v;
    cout<<"enter";
    cin>>u>>v;
    for(int i=0;i<v;i++){
        int a,b;
        cin>>a>>b;
        Graph[a].push_back(b);  
        Graph[b].push_back(a);   //undirected graph
    }
    CLIQUES(Graph,u);
    return 0;
}