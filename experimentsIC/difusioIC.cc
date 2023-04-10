#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "main.hh"
#include "difusioIC.hh"

using namespace std;

extern double p;
extern vector<vector<int>> graph; // adjacency list of the graph

int IC (set<int> minimumS) {
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
            if(not bactive[adj] and ((double)rand() / RAND_MAX < p)) {
                    qactive.push(adj);
                    bactive[adj] = true;
                    ++size;
            }
        }
    }
    return size;
}

