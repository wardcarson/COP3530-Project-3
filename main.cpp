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
public:
    map<string, vector<pair<City, int>>> graph;
private:
    //map<string, vector<pair<City, int>>> graph; // map<source, vector<destination, distance>>
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
    int bellmanFordShortestPath(Graph &graph, string src, string dest);
    int bellmanFordSafestPath(Graph &graph, string src, string dest);
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
            int adjacentNeighboSize = 4;
            for(int i = 0; i < adjacentNeighboSize; i++) {
                index = distr(gen);
                distance = distr2(gen2);
                if (cityName == allCities[index])  //should help us to avoid self loop
                {
                    adjacentNeighboSize++;
                    continue;
                }
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
    string temp = destination;
    while (working) {
        if (temp == src) {
            shortestPath.push_back(temp);
            working = false;
        }
        else {
            shortestPath.push_back(temp);
            temp = p[temp];
        }
    }
    return shortestPath;  //This stores the path in the backward direction.
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
    string temp = destination;
    while (working) {
        if (temp == src) {
            safestPath.push_back(temp);
            working = false;
        }
        else {
            safestPath.push_back(temp);
            temp = p[temp];
        }
    }
    return safestPath; //this stores the path in backeward direction like from desination to source
}

int Graph::bellmanFordShortestPath(Graph &graph, string src, string dest) {
    int v = graph.numCities;
    int infinity = ~(1 << 31);
    map<string, long long> d;   
    map<string, string> p;
    set<string> V_S;

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
   
    //iterate through all cities
    for (int i = 0; i < cityList.size(); i++) {
        V_S.insert(cityList[i]);

    }
    for (int i = 0; i < cityList.size(); i++) {
        int smallVal = infinity;
        string indexOfsmall;

        for (auto j : V_S)
        {
            if (d[j] <= smallVal) {
                smallVal = d[j];
                indexOfsmall = j;
            }
        }
      
        for (auto x : graph.graph.at(indexOfsmall))
        {
            if ((d[indexOfsmall] + x.second) < d[x.first.cityName]) {
                d[x.first.cityName] = d[indexOfsmall] + x.second;
                p[x.first.cityName] = indexOfsmall;
            }
        }

    }
    /*
   // check for negative cycle
    for (auto i: graph.graph.at(src))
    {
        if ((d[graph.graph.at(src).at(i).first.cityName] + (graph.graph.at(src).at(i).second)) > d[graph.graph.at(dest).at(i).first.cityName])
        {
            cout << "Error! Graph Contains a Negative Cycle";
        }

    }
     */

    for (int i = 0; i < numEdges; i++) {
        for (int j = 0; j < cityList[i].size(); j++) {
            int u = graph.graph[src][j].second;
            int v = graph.graph[dest][j].second;
            int weight = graph.graph[cityList[i]][j].second;
            if (d[graph.graph[src][j].first.cityName] != INT_MAX && d[graph.graph[src][j].first.cityName] + weight < d[graph.graph[dest][j].first.cityName]) {
                cout << "Error! Graph Contains a Negative Cycle";
            }
        }
    }

    
    int shortestPath = 0;
    bool working = true;
    string temp = dest;
    while (working) {
        if (temp == src) {
            shortestPath += d[temp];
            working = false;
        }
        else {
            shortestPath += d[temp];
            temp = p[temp];
        }
    }
    return shortestPath;  //This stores the path in the backward direction.


}

int Graph::bellmanFordSafestPath(Graph &graph, string src, string dest) {
    
    int v = graph.numCities;
    int infinity = ~(1 << 31);
    map<string, long long> d;   
    map<string, string> p;
    set<string> V_S;

    //iterate through all cities
    for (int i = 0; i < cityList.size(); i++) {
        V_S.insert(cityList[i]);
    }

    //iterating through all cities
    for (int i = 0; i < cityList.size(); i++)
    {
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
    int smallVal = infinity;
    string indexOfsmall;
    for (auto j : V_S)
    {
        if (d[j] <= smallVal) {
            smallVal = d[j];
            indexOfsmall = j;
        }
    }
    for (auto x : graph.graph.at(indexOfsmall))
    {
        if ((d[indexOfsmall] + x.first.numCovidCases) < d[x.first.cityName]) {
            d[x.first.cityName] = d[indexOfsmall] + x.first.numCovidCases;
            p[x.first.cityName] = indexOfsmall;
        }
    }

   /*
    //check for negative cycle
    for (int i = 0; i < numEdges; i++)
    {
        if ((d[graph.graph.at(src).at(i).first.cityName] + (graph.graph.at(src).at(i).second)) < d[graph.graph.at(dest).at(i).first.cityName])
        {
            cout << "Error! Graph Contains a Negative Cycle";
        }

    }
    */
    int safestPath = 0;
    bool working = true;
    string temp = dest;
    while (working) {
        if (temp == src) {
            safestPath += d[temp];
            working = false;
        }
        else {
            safestPath += d[temp];
            temp = p[temp];
        }
    }
    return safestPath; //this stores the path in backeward direction like from desination to source
}



int main() {
    Graph* graph = new Graph();
    //graph->readCSVFindAllCities("coviddatapoints.csv");
    //graph->readCSVFindAllCities("testing.csv");
    //graph->printGraph();
    bool runProgram = true;

    string source;
    string destination;
    string option;
    //graph->readCSVFindAllCities("testing.csv");
    graph->readCSVFindAllCities("smallerdatapoints.csv");
    while(runProgram) {
        cout << "Welcome to the Covid travel Calculator " << endl;
        cout << "Please select an option: " << endl;
        cout << "1. Find shortest route between locations" << endl;
        cout << "2. Find safest route between locations" << endl;
        cout << "3. Compare safest route and shortest route between locations" << endl;
        cout << "4. Exit Program" << endl;
        cin >> option;
        if(option == "1") {
            cout << "Please enter a starting location: " << endl;
            cin >> ws;
            getline(cin,source);
            cout << "Please enter a destination: " << endl;
            cin >> ws;
            getline(cin,destination);
            cout << "Calculating..." << endl;
            vector<string> vec;

            vec = graph->shortestDistance(*graph, source, destination);
            cout << "The shortest path from " << source << " to " << destination  <<" is: " /* put results from func*/ <<endl;
            for(int i = 0; i < vec.size(); i++ ) {
                cout << vec[vec.size()-1-i];
                if(i != vec.size()-1) {
                    cout << " -> ";
                }
            }
                // now the string of vector should work//need to change it to iteration of vector
            int bfsp = graph->bellmanFordShortestPath(*graph,source,destination);
            cout << endl;
            cout << "According to Bellman-ford, from " << source << " to " << destination << " is " << bfsp << " miles." << endl;
            //cout << "The shortest distance from " << source << " to " << destination  <<" is: "<< m <<endl;
        }
        if(option == "2") {

            cout << "Please enter a starting location: " << endl;
            cin >> ws;
            getline(cin,source);
            cout << "Please enter a destination: " << endl;
            cin >> ws;
            getline(cin,destination);
            cout << "Calculating..." << endl;
            // put in safestCovidPath function call
            vector<string> vec;
            //vec = graph->safestCovidPath(*graph, source, destination); // now the string of vector should work//need to change it to iteration of vector
            //graph->printGraph();
            cout << "The safest path from " << source << " to " << destination  <<" is: " /* put results from func*/ <<endl;

            vec = graph->safestCovidPath(*graph, source, destination);
            for(int i = 0; i < vec.size(); i++ ) {
                cout << vec[vec.size()-1-i];
                if(i != vec.size()-1) {
                    cout << " -> ";
                }
            }
                // now the string of vector should work//need to change it to iteration of vector
            int bfcovidpath = graph->bellmanFordSafestPath(*graph,source,destination);
            cout << endl;
            cout << "According to Bellman-ford, this route contains " << bfcovidpath << " covid cases." << endl;
            continue;
        }
        if(option == "3") {
            //find quick way to compare, showing cases/miles between two cities
            cout << "Please enter a starting location: " << endl;
            cin >> ws;
            getline(cin,source);
            cout << "Please enter a destination: " << endl;
            cin >> ws;
            getline(cin,destination);
            cout << "Calculating..." << endl;
            vector<string> vec;


            vec = graph->shortestDistance(*graph, source, destination);
            cout << "The shortest path from " << source << " to " << destination  <<" is: " /* put results from func*/ <<endl;
            for(int i = 0; i < vec.size(); i++ ) {
                cout << vec[vec.size()-1-i];
                if(i != vec.size()-1) {
                    cout << " -> ";
                }
            }
            // now the string of vector should work//need to change it to iteration of vector
            int bfsp = graph->bellmanFordShortestPath(*graph,source,destination);
            cout << endl;
            cout << "According to Bellman-ford, from " << source << " to " << destination << " is " << bfsp << " miles." << endl;
            cout << "The safest path from " << source << " to " << destination  <<" is: " /* put results from func*/ <<endl;

            vec = graph->safestCovidPath(*graph, source, destination);
            for(int i = 0; i < vec.size(); i++ ) {
                cout << vec[vec.size()-1-i];
                if(i != vec.size()-1) {
                    cout << " -> ";
                }
            }
            // now the string of vector should work//need to change it to iteration of vector
            int bfcovidpath = graph->bellmanFordSafestPath(*graph,source,destination);
            cout << endl;
            cout << "According to Bellman-ford, this route contains " << bfcovidpath << " covid cases." << endl;

            continue;
        }
        if(option == "4") {
            break;
        }
        else {
            continue;
        }


    }

}

