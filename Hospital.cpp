#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department
{
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType
{
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //
class Patient
{
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:
    Patient(int pid, string n, int a, string c)
    {
        id = pid;
        name = n;
        age = a;
        contact = c;
        isAdmitted = false;
    }

    void admitPatient(RoomType type)
    {
        roomType = type;
        isAdmitted = true;
        addMedicalRecord("The patient is admitted.");
    }

    void dischargePatient()
    {
        isAdmitted = false;
        addMedicalRecord("The patient is discharged.");
    }

    void addMedicalRecord(string record)
    {
        medicalHistory.push(record);
    }

    void requestTest(string testName)
    {
        testQueue.push(testName);
        addMedicalRecord("Test (" + testName + ") is requested.");
    }

    string performTest()
    {
        if (!testQueue.empty())
        {
            string test = testQueue.front();
            addMedicalRecord("Test (" + test + ") is performed.");
            testQueue.pop();
            return "";
        }
        return "No tests pending";
    }

    void displayHistory()
    {
        stack<string> temp;

        while (!medicalHistory.empty())
        {
            string Top = medicalHistory.top();
            temp.push(Top);
            medicalHistory.pop();
        }

        cout << "Medical history of patient: " << endl;
        cout << "--------------------------------------" << endl;

        while (!temp.empty())
        {
            string record = temp.top();
            cout << record << endl;
            medicalHistory.push(record);
            temp.pop();
        }
    }

    int getId()
    {
        return id;
    }

    string getName()
    {
        return name;
    }

    bool getAdmissionStatus()
    {
        return isAdmitted;
    }
};

// ========== DOCTOR CLASS ========== //
class Doctor
{
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d)
    {
        id = did;
        name = n;
        department = d;
    }

    void addAppointment(int patientId)
    {
        appointmentQueue.push(patientId);
    }

    int seePatient()
    {
        if (appointmentQueue.empty())
        {
            cout << "No more Patients" << endl;
            return -1;
        }

        int next_Patient = appointmentQueue.front();
        appointmentQueue.pop();
        return next_Patient;
    }

    int getId()
    {
        return id;
    }
    string getName()
    {
        return name;
    }
    string getDepartment()
    {
        switch (department)
        {
        case CARDIOLOGY:
            return "CARDIOLOGY";
        case NEUROLOGY:
            return "NEUROLOGY";
        case ORTHOPEDICS:
            return "ORTHOPEDICS";
        case PEDIATRICS:
            return "PEDIATRICS";
        case EMERGENCY:
            return "EMERGENCY";
        case GENERAL:
            return "GENERAL";

        default:
            return "Unknown";
        }
    }
};

// ========== HOSPITAL CLASS ========== //
class Hospital
{
private:
    vector<Patient *> patients;
    vector<Doctor *> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:
    Hospital()
    {
        patientCounter = 1;
        doctorCounter = 1;
    }

    int registerPatient(string name, int age, string contact)
    {
        Patient *addPatient = new Patient(patientCounter, name, age, contact);
        patients.push_back(addPatient);
        cout << "Patient " << name << "  ID: " << patientCounter << " is registed successfully" << endl;
        return patientCounter++;
    }

    int addDoctor(string name, Department dept)
    {
        Doctor *extraDoctor = new Doctor(doctorCounter, name, dept);
        doctors.push_back(extraDoctor);
        cout << "Doctor " << name << "  ID: " << doctorCounter << " is added successfully" << endl;
        return doctorCounter++;
    }

    void admitPatient(int patientId, RoomType type)
    {
        for (auto &patient : patients)
        {
            if (patient->getId() == patientId)
            {
                if (patient->getAdmissionStatus())
                {
                    cout << "Patient " << patientId << " already admitted.\n";
                    return;
                }
                patient->admitPatient(type);
                cout << "Paitient " << patientId << "  admitted successfully.\n";
                return;
            }
        }
        cout << "Patient not found,\n";
    }
    void addEmergency(int patientId)
    {
        emergencyQueue.push(patientId);
        cout << "Emergency  added for patient ID: " << patientId << endl;
    }

    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
            cout << "No emergency cases.\n";
            return -1;
        }

        int patientId = emergencyQueue.front();
        emergencyQueue.pop();

        Patient *patientPtr = nullptr; // to add to medical records
        for (auto &pat : patients)
            if (pat->getId() == patientId)
                patientPtr = pat;

        if (patientPtr)
            patientPtr->addMedicalRecord("Emergency handeled for patient. ");

        cout << "Handling emergency for patient ID: " << patientId << endl;
        return patientId;
    }

    void bookAppointment(int doctorId, int patientId)
    {
        Doctor *doctorPtr = nullptr;
        Patient *patientPtr = nullptr;
        for (auto &doc : doctors)
            if (doc->getId() == doctorId)
                doctorPtr = doc;

        for (auto &pat : patients)
            if (pat->getId() == patientId)
                patientPtr = pat;
        if (!doctorPtr)
        {
            cout << "Doctor not found.\n";
            return;
        }
        if (!patientPtr)
        {
            cout << "Patient not found.\n";
            return;
        }
        doctorPtr->addAppointment(patientId);
        cout << "Appointment booked for patient ID: " << patientId << " Name: " << patientPtr->getName() << endl
             << " with doctor ID: " << doctorId << " Name: " << doctorPtr->getName() << endl;
        patientPtr->addMedicalRecord("Appointment booked with " + doctorPtr->getName() + "[" + doctorPtr->getDepartment() + "]"); // add to medical records
    }

    void displayPatientInfo(int patientId)
    {
        for (int i = 0; i < patients.size(); i++)
        {
            if (patients[i]->getId() == patientId)
            {
                cout << "Patient ID: " << patients[i]->getId() << endl;
                cout << "Name: " << patients[i]->getName() << endl;
                cout << "Admission Status: " << (patients[i]->getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
                cout << "Medical History: ";
                patients[i]->displayHistory();
                cout << endl;
                return;
            }
        }
        cout << "You entered wrong ID or Patient not found!" << endl;
    }
    void displayDoctorInfo(int doctorId)
    {
        for (int i = 0; i < doctors.size(); i++)
        {
            if (doctors[i]->getId() == doctorId)
            {
                cout << "Doctor ID: " << doctors[i]->getId() << endl;
                cout << "Doctor Name: " << doctors[i]->getName() << endl;
                cout << "Doctor Department: " << doctors[i]->getDepartment() << endl;
                return;
            }
        }
        cout << "You entered wrong ID or Doctor not found!" << endl;
    }
};
// ========== MAIN PROGRAM ========== //
int main()
{
    Hospital hospital;

    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient

    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients

    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies

    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects

    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor

    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors

    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    return 0;
}
