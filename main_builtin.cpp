#include <bits/stdc++.h>

using namespace std;

struct Patient {
    int patient_id;
    string name;
    string dob;
    string gender;
};

struct Doctor {
    int doctor_id;
    string name;
    string specialization;
};

struct Appointment {
    int appointment_id;
    int patient_id;
    int doctor_id;
    string date;
};

// creating empty lists that references the respective strucutres 
//patient, doctor and appointment
list<Patient> patientsLL;
list<Doctor> doctorsLL;
list<Appointment> appointments;

void registerPatient() {
    //new patient stucture is created
    Patient newPatient;
    cout << "Enter patient ID: ";
    //prompted to enter patient ID
    cin >> newPatient.patient_id;
    for (auto it = patientsLL.begin(); it != patientsLL.end(); it++) {
        // it->patient_id is a shorthand for (*it).patient_id
        if (it->patient_id == newPatient.patient_id) {
            cout << "Error: Patient with this ID already exists." << endl;
            return;
        }
    }
    cout << "Enter patient name: ";
    cin >> newPatient.name;
    cout << "Enter patient DOB: ";
    cin >> newPatient.dob;
    cout << "Enter patient gender: ";
    cin >> newPatient.gender;
    patientsLL.push_back(newPatient);
}

void registerDoctor() {
    Doctor newDoctor;
    cout << "Enter doctor ID: ";
    cin >> newDoctor.doctor_id;
    for (auto it = doctorsLL.begin(); it != doctorsLL.end(); it++) {
        if (it->doctor_id == newDoctor.doctor_id) {
            cout << "Error: Doctor with this ID already exists." << endl;
            return;
        }
    }
    cout << "Enter doctor name: ";
    cin >> newDoctor.name;
    cout << "Enter doctor specialization: ";
    cin >> newDoctor.specialization;
    // adds the element from the end of the list
    doctorsLL.push_back(newDoctor);
}

void registerAppointment() {
    Appointment newAppointment;
    cout << "Enter appointment ID: ";
    cin >> newAppointment.appointment_id;
    for (auto it = appointments.begin(); it != appointments.end(); it++) {
        if (it->appointment_id == newAppointment.appointment_id) {
            cout << "Error: Appointment with this ID already exists." << endl;
            return;
        }
    }
    cout << "Enter patient ID: ";
    cin >> newAppointment.patient_id;
    for (auto it = patientsLL.begin(); it != patientsLL.end(); it++) {
        if (it->patient_id == newAppointment.patient_id) {
            break;
        } else {
            cout << "Error: Patient with this ID does not exist." << endl;
            return;
        }
    }
    cout << "Enter doctor ID: ";
    cin >> newAppointment.doctor_id;
    for (auto it = doctorsLL.begin(); it != doctorsLL.end(); it++) {
        if (it->doctor_id == newAppointment.doctor_id) {
            break;
        } else {
            cout << "Error: Doctor with this ID does not exist." << endl;
            return;
        }
    }
    cout << "Enter appointment date: ";
    cin >> newAppointment.date;
    appointments.push_back(newAppointment);
}

void displayPatients() {
    for (auto it = patientsLL.begin(); it != patientsLL.end(); it++) {
        cout << "Patient ID: " << it->patient_id << ", Name: " << it->name << ", DOB: " << it->dob << ", Gender: " << it->gender << endl;
    }
}

void displayDoctors() {
    for (auto it = doctorsLL.begin(); it != doctorsLL.end(); it++) {
        cout << "Doctor ID: " << it->doctor_id << ", Name: " << it->name << ", Specialization: " << it->specialization << endl;
    }
}

void displayAppointments() {
    for (auto it = appointments.begin(); it != appointments.end(); it++) {
        cout << "Appointment ID: " << it->appointment_id << ", Patient ID: " << it->patient_id << ", Doctor ID: " << it->doctor_id << ", Date: " << it->date << endl;
    }
}

int main() {
    int choice;

    while (true) {
        cout << "1. Register patient" << endl;
        cout << "2. Register doctor" << endl;
        cout << "3. Register appointment" << endl;
        cout << "4. Display patients" << endl;
        cout << "5. Display doctors" << endl;
        cout << "6. Display appointments" << endl;
        cout << "7. Exit" << endl;

        cout << "Menu: ";
        cin >> choice;

        switch (choice) {
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