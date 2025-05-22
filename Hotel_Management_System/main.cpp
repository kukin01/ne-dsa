#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;

struct Customer
{
    string name;
    string phone;
    string address;
    int days;
    vector<string> meals;
    double bill = 0;
};

struct Room
{
    int number;
    string type;
    double pricePerDay;
    bool isAvailable = true;
    int customerID = -1;
};
// Global Storage
map<int, Customer> customerDB;
vector<Room> rooms;
queue<Customer> waitlist;
int customerCounter = 1;

void initRooms()
{
    rooms.push_back({101, "Deluxe", 100});
    rooms.push_back({102, "Executive", 200});
    rooms.push_back({103, "Presidential", 300});
}
void bookRoom()
{
    Customer c;
    cout << "Name: ";
    cin.ignore();
    getline(cin, c.name);
    cout << "Phone: ";
    getline(cin, c.phone);
    cout << "Address: ";
    getline(cin, c.address);
    cout << "Days of Stay: ";
    cin >> c.days;

    for (auto &room : rooms)
    {
        if (room.isAvailable)
        {
            room.isAvailable = false;
            room.customerID = customerCounter;
            c.bill = room.pricePerDay * c.days;
            customerDB[customerCounter] = c;
            cout << "Room " << room.number << " booked for " << c.name << " (ID: " << customerCounter << ")\n";
            customerCounter++;
            return;
        }
    }

    cout << "No rooms available. Added to waitlist.\n";
    waitlist.push(c);
}
void orderMeal()
{
    int id;
    cout << "Enter customer ID: ";
    cin >> id;

    if (customerDB.count(id))
    {
        cout << "Select Meal (1. Breakfast - $10, 2. Lunch - $20, 3. Dinner - $25): ";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            customerDB[id].meals.push_back("Breakfast");
            customerDB[id].bill += 10;
            break;
        case 2:
            customerDB[id].meals.push_back("Lunch");
            customerDB[id].bill += 20;
            break;
        case 3:
            customerDB[id].meals.push_back("Dinner");
            customerDB[id].bill += 25;
            break;
        default:
            cout << "Invalid meal.\n";
            return;
        }
        cout << "Meal added successfully.\n";
    }
    else
    {
        cout << "Customer not found.\n";
    }
}
void showStatus()
{
    cout << "\n--- Room Allocation ---\n";
    for (auto &room : rooms)
    {
        cout << "Room " << room.number << " (" << room.type << "): ";
        if (room.isAvailable)
        {
            cout << "Available\n";
        }
        else
        {
            Customer &c = customerDB[room.customerID];
            cout << "Occupied by " << c.name << ", Phone: " << c.phone << ", Bill: $" << fixed << setprecision(2) << c.bill << "\n";
        }
    }

    cout << "\n--- Waitlist ---\n";
    queue<Customer> temp = waitlist;
    while (!temp.empty())
    {
        auto c = temp.front();
        temp.pop();
        cout << c.name << ", Phone: " << c.phone << endl;
    }
}

void editCustomer()
{
    int id;
    cout << "Enter customer ID to edit: ";
    cin >> id;

    if (customerDB.count(id))
    {
        Customer &c = customerDB[id];
        cout << "Editing " << c.name << ":\n";
        cout << "New name: ";
        cin.ignore();
        getline(cin, c.name);
        cout << "New phone: ";
        getline(cin, c.phone);
        cout << "New address: ";
        getline(cin, c.address);
        cout << "New days: ";
        cin >> c.days;
        for (auto &room : rooms)
            if (room.customerID == id)
                c.bill = room.pricePerDay * c.days;
        cout << "Details updated.\n";
    }
    else
    {
        cout << "Customer not found.\n";
    }
}
void checkout()
{
    int roomNo;
    cout << "Enter room number to checkout: ";
    cin >> roomNo;

    for (auto &room : rooms)
    {
        if (room.number == roomNo && !room.isAvailable)
        {
            Customer &c = customerDB[room.customerID];
            cout << "Checked out: " << c.name << ". Total bill: $" << fixed << setprecision(2) << c.bill << endl;
            customerDB.erase(room.customerID);
            room.isAvailable = true;
            room.customerID = -1;

            if (!waitlist.empty())
            {
                Customer next = waitlist.front();
                waitlist.pop();
                room.isAvailable = false;
                room.customerID = customerCounter;
                next.bill = room.pricePerDay * next.days;
                customerDB[customerCounter++] = next;
                cout << "Room " << room.number << " now allocated to " << next.name << " from waitlist.\n";
            }
            return;
        }
    }
    cout << "Room already vacant or invalid.\n";
}
int main()
{
    initRooms();
    int choice;

    do
    {
        cout << "\n--- Hotel Management ---\n";
        cout << "1. Book Room\n2. Order Meal\n3. Show Status\n4. Edit Customer\n5. Checkout\n0. Exit\nChoice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            bookRoom();
            break;
        case 2:
            orderMeal();
            break;
        case 3:
            showStatus();
            break;
        case 4:
            editCustomer();
            break;
        case 5:
            checkout();
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option.\n";
        }
    } while (choice != 0);

    return 0;
}

// queue for waitlist
// map for efficient customer access
// vector for room and meal management