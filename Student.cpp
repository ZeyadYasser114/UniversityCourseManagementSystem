#include "Student.h"
#include <iostream>

using namespace std;

Student::Student(string id, string n, string e, string pwd)
    : studentID(id), name(n), email(e), password(pwd), gpa(0.0), balance(0.0), scholarshipStatus(false) {}

string Student::getStudentID() {
    return studentID;
}

string Student::getName() {
    return name;
}

string Student::getEmail() {
    return email;
}

vector<string> Student::getEnrolledCourses() {
    return enrolledCourses;
}

double Student::getGPA() {
    return gpa;
}

double Student::getBalance() {
    return balance;
}

bool Student::hasScholarship() {
    return scholarshipStatus;
}

void Student::registerForCourse(string courseID) {
    enrolledCourses.push_back(courseID);
}

void Student::dropCourse(string courseID) {
    for (int i = 0; i < enrolledCourses.size(); i++) {
        if (enrolledCourses[i] == courseID) {
            enrolledCourses.erase(enrolledCourses.begin() + i);
            break;
        }
    }
}

void Student::getTranscript() {
    cout << "=== Student Transcript ===" << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "Name: " << name << endl;
    cout << "GPA: " << gpa << endl;
    cout << "Enrolled Courses: " << enrolledCourses.size() << endl;
}

void Student::getSchedule() {
    cout << "=== Student Schedule ===" << endl;
    cout << "Student ID: " << studentID << endl;
    cout << "Total Courses: " << enrolledCourses.size() << endl;
}

void Student::setGPA(double newGPA) {
    gpa = newGPA;
}

void Student::setBalance(double newBalance) {
    balance = newBalance;
}

void Student::setScholarshipStatus(bool status) {
    scholarshipStatus = status;
}

bool Student::validatePassword(string pwd) {
    return password == pwd;
}