#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<utility>
#include<map>
#include<list>
#include<set>
#include<functional>
#include<sstream>
using namespace std;

struct ParkingLot {
    string name;
    string location;
    int totalSlots;
    int availableSlots;
    vector<string> features;  // e.g., "Covered", "24/7", "Security", "EV Charging"

    string toCSV() const {
        string csv = name + "," + location + "," + to_string(totalSlots) + "," + to_string(availableSlots);
        for (const auto& f : features) csv += "," + f;
        return csv;
    }
};

class ParkingNetwork {
    map<string, ParkingLot> parkingLots;
    map<string, list<pair<string, int>>> graph;  // distance in meters

public:
    void addParkingLot(const string& id, const ParkingLot& p) {
        if (parkingLots.find(id) != parkingLots.end()) {
            cout << "Error: Parking lot with ID '" << id << "' already exists!" << endl;
            return;
        }
        parkingLots[id] = p;
        cout << "Successfully added parking lot '" << p.name << "' with ID '" << id << "'" << endl;
    }

    void updateParkingInfo(const string& id, string newName, string newLocation, 
                          int newTotalSlots, int newAvailableSlots, vector<string> newFeatures) {
        if (parkingLots.count(id)) {
            parkingLots[id].name = newName;
            parkingLots[id].location = newLocation;
            parkingLots[id].totalSlots = newTotalSlots;
            parkingLots[id].availableSlots = newAvailableSlots;
            parkingLots[id].features = newFeatures;
            cout << "Successfully updated parking lot with ID '" << id << "'" << endl;
        }
    }

    void connectParkingLots(const string& p1, const string& p2, int dist) {
        if (parkingLots.find(p1) == parkingLots.end() || parkingLots.find(p2) == parkingLots.end()) {
            cout << "Error: One or both parking lots do not exist!" << endl;
            return;
        }
        
        if (parkingLots.size() < 2) {
            cout << "Error: Need at least 2 parking lots to make a connection!" << endl;
            return;
        }

        graph[p1].push_back({p2, dist});
        graph[p2].push_back({p1, dist});
        cout << "Successfully connected parking lots " << p1 << " and " << p2 << endl;
    }

    // Find nearest parking with available slots
    pair<string, int> findNearestAvailableParking(const string& start, int requiredSlots) {
        if (parkingLots.find(start) == parkingLots.end()) {
            cout << "Error: Starting location does not exist!" << endl;
            return {"", -1};
        }

        map<string, int> dist;
        set<pair<int, string>> pq;
        string nearest = "";
        int minDist = INT_MAX;

        // Initialize distances
        for (const auto& [id, _] : parkingLots) {
            dist[id] = INT_MAX;
        }
        dist[start] = 0;
        pq.insert({0, start});

        while (!pq.empty()) {
            string u = pq.begin()->second;
            int d = pq.begin()->first;
            pq.erase(pq.begin());

            // Check if current parking lot meets slot requirement
            if (parkingLots[u].availableSlots >= requiredSlots && d < minDist) {
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
            cout << "No parking lot found with " << requiredSlots << "+ available slots!" << endl;
            return {"", -1};
        }

        return {nearest, minDist};
    }

    // Find shortest path between two parking lots
    pair<int, vector<string>> findShortestPath(const string& start, const string& end) {
        if (parkingLots.find(start) == parkingLots.end() || parkingLots.find(end) == parkingLots.end()) {
            cout << "Error: One or both parking lots do not exist!" << endl;
            return {-1, {}};
        }

        map<string, int> dist;
        map<string, string> prev;
        set<pair<int, string>> pq;

        for (const auto& [id, _] : parkingLots) {
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

        vector<string> path;
        for (string at = end; at != ""; at = prev[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        return {dist[end], path};
    }

    void printParkingLots() {
        for (auto& [id, p] : parkingLots) {
            cout << "Parking Lot ID: " << id << endl;
            cout << "Name: " << p.name << endl;
            cout << "Location: " << p.location << endl;
            cout << "Total Slots: " << p.totalSlots << endl;
            cout << "Available Slots: " << p.availableSlots << endl;
            cout << "Features: ";
            if (p.features.empty()) {
                cout << "None";
            } else {
                for (size_t i = 0; i < p.features.size(); ++i) {
                    cout << p.features[i];
                    if (i < p.features.size() - 1) cout << ", ";
                }
            }
            cout << endl << "------------------------" << endl;
        }
    }

    void printGraph() {
        for (auto& [from, edges] : graph) {
            cout << from << " --> ";
            for (auto& [to, dist] : edges) {
                cout << "(" << to << ", " << dist << " m) ";
            }
            cout << endl;
        }
    }

    // CSV Save/Load functions
    void saveToCSV(const string& filename) {
        ofstream out(filename);
        for (auto& [id, p] : parkingLots) {
            out << id << "," << p.toCSV() << endl;
        }
        out.close();
    }

    void loadFromCSV(const string& filename) {
        ifstream in(filename);
        if (!in.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string id, name, location, total_str, available_str;
            vector<string> features;
            
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, location, ',');
            getline(ss, total_str, ',');
            getline(ss, available_str, ',');
            
            int total = stoi(total_str);
            int available = stoi(available_str);
            
            string feature;
            while (getline(ss, feature, ',')) {
                features.push_back(feature);
            }
            
            ParkingLot p;
            p.name = name;
            p.location = location;
            p.totalSlots = total;
            p.availableSlots = available;
            p.features = features;
            
            parkingLots[id] = p;
        }
        in.close();
        cout << "Successfully loaded parking lots from " << filename << endl;
    }
};

int main() {
    ParkingNetwork net;
    int choice;

    do {
        cout << "\n--- Parking Management System ---\n";
        cout << "1. Add Parking Lot\n2. Connect Parking Lots\n3. View Parking Lots\n4. View Connections\n";
        cout << "5. Update Parking Information\n6. Find Nearest Available Parking\n7. Find Shortest Path\n";
        cout << "8. Save to CSV\n9. Load from CSV\n0. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            string id, name, location, feature;
            int totalSlots, availableSlots, featureCount;
            ParkingLot p;
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin.ignore(); getline(cin, p.name);
            cout << "Location: "; getline(cin, p.location);
            cout << "Total Slots: "; cin >> p.totalSlots;
            cout << "Available Slots: "; cin >> p.availableSlots;
            cout << "Number of features: "; cin >> featureCount;
            cin.ignore();
            for (int i = 0; i < featureCount; ++i) {
                cout << "Feature " << i+1 << ": ";
                getline(cin, feature);
                p.features.push_back(feature);
            }
            net.addParkingLot(id, p);
        }
        else if (choice == 2) {
            string p1, p2;
            int dist;
            cout << "Parking Lot 1 ID: "; cin >> p1;
            cout << "Parking Lot 2 ID: "; cin >> p2;
            cout << "Distance (meters): "; cin >> dist;
            net.connectParkingLots(p1, p2, dist);
        }
        else if (choice == 3) {
            net.printParkingLots();
        }
        else if (choice == 4) {
            net.printGraph();
        }
        else if (choice == 5) {
            string id;
            int newTotal, newAvailable;
            cout << "Parking Lot ID: "; cin >> id;
            cout << "New total slots: "; cin >> newTotal;
            cout << "New available slots: "; cin >> newAvailable;
            net.updateParkingInfo(id, "", "", newTotal, newAvailable, {});
        }
        else if (choice == 6) {
            string start;
            int requiredSlots;
            cout << "Start Location ID: "; cin >> start;
            cout << "Required number of slots: "; cin >> requiredSlots;
            auto [nearest, dist] = net.findNearestAvailableParking(start, requiredSlots);
            if (nearest != "") {
                cout << "Nearest parking with " << requiredSlots << "+ slots: " << nearest << endl;
                cout << "Distance: " << dist << " meters\n";
            }
        }
        else if (choice == 7) {
            string start, end;
            cout << "Start Parking Lot ID: "; cin >> start;
            cout << "End Parking Lot ID: "; cin >> end;
            auto [dist, path] = net.findShortestPath(start, end);
            if (dist != -1) {
                cout << "Shortest distance: " << dist << " meters\n";
                cout << "Path: ";
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << path[i];
                    if (i < path.size() - 1) cout << " -> ";
                }
                cout << endl;
            }
        }
        else if (choice == 8) {
            string filename;
            cout << "Enter filename to save: ";
            cin >> filename;
            net.saveToCSV(filename);
        }
        else if (choice == 9) {
            string filename;
            cout << "Enter filename to load: ";
            cin >> filename;
            net.loadFromCSV(filename);
        }

    } while (choice != 0);

    return 0;
} 