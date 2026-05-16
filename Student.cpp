#include "Student.h"
#include <iostream>
using namespace std;

// Grade-point mapping (standard 4.0 scale)
double Student::letterToPoints(const string &grade) {
    if (grade == "A")  return 4.0;
    if (grade == "A-") return 3.7;
    if (grade == "B+") return 3.3;
    if (grade == "B")  return 3.0;
    if (grade == "B-") return 2.7;
    if (grade == "C+") return 2.3;
    if (grade == "C")  return 2.0;
    if (grade == "C-") return 1.7;
    if (grade == "F")  return 0.0;
    return -1.0; // not graded yet
}

Student::Student(string id, string n, string e, string pwd)
    : studentID(id), name(n), email(e), password(pwd),
    gpa(0.0), balance(0.0), scholarshipStatus(false) {}

string Student::getStudentID()  { return studentID; }
string Student::getName()       { return name; }
string Student::getEmail()      { return email; }
string Student::getPassword()   { return password; }
double Student::getGPA()        { return gpa; }
double Student::getBalance()    { return balance; }
bool   Student::hasScholarship(){ return scholarshipStatus; }

vector<string>     Student::getEnrolledCourses() { return enrolledCourses; }
map<string,string> Student::getCourseGrades()    { return courseGrades; }

string Student::getGradeForCourse(const string &courseID) {
    auto it = courseGrades.find(courseID);
    if (it != courseGrades.end()) return it->second;
    return "";
}

void Student::registerForCourse(string courseID) {
    enrolledCourses.push_back(courseID);
    // Initialise with no grade
    if (courseGrades.find(courseID) == courseGrades.end())
        courseGrades[courseID] = "";
}

void Student::dropCourse(string courseID) {
    for (int i = 0; i < (int)enrolledCourses.size(); i++) {
        if (enrolledCourses[i] == courseID) {
            enrolledCourses.erase(enrolledCourses.begin() + i);
            break;
        }
    }
    courseGrades.erase(courseID);
}

void Student::setGradeForCourse(const string &courseID, const string &grade,
                                const map<string,int> &courseCredits) {
    courseGrades[courseID] = grade;
    recalculateGPA(courseCredits);
}

void Student::recalculateGPA(const map<string,int> &courseCredits) {
    double totalPoints  = 0.0;
    int    totalCredits = 0;

    for (auto &pair : courseGrades) {
        if (pair.second.empty()) continue;           // not graded yet
        double pts = letterToPoints(pair.second);
        if (pts < 0) continue;                       // unrecognised grade string

        int credits = 3; // default fallback
        auto it = courseCredits.find(pair.first);
        if (it != courseCredits.end()) credits = it->second;

        totalPoints  += pts * credits;
        totalCredits += credits;
    }

    gpa = (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0;
}

void Student::setGPA(double v)              { gpa = v; }
void Student::setBalance(double v)          { balance = v; }
void Student::setScholarshipStatus(bool v)  { scholarshipStatus = v; }
bool Student::validatePassword(string pwd)  { return password == pwd; }
//git