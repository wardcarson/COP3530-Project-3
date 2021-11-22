using namespace std;
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>



class Graph
{
private:
    map<string, vector<pair<string, int>>> graph; // map<source, vector<destination, distance>>
    map<string, vector<pair<string, int>>>::iterator it;
    
    //Creating these for the purpose of reading the files
    string cityName;
    int covidCaseNum,distance; 
    Graph(int covidCase,int distance) 
    {
        covidCaseNum = covidCase;
        this->distance = distance;
    }
    
public:
    void insertEdge(string from, string to, int distance);
    bool isEdge(string from, string to);
    int getWeight(string from, string to);
    vector<string> getNeighbors(string vertex);
    void printGraph();
    void readingCSVFile(string nameOfFile, map<string, Graph>& cityInfo);

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
//We may need to change the format of the map for the csv file and the way of setting the vertex  
void Graph::readingCSVFile(string nameOfFile, map<string, Graph>& cityInfo) 
{
    //Used the refrerence from Programming 2's project to read files
    ifstream openFile;
    openFile.open(nameOfFile);
    if (openFile.is_open())
    {
        string infoOfCity;
        getline(openFile, infoOfCity);
        while (getline(openFile,infoOfCity))
        {
            istringstream stream(infoOfCity);
            string city, stateId, covidCase, distance="";
            int covidCaseNo, distanceOfCity = 0;
            
            getline(stream, city, ',');
            getline(stream, stateId, ',');
            city = city + stateId;  //combine the city name and stateId,if it is US/Canada city
            
            //getting the confirmed covid case no
            getline(stream, covidCase, ',');
            covidCaseNo = stoi(covidCase);
            
            getline(stream, distance, ',');
            distanceOfCity = stoi(distance);
            
            Graph cityData(covidCaseNo, distanceOfCity);
            cityInfo.emplace(city, cityData);

        }   
    }
    else
    {
        cout << "File is Not Open!" << endl;
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

