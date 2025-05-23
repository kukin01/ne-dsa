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

struct Property {
    string name;
    string address;
    string type;  // "Apartment", "House", "Condo", etc.
    double price;
    int bedrooms;
    int bathrooms;
    double area;  // in square meters
    bool isAvailable;
    vector<string> features;  // e.g., "Furnished", "Pet Friendly", "Pool", "Gym"

    string toCSV() const {
        string csv = name + "," + address + "," + type + "," + to_string(price) + "," +
                    to_string(bedrooms) + "," + to_string(bathrooms) + "," + to_string(area) + "," +
                    (isAvailable ? "1" : "0");
        for (const auto& f : features) csv += "," + f;
        return csv;
    }
};

class RentalNetwork {
    map<string, Property> properties;
    map<string, list<pair<string, int>>> graph;  // distance in meters

    // Validation helper functions
    bool isValidPropertyType(const string& type) {
        vector<string> validTypes = {"Apartment", "House", "Condo", "Studio", "Townhouse"};
        return find(validTypes.begin(), validTypes.end(), type) != validTypes.end();
    }

    bool isValidPrice(double price) {
        return price > 0 && price <= 1000000; // Maximum $1M per month
    }

    bool isValidBedrooms(int bedrooms) {
        return bedrooms >= 0 && bedrooms <= 10; // Maximum 10 bedrooms
    }

    bool isValidBathrooms(int bathrooms) {
        return bathrooms >= 0 && bathrooms <= 10; // Maximum 10 bathrooms
    }

    bool isValidArea(double area) {
        return area > 0 && area <= 1000; // Maximum 1000 sq.m
    }

    bool isValidDistance(int dist) {
        return dist > 0 && dist <= 50000; // Maximum 50km
    }

    bool isValidPropertyId(const string& id) {
        return !id.empty() && id.length() <= 10 && 
               all_of(id.begin(), id.end(), [](char c) { 
                   return isalnum(c) || c == '_'; 
               });
    }

public:
    void addProperty(const string& id, const Property& p) {
        if (!isValidPropertyId(id)) {
            cout << "Error: Invalid property ID! Use alphanumeric characters and underscore only (max 10 chars)." << endl;
            return;
        }

        if (properties.find(id) != properties.end()) {
            cout << "Error: Property with ID '" << id << "' already exists!" << endl;
            return;
        }

        if (!isValidPropertyType(p.type)) {
            cout << "Error: Invalid property type! Use: Apartment, House, Condo, Studio, or Townhouse." << endl;
            return;
        }

        if (!isValidPrice(p.price)) {
            cout << "Error: Invalid price! Must be between $1 and $1,000,000 per month." << endl;
            return;
        }

        if (!isValidBedrooms(p.bedrooms)) {
            cout << "Error: Invalid number of bedrooms! Must be between 0 and 10." << endl;
            return;
        }

        if (!isValidBathrooms(p.bathrooms)) {
            cout << "Error: Invalid number of bathrooms! Must be between 0 and 10." << endl;
            return;
        }

        if (!isValidArea(p.area)) {
            cout << "Error: Invalid area! Must be between 1 and 1000 sq.m." << endl;
            return;
        }

        properties[id] = p;
        cout << "Successfully added property '" << p.name << "' with ID '" << id << "'" << endl;
    }

    void updatePropertyInfo(const string& id, string newName, string newAddress, 
                          string newType, double newPrice, int newBedrooms, 
                          int newBathrooms, double newArea, bool newAvailability,
                          vector<string> newFeatures) {
        if (!isValidPropertyId(id)) {
            cout << "Error: Invalid property ID!" << endl;
            return;
        }

        if (properties.find(id) == properties.end()) {
            cout << "Error: Property with ID '" << id << "' does not exist!" << endl;
            return;
        }

        if (!newType.empty() && !isValidPropertyType(newType)) {
            cout << "Error: Invalid property type!" << endl;
            return;
        }

        if (newPrice > 0 && !isValidPrice(newPrice)) {
            cout << "Error: Invalid price!" << endl;
            return;
        }

        if (newBedrooms > 0 && !isValidBedrooms(newBedrooms)) {
            cout << "Error: Invalid number of bedrooms!" << endl;
            return;
        }

        if (newBathrooms > 0 && !isValidBathrooms(newBathrooms)) {
            cout << "Error: Invalid number of bathrooms!" << endl;
            return;
        }

        if (newArea > 0 && !isValidArea(newArea)) {
            cout << "Error: Invalid area!" << endl;
            return;
        }

        if (!newName.empty()) properties[id].name = newName;
        if (!newAddress.empty()) properties[id].address = newAddress;
        if (!newType.empty()) properties[id].type = newType;
        if (newPrice > 0) properties[id].price = newPrice;
        if (newBedrooms > 0) properties[id].bedrooms = newBedrooms;
        if (newBathrooms > 0) properties[id].bathrooms = newBathrooms;
        if (newArea > 0) properties[id].area = newArea;
        properties[id].isAvailable = newAvailability;
        if (!newFeatures.empty()) properties[id].features = newFeatures;

        cout << "Successfully updated property with ID '" << id << "'" << endl;
    }

    void connectProperties(const string& p1, const string& p2, int dist) {
        if (!isValidPropertyId(p1) || !isValidPropertyId(p2)) {
            cout << "Error: Invalid property ID!" << endl;
            return;
        }

        if (properties.find(p1) == properties.end() || properties.find(p2) == properties.end()) {
            cout << "Error: One or both properties do not exist!" << endl;
            return;
        }
        
        if (p1 == p2) {
            cout << "Error: Cannot connect a property to itself!" << endl;
            return;
        }

        if (!isValidDistance(dist)) {
            cout << "Error: Invalid distance! Must be between 1 and 50000 meters." << endl;
            return;
        }

        // Check if connection already exists
        for (const auto& [to, existingDist] : graph[p1]) {
            if (to == p2) {
                cout << "Error: Properties are already connected!" << endl;
                return;
            }
        }

        graph[p1].push_back({p2, dist});
        graph[p2].push_back({p1, dist});
        cout << "Successfully connected properties " << p1 << " and " << p2 << endl;
    }

    // Find nearest available property matching criteria
    pair<string, int> findNearestProperty(const string& start, 
                                        double maxPrice,
                                        int minBedrooms,
                                        int minBathrooms,
                                        double minArea) {
        if (!isValidPropertyId(start)) {
            cout << "Error: Invalid starting location ID!" << endl;
            return {"", -1};
        }

        if (properties.find(start) == properties.end()) {
            cout << "Error: Starting location does not exist!" << endl;
            return {"", -1};
        }

        if (!isValidPrice(maxPrice)) {
            cout << "Error: Invalid maximum price!" << endl;
            return {"", -1};
        }

        if (!isValidBedrooms(minBedrooms)) {
            cout << "Error: Invalid minimum bedrooms!" << endl;
            return {"", -1};
        }

        if (!isValidBathrooms(minBathrooms)) {
            cout << "Error: Invalid minimum bathrooms!" << endl;
            return {"", -1};
        }

        if (!isValidArea(minArea)) {
            cout << "Error: Invalid minimum area!" << endl;
            return {"", -1};
        }

        map<string, int> dist;
        set<pair<int, string>> pq;
        string nearest = "";
        int minDist = INT_MAX;

        // Initialize distances
        for (const auto& [id, _] : properties) {
            dist[id] = INT_MAX;
        }
        dist[start] = 0;
        pq.insert({0, start});

        while (!pq.empty()) {
            string u = pq.begin()->second;
            int d = pq.begin()->first;
            pq.erase(pq.begin());

            // Check if current property meets requirements
            if (properties[u].isAvailable &&
                properties[u].price <= maxPrice &&
                properties[u].bedrooms >= minBedrooms &&
                properties[u].bathrooms >= minBathrooms &&
                properties[u].area >= minArea &&
                d < minDist) {
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
            cout << "No property found matching your criteria!" << endl;
            return {"", -1};
        }

        return {nearest, minDist};
    }

    // Find properties within price range
    vector<string> findPropertiesInPriceRange(double minPrice, double maxPrice) {
        if (!isValidPrice(minPrice) || !isValidPrice(maxPrice)) {
            cout << "Error: Invalid price range!" << endl;
            return {};
        }

        if (minPrice > maxPrice) {
            cout << "Error: Minimum price cannot be greater than maximum price!" << endl;
            return {};
        }

        vector<string> results;
        for (const auto& [id, p] : properties) {
            if (p.isAvailable && p.price >= minPrice && p.price <= maxPrice) {
                results.push_back(id);
            }
        }
        return results;
    }

    // Find properties by type
    vector<string> findPropertiesByType(const string& type) {
        vector<string> results;
        for (const auto& [id, p] : properties) {
            if (p.isAvailable && p.type == type) {
                results.push_back(id);
            }
        }
        return results;
    }

    void printProperties() {
        for (auto& [id, p] : properties) {
            cout << "Property ID: " << id << endl;
            cout << "Name: " << p.name << endl;
            cout << "Address: " << p.address << endl;
            cout << "Type: " << p.type << endl;
            cout << "Price: $" << p.price << "/month" << endl;
            cout << "Bedrooms: " << p.bedrooms << endl;
            cout << "Bathrooms: " << p.bathrooms << endl;
            cout << "Area: " << p.area << " sq.m" << endl;
            cout << "Available: " << (p.isAvailable ? "Yes" : "No") << endl;
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
        for (auto& [id, p] : properties) {
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
            string id, name, address, type, price_str, bedrooms_str, bathrooms_str, area_str, available_str;
            vector<string> features;
            
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, address, ',');
            getline(ss, type, ',');
            getline(ss, price_str, ',');
            getline(ss, bedrooms_str, ',');
            getline(ss, bathrooms_str, ',');
            getline(ss, area_str, ',');
            getline(ss, available_str, ',');
            
            double price = stod(price_str);
            int bedrooms = stoi(bedrooms_str);
            int bathrooms = stoi(bathrooms_str);
            double area = stod(area_str);
            bool available = (available_str == "1");
            
            string feature;
            while (getline(ss, feature, ',')) {
                features.push_back(feature);
            }
            
            Property p;
            p.name = name;
            p.address = address;
            p.type = type;
            p.price = price;
            p.bedrooms = bedrooms;
            p.bathrooms = bathrooms;
            p.area = area;
            p.isAvailable = available;
            p.features = features;
            
            properties[id] = p;
        }
        in.close();
        cout << "Successfully loaded properties from " << filename << endl;
    }
};

int main() {
    RentalNetwork net;
    int choice;

    do {
        cout << "\n--- Rental Management System ---\n";
        cout << "1. Add Property\n2. Connect Properties\n3. View Properties\n4. View Connections\n";
        cout << "5. Update Property Information\n6. Find Nearest Available Property\n";
        cout << "7. Find Properties in Price Range\n8. Find Properties by Type\n";
        cout << "9. Save to CSV\n10. Load from CSV\n0. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            string id, name, address, type, feature;
            double price, area;
            int bedrooms, bathrooms, featureCount;
            bool isAvailable;
            Property p;
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin.ignore(); getline(cin, p.name);
            cout << "Address: "; getline(cin, p.address);
            cout << "Type (Apartment/House/Condo): "; getline(cin, p.type);
            cout << "Price per month: "; cin >> p.price;
            cout << "Bedrooms: "; cin >> p.bedrooms;
            cout << "Bathrooms: "; cin >> p.bathrooms;
            cout << "Area (sq.m): "; cin >> p.area;
            cout << "Available (1/0): "; cin >> isAvailable;
            p.isAvailable = isAvailable;
            cout << "Number of features: "; cin >> featureCount;
            cin.ignore();
            for (int i = 0; i < featureCount; ++i) {
                cout << "Feature " << i+1 << ": ";
                getline(cin, feature);
                p.features.push_back(feature);
            }
            net.addProperty(id, p);
        }
        else if (choice == 2) {
            string p1, p2;
            int dist;
            cout << "Property 1 ID: "; cin >> p1;
            cout << "Property 2 ID: "; cin >> p2;
            cout << "Distance (meters): "; cin >> dist;
            net.connectProperties(p1, p2, dist);
        }
        else if (choice == 3) {
            net.printProperties();
        }
        else if (choice == 4) {
            net.printGraph();
        }
        else if (choice == 5) {
            string id;
            double newPrice, newArea;
            int newBedrooms, newBathrooms;
            bool newAvailable;
            cout << "Property ID: "; cin >> id;
            cout << "New price: "; cin >> newPrice;
            cout << "New bedrooms: "; cin >> newBedrooms;
            cout << "New bathrooms: "; cin >> newBathrooms;
            cout << "New area: "; cin >> newArea;
            cout << "Available (1/0): "; cin >> newAvailable;
            net.updatePropertyInfo(id, "", "", "", newPrice, newBedrooms, newBathrooms, newArea, newAvailable, {});
        }
        else if (choice == 6) {
            string start;
            double maxPrice, minArea;
            int minBedrooms, minBathrooms;
            cout << "Start Location ID: "; cin >> start;
            cout << "Maximum price: "; cin >> maxPrice;
            cout << "Minimum bedrooms: "; cin >> minBedrooms;
            cout << "Minimum bathrooms: "; cin >> minBathrooms;
            cout << "Minimum area (sq.m): "; cin >> minArea;
            auto [nearest, dist] = net.findNearestProperty(start, maxPrice, minBedrooms, minBathrooms, minArea);
            if (nearest != "") {
                cout << "Nearest matching property: " << nearest << endl;
                cout << "Distance: " << dist << " meters\n";
            }
        }
        else if (choice == 7) {
            double minPrice, maxPrice;
            cout << "Minimum price: "; cin >> minPrice;
            cout << "Maximum price: "; cin >> maxPrice;
            auto results = net.findPropertiesInPriceRange(minPrice, maxPrice);
            cout << "\nProperties in price range:\n";
            for (const auto& id : results) {
                cout << id << endl;
            }
        }
        else if (choice == 8) {
            string type;
            cout << "Property type (Apartment/House/Condo): "; cin.ignore(); getline(cin, type);
            auto results = net.findPropertiesByType(type);
            cout << "\nProperties of type " << type << ":\n";
            for (const auto& id : results) {
                cout << id << endl;
            }
        }
        else if (choice == 9) {
            string filename;
            cout << "Enter filename to save: ";
            cin >> filename;
            net.saveToCSV(filename);
        }
        else if (choice == 10) {
            string filename;
            cout << "Enter filename to load: ";
            cin >> filename;
            net.loadFromCSV(filename);
        }

    } while (choice != 0);

    return 0;
} 