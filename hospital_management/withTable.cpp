#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <regex>
using namespace std;

// Patient struct
struct Patient {
    int patient_id;
    string name;
    string dob;
    string gender;
    Patient* next;
};

// Doctor struct
struct Doctor {
    int doctor_id;
    string name;
    string specialization;
    Doctor* next;
};

// Appointment struct
struct Appointment {
    int appointment_id;
    int patient_id;
    int doctor_id;
    string date;
    Appointment* next;
};

// Patients linked list
class PatientsLL {
private:
    Patient* head;

public:
    PatientsLL() : head(nullptr) {}

    void addPatient(Patient newPatient) {
        Patient* newNode = new Patient(newPatient);
        newNode->next = head;
        head = newNode;
    }

    bool exists(int patient_id) {
        Patient* current = head;
        while (current != nullptr) {
            if (current->patient_id == patient_id) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    string getPatientName(int patient_id) {
        Patient* current = head;
        while (current != nullptr) {
            if (current->patient_id == patient_id) {
                return current->name;
            }
            current = current->next;
        }
        return "";
    }

    void display() {
        vector<string> headers = {"Patient ID", "Name", "DOB", "Gender"};
        vector<int> widths = {10, 20, 12, 10};
        
        printTableHeader(headers, widths);
        
        Patient* current = head;
        while (current != nullptr) {
            vector<string> row = {
                to_string(current->patient_id),
                current->name,
                current->dob,
                current->gender
            };
            printTableRow(row, widths);
            current = current->next;
        }
        
        printTableFooter(widths);
        cout << endl;
    }
};

// Doctors linked list
class DoctorsLL {
private:
    Doctor* head;

public:
    DoctorsLL() : head(nullptr) {}

    void addDoctor(Doctor newDoctor) {
        Doctor* newNode = new Doctor(newDoctor);
        newNode->next = head;
        head = newNode;
    }

    bool exists(int doctor_id) {
        Doctor* current = head;
        while (current != nullptr) {
            if (current->doctor_id == doctor_id) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    string getDoctorName(int doctor_id) {
        Doctor* current = head;
        while (current != nullptr) {
            if (current->doctor_id == doctor_id) {
                return current->name;
            }
            current = current->next;
        }
        return "";
    }

    void display() {
        vector<string> headers = {"Doctor ID", "Name", "Specialization"};
        vector<int> widths = {10, 20, 20};
        
        printTableHeader(headers, widths);
        
        Doctor* current = head;
        while (current != nullptr) {
            vector<string> row = {
                to_string(current->doctor_id),
                current->name,
                current->specialization
            };
            printTableRow(row, widths);
            current = current->next;
        }
        
        printTableFooter(widths);
        cout << endl;
    }
};

// Appointments linked list
class Appointments {
private:
    Appointment* head;
    PatientsLL& patientsLL;
    DoctorsLL& doctorsLL;

public:
    Appointments(PatientsLL& pLL, DoctorsLL& dLL) : head(nullptr), patientsLL(pLL), doctorsLL(dLL) {}

    void addAppointment(Appointment newAppointment) {
        Appointment* newNode = new Appointment(newAppointment);
        newNode->next = head;
        head = newNode;
    }

    bool exists(int appointment_id) {
        Appointment* current = head;
        while (current != nullptr) {
            if (current->appointment_id == appointment_id) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void display() {
        vector<string> headers = {"Appt ID", "Patient ID", "Patient Name", "Doctor ID", "Doctor Name", "Date"};
        vector<int> widths = {8, 10, 20, 10, 20, 12};
        
        printTableHeader(headers, widths);
        
        Appointment* current = head;
        while (current != nullptr) {
            vector<string> row = {
                to_string(current->appointment_id),
                to_string(current->patient_id),
                patientsLL.getPatientName(current->patient_id),
                to_string(current->doctor_id),
                doctorsLL.getDoctorName(current->doctor_id),
                current->date
            };
            printTableRow(row, widths);
            current = current->next;
        }
        
        printTableFooter(widths);
        cout << endl;
    }
};

// Helper functions for table display
void printTableHeader(const vector<string>& headers, const vector<int>& widths) {
    cout << "+";
    for (int width : widths) {
        cout << setfill('-') << setw(width + 2) << "+";
    }
    cout << setfill(' ') << endl;
    
    cout << "|";
    for (size_t i = 0; i < headers.size(); ++i) {
        cout << " " << left << setw(widths[i]) << headers[i] << " |";
    }
    cout << endl;
    
    cout << "+";
    for (int width : widths) {
        cout << setfill('-') << setw(width + 2) << "+";
    }
    cout << setfill(' ') << endl;
}

void printTableRow(const vector<string>& values, const vector<int>& widths) {
    cout << "|";
    for (size_t i = 0; i < values.size(); ++i) {
        cout << " " << left << setw(widths[i]) << values[i] << " |";
    }
    cout << endl;
}

void printTableFooter(const vector<int>& widths) {
    cout << "+";
    for (int width : widths) {
        cout << setfill('-') << setw(width + 2) << "+";
    }
    cout << setfill(' ') << endl;
}

// Validation functions
bool validateString(const string& s) {
    for (const char c : s) {
        if (!isalpha(c) && !isspace(c))
            return false;
    }
    return true;
}

bool validateInteger() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid number." << endl;
        return false;
    }
    return true;
}

bool validateDate(const string& date) {
    regex date_pattern("\\d{2}/\\d{2}/\\d{4}");
    if (!regex_match(date, date_pattern)) {
        return false;
    }

    int day, month, year;
    sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);

    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return false;
    }

    if (month == 2) {
        bool is_leap_year = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !is_leap_year)) {
            return false;
        }
    }

    return true;
}

// Registration functions
void registerPatient(PatientsLL& patientsLL) {
    cout << "\nPATIENT REGISTRATION" << endl;
    cout << "-------------------" << endl;

    Patient newPatient;
    cout << "ID: ";
    cin >> newPatient.patient_id;

    while (!validateInteger()) {
        cout << "Error: Please enter a valid integer for Patient ID." << endl;
        cin >> newPatient.patient_id;
    }

    while (patientsLL.exists(newPatient.patient_id)) {
        cout << "Error: Patient with this ID already exists." << endl;
        cin >> newPatient.patient_id;
    }
    cin.ignore();
    cout << "NAME: ";
    getline(cin, newPatient.name);
    while (!validateString(newPatient.name)) {
        cout << "Invalid name. Please enter a valid name: ";
        getline(cin, newPatient.name);
    }
    cout << "DoB: ";
    getline(cin, newPatient.dob);
    while (!validateDate(newPatient.dob)) {
        cout << "Invalid dob. Please enter a valid dob in the format dd/mm/yyyy: ";
        getline(cin, newPatient.dob);
    }
    cout << "GENDER: ";
    getline(cin, newPatient.gender);
    while (!validateString(newPatient.gender)) {
        cout << "Invalid input. Please enter a valid Gender: ";
        getline(cin, newPatient.gender);
    }
    newPatient.next = nullptr;
    patientsLL.addPatient(newPatient);
    cout << endl;
}

void registerDoctor(DoctorsLL& doctorsLL) {
    cout << "\nDOCTOR REGISTRATION" << endl;
    cout << "------------------" << endl;

    Doctor newDoctor;
    cout << "ID: ";
    cin >> newDoctor.doctor_id;

    while (!validateInteger()) {
        cout << "Error: Please enter a valid integer for Doctor ID." << endl;
        cin >> newDoctor.doctor_id;
    }

    while (doctorsLL.exists(newDoctor.doctor_id)) {
        cout << "Error: Doctor with this ID already exists." << endl;
        cin >> newDoctor.doctor_id;
    }
    cin.ignore();
    cout << "NAME: ";
    getline(cin, newDoctor.name);
    while (!validateString(newDoctor.name)) {
        cout << "Invalid name. Please enter a valid name: ";
        getline(cin, newDoctor.name);
    }
    cout << "SPECIALIZATION: ";
    getline(cin, newDoctor.specialization);
    while (!validateString(newDoctor.specialization)) {
        cout << "Invalid specialization. Please enter a valid specialization: ";
        getline(cin, newDoctor.specialization);
    }
    newDoctor.next = nullptr;
    doctorsLL.addDoctor(newDoctor);
    cout << endl;
}

void registerAppointment(Appointments& appointmentsLL, PatientsLL& patientsLL, DoctorsLL& doctorsLL) {
    cout << "\nAPPOINTMENT REGISTRATION" << endl;
    cout << "-----------------------" << endl;

    Appointment newAppointment;
    cout << "ID: ";
    cin >> newAppointment.appointment_id;

    while (!validateInteger()) {
        cout << "Error: Please enter a valid integer for Appointment ID." << endl;
        cin >> newAppointment.appointment_id;
    }

    while (appointmentsLL.exists(newAppointment.appointment_id)) {
        cout << "Error: Appointment with this ID already exists." << endl;
        cin >> newAppointment.appointment_id;
    }
    cout << "P_ID: ";
    cin >> newAppointment.patient_id;

    while (!validateInteger()) {
        cout << "Error: Please enter a valid integer for Patient ID." << endl;
        cin >> newAppointment.patient_id;
    }

    if (!patientsLL.exists(newAppointment.patient_id)) {
        cout << "Error: Patient with this ID does not exist." << endl;
        cin >> newAppointment.patient_id;
    }

    cout << "D_ID: ";
    cin >> newAppointment.doctor_id;

    while (!validateInteger()) {
        cout << "Error: Please enter a valid integer for Doctor ID." << endl;
        cin >> newAppointment.doctor_id;
    }

    if (!doctorsLL.exists(newAppointment.doctor_id)) {
        cout << "Error: Doctor with this ID does not exist." << endl;
        cin >> newAppointment.doctor_id;
    }
    cin.ignore();
    cout << "DATE: ";
    getline(cin, newAppointment.date);

    while (!validateDate(newAppointment.date)) {
        cout << "Invalid date. Please enter a valid date (dd/mm/yyyy): ";
        getline(cin, newAppointment.date);
    }

    newAppointment.next = nullptr;
    appointmentsLL.addAppointment(newAppointment);
    cout << endl;
}

int main() {
    PatientsLL patientsLL;
    DoctorsLL doctorsLL;
    Appointments appointmentsLL(patientsLL, doctorsLL);

    int choice;

    while (true) {
        cout << "\nMenu: " << endl;
        cout << "1. Register a patient" << endl;
        cout << "2. Register a doctor" << endl;
        cout << "3. Register an appointment" << endl;
        cout << "4. Display patients" << endl;
        cout << "5. Display doctors" << endl;
        cout << "6. Display appointments" << endl;
        cout << "7. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        if (!validateInteger()) {
            continue;
        }

        switch (choice) {
            case 1:
                registerPatient(patientsLL);
                break;
            case 2:
                registerDoctor(doctorsLL);
                break;
            case 3:
                registerAppointment(appointmentsLL, patientsLL, doctorsLL);
                break;
            case 4:
                patientsLL.display();
                break;
            case 5:
                doctorsLL.display();
                break;
            case 6:
                appointmentsLL.display();
                break;
            case 7:
                return 0;
            default:
                cout << "Invalid option. Please choose again." << endl;
        }
    }

    return 0;
}