#ifndef ADMIN_H
#define ADMIN_H

#include <string>

using namespace std;

class Admin {
private:
    string adminID;
    string username;
    string password;
    string email;

public:
    Admin(string id, string user, string pwd, string e);

    string getAdminID();
    string getUsername();
    string getEmail();
    bool validatePassword(string pwd);
    void addStudent(string studentID, string name, string email);
    void deleteStudent(string studentID);
    void createCourse(string courseID, string name, int credits, int capacity);
    void editCourseInfo(string courseID, string field, string value);
    void assignInstructor(string courseID, string professorID);
};

#endif