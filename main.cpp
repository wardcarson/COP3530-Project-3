using namespace std;
#include <vector>
#include <iostream>
#include <map>



class Graph
{
private:
    map<string, vector<pair<string, int>>> graph; // map<source, vector<destination, distance>>
    map<string, vector<pair<string, int>>>::iterator it;

public:
    void insertEdge(string from, string to, int distance);
    bool isEdge(string from, string to);
    int getWeight(string from, string to);
    vector<string> getNeighbors(string vertex);
    void printGraph();
};


void Graph::insertEdge(string from, string to, int distance) {

    graph[from].push_back(make_pair(to, distance));
}

bool Graph::isEdge(string from, string to) {

    if (graph.find(from) == graph.end()) {
        return false;
    }
    else {
        for (int i = 0; i < graph[from].size(); i++) {
            if (graph[from][i].first == to) {
                return true;
            }
        }
    }
    return false;
}

int Graph::getWeight(string from, string to) {

    if (graph.find(from) == graph.end()) {
        return 0;
    }
    else {
        for (int i = 0; i < graph[from].size(); i++) {
            if (graph[from][i].first == to) {
                return graph[from][i].second;
            }
        }
    }
    return 0;
}

vector<string> Graph::getNeighbors(string vertex) {

    vector<string> neighbors;

    if (graph.find(vertex) == graph.end()) {
        return neighbors;
    }
    for (int i = 0; i < graph[vertex].size(); i++) {
        neighbors.push_back(graph[vertex][i].first);
    }
    return neighbors;
}

void Graph::printGraph()
{
    int cnt = 0;
    for(it = graph.begin(); it != graph.end(); ++it)
    {
        cnt++;
        cout << "From " << it->first << ": ";
        for(int j = 0; j < it->second.size(); j++) {
            cout << "to " << it->second[j].first << " is " << it->second[j].second << " miles, ";
        }
        cout << endl;
    }
}

int main() {
    /*
    Graph *graph = new Graph;
    graph->insertEdge("Orlando", "LA", 500);
    graph->insertEdge("Orlando", "New Orleans", 200);
    graph->insertEdge("LA", "Orlando", 500);
    graph->insertEdge("New Orleans" , "Orlando", 200);
    graph->printGraph();
     */

}

