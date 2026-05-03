#include "UI.h"
#include <iostream>

using namespace std;

void UI::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void UI::printMainMenu() {
    clearScreen();
    cout << "\n";
    cout << "==========================================\n";
    cout << "|  UNIVERSITY COURSE MANAGEMENT SYSTEM   |\n";
    cout << "==========================================\n\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Exit\n\n";
    cout << "Enter your choice: ";
}

void UI::printLoginMenu() {
    clearScreen();
    cout << "\n╔═══════════════════════════╗\n";
    cout << "║      LOGIN TO SYSTEM      ║\n";
    cout << "╚═══════════════════════════╝\n\n";
}

void UI::printAdminMenu() {
    clearScreen();
    cout << "\n╔═══════���═══════════════════════════════╗\n";
    cout << "║         ADMIN DASHBOARD               ║\n";
    cout << "╚═══════════════════════════════════════╝\n\n";
    cout << "1. Add Student\n";
    cout << "2. Delete Student\n";
    cout << "3. Create Course\n";
    cout << "4. Edit Course Info\n";
    cout << "5. Assign Instructor\n";
    cout << "6. View All Courses\n";
    cout << "7. Search Course\n";
    cout << "8. Logout\n\n";
    cout << "Enter your choice: ";
}

void UI::printStudentMenu() {
    clearScreen();
    cout << "\n╔═══════════════════════════════════════╗\n";
    cout << "║        STUDENT DASHBOARD              ║\n";
    cout << "╚═══════════════════════════════════════╝\n\n";
    cout << "1. View Transcript\n";
    cout << "2. View Schedule\n";
    cout << "3. Register for Course\n";
    cout << "4. Drop Course\n";
    cout << "5. Logout\n\n";
    cout << "Enter your choice: ";
}

void UI::printProfessorMenu() {
    clearScreen();
    cout << "==========================================\n";
    cout << "|         PROFESSOR DASHBOARD            |\n";
    cout << "==========================================\n\n";
    cout << "1. View Assigned Courses\n";
    cout << "2. Submit Grades\n";
    cout << "3. View Enrolled Students\n";
    cout << "4. Generate Course Report\n";
    cout << "5. Update Office Hours\n";
    cout << "6. Logout\n\n";
    cout << "Enter your choice: ";
}

void UI::printAddStudentMenu() {
    clearScreen();
    cout << "\n╔═══════════════════════════════════════╗\n";
    cout << "║         ADD NEW STUDENT               ║\n";
    cout << "╚═══════════════════════════════════════╝\n\n";
}

void UI::printDeleteStudentMenu() {
    clearScreen();
    cout << "\n╔═══════════════════════════════════════╗\n";
    cout << "║         DELETE STUDENT                ║\n";
    cout << "╚═══════════════════════════════════════╝\n\n";
}

void UI::printCreateCourseMenu() {
    clearScreen();
    cout << "\n╔═══════════════════════════════════════╗\n";
    cout << "║         CREATE NEW COURSE             ║\n";
    cout << "╚═══════════════════════════════════════╝\n\n";
}

void UI::printEditCourseMenu() {
    clearScreen();
    cout << "\n╔═══════════════════════════════════════╗\n";
    cout << "║         EDIT COURSE INFO              ║\n";
    cout << "╚═══════════════════════════════════════╝\n\n";
}

void UI::printMessage(string msg) {
    cout << msg << endl;
}

void UI::printError(string msg) {
    cout << "\n❌ ERROR: " << msg << endl;
}

void UI::printSuccess(string msg) {
    cout << "\n✓ SUCCESS: " << msg << endl;
}

string UI::promptInput(string label) {
    string input;
    cout << label << ": ";
    getline(cin, input);
    return input;
}

void UI::displayTable(vector<string> headers, vector<vector<string>> data) {
    cout << "\n";
    for (auto header : headers) {
        cout << header << "\t";
    }
    cout << "\n";
    for (auto row : data) {
        for (auto cell : row) {
            cout << cell << "\t";
        }
        cout << "\n";
    }
}