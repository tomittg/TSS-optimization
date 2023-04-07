#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <set>
#include <chrono>

using namespace std;

int POP_SIZE;
int GENERATIONS;
double MUTATION_RATE;
double CROSSOVER_RATE = 0.8;
extern double r;
extern vector<vector<int>> graph; // adjacency list of the graph

struct Graph {
    int V;
    vector<vector<int>> adj;
};

/*void addEdge(Graph &g, int u, int v) {
    g.adj[u].push_back(v);
    g.adj[v].push_back(u);
}*/

vector<int> generate_individual(const Graph &g) {
    vector<int> individual(g.V);
    for (int i = 0; i < g.V; ++i) {
        individual[i] = rand() % 2;
    }
    return individual;
}

vector<vector<int>> initial_population(const Graph &g) {
    vector<vector<int>> population;
    for (int i = 0; i < POP_SIZE; ++i) {
        vector<int> individual(g.V, 0);
        
        population.push_back(generate_individual(g));
       
    }
    return population;
}

int lt_diffusion(const Graph &g, const vector<int> &individual) {
   
    set<int> activated_nodes;

    for (int i = 0; i < g.V; ++i) {
        if (individual[i]) {
            activated_nodes.insert(i);
        }
    }

    bool new_activation = true;

    while (new_activation) {
        new_activation = false;

        for (int u = 0; u < g.V; ++u) {
            if (!activated_nodes.count(u)) {
                int activated_neighbors = 0;
                for (int v : g.adj[u]) {
                    if (activated_nodes.count(v)) {
                        ++activated_neighbors;
                    }
                }
                if (activated_neighbors / static_cast<double>(g.adj[u].size()) >= r) {
                    activated_nodes.insert(u);
                    new_activation = true;
                }
            }
        }
    }

    return g.V - activated_nodes.size();
}

bool compare_individuals(const pair<vector<int>, int> &a, const pair<vector<int>, int> &b) {
    if (a.second == b.second) {
        int active_nodes_a = count(a.first.begin(), a.first.end(), 1);
        int active_nodes_b = count(b.first.begin(), b.first.end(), 1);
        return active_nodes_a < active_nodes_b;
    }
    return a.second < b.second;
}

void mutate(vector<int> &individual) {
    for (int i = 0; i < individual.size(); ++i) {
        if (static_cast<double>(rand()) / RAND_MAX < MUTATION_RATE) {
            individual[i] = 1 - individual[i];
        }
    }
}

vector<int> crossover(const vector<int> &parent1, const vector<int> &parent2) {
    vector<int> offspring(parent1.size());
    for (int i = 0; i < parent1.size(); ++i) {
        offspring[i] = (static_cast<double>(rand()) / RAND_MAX < CROSSOVER_RATE) ? parent1[i] : parent2[i];
    }
    return offspring;
}

vector<vector<int>> selection(const Graph &g, vector<pair<vector<int>, int>> &population_lt) {
    sort(population_lt.begin(), population_lt.end(), compare_individuals);
    vector<vector<int>> new_population;
    int best_size = POP_SIZE / 10;

    for (int i = 0; i < best_size; ++i) {
        new_population.push_back(population_lt[i].first);
    }

    while (new_population.size() < POP_SIZE) {
        int padre1 = rand() % POP_SIZE;
        int padre2 = rand() % POP_SIZE;
        if (padre1 != padre2) {
            vector<int> offspring = crossover(population_lt[padre1].first, population_lt[padre2].first);
            mutate(offspring);
            new_population.push_back(offspring);
        }
    }

    return new_population;
}

set<int> biased_random_genetic_algorithm() {
    Graph g;
    g.V = graph.size();
    g.adj = graph;
    POP_SIZE = 5*graph.size();
    MUTATION_RATE = 1.0 / double(graph.size());
    GENERATIONS = g.V;
    srand(time(nullptr));
    
    vector<vector<int>> population = initial_population(g);

    for (int gen = 0; gen < GENERATIONS; ++gen) {
        vector<pair<vector<int>, int>> population_lt;
        for (const auto &individual : population) {
            population_lt.push_back({individual, lt_diffusion(g, individual)});
        }

        population = selection(g, population_lt);
    }

    vector<pair<vector<int>, int>> population_lt;
    for (const auto &individual : population) {
        population_lt.push_back({individual, lt_diffusion(g, individual)});
    }

    
    vector<int> correctNodes = min_element(population_lt.begin(), population_lt.end(), compare_individuals)->first;
    set<int> setNodes(correctNodes.begin(), correctNodes.end());
    return setNodes;
}

/*int main() {
    
    Graph g;
    char c;
    cin.ignore(7);
    int n, m, v, u;
    cin >> n >> m;
    g.V = n;
    g.adj.resize(g.V);

    for(int i = 0; i < m; ++i){
        cin >> c >> u >> v;
        addEdge(g, u-1, v-1);
    }

//Probad diferentes valores
    POP_SIZE = 5*g.V;
    MUTATION_RATE = 1.0 / double(g.V);
    GENERATIONS = g.V;
auto start_time = std::chrono::high_resolution_clock::now();
    vector<int> solution = biased_random_genetic_algorithm(g);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    cout << "Solution: ";
    int count = 0;
    for (int i = 0; i < solution.size(); ++i) {
        if (solution[i]) {
            cout << i + 1<< " ";
            ++count;
        }
    }
    cout << endl << count << endl;

    cout << "Execution time: " << double (duration)/1000 << endl;
    return 0;

}*/