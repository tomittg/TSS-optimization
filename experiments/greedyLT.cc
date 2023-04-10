#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "main.hh"
#include "greedyLT.hh"

using namespace std;

extern double r;
extern vector<vector<int>> graph; // adjacency list of the graph
vector<bool> activatedNodesv2;      // list of nodes that have been activated
int sumActivatedNodesv2;            // sum of activated nodes

struct comparePairs
{
   bool operator()(const pair<int, int> &p1, const pair<int, int> &p2)
   {
      return p1.second < p2.second;
   }
};

void LTDiffusionv2(int node, priority_queue<pair<int, int>, vector<pair<int, int>>, comparePairs> &candidateNodes, vector<int> &nodeUpdates, vector<int> &currentDegree)
{
   queue<int> activationQueue;
   activationQueue.push(node);
   activatedNodesv2[node] = true;
   sumActivatedNodesv2++;
   for (int neighbourNode : graph[node])
   {
      if(!activatedNodesv2[neighbourNode]) {
         currentDegree[neighbourNode]--;
         candidateNodes.push({neighbourNode, currentDegree[neighbourNode]});
         nodeUpdates[neighbourNode]++;
      }
   }

   while (!activationQueue.empty()) 
   {
        int currentNode = activationQueue.front();
        activationQueue.pop();

        for (int adjacentNode : graph[currentNode])
        {
            if (!activatedNodesv2[adjacentNode] and (graph[adjacentNode].size() - currentDegree[adjacentNode] >= r * graph[adjacentNode].size()))
            {
                activationQueue.push(adjacentNode);
                activatedNodesv2[adjacentNode] = true;
                sumActivatedNodesv2++;
                for (int neighbourNode : graph[adjacentNode])
                {
                    if(!activatedNodesv2[neighbourNode]){
                        currentDegree[neighbourNode]--;
                        candidateNodes.push({neighbourNode, currentDegree[neighbourNode]});
                        nodeUpdates[neighbourNode]++;
                    }
                }
            }
        }
    }
}

set<int> greed()
{
   priority_queue<pair<int, int>, vector<pair<int, int>>, comparePairs> candidateNodes;
   activatedNodesv2 = vector<bool> (graph.size(), false);
   sumActivatedNodesv2 = 0;
   vector<int> nodeUpdates(graph.size(), 0);
   vector<int> currentDegree(graph.size(), 0);
   set<int> S;

   for (int i = 1; i < graph.size(); ++i)
   {
      int iDegree = graph[i].size();
      candidateNodes.push({i, iDegree});
      currentDegree[i] = iDegree;
   }

   while (sumActivatedNodesv2 < graph.size() - 1)
   {
      int node = candidateNodes.top().first;
      candidateNodes.pop();
    
      if (nodeUpdates[node] > 0)
        nodeUpdates[node]--;
      else if (not activatedNodesv2[node])
      {
         LTDiffusionv2(node, candidateNodes, nodeUpdates, currentDegree);
         S.insert(node);
      }
   }
   return S;
}
