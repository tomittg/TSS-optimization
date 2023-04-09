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
#include <queue>
#include <unordered_set>

using namespace std;


int POP_SIZE;
int GEN_SIZE;
int GENERATIONS;
double MUTATION_RATE;
double CROSSOVER_RATE = 0.8;
extern vector<vector<int>> graph; // adjacency list of the graph
mt19937 generador(random_device{}());

struct Chromosoma {
    vector<bool> genes;
    vector<bool> activos;
    int fitness;
    bool evualated;
};

pair<vector<bool>,int> calcularFitness(vector<bool>& genes){
    set<int> setNodes;
    for(int i = 0; i < genes.size();++i){
        if(genes[i]){
            setNodes.insert(i);
        }
    }
    pair<vector<bool>,int> LTentero = LTBool(setNodes);
    return LTentero;
}

/*Idea todos los nodos diferentes que activarian */
int calcularFitness2(vector<bool>& cromosoma){
    vector<int> vectorA;
    for(int i = 0; i < cromosoma.size(); ++i){
        if(cromosoma[i]){
            vectorA.push_back(i);
        }
    }
    //cout << "calcularFitness2 vectorA " << vectorA.size() << endl;
    set<int> vec;
    for(int i = 0; i < vectorA.size(); ++i){
        for(int j = 0; j < graph[i].size();++j) {
            vec.insert(j);
        }
    }
    
    //cout << "calcularFitness2 " << vec.size() << endl;
    return vec.size();
}

int calcularFitnessHijo(vector<bool>& cromosoma, vector<bool>& solutionFather){
    set<int> setNodes;
    for(int i = 0; i < cromosoma.size();++i){
        if(cromosoma[i]){
            setNodes.insert(i);
        }
    }
    /*int numVertices = solutionFather.size();
    int numVerticesCubiertos = 0;
    vector<bool> cubiertos(numVertices, false);
    
    // Recorremos los vértices del cromosoma
    for(int i = 0; i < numVertices; i++) {
        if(cromosoma[i]) { // Si el vértice está incluido en la solución
            if(!solutionFather[i]) numVerticesCubiertos++;
            cubiertos[i] = true;
            
            // Marcamos como cubiertos todos los vértices adyacentes al vértice i
            for(int j = 0; j < numVertices; j++) {
                int adj_actives = 0;
                if(!cubiertos[j]) {
                    for (auto neighbour_adj : graph[j]) { 
                        //counts the number of actives nodes that has the adjacent node to the active
                        if(solutionFather[neighbour_adj]) ++adj_actives;
                    }
                    if(r*graph[j].size() <= adj_actives){
                        cubiertos[j] = true;
                        numVerticesCubiertos++;
                    }
                }
            }
        }
    }*/
    
    // El fitness es el número de vértices cubiertos
    int numVerticesCubiertos = LT(setNodes);
    return numVerticesCubiertos;
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
    return evalA < evalB;
}

void evaluar_population(std::vector<Chromosoma>& hijos) {
    for (int i = 0; i < hijos.size(); i++) {
        if (!hijos[i].evualated) { 
            //pair<vector<bool>,int> fitness  = calcularFitness(hijos[i].genes);
            hijos[i].fitness = calcularFitness2(hijos[i].genes);
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
    /*if(population_lt[0].fitness == graph.size()){
        vector<Chromosoma> end;
        end.push_back(population_lt[0]);
        return end;
    }*/
    
    vector<Chromosoma> new_population;
    if(population_lt.size() > 2){
        //int best_size = POP_SIZE / 10;

        /*for (int i = 0; i < best_size; ++i) {
            new_population.push_back(population_lt[i]);
        }*/

        int fitnessActual = population_lt[0].fitness;
        //cout << "fitnessActual " << fitnessActual << endl;
        //cout << "Entramos aqui" << endl;
        for(int i = 0; i < population_lt.size();++i) {
            //pair<int,int> parents = seleccion_torneo(population_lt,100);
            //cout << "End Seleccion Torneo" << endl;
            uniform_int_distribution<int> distribucion(0, population_lt.size()-1);
            int superar;
            int padre1 = distribucion(generador);
            int padre2 = distribucion(generador);
            int fitnessActual = population_lt[padre1].fitness >= population_lt[padre2].fitness ? population_lt[padre1].fitness : population_lt[padre2].fitness;
            superar = population_lt[padre1].fitness >= population_lt[padre2].fitness ? padre1 : padre2;
            //if (padre1 != padre2) {
                Chromosoma offspring = crossover(population_lt[padre1], population_lt[padre2]);
                //cout << "end Crossover" << endl;
                int fitnesHijo = calcularFitness2(offspring.genes);
                //cout << "fitnessActual: " << fitnessActual << " fitnesHijo " << fitnesHijo << endl;
                //cout << "fin de calcularFitnessHijo" << endl;
                if((fitnesHijo > fitnessActual) || 
                (fitnesHijo == fitnessActual && count(offspring.genes.begin(),offspring.genes.end(),1) <= count(population_lt[superar].genes.begin(),population_lt[superar].genes.end(),1))) {
                    mutate(offspring);
                    new_population.push_back(offspring);
                    fitnesHijo = fitnessActual;   
                }
            //}
        }
    }
    else {
        for(int i = 0; i < population_lt.size(); ++i){
            mutate(population_lt[i]);
            new_population.push_back(population_lt[i]);
        }
    }
    return new_population;
}

set<int> algorimtoGenetico() {
    POP_SIZE = graph.size();
    GEN_SIZE = graph.size();
    MUTATION_RATE = 1/graph.size();
    GENERATIONS = graph.size();
    srand(time(nullptr));
    
    vector<Chromosoma> population = initial_population();
    bool found = false;
    int gen = 0;

    while(gen < GENERATIONS && !found) {
        cout << "Estamos en la generación "  <<  gen << endl;
        evaluar_population(population);
        population = selection(population);
        if(population.size() <= 2){
            cout << "population size: " << population.size() << endl;
            found = true;
        }
        cout << "population size: " << population.size() << endl;
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

/*int calculate_fitness(vector<bool>& solution) {
        // inicializar la lista de vértices cubiertos con la solución candidata
        unordered_set<int> covered_vertices;
        for (int i = 0; i < solution.size(); i++) {
            if (solution[i]) {
                covered_vertices.insert(i);
            }
        }

        // calcular el fitness inicial
        int fitness = covered_vertices.size();

        // para cada hijo de la solución candidata
        for (int i = 0; i < solution.size(); i++) {
            // si el hijo no cambia la solución actual, actualizar su fitness con el valor previo
            if (solution[i]) {
                fitness--;
            } else {
                // si el hijo cubre un vértice que no estaba cubierto previamente, actualizar el conjunto de vértices cubiertos
                for (int j = 0; j < vertices[i].size(); j++) {
                    int v = vertices[i][j];
                    if (!covered_vertices.count(v)) {
                        covered_vertices.insert(v);
                        fitness++;
                    }
                }
            }
        }

        return fitness;
}*/