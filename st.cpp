#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;
#pragma pack(push, 1)
class student {
    int rollno;
    char name[50];
    int p_marks, c_marks, m_marks, e_marks, cs_marks;
    double per;
    char Grade;

    void calculator(); // calculates grade
public:
    void getdata();
    void showdata() const;
    void show_tabular() const;
    int retrollno() const;
};
#pragma pack(pop)

void student::calculator() {
    per = (p_marks + c_marks + m_marks + e_marks + cs_marks) / 5.0;
    if (per >= 85) Grade = 'A';
    else if (per >= 75) Grade = 'B';
    else if (per >= 65) Grade = 'C';
    else Grade = 'F';
}

void student::getdata() {
    cout << "\nEnter Roll No: ";
    cin >> rollno;
    cout << "Enter Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Enter Physics marks: ";
    cin >> p_marks;
    cout << "Enter Chemistry marks: ";
    cin >> c_marks;
    cout << "Enter Maths marks: ";
    cin >> m_marks;
    cout << "Enter English marks: ";
    cin >> e_marks;
    cout << "Enter Computer Science marks: ";
    cin >> cs_marks;
    calculator();
}

void student::showdata() const {
    cout << "\nRoll No: " << rollno;
    cout << "\nName: " << name;
    cout << "\nPhysics: " << p_marks;
    cout << "\nChemistry: " << c_marks;
    cout << "\nMaths: " << m_marks;
    cout << "\nEnglish: " << e_marks;
    cout << "\nCS: " << cs_marks;
    cout << "\nPercentage: " << per;
    cout << "\nGrade: " << Grade << endl;
}

void student::show_tabular() const {
    cout << rollno << setw(10) << name << setw(6) << p_marks
         << setw(6) << c_marks << setw(6) << m_marks
         << setw(6) << e_marks << setw(6) << cs_marks
         << setw(8) << per << setw(6) << Grade << endl;
}

int student::retrollno() const {
    return rollno;
}

// --- Function Declarations ---
void write_student();
void display_all();
void display_sp(int);
void modify_student(int);
void delete_student(int);
void class_result();
void result();
void intro();
void entry_menu();

// --- Main Program ---
int main() {
    system("color 70");
    char ch;
    cout.setf(ios::fixed | ios::showpoint);
    cout << setprecision(2);
    intro();
    do {
        system("cls");
        cout << "\n\nMAIN MENU";
        cout << "\n1. RESULT MENU";
        cout << "\n2. ENTRY/EDIT MENU";
        cout << "\n3. EXIT";
        cout << "\n\nSelect your option (1-3): ";
        cin >> ch;
        switch (ch) {
            case '1': result(); break;
            case '2': entry_menu(); break;
            case '3': break;
            default: cout << "\a";
        }
    } while (ch != '3');
    return 0;
}

// --- Functions Implementation ---

void write_student() {
    student st;
    ofstream outFile("student.dat", ios::binary | ios::app);
    st.getdata();
    outFile.write(reinterpret_cast<char*>(&st), sizeof(student));
    outFile.close();
    cout << "\nStudent record has been created.";
    cin.ignore();
    cin.get();
}

void display_all() {
    student st;
    ifstream inFile("student.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened. Press any key...";
        cin.ignore(); cin.get();
        return;
    }
    cout << "\n\n\t\tALL STUDENTS RECORD\n\n";
    while (inFile.read(reinterpret_cast<char*>(&st), sizeof(student))) {
        st.showdata();
        cout << "\n-----------------------------------\n";
    }
    inFile.close();
    cin.ignore(); cin.get();
}

void display_sp(int n) {
    student st;
    ifstream inFile("student.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened. Press any key...";
        cin.ignore(); cin.get();
        return;
    }
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&st), sizeof(student))) {
        if (st.retrollno() == n) {
            st.showdata();
            found = true;
        }
    }
    inFile.close();
    if (!found) cout << "\n\nRecord does not exist.";
    cin.ignore(); cin.get();
}

void modify_student(int n) {
    student st;
    fstream file("student.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File could not be opened. Press any key...";
        cin.ignore(); cin.get();
        return;
    }
    bool found = false;
    while (!file.eof() && !found) {
        int pos = file.tellg();
        file.read(reinterpret_cast<char*>(&st), sizeof(student));
        if (st.retrollno() == n) {
            cout << "\nExisting record:";
            st.showdata();
            cout << "\nEnter new details:\n";
            st.getdata();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&st), sizeof(student));
            cout << "\n\nRecord updated.";
            found = true;
        }
    }
    file.close();
    if (!found) cout << "\n\nRecord not found.";
    cin.ignore(); cin.get();
}

void delete_student(int n) {
    student st;
    ifstream inFile("student.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened. Press any key...";
        cin.ignore(); cin.get();
        return;
    }
    ofstream outFile("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&st), sizeof(student))) {
        if (st.retrollno() != n) {
            outFile.write(reinterpret_cast<char*>(&st), sizeof(student));
        }
    }
    inFile.close();
    outFile.close();
    remove("student.dat");
    rename("temp.dat", "student.dat");
    cout << "\n\nRecord Deleted.";
    cin.ignore(); cin.get();
}

void class_result() {
    student st;
    ifstream inFile("student.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened. Press any key...";
        cin.ignore(); cin.get();
        return;
    }
    cout << "\n\nALL STUDENT RESULTS\n\n";
    cout << "============================================================\n";
    cout << "R.No      Name     P   C   M   E   CS    %age   Grade\n";
    cout << "============================================================\n";
    while (inFile.read(reinterpret_cast<char*>(&st), sizeof(student))) {
        st.show_tabular();
    }
    inFile.close();
    cin.ignore(); cin.get();
}

void result() {
    char ch;
    int rno;
    system("cls");
    cout << "\n\nRESULT MENU";
    cout << "\n1. Class Result";
    cout << "\n2. Student Report Card";
    cout << "\n3. Back to Main Menu";
    cout << "\nEnter your choice (1-3): ";
    cin >> ch;
    system("cls");
    switch (ch) {
        case '1': class_result(); break;
        case '2': cout << "Enter Roll No: "; cin >> rno; display_sp(rno); break;
        case '3': break;
        default: cout << "\a";
    }
}

void intro() {
    cout << "\n\n\tSTUDENT REPORT CARD PROJECT";
    cout << "\n\n\tMADE BY: SHIVESH CHOUKSEY";
    cout << "\n\n\tSCHOOL: ST PAUL SCHOOL\n";
    cin.ignore(); cin.get();
}

void entry_menu() {
    char ch;
    int num;
    system("cls");
    cout << "\n\nENTRY MENU";
    cout << "\n1. CREATE STUDENT RECORD";
    cout << "\n2. DISPLAY ALL STUDENTS RECORD";
    cout << "\n3. SEARCH STUDENT RECORD";
    cout << "\n4. MODIFY STUDENT RECORD";
    cout << "\n5. DELETE STUDENT RECORD";
    cout << "\n6. BACK TO MAIN MENU";
    cout << "\nEnter your choice (1-6): ";
    cin >> ch;
    switch (ch) {
        case '1': write_student(); break;
        case '2': display_all(); break;
        case '3': cout << "Enter Roll No: "; cin >> num; display_sp(num); break;
        case '4': cout << "Enter Roll No: "; cin >> num; modify_student(num); break;
        case '5': cout << "Enter Roll No: "; cin >> num; delete_student(num); break;
        case '6': break;
        default: cout << "\a"; entry_menu();
    }
}

