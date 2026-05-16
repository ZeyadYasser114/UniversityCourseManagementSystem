#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <string>
#include <vector>
using namespace std;

class Professor {
private:
    string         professorID;
    string         name;
    string         email;
    string         password;
    string         department;
    vector<string> assignedCourses;
    string         officeHours;

public:
    Professor(string id, string n, string e, string pwd, string dept);

    string getProfessorID();
    string getName();
    string getEmail();
    string getPassword();
    string getDepartment();
    string getOfficeHours();
    vector<string> getAssignedCourses();

    void assignCourse(string courseID);
    void updateOfficeHours(string schedule);
    bool validatePassword(string pwd);
};
//git
#endif