#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "main.hh"
#include "greedyLT.hh"

using namespace std;

extern double r;
extern vector<vector<int>> graph; // adjacency list of the graph
vector<bool> activatedNodes;      // list of nodes that have been activated
int sumActivatedNodes;            // sum of activated nodes

struct comparePairs
{
   bool operator()(const pair<int, int> &p1, const pair<int, int> &p2)
   {
      return p1.second < p2.second;
   }
};

void LTDiffusion(int node, priority_queue<pair<int, int>, vector<pair<int, int>>, comparePairs> &candidateNodes, vector<int> &nodeUpdates, vector<int> &currentDegree)
{
   queue<int> activationQueue;
   activationQueue.push(node);
   activatedNodes[node] = true;
   sumActivatedNodes++;
   for (int neighbourNode : graph[node])
   {
      currentDegree[neighbourNode]--;
      candidateNodes.push({neighbourNode, currentDegree[neighbourNode]});
      nodeUpdates[neighbourNode]++;
   }

   while (!activationQueue.empty())
   {
      int currentNode = activationQueue.front();
      activationQueue.pop();

      for (int adjacentNode : graph[currentNode])
      {
         if (!activatedNodes[adjacentNode] and (graph[adjacentNode].size() - currentDegree[adjacentNode] >= r * graph[adjacentNode].size()))
         {
            activationQueue.push(adjacentNode);
            activatedNodes[adjacentNode] = true;
            sumActivatedNodes++;
            for (int neighbourNode : graph[adjacentNode])
            {
               currentDegree[neighbourNode]--;
               candidateNodes.push({neighbourNode, currentDegree[neighbourNode]});
               nodeUpdates[neighbourNode]++;
            }
         }
      }
   }
}

set<int> greed()
{
   priority_queue<pair<int, int>, vector<pair<int, int>>, comparePairs> candidateNodes;
   activatedNodes = vector<bool> (graph.size(), false);
   sumActivatedNodes = 0;
   vector<int> nodeUpdates(graph.size(), 0);
   vector<int> currentDegree(graph.size(), 0);
   set<int> S;

   for (int i = 1; i < graph.size(); ++i)
   {
      int iDegree = graph[i].size();
      candidateNodes.push({i, iDegree});
      currentDegree[i] = iDegree;
   }

   while (sumActivatedNodes < graph.size() - 1)
   {
      int node = candidateNodes.top().first;
      candidateNodes.pop();
    
      if (nodeUpdates[node] > 0)
         nodeUpdates[node]--;
      else if (not activatedNodes[node])
      {
         LTDiffusion(node, candidateNodes, nodeUpdates, currentDegree);
         S.insert(node);
      }
   }
   return S;
}
