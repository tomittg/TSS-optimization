#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include "main.hh"
#include "genetico.hh"
#include "difusioLT.hh"

using namespace std;
namespace fs = std::filesystem;

double r;
double sizeEdges;
vector<vector<int>> graph;  // adjacency list of the graph

void Usage() {
    cout << "Usage: ./main p" << endl;
    exit(1);
}

void readFileGraph(string s);

void writeSetsOnFile(const set<int>& minimumS, double time, int size, string s);

bool compare_file_size(const fs::directory_entry& file1, const fs::directory_entry& file2) {
    return fs::file_size(file1.path()) < fs::file_size(file2.path());
}

int main(int argc, char* argv[]) {
    if (argc != 2) Usage();
    r = stod(argv[1]);
    
    string outPut = "ResultsGeneticoLT";
    outPut += r == 0.75 ? "075" : "";
    fs::path directoryInstances = fs::current_path() / "instances"; //directory with graphs
    fs::path directorylocalSearchLT = fs::current_path() / outPut; //directory for localSearchResultsLTv2
    std::vector<fs::directory_entry> files;
    cout << directorylocalSearchLT;


    for (const auto& file : fs::directory_iterator(directoryInstances)) {
        files.push_back(file);
    }

    std::sort(files.begin(), files.end(), compare_file_size);
    set<int> minimumS;
    int total = 0;

    for (auto file : files) { //go through the file's
        if (!file.is_directory()) {
            cout << file.path().filename().string() << endl;
            string name = file.path().filename().string();

            //localSearch version 1
            
            readFileGraph(file.path().string());

            auto startg1 = chrono::high_resolution_clock::now();
            minimumS = algorimtoGenetico();
            auto endg1 = chrono::high_resolution_clock::now();
            auto timeg1 = chrono::duration_cast<chrono::microseconds>(endg1 - startg1).count() / 1000000.0;

            cout << "The minimum set is: " << minimumS.size() << endl;
            cout << "algorimtoGenetico time is: " << timeg1 << endl;

            int total = 0;
            total = LT(minimumS);
            cout << "The total number of nodes is: " << graph.size()-1 << " and the total active number of nodes is: " << total << endl;

            writeSetsOnFile(minimumS, timeg1, minimumS.size(), directorylocalSearchLT.string()+"/"+name+".out");
        }
    }
}

void readFileGraph(string s) {
    ifstream file(s);
    string line;
    int n, m;

    if(file.is_open())  {
        getline(file, line); //Reads the first line
        sscanf(line.c_str(), "p edge %d %d", &n, &m); 
        sizeEdges = m;
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

void writeSetsOnFile(const set<int>& minimumS, double time, int size, string s) {
    ofstream file(s);
    if(file.is_open()) {
        file << time << " " << size << endl;
        for(auto s : minimumS) file << s << endl;
        file.close();
    }
}
