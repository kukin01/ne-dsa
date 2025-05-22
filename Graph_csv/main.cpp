#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<utility>
#include<map>
#include<list>
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
    map<string, Hospital> hospitals;
    map<string, list<pair<string, int>>> graph;

public:
    void addHospital(const string& id, const Hospital& h) {
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

    // Load functions could be added similarly.
};
int main() {
    HospitalNetwork net;
    int choice;

    do {
        cout << "\n--- Hospital Network System ---\n";
        cout << "1. Add Hospital\n2. Connect Hospitals\n3. View Hospitals\n4. View Connections\n";
        cout << "5. Update Hospital Information\n6. Update Distance\n7. Delete Hospital\n8. Disconnect Hospitals\n";
        cout << "9. Save to CSV\n0. Exit\nChoose: ";
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
            net.saveHospitalsToCSV("hospitals.csv");
            net.saveGraphToCSV("graph.csv");
            cout << "Saved to CSV.\n";
        }

    } while (choice != 0);

    return 0;
}



