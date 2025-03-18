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

vector<int> findDegeneracyOrdering(int vertices,vector<pair<int,int>>& edges)
{
    vector<vector<int>> graph(vertices+1);//since 1 based indexing
    for (auto [u,v]:edges) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    vector<int> degree(vertices+1);
    int maxDeg = 0;
    for (int i = 1; i <= vertices; i++)
    {
        degree[i]=graph[i].size();//no. of elements in adjacency list of that element=neighbours=degree
        maxDeg=max(maxDeg,degree[i]);
    }
    vector<list<int>> buckets(maxDeg+1);//each bucket correspods to all the vertices removed while progressing from k-core to k+1 core of the graph
    for (int i = 1; i <= vertices; i++)
        buckets[degree[i]].push_back(i);
    vector<bool> removed(vertices+1,false);
    vector<int> ordering;
    ordering.reserve(vertices);// says that minimum "vertices" number of indices should be there, see here : https://cplusplus.com/reference/vector/vector/reserve/

    for (int i=0;i<vertices;i++)
    {
        int d=0;
        while(d <= maxDeg && buckets[d].empty())
            d++;
        int v=buckets[d].front();
        buckets[d].pop_front();
        removed[v]=true;
        ordering.push_back(v);
        for (int u : graph[v])
        {
            if (!removed[u])
            {
                buckets[degree[u]].remove(u);
                degree[u]--;
                buckets[degree[u]].push_back(u);
            }
        }
    }
    return ordering;
}
void bronKerboschPivot(set<int> R, set<int> P, set<int> X, const vector<vector<int>>& graph, int& count/*reference here coz we are making changes in the count variable in bronkerboschdegeneracy method*/) {
    //if there are no common elements and we are only left with all vertices with degree>=d, we say that is maximal clique
    if (P.empty() && X.empty())
    {
        cout << "\nMaximal Clique: \n";
        for (int v:R)
            cout << v << " ";
        count++;
        return;
    }
    //we use pivoting to randomly pick the top element of the set so that we stop recursive calls when we reach there
    int pivot=*P.begin();
    for (int v : P)
        if (graph[v].size()>graph[pivot].size())
            pivot=v;
    set<int> non_neighbors;
    for (int v : P)
        if (find(graph[pivot].begin(),graph[pivot].end(),v)==graph[pivot].end())//used graph[pivot].find(x) but apparently not supported by #include<its/stdc++.h>, only by #include<algorithm>
            non_neighbors.insert(v);
    //below is same as bronKerboschDegeneracy ka recursive step
    for (int v : non_neighbors) {
        set<int> newR=R;
        set<int> newP,newX;
        newR.insert(v);
        for (int u : graph[v])
        {
            if (P.count(u))
                newP.insert(u);
            if (X.count(u))
                newX.insert(u);
        }
        bronKerboschPivot(newR,newP,newX,graph,count);
        P.erase(v);
        X.insert(v);
    }
}

int bronKerboschDegeneracy(int vertices,vector<pair<int,int>>& edges)//& only because giving pointer deferenceing exception
{
    vector<vector<int>> graph(vertices+1);
    for (auto [u,v]:edges) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    //first we find degeneracy ordering which will be passed as input to bronKerboschpivoting function
    vector<int> ordering=findDegeneracyOrdering(vertices,edges);
    set<int> P,R,X; //used vector first but should eliminate duplicates so set best option
    for (int i=1;i<vertices+1;i++)//if ordering is not continuous, should add map here instead of inserting i
        P.insert(i);
    int count=0;//only prints total number of cliques

    for (int i=0;i<ordering.size();i++)
    {
        set<int> newP,newX;
        for (int u:graph[ordering[i]])//this iterates over all neighbours of the ith vertex in ordering
        {
            //see if they are in both the {neighbours} set and {all vertices after ith vertex in ordering} set.
            if (P.count(u))
                newP.insert(u);
            if (X.count(u))
                newX.insert(u);
        }
        bronKerboschPivot({ordering[i]}, newP, newX, graph, count);//we keep on iterating till we go to a leaf node where neighbours is either 1/0, then P&X will be empty and then we'll break from recursive step
        P.erase(ordering[i]);
        X.insert(ordering[i]);//to mark that we won't be using this vertex, i.e. this is visited, we add it to X to remind it should be excluded
    }
    return count;
}

int main() {
    int verticesNumber,edgesNumber;
    cout<<"Enter number of vertices and edges: \n";
    cin>>verticesNumber>>edgesNumber;
    vector<pair<int,int>> edges;
    for (int i=0;i<edgesNumber;i++)
    {
        int a,b;
        cin>>a>>b;
        edges.push_back({a,b});
    }
    cout<<"final no. of cliques with edited bron kerbosch"<<bronKerboschDegeneracy(verticesNumber,edges)<<endl;
    return 0;
}
