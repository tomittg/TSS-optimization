#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

using namespace std;


vector<vector<int>> parseEdges(const string& inputString, vector<vector<int>>& adjacency_list){
    const char inner_sep = '-';
    const char outer_sep = ' ';

    vector<vector<int>> result;
    vector<int> edge(2);

    string token;
    istringstream tokenStream(inputString);

    while (getline(tokenStream, token, outer_sep)) {
        istringstream edgeStream(token);
        getline(edgeStream, token, inner_sep);
        edge[0] = stoi(token);
        getline(edgeStream, token, inner_sep);
        edge[1] = stoi(token);
        adjacency_list[edge[0]].push_back(edge[1]);
        adjacency_list[edge[1]].push_back(edge[0]);
        result.push_back(edge);
    }

    return result;
}

unordered_set<int> parseInitialSet(const string& inputString){
    const char sep = ' ';

    unordered_set<int> S;

    string token;
    istringstream tokenStream(inputString);

    while(getline(tokenStream, token, sep)){
        S.insert(stoi(token));
    }
    
    return S;
}

unordered_set<int> icDiffusion(vector<vector<int>> adjacency_list, unordered_set<int> S, double p) {
    // Initialize the set of activated nodes to the seed set
    unordered_set<int> A = S;
    // Initialize the queue of newly activated nodes
    queue<int> Q;
    for (int s : S) {
        Q.push(s);
    }
    // While there are still nodes in the queue
    while (!Q.empty()) {
        // Dequeue the next node
        int v = Q.front();
        Q.pop();
        // Iterate over v's neighbors
        for (int edge : adjacency_list[v]) {
            if (A.find(edge) == A.end()) {
                // If the neighbor is not already activated, activate it with probability p
                if ((double)rand() / RAND_MAX < p) {
                    A.insert(edge);
                    Q.push(edge);
                }
            }
        }
    }
    // Return the set of activated nodes
    return A;
}

int main() {
    int size;
    cin >> size;
    cin.ignore();
    
    vector<vector<int>> adjacency_list(size);
    vector<vector<int>> G;
    unordered_set<int> S;
    float P;
    string line;
    unordered_set<int> A;
    int numberOfAttempts;
    vector<int> nOfTimesReachedNode(size);
    float numberOfCompletions = 0;

    getline(cin, line);
    G = parseEdges(line, adjacency_list);

    getline(cin, line);
    S = parseInitialSet(line);

    cin >> P;
    cin.ignore();
    
    cin >> numberOfAttempts;
    cin.ignore();

    
    srand(time(NULL));
    int totalNodesReached = 0;
    for(int i = 0; i < numberOfAttempts; i++){
        A = icDiffusion(adjacency_list, S, P);
        for (int a : A){
            nOfTimesReachedNode[a]++;
        }
        int resultSize = A.size();
        totalNodesReached += resultSize;
        if (resultSize == size) numberOfCompletions++;
    }
    float averageNodesReached = totalNodesReached/numberOfAttempts;
    float averagePercReached = averageNodesReached/size;
    cout << "Probability: " << P << endl;
    cout << "Number of attempts: " << numberOfAttempts << endl;
    cout << "Graph size: " << size << endl;
    cout << "Starting active nodes size: " << S.size() << endl;
    cout << "Average nodes reached: " << averageNodesReached << endl;
    cout << "Average for each node: " << endl;
    for (int i = 0; i < nOfTimesReachedNode.size(); i++){
        cout << "\t" << i << " -> " << float(nOfTimesReachedNode[i])/float(numberOfAttempts) << endl;
    }
    cout << endl;
    cout << "Average part of the graph reached: " << averagePercReached << endl;
    cout << "Percentage of times the graph was fully reached: " << numberOfCompletions/float(numberOfAttempts) << endl;
    return 0;
}