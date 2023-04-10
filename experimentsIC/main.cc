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
#include "difusioIC.hh"

using namespace std;
namespace fs = std::filesystem;

double p;
vector<vector<int>> graph;  // adjacency list of the graph

void Usage() {
    cout << "Usage: ./main p" << endl;
    exit(1);
}

void readFileGraph(string s);

void writeSetsOnFile(const set<int>& minimumS, string s);

void writePercentsActOnFile(string s, const vector<pair<int, double>>& percent);

void writeTimeOnFile(string s, double time);

bool compare_file_size(const fs::directory_entry& file1, const fs::directory_entry& file2) {
    return fs::file_size(file1.path()) < fs::file_size(file2.path());
}

int main(int argc, char* argv[]) {
    if (argc != 2) Usage();
    p = stod(argv[1]);

    string outPut = "ResultsGeneticoIC";
    outPut += p == 0.75 ? "075" : "";
    fs::path directoryInstances = fs::current_path() / "instances"; //directory with graphs
    fs::path directoryTimes = fs::current_path() / outPut/"times"; 
    fs::path directoryPercent = fs::current_path() / outPut/"percentOfActivation";
    vector<fs::directory_entry> files;
    cout << directoryTimes << endl;
    cout << directoryPercent << endl;

    for (const auto& file : fs::directory_iterator(directoryInstances)) {
        files.push_back(file);
    }

    sort(files.begin(), files.end(), compare_file_size); 

    set<int> minimumS;
    int total;

    for (auto file : files) { //go through the file's
        if (!file.is_directory()) {
            cout << file.path().filename().string() << endl;

            string name = file.path().filename().string();
            readFileGraph(file.path().string());

            //Greedy version 1
       
            vector<pair<int, double>> percentActSet(5);
            double totalTime = 0;

            for(int i = 0; i < 5; ++i) {
                int totalSize = 0;

                auto start = chrono::high_resolution_clock::now();

                minimumS = algorimtoGenetico();
                
                auto end = chrono::high_resolution_clock::now();
                auto time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000000.0;
                totalTime += time;

                for(int j = 0; j < 50; ++j) {
                    totalSize += IC(minimumS);
                }
                double percentAct = ((totalSize/100.0)*100)/(graph.size()-1);
                percentActSet[i] = make_pair(minimumS.size(), percentAct);
                
            }
            cout << "Writefile" << endl;
            writePercentsActOnFile(directoryPercent.string()+"/"+name+".out", percentActSet);
            writeTimeOnFile(directoryTimes.string()+"/"+name+".out", totalTime/10);
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

void writeSetsOnFile(const set<int>& minimumS, string s) {
    ofstream file(s);
    if(file.is_open()) {
        for(auto s : minimumS) file << s << " ";
        file << endl;
        file.close();
    }
}

void writePercentsActOnFile(string s, const vector<pair<int, double>>& percent) {
    ofstream file(s);
    if(file.is_open()) {
        for(auto p : percent) file << p.first << " " << p.second << endl; //GraphName SizeMinumumSet PercentOfActivation
        file.close();
    }
}

void writeTimeOnFile(string s, double time) {
    ofstream file(s);
    if(file.is_open()) {
        file << time << endl; //GraphName MedianOfAlgorithmTime
        file.close();
    }
}


