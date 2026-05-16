#include "Professor.h"
using namespace std;

Professor::Professor(string id, string n, string e, string pwd, string dept)
    : professorID(id), name(n), email(e), password(pwd), department(dept) {}

string Professor::getProfessorID() { return professorID; }
string Professor::getName()        { return name; }
string Professor::getEmail()       { return email; }
string Professor::getPassword()    { return password; }
string Professor::getDepartment()  { return department; }
string Professor::getOfficeHours() { return officeHours; }

vector<string> Professor::getAssignedCourses() { return assignedCourses; }

void Professor::assignCourse(string courseID) {
    assignedCourses.push_back(courseID);
}

void Professor::updateOfficeHours(string schedule) {
    officeHours = schedule;
}

bool Professor::validatePassword(string pwd) { return password == pwd; }
//git