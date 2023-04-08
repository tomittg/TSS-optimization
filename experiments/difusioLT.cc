#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "main.hh"
#include "difusioLT.hh"

using namespace std;

extern double r;
extern vector<vector<int>> graph; // adjacency list of the graph

int LT (set<int> minimumS) {
    vector<bool> bactive (graph.size(), false);
    queue<int> qactive;
    for(auto act : minimumS) {
        qactive.push(act);
        bactive[act] = true;
    }
    int size = minimumS.size();
    while(!qactive.empty()) { 
        int node = qactive.front(); qactive.pop();
        for (auto adj : graph[node]) {
            if(not bactive[adj]) {
                double adj_actives = 0;
                for (auto neighbour_adj : graph[adj]) { //counts the number of actives nodes that has the adjacent node to the active
                    if(bactive[neighbour_adj]) ++adj_actives;
                }
                if (r*graph[adj].size() <= adj_actives) { //looks if the adjacent node to the active could be activated
                    qactive.push(adj);
                    bactive[adj] = true;
                    ++size;
                }
            }
        }
    }
    return size;
}


int LTPercentage (set<int> minimumS, float percentage) {
    vector<bool> bactive (graph.size(), false);
    queue<int> qactive;
    for(auto act : minimumS) {
        qactive.push(act);
        bactive[act] = true;
    }
    int size = minimumS.size();
    while(!qactive.empty() && qactive.size()/100 < percentage) { 
        int node = qactive.front(); qactive.pop();
        for (auto adj : graph[node]) {
            if(not bactive[adj]) {
                double adj_actives = 0;
                for (auto neighbour_adj : graph[adj]) { //counts the number of actives nodes that has the adjacent node to the active
                    if(bactive[neighbour_adj]) ++adj_actives;
                }
                if (r*graph[adj].size() <= adj_actives) { //looks if the adjacent node to the active could be activated
                    qactive.push(adj);
                    bactive[adj] = true;
                    ++size;
                }
            }
        }
    }
    return size;
}