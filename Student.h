#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <map>
using namespace std;

class Student {
private:
    string             studentID;
    string             name;
    string             email;
    string             password;
    vector<string>     enrolledCourses;
    map<string,string> courseGrades;   // courseID -> letter grade (empty = not yet graded)
    double             gpa;
    double             balance;
    bool               scholarshipStatus;

    static double letterToPoints(const string &grade);

public:
    Student(string id, string n, string e, string pwd);

    string getStudentID();
    string getName();
    string getEmail();
    string getPassword();
    double getGPA();
    double getBalance();
    bool   hasScholarship();

    vector<string>     getEnrolledCourses();
    map<string,string> getCourseGrades();
    string             getGradeForCourse(const string &courseID);

    void registerForCourse(string courseID);
    void dropCourse(string courseID);

    // Sets the letter grade for a course then recomputes GPA.
    // courseCredits maps courseID -> credit hours (used for weighted avg).
    void setGradeForCourse(const string &courseID, const string &grade,
                           const map<string,int> &courseCredits);

    // Recomputes GPA from scratch given a credits map.
    void recalculateGPA(const map<string,int> &courseCredits);

    void setGPA(double v);
    void setBalance(double v);
    void setScholarshipStatus(bool v);
    bool validatePassword(string pwd);
};
//git
#endif