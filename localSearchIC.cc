#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

double P = 0.5;

vector<vector<int>> graph;  // adjacency list of the graph
vector<bool> activatedNodes;  // list of nodes that have been activated
int sumActivatedNodes;  // sum of activated nodes 

struct comparePairs {
    bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) {
        return p1.second < p2.second;
    }
};

void readGraph();

void ICDiffusion(int node, priority_queue<pair<int,int>, vector<pair<int,int>>, comparePairs>& candidateNodes, vector<int>& nodeUpdates, vector<int>& currentDegree);

set<int> greed();

int main() {
   srand(time(NULL));

    readGraph();
    set<int> minimumS = greed();

    for (int n : minimumS){
        cout << n << " ";
    }
    cout << endl;

    return 0;
}

void readGraph(){
    int n, m;  // number of vertices and edges in the graph
    char c;
    cin.ignore(7);
    cin >> n >> m;
    graph.resize(n+1);
    activatedNodes.resize(n+1);
    sumActivatedNodes = 0;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> c >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
}

void ICDiffusion(int node, priority_queue<pair<int,int>, vector<pair<int,int>>, comparePairs>& candidateNodes, vector<int>& nodeUpdates, vector<int>& currentDegree){
   queue<int> activationQueue;
   activationQueue.push(node);
   activatedNodes[node] = true;
   sumActivatedNodes++;

   while(!activationQueue.empty()){
      int currentNode = activationQueue.front();
      activationQueue.pop();
      for (int adjacentNode : graph[currentNode]) {
         if (!activatedNodes[adjacentNode] and ((double)rand() / RAND_MAX < P)) {
            activationQueue.push(adjacentNode);
            activatedNodes[adjacentNode] = true; 
            sumActivatedNodes++;
            for(int neighbourNode : graph[adjacentNode]){
               currentDegree[neighbourNode]--;
               candidateNodes.push({neighbourNode, currentDegree[neighbourNode]});
               nodeUpdates[neighbourNode]++;
            }
         }
      }
   }
}

set<int> greed(){
   priority_queue<pair<int,int>, vector<pair<int,int>>, comparePairs> candidateNodes;
   vector<int> nodeUpdates(graph.size());
   vector<int> currentDegree(graph.size());
   set<int> S;

   for (int i = 1; i < graph.size(); ++i){
      int iDegree = graph[i].size();
      candidateNodes.push({i, iDegree});
      currentDegree[i] = iDegree;
   }

   while(sumActivatedNodes < graph.size()-1){
      int node = candidateNodes.top().first;
      candidateNodes.pop();
      
      if(nodeUpdates[node] > 0) nodeUpdates[node]--;

      else if(!activatedNodes[node]){
         ICDiffusion(node, candidateNodes, nodeUpdates, currentDegree);
         S.insert(node);
      }
   }
   return S;
}