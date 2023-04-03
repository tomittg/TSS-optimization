#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;

using Graph = vector<vector<int>>;

unordered_set<int> efficient_greedy_vertex_cover(Graph &graph) {
    unordered_set<int> vertex_cover;

    // Create a max heap of vertices with their degrees as keys
    priority_queue<pair<int, int>> max_heap;
    for (int i = 0; i < graph.size(); ++i) {
        max_heap.push({graph[i].size(), i});
    }

    while (!max_heap.empty()) {
        // Pop the vertex with the highest degree from the max heap
        int highest_degree_vertex = max_heap.top().second;
        max_heap.pop();

        // If the vertex has no edges, skip it
        if (graph[highest_degree_vertex].empty()) {
            continue;
        }

        // Add the highest-degree vertex to the vertex cover set
        vertex_cover.insert(highest_degree_vertex);

        // Remove all edges connected to the highest-degree vertex from the graph
        for (int neighbor : graph[highest_degree_vertex]) {
            graph[neighbor].erase(remove(graph[neighbor].begin(), graph[neighbor].end(), highest_degree_vertex), graph[neighbor].end());
            max_heap.push({graph[neighbor].size(), neighbor});
        }
        graph[highest_degree_vertex].clear();
    }

    return vertex_cover;
}

int main() {
    Graph graph = {
        {1, 3, 2},
        {0, 2, 3},
        {0, 1, 3},
        {0, 1, 2}
    };

    unordered_set<int> vertex_cover = efficient_greedy_vertex_cover(graph);
    cout << "Efficient Greedy Vertex Cover: ";
    copy(vertex_cover.begin(), vertex_cover.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}