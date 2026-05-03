#include "Course.h"

using namespace std;

Course::Course(string id, string n, int c, int cap, string sch, string instrID)
    : courseID(id), name(n), credits(c), capacity(cap), schedule(sch), instructorID(instrID), enrolledCount(0) {}

string Course::getCourseID() {
    return courseID;
}

string Course::getCourseName() {
    return name;
}

int Course::getCredits() {
    return credits;
}

int Course::getCapacity() {
    return capacity;
}

int Course::getEnrolledCount() {
    return enrolledCount;
}

bool Course::isAvailable() {
    return enrolledCount < capacity;
}

void Course::addStudent(string studentID) {
    if (isAvailable()) {
        enrolledStudents.push_back(studentID);
        enrolledCount++;
    }
}

void Course::removeStudent(string studentID) {
    for (int i = 0; i < enrolledStudents.size(); i++) {
        if (enrolledStudents[i] == studentID) {
            enrolledStudents.erase(enrolledStudents.begin() + i);
            enrolledCount--;
            break;
        }
    }
}

void Course::setInstructor(string professorID) {
    instructorID = professorID;
}

string Course::getSchedule() {
    return schedule;
}