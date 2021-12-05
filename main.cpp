using namespace std;
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <random>
#include <set>
#include <list>

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
    map<string, vector<pair<City, int>>>::iterator it;
    vector<string> cityList;
    int numEdges = 0;
    int numCities = 0;
    
public:
    void insertEdge(string from, City to, int distance);
    bool isEdge(string from, string to);
    int getWeight(string from, string to);
    vector<string> getNeighbors(string vertex);
    void printGraph();
    void readCSVFindAllCities(string nameOfFile);
    void readCSVAddtoGraph(string nameOfFile, map<string, City> &mapOfallCities, vector<string> &allCities);
    vector<string> shortestDistance(const Graph& graph, string src, string destination);
    vector<string> safestCovidPath(const Graph& graph, string src, string destination);
    vector<string> bellmanFordShortestPath(Graph &graph, string src, string dest);
    vector<string> bellmanFordSafestPath(Graph &graph, string src, string dest);
};

void Graph::insertEdge(string from, City to, int distance) {

    //Update: what is the best set up for our map? I think we can do string here right??
    graph[from].push_back(make_pair(to, distance));
    numEdges++;
}

bool Graph::isEdge(string from, string to) {

    if (graph.find(from) == graph.end()) {
        return false;
    }
    else {
        for (int i = 0; i < graph[from].size(); i++) {
            if (graph[from][i].first.cityName == to) {
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
            if (graph[from][i].first.cityName == to) {
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
        neighbors.push_back(graph[vertex][i].first.cityName);
    }
    return neighbors;
}

void Graph::printGraph() {
    int cnt = 0;
    for(it = graph.begin(); it != graph.end(); ++it)
    {
        cnt++;
        cout << "From " << it->first << ": ";
        for(int j = 0; j < it->second.size(); j++) {
            cout << "to " << it->second[j].first.cityName << " is " << it->second[j].second << " miles, ";
        }
        cout << endl;
    }
}

void Graph::readCSVFindAllCities(string nameOfFile) {        //The Csv files has only two column city and covid case
    vector<string> allCities;
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
            string cityName, stateId, covidCase="";
            int covidCaseNo = 0;
            
            getline(stream, cityName, ',');
            //getline(stream, stateId, ',');   
           // cityName = cityName + stateId;
            
            //getting the confirmed covid case no
            getline(stream, covidCase, ',');
            covidCaseNo = stoi(covidCase);

            
            

            //Update: no need for third column in csv file, please get rid of it   //Done
            //Now this function's purpose is to create a map of all the cities in the csv file
            City cityData(covidCaseNo, cityName);
            mapOfallCities.emplace(cityName, cityData);
            allCities.push_back(cityName);
            numCities++;
        }   
    }
    else
    {
        cout << "File is Not Open!" << endl;
    }

    //Update: call new readCSV file function to insert cities into our graph
    readCSVAddtoGraph(nameOfFile, mapOfallCities, allCities);
    cityList = allCities;
}

void Graph::readCSVAddtoGraph(string nameOfFile, map<string, City> &mapOfallCities, vector<string> &allCities) {
    ifstream openFile;
    openFile.open(nameOfFile);
    if (openFile.is_open())
    {
        string infoOfCity;
        getline(openFile, infoOfCity);
        while (getline(openFile,infoOfCity))
        {
            istringstream stream(infoOfCity);
            string cityName, stateId, covidCase="";
            int covidCaseNo = 0;

            getline(stream, cityName, ',');
            //getline(stream, stateId, ','); //don't need state name anymore
            //cityName = cityName + stateId;

            getline(stream, covidCase, ',');
            covidCaseNo = stoi(covidCase);

      
            //Update: no need for third column in csv file, please get rid of it   //Done
            //Now this function's purpose is to insert cities into our graph

            //Update: insert new city into graph using insertEdge
            //randomly generate four neighbors using mapOfAllCities and use random distances
            int index = 0;
            int distance = 0;
            City newCity(covidCaseNo,cityName);
            std::random_device rd; // obtain a random number from hardware
            std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<> distr(0, allCities.size()-1); // define the range
            std::mt19937 gen2(rd()); // seed the generator
            std::uniform_int_distribution<> distr2(0, 10000); // define the range
            //auto temp = mapOfallCities[names.at(index)];
            for(int i = 0; i < 4; i++) {
                index = distr(gen);
                distance = distr2(gen2);
                insertEdge(newCity.cityName,mapOfallCities.at(allCities[index]), distance);  
            }
        }
    }
    else
    {
        cout << "File is Not Open!" << endl;
    }
}

vector<string> Graph::shortestDistance(const Graph& graph, string src, string destination) {

    int v = graph.numCities;
    int infinity = ~(1 << 31);
    map<string, long long> d;   //chnanged map value from int to long long to avoid integer overflow for large distance
    map<string, string> p;
    set<string> S = {src};
    set<string> V_S;

    //iterte through all cities
    for (int i = 0; i<cityList.size(); i++) {
        V_S.insert(cityList[i]);
    }

    //iterating through all cities
    for (int i = 0; i < cityList.size(); i++) {
        p[cityList[i]] = src;
        for (auto x : graph.graph.at(src))
        {
            if (x.first.cityName == cityList[i])
            {
                d[cityList[i]] = x.second;
            }
            else
            {
                d[cityList[i]] = infinity;
            }
        }
    }
    d[src] = 0;

    while (!V_S.empty())
    {
        int smallVal = infinity;
        string indexOfsmall;

        for (auto j : V_S)
        {
            if (d[j] <= smallVal) {
                smallVal = d[j];
                indexOfsmall = j;
            }
        }
        V_S.erase(indexOfsmall);
        S.insert(indexOfsmall);

        for (auto x : graph.graph.at(indexOfsmall))
        {
            if ((d[indexOfsmall] + x.second) < d[x.first.cityName]) {
                d[x.first.cityName] = d[indexOfsmall] + x.second;
                p[x.first.cityName] = indexOfsmall;
            }
        }
    }
    vector<string> shortestPath;
    bool working = true;
    string temp = src;
    while (working) {
        if (temp == destination) {
            shortestPath.push_back(temp);
            working = false;
        }
        else {
            shortestPath.push_back(temp);
            temp = p[temp];
        }
    }
    return shortestPath;
}

vector<string> Graph::safestCovidPath(const Graph& graph, string src, string destination) {
    int v = graph.numCities;
    int infinity = ~(1 << 31);
    map<string, int> d;
    map<string, string> p;
    set<string> S = {src};
    set<string> V_S;

    //iterate through all cities
    for (int i = 0; i<cityList.size(); i++) {
        V_S.insert(cityList[i]);
    }

    //iterating through all cities
    for (int i = 0; i < cityList.size(); i++) {
        p[cityList[i]] = src;
        for (auto x : graph.graph.at(src))
        {
            if (x.first.cityName == cityList[i])
            {
                d[cityList[i]] = x.first.numCovidCases;
            }
            else
            {
                d[cityList[i]] = infinity;
            }
        }
    }
    d[src] = 0;

    while (!V_S.empty())
    {
        int smallVal = infinity;
        string indexOfsmall;

        for (auto j : V_S)
        {
            if (d[j] <= smallVal) {
                smallVal = d[j];
                indexOfsmall = j;
            }
        }
        V_S.erase(indexOfsmall);
        S.insert(indexOfsmall);

        for (auto x : graph.graph.at(indexOfsmall))
        {
            if ((d[indexOfsmall] + x.first.numCovidCases) < d[x.first.cityName]) {
                d[x.first.cityName] = d[indexOfsmall] + x.first.numCovidCases;
                p[x.first.cityName] = indexOfsmall;
            }
        }
    }
    vector<string> safestPath;
    bool working = true;
    string temp = src;
    while (working) {
        if (temp == destination) {
            safestPath.push_back(temp);
            working = false;
        }
        else {
            safestPath.push_back(temp);
            temp = p[temp];
        }
    }
    return safestPath;
}

vector<string> Graph::bellmanFordShortestPath(Graph &graph, string src, string dest) {
    map<string, pair<string, int>> safestDistances;
    vector<pair<City, int>>::iterator traverse;

    for (int i = 0; i <= cityList.size(); ++i) {
        safestDistances[cityList[i]].first = "";
        safestDistances[cityList[i]].second = -1;
    }

    safestDistances[src].first = src; //src
    safestDistances[src].second = 0;

    for (int i = 1; i <= cityList.size() - 1; ++i) {
        for (int j = 1; j <= cityList.size(); ++j) {

            traverse = graph.graph[cityList[j]].begin();

            while (traverse != graph.graph[cityList[j]].end()) {
                if (safestDistances[cityList[j]].first == "") {
                    ++traverse;
                    continue;
                }

                if ((*traverse).second + safestDistances[cityList[j]].second < safestDistances[(*traverse).first.cityName].second) {
                    safestDistances[(*traverse).first.cityName].first = (*traverse).first.cityName;
                    safestDistances[(*traverse).first.cityName].second = j;
                }
                ++traverse;
            }
        }
    }

}

vector<string> Graph::bellmanFordSafestPath(Graph &graph, string src, string dest) {
    map<string, pair<string, int>> safestDistances;
    vector<pair<City, int>>::iterator traverse;

    for (int i = 0; i <= cityList.size(); ++i) {
        safestDistances[cityList[i]].first = "";
        safestDistances[cityList[i]].second = -1;
    }

    safestDistances[src].first = src; //src
    safestDistances[src].second = 0;

    for (int i = 1; i <= cityList.size() - 1; ++i) {
        for (int j = 1; j <= cityList.size(); ++j) {

            traverse = graph.graph[cityList[j]].begin();

            while (traverse != graph.graph[cityList[j]].end()) {
                if (safestDistances[cityList[j]].first == "") {
                    ++traverse;
                    continue;
                }

                if ((*traverse).first.numCovidCases + safestDistances[cityList[j]].second < safestDistances[(*traverse).first.cityName].second) {
                    safestDistances[(*traverse).first.cityName].first = (*traverse).first.cityName;
                    safestDistances[(*traverse).first.cityName].second = j;
                }
                ++traverse;
            }
        }
    }

}






int main() {
    /*
    Graph *graph = new Graph;
    graph->readCSVFindAllCities("Covid Data points.csv");
    graph->insertEdge("Orlando", "LA", 500);
    graph->insertEdge("Orlando", "New Orleans", 200);
    graph->insertEdge("LA", "Orlando", 500);
    graph->insertEdge("New Orleans" , "Orlando", 200);
    graph->printGraph();
     */

}

