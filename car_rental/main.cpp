#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Car structure
struct Car {
    int id;
    string brand;
    string model;
    int price;
    bool isAvailable = true;
};

// Customer structure
struct Customer {
    int id;
    string name;
    int age;
    string address;
    vector<int> rentedCarIds; // Track rental history
};

// Rental Record
struct RentalRecord {
    int carId;
    int customerId;
    string rentalDate;
    string returnDate;
};

class CarRentalSystem {
private:
    vector<Car> cars;
    vector<Customer> customers;
    vector<RentalRecord> rentals;

public:
    void addCar(int id, string brand,string model, int price) {
        cars.push_back({id, brand,model,price,true});
        cout << "✅ Car added successfully!\n";
    }

    void showAvailableCars() {
        cout << "\n Available Cars:\n";
        for (const auto& car : cars) {
            if (car.isAvailable)
                cout << "ID: " << car.id << ", " << car.brand << " " << car.model <<" "<<car.price <<"\n";
        }
    }

    void rentCar(int customerId, int carId, string rentalDate) {
        for (auto& car : cars) {
            if (car.id == carId && car.isAvailable) {
                car.isAvailable = false;

                // Update customer
                bool found = false;
                for (auto& customer : customers) {
                    if (customer.id == customerId) {
                        customer.rentedCarIds.push_back(carId);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "⚠️ Customer not found. Creating new one.\n";
                    Customer newCustomer;
                    newCustomer.id = customerId;
                    cout << "Enter customer name: ";
                    cin.ignore();
                    getline(cin, newCustomer.name);
                    newCustomer.rentedCarIds.push_back(carId);
                    customers.push_back(newCustomer);
                }

                // Create rental record
                rentals.push_back({carId, customerId, rentalDate, "N/A"});
                cout << "✅ Car rented successfully!\n";
                return;
            }
        }
        cout << "❌ Car not available or ID not found.\n";
    }

    void returnCar(int carId, string returnDate) {
        for (auto& car : cars) {
            if (car.id == carId && !car.isAvailable) {
                car.isAvailable = true;

                for (auto& record : rentals) {
                    if (record.carId == carId && record.returnDate == "N/A") {
                        record.returnDate = returnDate;
                        cout << "✅ Car returned successfully!\n";
                        return;
                    }
                }
            }
        }
        cout << "❌ Invalid return operation.\n";
    }

    void viewRentalHistory() {
        cout << "\n📜 Rental Records:\n";
        for (const auto& r : rentals) {
            cout << "Car ID: " << r.carId
                 << ", Customer ID: " << r.customerId
                 << ", Rented on: " << r.rentalDate
                 << ", Returned on: " << r.returnDate << "\n";
        }
    }
};

// ==== MAIN MENU ====
int main() {
    CarRentalSystem system;
    int choice;

    do {
        cout << "\n========== Car Rental System ==========\n";
        cout << "1. Add a Car\n";
        cout << "2. Show Available Cars\n";
        cout << "3. Rent a Car\n";
        cout << "4. Return a Car\n";
        cout << "5. View Rental History\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) {
            int id,price;
            string brand, model;
            cout << "Enter Car ID: ";
            cin >> id;
            cout << "Enter Brand: ";
            cin >> brand;
            cout << "Enter Model: ";
            cin >> model;
            cout << "Enter Price: ";
            cin >> price;
            system.addCar(id, brand, model,price);
        }
        else if (choice == 2) {
            system.showAvailableCars();
        }
        else if (choice == 3) {
            int custId, carId;
            string date;
            cout << "Enter Customer ID: ";
            cin >> custId;
            cout << "Enter Car ID to rent: ";
            cin >> carId;
            cout << "Enter Rental Date (YYYY-MM-DD): ";
            cin >> date;
            system.rentCar(custId, carId, date);
        }
        else if (choice == 4) {
            int carId;
            string returnDate;
            cout << "Enter Car ID to return: ";
            cin >> carId;
            cout << "Enter Return Date (YYYY-MM-DD): ";
            cin >> returnDate;
            system.returnCar(carId, returnDate);
        }
        else if (choice == 5) {
            system.viewRentalHistory();
        }
        else if (choice == 0) {
            cout << "👋 Exiting system. Goodbye!\n";
        }
        else {
            cout << "❌ Invalid option.\n";
        }

    } while (choice != 0);

    return 0;
}
