#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <string>
#include <vector>

using namespace std;

class Professor {
private:
    string professorID;
    string name;
    string email;
    string password;
    string department;
    vector<string> assignedCourses;
    string officeHours;

public:
    Professor(string id, string n, string e, string pwd, string dept);

    string getProfessorID();
    string getName();
    string getEmail();
    string getDepartment();
    vector<string> getAssignedCourses();
    void assignCourse(string courseID);
    void submitGrade(string studentID, string courseID, string grade);
    void viewEnrolledStudents(string courseID);
    void generateCourseReport(string courseID);
    void updateOfficeHours(string schedule);
    bool validatePassword(string pwd);
};

#endif