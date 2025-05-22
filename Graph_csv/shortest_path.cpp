#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<utility>
#include<map>
#include<list>
#include<sstream>
#include<set>
#include<functional>
using namespace std;

struct Hospital {
    string name;
    string location;
    int beds;
    vector<string> services;

    string toCSV() const {
        string csv = name + "," + location + "," + to_string(beds);
        for (const auto& s : services) csv += "," + s;
        return csv;
    }
};
class HospitalNetwork {
    //defining a map to store all hospitals
    map<string, Hospital> hospitals;
    //defining a map to store all hospitals and their connections
    map<string, list<pair<string, int>>> graph;

public:
    void addHospital(const string& id, const Hospital& h) {
        //checking if the hospital already exists
        
        if (hospitals.find(id) != hospitals.end()) {
            cout << "Error: Hospital with ID '" << id << "' already exists!" << endl;
            return;
        }
        hospitals[id] = h;
        cout << "Successfully added hospital '" << h.name << "' with ID '" << id << "'" << endl;
    }

    void updateHospitalInfo(const string& id, string newName, string newLocation, int newBeds, vector<string> newServices) {
        if (hospitals.count(id)) {
            hospitals[id].name = newName;
            hospitals[id].location = newLocation;
            hospitals[id].beds = newBeds;
            hospitals[id].services = newServices;
            cout << "Successfully updated hospital with ID '" << id << "'" << endl;
        }
    }

    void deleteHospital(const string& id) {
        hospitals.erase(id);
        graph.erase(id);
        for (auto& [from, edges] : graph) {
            edges.remove_if([&](pair<string, int> e) { return e.first == id; });
        }
        cout << "Successfully deleted hospital with ID '" << id << "'" << endl;
    }

    void connectHospitals(const string& h1, const string& h2, int dist) {
        // Check if both hospitals exist
        if (hospitals.find(h1) == hospitals.end() || hospitals.find(h2) == hospitals.end()) {
            cout << "Error: One or both hospitals do not exist!" << endl;
            return;
        }
        
        // Check if there are at least 2 hospitals
        if (hospitals.size() < 2) {
            cout << "Error: Need at least 2 hospitals to make a connection!" << endl;
            return;
        }

        graph[h1].push_back({h2, dist});
        graph[h2].push_back({h1, dist});
        cout << "Successfully connected hospitals " << h1 << " and " << h2 << endl;
    }

    void updateDistance(const string& h1, const string& h2, int newDist) {
        for (auto& [to, d] : graph[h1]) if (to == h2) d = newDist;
        for (auto& [to, d] : graph[h2]) if (to == h1) d = newDist;
    }

    void disconnectHospitals(const string& h1, const string& h2) {
        graph[h1].remove_if([&](pair<string, int> e) { return e.first == h2; });
        graph[h2].remove_if([&](pair<string, int> e) { return e.first == h1; });
    }

    void printHospitals() {
        for (auto& [id, h] : hospitals) {
            cout << "Hospital ID: " << id << endl;
            cout << "Name: " << h.name << endl;
            cout << "Location: " << h.location << endl;
            cout << "Beds: " << h.beds << endl;
            cout << "Services: ";
            if (h.services.empty()) {
                cout << "None";
            } else {
                for (size_t i = 0; i < h.services.size(); ++i) {
                    cout << h.services[i];
                    if (i < h.services.size() - 1) cout << ", ";
                }
            }
            cout << endl << "------------------------" << endl;
        }
    }

    void printGraph() {
        for (auto& [from, edges] : graph) {
            cout << from << " --> ";
            for (auto& [to, dist] : edges) {
                cout << "(" << to << ", " << dist << " km) ";
            }
            cout << endl;
        }
    }

    // CSV Save
    void saveHospitalsToCSV(const string& filename) {
        ofstream out(filename);
        for (auto& [id, h] : hospitals) {
            out << id << "," << h.toCSV() << endl;
        }
        out.close();
    }

    void saveGraphToCSV(const string& filename) {
        ofstream out(filename);
        for (auto& [from, edges] : graph) {
            for (auto& [to, dist] : edges) {
                if (from < to) // avoid duplicate for undirected
                    out << from << "," << to << "," << dist << endl;
            }
        }
        out.close();
    }

    // Load hospitals from CSV
    void loadHospitalsFromCSV(const string& filename) {
        ifstream in(filename);
        if (!in.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string id, name, location, beds_str;
            vector<string> services;
            
            // Read ID
            getline(ss, id, ',');
            
            // Read name
            getline(ss, name, ',');
            
            // Read location
            getline(ss, location, ',');
            
            // Read beds
            getline(ss, beds_str, ',');
            int beds = stoi(beds_str);
            
            // Read services
            string service;
            while (getline(ss, service, ',')) {
                services.push_back(service);
            }
            
            // Create and add hospital
            Hospital h;
            h.name = name;
            h.location = location;
            h.beds = beds;
            h.services = services;
            
            hospitals[id] = h;
        }
        in.close();
        cout << "Successfully loaded hospitals from " << filename << endl;
    }

    // Load graph from CSV
    void loadGraphFromCSV(const string& filename) {
        ifstream in(filename);
        if (!in.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string from, to, dist_str;
            
            // Read from hospital
            getline(ss, from, ',');
            
            // Read to hospital
            getline(ss, to, ',');
            
            // Read distance
            getline(ss, dist_str, ',');
            int dist = stoi(dist_str);
            
            // Add connection to graph
            graph[from].push_back({to, dist});
            graph[to].push_back({from, dist});
        }
        in.close();
        cout << "Successfully loaded graph from " << filename << endl;
    }

    // Dijkstra's algorithm for shortest path
    pair<int, vector<string>> findShortestPath(const string& start, const string& end) {
        if (hospitals.find(start) == hospitals.end() || hospitals.find(end) == hospitals.end()) {
            cout << "Error: One or both hospitals do not exist!" << endl;
            return {-1, {}};
        }

        map<string, int> dist;
        map<string, string> prev;
        set<pair<int, string>> pq;

        // Initialize distances
        for (const auto& [id, _] : hospitals) {
            dist[id] = INT_MAX;
        }
        dist[start] = 0;
        pq.insert({0, start});

        while (!pq.empty()) {
            string u = pq.begin()->second;
            pq.erase(pq.begin());

            if (u == end) break;

            for (const auto& [v, weight] : graph[u]) {
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    pq.erase({dist[v], v});
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.insert({dist[v], v});
                }
            }
        }

        if (dist[end] == INT_MAX) {
            cout << "No path exists between " << start << " and " << end << endl;
            return {-1, {}};
        }

        // Reconstruct path
        vector<string> path;
        for (string at = end; at != ""; at = prev[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        return {dist[end], path};
    }

    // Cycle detection using DFS
    bool hasCycle() {
        map<string, bool> visited;
        map<string, bool> recStack;

        function<bool(const string&)> dfs = [&](const string& v) {
            visited[v] = true;
            recStack[v] = true;

            for (const auto& [u, _] : graph[v]) {
                if (!visited[u] && dfs(u)) return true;
                else if (recStack[u]) return true;
            }

            recStack[v] = false;
            return false;
        };

        for (const auto& [id, _] : hospitals) {
            if (!visited[id] && dfs(id)) return true;
        }
        return false;
    }

    // Floyd-Warshall algorithm for all-pairs shortest path
    map<string, map<string, int>> findAllPairsShortestPath() {
        map<string, map<string, int>> dist;
        
        // Initialize distances
        for (const auto& [i, _] : hospitals) {
            for (const auto& [j, _] : hospitals) {
                dist[i][j] = (i == j) ? 0 : INT_MAX;
            }
        }

        // Set direct edge weights
        for (const auto& [u, edges] : graph) {
            for (const auto& [v, w] : edges) {
                dist[u][v] = w;
            }
        }

        // Floyd-Warshall algorithm
        for (const auto& [k, _] : hospitals) {
            for (const auto& [i, _] : hospitals) {
                for (const auto& [j, _] : hospitals) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX &&
                        dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }

        return dist;
    }

    // Kruskal's algorithm for Minimum Spanning Tree
    vector<pair<string, string>> findMinimumSpanningTree() {
        vector<pair<int, pair<string, string>>> edges;
        map<string, string> parent;
        vector<pair<string, string>> mst;

        // Initialize disjoint set
        for (const auto& [id, _] : hospitals) {
            parent[id] = id;
        }

        // Collect all edges
        for (const auto& [u, edges_list] : graph) {
            for (const auto& [v, w] : edges_list) {
                if (u < v) { // Avoid duplicates in undirected graph
                    edges.push_back({w, {u, v}});
                }
            }
        }

        // Sort edges by weight
        sort(edges.begin(), edges.end());

        // Find function for disjoint set
        function<string(string)> find = [&](string x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        };

        // Union function for disjoint set
        auto Union = [&](string x, string y) {
            parent[find(x)] = find(y);
        };

        // Kruskal's algorithm
        for (const auto& [w, edge] : edges) {
            string u = edge.first;
            string v = edge.second;
            if (find(u) != find(v)) {
                mst.push_back({u, v});
                Union(u, v);
            }
        }

        return mst;
    }

    // Emergency routing to find nearest hospital with minimum capacity
    pair<string, int> findNearestHospitalWithCapacity(const string& start, int minBeds) {
        if (hospitals.find(start) == hospitals.end()) {
            cout << "Error: Starting hospital does not exist!" << endl;
            return {"", -1};
        }

        map<string, int> dist;
        set<pair<int, string>> pq;
        string nearest = "";
        int minDist = INT_MAX;

        // Initialize distances
        for (const auto& [id, _] : hospitals) {
            dist[id] = INT_MAX;
        }
        dist[start] = 0;
        pq.insert({0, start});

        while (!pq.empty()) {
            string u = pq.begin()->second;
            int d = pq.begin()->first;
            pq.erase(pq.begin());

            // Check if current hospital meets capacity requirement
            if (hospitals[u].beds >= minBeds && d < minDist) {
                nearest = u;
                minDist = d;
            }

            for (const auto& [v, weight] : graph[u]) {
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    pq.erase({dist[v], v});
                    dist[v] = dist[u] + weight;
                    pq.insert({dist[v], v});
                }
            }
        }

        if (nearest == "") {
            cout << "No hospital found with minimum " << minBeds << " beds!" << endl;
            return {"", -1};
        }

        return {nearest, minDist};
    }

    // Print all-pairs shortest paths
    void printAllPairsShortestPath() {
        auto dist = findAllPairsShortestPath();
        cout << "\nAll Pairs Shortest Paths:\n";
        for (const auto& [i, _] : hospitals) {
            for (const auto& [j, _] : hospitals) {
                if (i != j) {
                    cout << i << " to " << j << ": ";
                    if (dist[i][j] == INT_MAX) {
                        cout << "No path exists\n";
                    } else {
                        cout << dist[i][j] << " km\n";
                    }
                }
            }
        }
    }

    // Print minimum spanning tree
    void printMinimumSpanningTree() {
        auto mst = findMinimumSpanningTree();
        cout << "\nMinimum Spanning Tree Edges:\n";
        for (const auto& [u, v] : mst) {
            cout << u << " -- " << v << endl;
        }
    }
};
int main() {
    HospitalNetwork net;
    int choice;

    do {
        cout << "\n--- Hospital Network System ---\n";
        cout << "1. Add Hospital\n2. Connect Hospitals\n3. View Hospitals\n4. View Connections\n";
        cout << "5. Update Hospital Information\n6. Update Distance\n7. Delete Hospital\n8. Disconnect Hospitals\n";
        cout << "9. Save to CSV\n10. Load Hospitals from CSV\n11. Load Graph from CSV\n";
        cout << "12. Find Shortest Path\n13. Check for Cycles\n14. View All Pairs Shortest Paths\n";
        cout << "15. View Minimum Spanning Tree\n16. Emergency Routing\n0. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            string id, name, location, service;
            int beds, svcCount;
            Hospital h;
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin.ignore(); getline(cin, h.name);
            cout << "Location: "; getline(cin, h.location);
            cout << "Beds: "; cin >> h.beds;
            cout << "Number of services: "; cin >> svcCount;
            cin.ignore();
            for (int i = 0; i < svcCount; ++i) {
                cout << "Service " << i+1 << ": ";
                getline(cin, service);
                h.services.push_back(service);
            }
            net.addHospital(id, h);
        }
        else if (choice == 2) {
            string h1, h2;
            int dist;
            cout << "Hospital 1 ID: "; cin >> h1;
            cout << "Hospital 2 ID: "; cin >> h2;
            cout << "Distance (km): "; cin >> dist;
            net.connectHospitals(h1, h2, dist);
        }
        else if (choice == 3) {
            net.printHospitals();
        }
        else if (choice == 4) {
            net.printGraph();
        }
        else if (choice == 5) {
            string id; int newBeds;
            cout << "Hospital ID: "; cin >> id;
            cout << "New bed count: "; cin >> newBeds;
            net.updateHospitalInfo(id, "", "", newBeds, {});
        }
        else if (choice == 6) {
            string h1, h2;
            int newDist;
            cout << "Hospital 1 ID: "; cin >> h1;
            cout << "Hospital 2 ID: "; cin >> h2;
            cout << "New Distance: "; cin >> newDist;
            net.updateDistance(h1, h2, newDist);
        }
        else if (choice == 7) {
            string id;
            cout << "Hospital ID to delete: "; cin >> id;
            net.deleteHospital(id);
        }
        else if (choice == 8) {
            string h1, h2;
            cout << "Hospital 1 ID: "; cin >> h1;
            cout << "Hospital 2 ID: "; cin >> h2;
            net.disconnectHospitals(h1, h2);
        }
        else if (choice == 9) {
            string filename;
            cout << "Enter filename to save hospitals: ";
            cin >> filename;
            net.saveHospitalsToCSV(filename);
            
            cout << "Enter filename to save graph: ";
            cin >> filename;
            net.saveGraphToCSV(filename);
        }
        else if (choice == 10) {
            string filename;
            cout << "Enter filename to load hospitals from: ";
            cin >> filename;
            net.loadHospitalsFromCSV(filename);
        }
        else if (choice == 11) {
            string filename;
            cout << "Enter filename to load graph from: ";
            cin >> filename;
            net.loadGraphFromCSV(filename);
        }
        else if (choice == 12) {
            string start, end;
            cout << "Start Hospital ID: "; cin >> start;
            cout << "End Hospital ID: "; cin >> end;
            auto [dist, path] = net.findShortestPath(start, end);
            if (dist != -1) {
                cout << "Shortest distance: " << dist << " km\n";
                cout << "Path: ";
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << path[i];
                    if (i < path.size() - 1) cout << " -> ";
                }
                cout << endl;
            }
        }
        else if (choice == 13) {
            if (net.hasCycle()) {
                cout << "The network contains cycles.\n";
            } else {
                cout << "The network is acyclic.\n";
            }
        }
        else if (choice == 14) {
            net.printAllPairsShortestPath();
        }
        else if (choice == 15) {
            net.printMinimumSpanningTree();
        }
        else if (choice == 16) {
            string start;
            int minBeds;
            cout << "Start Hospital ID: "; cin >> start;
            cout << "Minimum required beds: "; cin >> minBeds;
            auto [nearest, dist] = net.findNearestHospitalWithCapacity(start, minBeds);
            if (nearest != "") {
                cout << "Nearest hospital with " << minBeds << "+ beds: " << nearest << endl;
                cout << "Distance: " << dist << " km\n";
            }
        }

    } while (choice != 0);

    return 0;
}


