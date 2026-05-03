#include "Professor.h"
#include <iostream>

using namespace std;

Professor::Professor(string id, string n, string e, string pwd, string dept)
    : professorID(id), name(n), email(e), password(pwd), department(dept) {}

string Professor::getProfessorID() {
    return professorID;
}

string Professor::getName() {
    return name;
}

string Professor::getEmail() {
    return email;
}

string Professor::getDepartment() {
    return department;
}

vector<string> Professor::getAssignedCourses() {
    return assignedCourses;
}

void Professor::assignCourse(string courseID) {
    assignedCourses.push_back(courseID);
}

void Professor::submitGrade(string studentID, string courseID, string grade) {
    cout << "Grade " << grade << " submitted for Student " << studentID << " in Course " << courseID << endl;
}

void Professor::viewEnrolledStudents(string courseID) {
    cout << "=== Enrolled Students for Course " << courseID << " ===" << endl;
}

void Professor::generateCourseReport(string courseID) {
    cout << "=== Course Report for " << courseID << " ===" << endl;
}

void Professor::updateOfficeHours(string schedule) {
    officeHours = schedule;
    cout << "Office hours updated to: " << schedule << endl;
}

bool Professor::validatePassword(string pwd) {
    return password == pwd;
}