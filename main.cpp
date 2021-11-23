using namespace std;
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>

class Graph {

    struct City {
        string cityName;
        int numCovidCases;

        City(int _numCovidCases, string _cityName) {
            numCovidCases = _numCovidCases;
            cityName = _cityName;
        }
    };
private:
    map<string, vector<pair<City, int>>> graph; // map<source, vector<destination, distance>>
    map<City, vector<pair<City, int>>>::iterator it;

    
public:
    void insertEdge(City from, City to, int distance);
    bool isEdge(string from, string to);
    int getWeight(string from, string to);
    vector<string> getNeighbors(string vertex);
    void printGraph();
    void readCSVFindAllCities(string nameOfFile);
    void readCSVAddtoGraph(string nameOfFile, map<string, City> &mapOfallCities);

};

void Graph::insertEdge(City from, City to, int distance) {

    //Update: what is the best set up for our map? I think we can do string here right??
    graph[from.cityName].push_back(make_pair(to, distance));
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

void Graph::readCSVFindAllCities(string nameOfFile) {
    //Update: made this a variable in here
    map<string, City> mapOfallCities; //map<city name, City node>
    ifstream openFile;
    openFile.open(nameOfFile);
    if (openFile.is_open())
    {
        string infoOfCity;
        getline(openFile, infoOfCity);
        while (getline(openFile,infoOfCity))
        {
            istringstream stream(infoOfCity);
            string cityName, stateId, covidCase, distance="";
            int covidCaseNo, distanceOfCity = 0;
            
            getline(stream, cityName, ',');
            getline(stream, stateId, ',');
            cityName = cityName + stateId;
            
            //getting the confirmed covid case no
            getline(stream, covidCase, ',');
            covidCaseNo = stoi(covidCase);
            
            getline(stream, distance, ',');
            distanceOfCity = stoi(distance);


            //Update: no need for third column in csv file, please get rid of it
            //Now this function's purpose is to create a map of all the cities in the csv file
            City cityData(covidCaseNo, cityName);
            mapOfallCities.emplace(cityName, cityData);

        }   
    }
    else
    {
        cout << "File is Not Open!" << endl;
    }

    //Update: call new readCSV file function to insert cities into our graph
    readCSVAddtoGraph(nameOfFile, mapOfallCities);
}

void Graph::readCSVAddtoGraph(string nameOfFile, map<string, City> &mapOfallCities) {
    ifstream openFile;
    openFile.open(nameOfFile);
    if (openFile.is_open())
    {
        string infoOfCity;
        getline(openFile, infoOfCity);
        while (getline(openFile,infoOfCity))
        {
            istringstream stream(infoOfCity);
            string cityName, stateId, covidCase, distance="";
            int covidCaseNo, distanceOfCity = 0;

            getline(stream, cityName, ',');
            getline(stream, stateId, ',');
            cityName = cityName + stateId;

            getline(stream, covidCase, ',');
            covidCaseNo = stoi(covidCase);

            getline(stream, distance, ',');
            distanceOfCity = stoi(distance);

            //Update: no need for third column in csv file, please get rid of it
            //Now this function's purpose is to insert cities into our graph

            //Update: insert new city into graph using insertEdge
            //randomly generate four neighbors using mapOfAllCities and use random distances



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

