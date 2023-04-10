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
#include "greedyLT.hh"
#include <queue>
#include <unordered_set>

using namespace std;


int POP_SIZE;
int GEN_SIZE;
int GENERATIONS;
double MUTATION_RATE;
double CROSSOVER_RATE = 0.8;
extern vector<vector<int>> graph; // adjacency list of the graph
extern double sizeEdges;
mt19937 generador(random_device{}());

struct Chromosoma {
    vector<bool> genes;
    int fitness;
    bool evualated;
};

bool equalChromosoma(Chromosoma a,Chromosoma b){
    for(int i = 0 ; i < a.genes.size();++i){
        if(a.genes[i] != b.genes[i]) return false;
    }
    return true;
}

void mutate(Chromosoma &individual, double Mutation_Rate ) {
    for (int i = 1; i < individual.genes.size(); ++i) {
        if (individual.genes[i] && static_cast<double>(rand()) / RAND_MAX < Mutation_Rate) {
            individual.genes[i] = 1 - individual.genes[i];
        }
    }
}

int calcularFitness1(vector<bool>& genes){
    set<int> setNodes;
    for(int i = 1; i < genes.size();++i){
        if(genes[i]){
            setNodes.insert(i);
        }
    }
    int LTentero = LT(setNodes);
    return LTentero;
}

Chromosoma generate_individual(Chromosoma gredy) {
    /*Chromosoma individual;
    individual.evualated = false;
    individual.fitness = 0.0;*/
    mutate(gredy,0.25);
    //individual.genes.resize(GEN_SIZE);
    /*for (int i = 1; i < individual.genes.size(); ++i) {
        individual.genes[i] = rand() % 2;
    }*/
    return gredy;
}

vector<Chromosoma> initial_population() {
    set<int> gredy = greed();
    Chromosoma a;
    vector<Chromosoma> population;
    for (int i = 0; i < POP_SIZE; ++i) {
        if(i == 0) {
            a.genes.resize(GEN_SIZE);
            for(int gred: gredy){
               a.genes[gred] = true; 
            }
            population.push_back(a);
        }
        else{
            population.push_back(generate_individual(a));
        }
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
        if (!hijos[i].evualated) { 
            //pair<vector<bool>,int> fitness  = calcularFitness(hijos[i].genes);
            hijos[i].fitness = calcularFitness1(hijos[i].genes);
            hijos[i].evualated = true;
            if(i == 0) {
                int a = count(hijos[i].genes.begin(),hijos[i].genes.end(),1);
            }
        }
    }
}

Chromosoma crossover(const Chromosoma &parent1, const Chromosoma&parent2) {
    Chromosoma offspring;
    offspring.evualated = false;
    offspring.fitness = 0.0;
    offspring.genes.resize(parent1.genes.size());
    for (int i = 1; i < parent1.genes.size(); ++i) {
        offspring.genes[i] = (static_cast<double>(rand()) / RAND_MAX < CROSSOVER_RATE) ? parent1.genes[i] : parent2.genes[i];
    }
    return offspring;
}

pair<int,int> seleccion_torneo(const vector<Chromosoma>& poblacion, int tam_torneo) {
    int idx_padre1 = rand() % (poblacion.size());
    for (int i = 0; i < tam_torneo - 1; i++) {
        int idx = rand() % (poblacion.size());
        if (poblacion[idx].fitness > poblacion[idx_padre1].fitness) {
            idx_padre1 = idx;
        }
    }
    int idx_padre2 = rand() % (poblacion.size());
    for (int i = 0; i < tam_torneo - 1; i++) {
        int idx = rand() % (poblacion.size());
        if (poblacion[idx].fitness > poblacion[idx_padre2].fitness) {
            idx_padre2 = idx;
        }
    }
    return make_pair(idx_padre1,idx_padre2);
}

vector<Chromosoma> selection(vector<Chromosoma> &population_lt) {
    sort(population_lt.begin(), population_lt.end(), compare_individuals);
    vector<Chromosoma> new_population;
    for(int i = 0; i < population_lt.size()/10;++i) {
        new_population.push_back(population_lt[i]);
    }
    while(new_population.size() < population_lt.size()) {
        uniform_int_distribution<int> distribucion(0, population_lt.size()-1);
        int padre1 = distribucion(generador);
        int padre2 = distribucion(generador);
        int superar = population_lt[padre1].fitness >= population_lt[padre2].fitness ? padre1 : padre2;
        int fitnessPadre = population_lt[superar].fitness;
        if(padre1 != padre2){
            Chromosoma offspring = crossover(population_lt[padre1], population_lt[padre2]);
            mutate(offspring,MUTATION_RATE);
            int fitnesHijo = calcularFitness1(offspring.genes);
            //if((fitnesHijo > fitnessPadre) || (fitnesHijo == fitnessPadre && count(offspring.genes.begin(),offspring.genes.end(),1) <= count(population_lt[superar].genes.begin(),population_lt[superar].genes.end(),1))) {
                offspring.fitness = fitnesHijo;
                offspring.evualated = true;
                new_population.push_back(offspring);
            //}
        }
    }
    sort(new_population.begin(), new_population.end(), compare_individuals);
    return new_population;
}

set<int> algorimtoGenetico() {
    int ratio = 4 ;
    POP_SIZE = 200;
    GEN_SIZE = graph.size();
    MUTATION_RATE = (1/graph.size());
    GENERATIONS = 200; //graph.size();
    srand(time(nullptr));
    
    vector<Chromosoma> population = initial_population();
    bool found = false;
    int gen = 0;
    int geneReapet = 0;
    Chromosoma fitnessBest;

    while(gen < GENERATIONS && !found) {
        cout << "generacion: " << gen << endl;
        evaluar_population(population);
        population = selection(population);
        if(gen == 0){
            fitnessBest = population[0];
        }
        bool equal = equalChromosoma(population[0],fitnessBest);
        if(equal && geneReapet > ratio){
            found = true;
        }
        else if(equal){
            ++geneReapet;
        }
        else {
            fitnessBest = population[0];
            geneReapet = 0;
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
