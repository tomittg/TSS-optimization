#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <random>
#include <algorithm>
#include <chrono>


using namespace std;

double P = 0.5;

vector<vector<int>> graph;  // adjacency list of the graph


struct comparePairs {
    bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) {
        return p1.second < p2.second;
    }
};

void readGraph();

int localSearchLTDiffusion(int node, vector<bool>& activatedNodes, vector<int>& currentDegree);

int localSearchLT(int node, vector<bool> activatedNodes, vector<int> currentDegree);

set<int> localSearch();

int main() {
   srand(time(NULL));

   readGraph();

   // Obtener el tiempo actual
    auto inicio = chrono::high_resolution_clock::now();


   set<int> minimumS = localSearch();

   // Obtener el tiempo después de la ejecución de la función
    auto fin = chrono::high_resolution_clock::now();

    // Calcular la duración de la ejecución en segundos
    auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count() / 1000000.0;

    // Imprimir la duración de la ejecución en segundos
    cout << "Tiempo de ejecución: " << duracion << " segundos" << endl;

   cout << minimumS.size() << endl;

   for (int n : minimumS){
      cout << n << endl;
   }

   return 0;
}

void readGraph(){
    int n, m;  // number of vertices and edges in the graph
    char c;
    
    cin.ignore(7);
    cin >> n >> m;

    graph.resize(n+1);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> c >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
}

int localSearchLTDiffusion(int node, vector<int>& nodes, vector<bool>& activatedNodes, vector<int>& currentDegree){
   queue<int> activationQueue;
   activationQueue.push(node);
   activatedNodes[node] = true;
   int sumActivatedNodes = 1;
      
   for(int neighbourNode : graph[node]){
      currentDegree[neighbourNode]--;
   }

   while(!activationQueue.empty()){
      int currentNode = activationQueue.front();
      activationQueue.pop();
      for (int adjacentNode : graph[currentNode]) { 
         if (!activatedNodes[adjacentNode] and (graph[adjacentNode].size()-currentDegree[adjacentNode] >= P*graph[adjacentNode].size())){
            activationQueue.push(adjacentNode);
            activatedNodes[adjacentNode] = true; 
            sumActivatedNodes++;
            for(int neighbourNode : graph[adjacentNode]){
               currentDegree[neighbourNode]--;
            }
         }
      }
   }
   return sumActivatedNodes;
}

int localSearchLT(int node, vector<bool> activatedNodes, vector<int> currentDegree){
   queue<int> activationQueue;
   activationQueue.push(node);
   activatedNodes[node] = true;
   int sumActivatedNodes = 1;

   for(int neighbourNode : graph[node]){
      currentDegree[neighbourNode]--;
   }

   while(!activationQueue.empty()){
      int currentNode = activationQueue.front();
      activationQueue.pop();
      for (int adjacentNode : graph[currentNode]) { 
         if (!activatedNodes[adjacentNode] and (graph[adjacentNode].size()-currentDegree[adjacentNode] >= P*graph[adjacentNode].size())){
            activationQueue.push(adjacentNode);
            activatedNodes[adjacentNode] = true; 
            sumActivatedNodes++;
            for(int neighbourNode : graph[adjacentNode]){
               currentDegree[neighbourNode]--;
            }
         }
      }
   }
   return sumActivatedNodes;
}

set<int> localSearch(){
   vector<int> currentDegree(graph.size());
   vector<int> nodes(graph.size()-1);
   vector<bool> activatedNodes (graph.size(), false);  // list of nodes that have been activated
   int sumActivatedNodes = 0;  // sum of activated nodes 
   set<int> S;

   for (int i = 1; i < graph.size(); ++i){
      int iDegree = graph[i].size();
      currentDegree[i] = iDegree;
      nodes[i-1] = i;
   }

   mt19937 mt{random_device{}()};
   shuffle(nodes.begin(), nodes.end(), mt);

   pair<int,int> maxActivated = make_pair(-1,-1); //
   while(nodes.size() > 0 and sumActivatedNodes < graph.size()-1) {
      maxActivated = make_pair(-1, -1);
      for(int i = 0; i < nodes.size(); ++i) {
         if(not activatedNodes[nodes[i]]) {
            int activated = localSearchLT(nodes[i], activatedNodes, currentDegree);
            if(activated>maxActivated.second) maxActivated = make_pair(i, activated);
         }
         else nodes.erase(nodes.begin()+i);
      }
      sumActivatedNodes += localSearchLTDiffusion(nodes[maxActivated.first], nodes, activatedNodes,  currentDegree);
      S.insert(nodes[maxActivated.first]);
      nodes.erase(nodes.begin()+maxActivated.first);
   }
   return S;
}
