#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

using namespace std;

class Student {
private:
    string studentID;
    string name;
    string email;
    string password;
    vector<string> enrolledCourses;
    double gpa;
    double balance;
    bool scholarshipStatus;

public:
    Student(string id, string n, string e, string pwd);

    string getStudentID();
    string getName();
    string getEmail();
    vector<string> getEnrolledCourses();
    double getGPA();
    double getBalance();
    bool hasScholarship();
    void registerForCourse(string courseID);
    void dropCourse(string courseID);
    void getTranscript();
    void getSchedule();
    void setGPA(double newGPA);
    void setBalance(double newBalance);
    void setScholarshipStatus(bool status);
    bool validatePassword(string pwd);
};

#endif