#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "main.hh"
#include "greedyLT.hh"
#include "genetico.hh"
#include "difusioLT.hh"

using namespace std;
namespace fs = std::filesystem;

double r;
vector<vector<int>> graph;  // adjacency list of the graph

void Usage() {
    cout << "Usage: ./main p" << endl;
    exit(1);
}

void readFileGraph(string s);

void writeOnFile(const set<int>& minimumS, string s);

int main(int argc, char* argv[]) {
    if (argc != 2) Usage();
    r = stod(argv[1]);
    fs::path directoryInstances = fs::current_path() / "instances"; //directory with graphs
    fs::path directoryGreedy = fs::current_path() / "greedyResults"; //directory for greedyResults

    for (auto file : fs::directory_iterator(directoryInstances)) { //go through the file's
        if (!file.is_directory()) {
            cout << file.path().filename().string() << endl;

            readFileGraph(file.path().string());

            auto start = chrono::high_resolution_clock::now();

            //set<int> minimumS = greed();

            auto end = chrono::high_resolution_clock::now();
            auto time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000000.0;


            //cout << "The minimum set is: " << minimumS.size() << endl;
            //cout << "Greedy time is: " << time << endl;

            //int total = LT(minimumS);
            //cout << "The total number of nodes is: " << graph.size()-1 << " and the total active number of nodes is: " << total << endl;

            start = chrono::high_resolution_clock::now();

            set<int> minimumS = algorimtoGenetico();

            end = chrono::high_resolution_clock::now();
            time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000000.0;


            cout << "The minimum set is: " << minimumS.size() << endl;
            cout << "algorimtoGenetico time is: " << time << endl;

            int total = 0;
            total = LT(minimumS);
            cout << "The total number of nodes is: " << graph.size()-1 << " and the total active number of nodes is: " << total << endl;
            
            writeOnFile(minimumS, directoryGreedy.string()+"/"+file.path().filename().string()+".out");
        }
        cout << endl;
    }
}

void readFileGraph(string s) {
    ifstream file(s);
    string line;
    int n, m;

    if(file.is_open())  {
        getline(file, line); //Reads the first line
        sscanf(line.c_str(), "p edge %d %d", &n, &m); 
        graph = vector<vector<int>> (n+1);
        for(int i = 0; i < m; ++i) {
            int u, v;
            getline(file, line);
            sscanf(line.c_str(), "e %d %d", &u, &v);
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
        file.close();
    } else cout << "Error opening the file" << endl;
}

void writeOnFile(const set<int>& minimumS, string s) {
    ofstream file(s);
    if(file.is_open()) {
        for(auto s : minimumS) file << s << " ";
        file << endl;
        file.close();
    }
}


