#include "Admin.h"
#include <iostream>

using namespace std;

Admin::Admin(string id, string user, string pwd, string e)
    : adminID(id), username(user), password(pwd), email(e) {}

string Admin::getAdminID() {
    return adminID;
}

string Admin::getUsername() {
    return username;
}

string Admin::getEmail() {
    return email;
}

bool Admin::validatePassword(string pwd) {
    return password == pwd;
}

void Admin::addStudent(string studentID, string name, string email) {
    cout << "Student " << studentID << " (" << name << ") added successfully." << endl;
}

void Admin::deleteStudent(string studentID) {
    cout << "Student " << studentID << " deleted and archived." << endl;
}

void Admin::createCourse(string courseID, string name, int credits, int capacity) {
    cout << "Course " << courseID << " (" << name << ") created with capacity " << capacity << "." << endl;
}

void Admin::editCourseInfo(string courseID, string field, string value) {
    cout << "Course " << courseID << " field '" << field << "' updated to '" << value << "'." << endl;
}

void Admin::assignInstructor(string courseID, string professorID) {
    cout << "Professor " << professorID << " assigned to Course " << courseID << "." << endl;
}