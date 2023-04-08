#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <set>
#include <chrono>
#include <random>
#include "main.hh"
#include "genetico.hh"
#include "difusioLT.hh"

using namespace std;


int POP_SIZE;
int GEN_SIZE;
int GENERATIONS;
double MUTATION_RATE;
double CROSSOVER_RATE = 0.8;
extern vector<vector<int>> graph; // adjacency list of the graph

struct Chromosoma {
    vector<int> genes;
    float fitness;
    bool evualated;
};

int calcularFitness(vector<int>& genes){
    set<int> setNodes;
    for(int i = 0; i < genes.size();++i){
        if(genes[i] > 0){
            setNodes.insert(i);
        }
    }
    int LTentero = LTPercentage(setNodes,0.2);
    return LTentero;
}

Chromosoma generate_individual() {
    Chromosoma individual;
    individual.evualated = false;
    individual.fitness = 0.0;
    individual.genes.resize(GEN_SIZE);
    
    for (int i = 0; i < individual.genes.size(); ++i) {
        individual.genes[i] = rand() % 2;
    }

    return individual;
}

vector<Chromosoma> initial_population() {
    vector<Chromosoma> population;
    for (int i = 0; i < POP_SIZE; ++i) {
        population.push_back(generate_individual());
       
    }
    return population;
}

bool compare_individuals(const Chromosoma &a, Chromosoma &b) {
    int evalA = a.fitness;
    int evalB = b.fitness;
    if(evalA == evalB){
        int active_nodes_a = count(a.genes.begin(), a.genes.end(), 1);
        int active_nodes_b = count(b.genes.begin(), b.genes.end(), 1);
        return active_nodes_a < active_nodes_b;
    }
    return evalA > evalB;
}

void evaluar_population(std::vector<Chromosoma>& hijos) {
    for (int i = 0; i < hijos.size(); i++) {
        if (!hijos[i].evualated) {  // Comprobar si el individuo ha sido evaluado en la generación actual
            // Evaluar el individuo y actualizar su valor objetivo y su variable "evaluado"
            int fintnes = calcularFitness(hijos[i].genes);
            float fitness  = calcularFitness(hijos[i].genes);
            hijos[i].fitness = fitness;
            hijos[i].evualated = true;
        }
    }
}

void mutate(Chromosoma &individual) {
    for (int i = 0; i < individual.genes.size(); ++i) {
        if (static_cast<double>(rand()) / RAND_MAX < MUTATION_RATE) {
            individual.genes[i] = 1 - individual.genes[i];
        }
    }
}

Chromosoma crossover(const Chromosoma &parent1, const Chromosoma&parent2) {
    Chromosoma offspring;
    offspring.evualated = false;
    offspring.fitness = 0.0;
    offspring.genes.resize(parent1.genes.size());
    for (int i = 0; i < parent1.genes.size(); ++i) {
        offspring.genes[i] = (static_cast<double>(rand()) / RAND_MAX < CROSSOVER_RATE) ? parent1.genes[i] : parent2.genes[i];
    }
    return offspring;
}

pair<int,int> seleccion_torneo(const vector<Chromosoma>& poblacion, int tam_torneo) {
    int idx_padre1 = rand() % (poblacion.size()/4);
    for (int i = 0; i < tam_torneo - 1; i++) {
        int idx = rand() % (poblacion.size()/4);
        if (poblacion[idx].fitness > poblacion[idx_padre1].fitness) {
            idx_padre1 = idx;
        }
    }
    int idx_padre2 = rand() % (poblacion.size()/4);
    for (int i = 0; i < tam_torneo - 1; i++) {
        int idx = rand() % (poblacion.size()/4);
        if (poblacion[idx].fitness > poblacion[idx_padre2].fitness) {
            idx_padre2 = idx;
        }
    }
    return make_pair(idx_padre1,idx_padre2);
}

vector<Chromosoma> selection(vector<Chromosoma> &population_lt) {
    sort(population_lt.begin(), population_lt.end(), compare_individuals);
    /*if(population_lt[0].fitness == graph.size()){
        vector<Chromosoma> end;
        end.push_back(population_lt[0]);
        return end;
    }*/
    
    vector<Chromosoma> new_population;
    int best_size = POP_SIZE / 10;

    for (int i = 0; i < best_size; ++i) {
        new_population.push_back(population_lt[i]);
    }

    while (new_population.size() < POP_SIZE) {
        pair<int,int> parents = seleccion_torneo(population_lt,100);
        int padre1 = parents.first;
        int padre2 = parents.second;
        if (padre1 != padre2) {
            Chromosoma offspring = crossover(population_lt[padre1], population_lt[padre2]);
            mutate(offspring);
            new_population.push_back(offspring);
        }
    }

    return new_population;
}

set<int> algorimtoGenetico() {
    std::random_device rd;
    std::mt19937 generador(rd());
    POP_SIZE = graph.size();
    GEN_SIZE = graph.size();
    MUTATION_RATE = 1/graph.size();
    GENERATIONS = graph.size()/4;
    srand(time(nullptr));
    
    vector<Chromosoma> population = initial_population();
    bool found = false;
    int gen = 0;

    while(gen < GENERATIONS && !found) {
        evaluar_population(population);
        population = selection(population);
        if(population.size() == 1){
            found = true;
        }
        ++gen;
    }
    
    evaluar_population(population);
    sort(population.begin(), population.end(), compare_individuals);

    set<int> setNodes;
    for(int i = 0; i < population[0].genes.size();++i){
        if(population[0].genes[i] > 0){
            setNodes.insert(i);
        }
    }
    
    return setNodes;
}

