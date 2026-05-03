#include "AuthenticationManager.h"
#include <iostream>

using namespace std;

bool AuthenticationManager::loginAdmin(string username, string password) {
    cout << "Admin login attempt for user: " << username << endl;
    return true;
}

bool AuthenticationManager::loginStudent(string studentID, string password) {
    cout << "Student login attempt for ID: " << studentID << endl;
    return true;
}

bool AuthenticationManager::loginProfessor(string professorID, string password) {
    cout << "Professor login attempt for ID: " << professorID << endl;
    return true;
}

bool AuthenticationManager::registerStudent(string studentID, string name, string email, string password) {
    cout << "Registering student: " << name << " (ID: " << studentID << ")" << endl;
    return true;
}

bool AuthenticationManager::registerProfessor(string professorID, string name, string email, string password, string department) {
    cout << "Registering professor: " << name << " (ID: " << professorID << ")" << endl;
    return true;
}