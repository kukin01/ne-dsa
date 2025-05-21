#include <bits/stdc++.h>

using namespace std;

// patient struct

struct Patient
{
    int patient_id;
    string name;
    string dob;
    string gender;
    Patient *next;
};

// doctor struct

struct Doctor
{
    int doctor_id;
    string name;
    string specialization;
    Doctor *next;
};

// appointment struct

struct Appointment
{
    int appointment_id;
    int patient_id;
    int doctor_id;
    string date;
    Appointment *next;
};

// patients linked list

class PatientsLL
{
private:
//pointer to the first node
    Patient *head;

public:
    // linked list is empty head is initialized to nullptr
    PatientsLL() : head(nullptr) {}

    void addPatient(Patient newPatient)
    {
        Patient *newNode = new Patient(newPatient);
        //new node points to the old head of the list
        newNode->next = head;
        //update the node to the new node
        head = newNode;
    }

    bool exists(int patient_id)
    {
        //starting from the head of the list
        Patient *current = head;
        //loop through the list until it reaches the end (nullptr)
        while (current != nullptr)
        {
            if (current->patient_id == patient_id)
            {
                return true;
            }
            //updates the current to the next node and continues searching
            current = current->next;
        }
        return false;
    }

    string getPatientName(int patient_id)
    {
        Patient *current = head;
        while (current != nullptr)
        {
            if (current->patient_id == patient_id)
            {
                return current->name;
            }
            current = current->next;
        }
        return "";
    }

    void display()
    {
        Patient *current = head;
        while (current != nullptr)
        {
            cout << "Patient ID: " << current->patient_id << ", Name: " << current->name
                 << ", DOB: " << current->dob << ", Gender: " << current->gender << endl;
            current = current->next;
        }
    }
};
PatientsLL patientsLL;

// doctors linked list

class DoctorsLL
{
private:
    Doctor *head;

public:
    DoctorsLL() : head(nullptr) {}

    void addDoctor(Doctor newDoctor)
    {
        Doctor *newNode = new Doctor(newDoctor);
        newNode->next = head;
        head = newNode;
    }

    bool exists(int doctor_id)
    {
        Doctor *current = head;
        while (current != nullptr)
        {
            if (current->doctor_id == doctor_id)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    string getDoctorName(int doctor_id)
    {
        Doctor *current = head;
        while (current != nullptr)
        {
            if (current->doctor_id == doctor_id)
            {
                return current->name;
            }
            current = current->next;
        }
        return "";
    }

    void display()
    {
        Doctor *current = head;
        while (current != nullptr)
        {
            cout << "Doctor ID: " << current->doctor_id << ", Name: " << current->name
                 << ", Specialization: " << current->specialization << endl;
            current = current->next;
        }
    }
};

DoctorsLL doctorsLL;
// appointments linked list

class Appointments
{
private:
    Appointment *head;

public:
    Appointments() : head(nullptr) {}

    void addAppointment(Appointment newAppointment)
    {
        Appointment *newNode = new Appointment(newAppointment);
        newNode->next = head;
        head = newNode;
    }

    bool exists(int appointment_id)
    {
        Appointment *current = head;
        while (current != nullptr)
        {
            if (current->appointment_id == appointment_id)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void display()
    {
        Appointment *current = head;
        while (current != nullptr)
        {
            cout << "Appointment ID: " << current->appointment_id << ", Patient ID: " << current->patient_id<< ", Patient Name: "<< patientsLL.getPatientName(current -> patient_id)
                 << ", Doctor ID: " << current->doctor_id<< ", Doctor Name: "<< doctorsLL.getDoctorName(current -> doctor_id)<< ", Date: "<< current->date << endl;
            current = current->next;
        }
    }
};

// print a nice header, pass in the title of the header

string printNiceHeader(string headerContent)
{
    cout << "\n" + headerContent << endl;
    string result;
    for (int i = 0; i < headerContent.length(); i++)
    {
        result += "_";
    }
    return result;
}

// validate if an input is a string

bool validateString(const string &s)
{
    for (const char c : s)
    {
        if (!isalpha(c) && !isspace(c))
            return false;
    }
    return true;
}

// validate if an input is an integer

bool validateInteger()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid number." << endl;
        return false;
    }
    return true;
}

// validate if an input has correct date format

bool validateDate(const string &date)
{
    regex date_pattern("\\d{2}/\\d{2}/\\d{4}");
    if (!regex_match(date, date_pattern))
    {
        return false;
    }

    int day, month, year;
    sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);

    if (month < 1 || month > 12 || day < 1 || day > 31)
    {
        return false;
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        return false;
    }

    if (month == 2)
    {
        bool is_leap_year = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !is_leap_year))
        {
            return false;
        }
    }

    return true;
}

// instances of patients, doctors, and appointments linked lists.


Appointments appointmentsLL;

// register patient with proper validation

void registerPatient()
{
    cout << printNiceHeader("PATIENT REGISTRATION") << endl;

    Patient newPatient;
    cout << "ID: ";
    cin >> newPatient.patient_id;

    while (!validateInteger())
    {
        cout << "Error: Please enter a valid integer for Patient ID." << endl;
        cin >> newPatient.patient_id;
    }

    while (patientsLL.exists(newPatient.patient_id))
    {
        cout << "Error: Patient with this ID already exists." << endl;
        cin >> newPatient.patient_id;
    }
    cin.ignore();
    cout << "NAME: ";
    getline(cin, newPatient.name);
    while (!validateString(newPatient.name))
    {
        cout << "Invalid name. Please enter a valid name: ";
        getline(cin, newPatient.name);
    }
    cout << "DoB: ";
    getline(cin, newPatient.dob);
    while (!validateDate(newPatient.dob))
    {
        cout << "Invalid dob. Please enter a valid dob in the format dd/mm/yyyy: ";
        getline(cin, newPatient.dob);
    }
    cout << "GENDER: ";
    getline(cin, newPatient.gender);
    while (!validateString(newPatient.gender))
    {
        cout << "Invalid input. Please enter a valid Gender: ";
        getline(cin, newPatient.gender);
    }
    newPatient.next = nullptr;
    patientsLL.addPatient(newPatient);
    cout << endl;
}

// register doctor with proper validation

void registerDoctor()
{
    cout << printNiceHeader("DOCTOR REGISTRATION") << endl;

    Doctor newDoctor;
    cout << "ID: ";
    cin >> newDoctor.doctor_id;

    while (!validateInteger())
    {
        cout << "Error: Please enter a valid integer for Doctor ID." << endl;
        cin >> newDoctor.doctor_id;
    }

    while (doctorsLL.exists(newDoctor.doctor_id))
    {
        cout << "Error: Doctor with this ID already exists." << endl;
        cin >> newDoctor.doctor_id;
    }
    cin.ignore();
    cout << "NAME: ";
    getline(cin, newDoctor.name);
    while (!validateString(newDoctor.name))
    {
        cout << "Invalid name. Please enter a valid name: ";
        getline(cin, newDoctor.name);
    }
    cout << "SPECIALIZATION: ";
    getline(cin, newDoctor.specialization);
    while (!validateString(newDoctor.specialization))
    {
        cout << "Invalid specialization. Please enter a valid specialization: ";
        getline(cin, newDoctor.specialization);
    }
    newDoctor.next = nullptr;
    doctorsLL.addDoctor(newDoctor);
    cout << endl;
}

// register appointment with proper validation

void registerAppointment()
{
    cout << printNiceHeader("APPOINTMENT REGISTRATION") << endl;

    Appointment newAppointment;
    cout << "ID: ";
    cin >> newAppointment.appointment_id;

    while (!validateInteger())
    {
        cout << "Error: Please enter a valid integer for Appointment ID." << endl;
        cin >> newAppointment.appointment_id;
    }

    while (appointmentsLL.exists(newAppointment.appointment_id))
    {
        cout << "Error: Appointment with this ID already exists." << endl;
        cin >> newAppointment.appointment_id;
    }
    cout << "P_ID: ";
    cin >> newAppointment.patient_id;

    while (!validateInteger())
    {
        cout << "Error: Please enter a valid integer for Patient ID." << endl;
        cin >> newAppointment.patient_id;
    }

    if (!patientsLL.exists(newAppointment.patient_id))
    {
        cout << "Error: Patient with this ID does not exist." << endl;
        cin >> newAppointment.patient_id;
    }

    cout << "D_ID: ";
    cin >> newAppointment.doctor_id;

    while (!validateInteger())
    {
        cout << "Error: Please enter a valid integer for Doctor ID." << endl;
        newAppointment.doctor_id;
    }

    if (!doctorsLL.exists(newAppointment.doctor_id))
    {
        cout << "Error: Doctor with this ID does not exist." << endl;
        newAppointment.doctor_id;
    }
    cin.ignore();
    cout << "DATE: ";
    getline(cin, newAppointment.date);

    while (!validateDate(newAppointment.date))
    {
        cout << "Invalid date. Please enter a valid date (YYYY-MM-DD): ";
        getline(cin, newAppointment.date);
    }

    newAppointment.next = nullptr;
    appointmentsLL.addAppointment(newAppointment);
    cout << endl;
}

// display patients from the patients linked list

void displayPatients()
{
    patientsLL.display();
    cout << endl;
}

// display doctors from doctors linked list

void displayDoctors()
{
    doctorsLL.display();
    cout << endl;
}

// display appointments from appointments linked list

void displayAppointments()
{
    appointmentsLL.display();
    cout << endl;
}

int main()
{
    // choice variable, holds the option of the user from the list of available options
    int choice;

    while (true)
    {
        // print the available menu

        cout << "Menu: " << endl;
        cout << "1. Register a patient" << endl;
        cout << "2. Register a doctor" << endl;
        cout << "3. Register an appointment" << endl;
        cout << "4. Display patients" << endl;
        cout << "5. Display doctors" << endl;
        cout << "6. Display appointments" << endl;
        cout << "7. Exit" << endl;

        cout << "Enter your choice: ";

        cin >> choice;
        // validate whether the user's choice is an integer, if it is, continue, otherwise don't
        if (!validateInteger())
        {
            continue;
        }

        // run the above functions based off of the user's input.

        switch (choice)
        {
        case 1:
            registerPatient();
            break;
        case 2:
            registerDoctor();
            break;
        case 3:
            registerAppointment();
            break;
        case 4:
            displayPatients();
            break;
        case 5:
            displayDoctors();
            break;
        case 6:
            displayAppointments();
            break;
        case 7:
            return 0;
        default:
            cout << "Invalid option. Please choose again." << endl;
        }
    }

    return 0;
}
