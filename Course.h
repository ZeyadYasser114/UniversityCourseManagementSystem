#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

using namespace std;

class Course {
private:
    string courseID;
    string name;
    int credits;
    int capacity;
    int enrolledCount;
    string schedule;
    string instructorID;
    vector<string> enrolledStudents;

public:
    Course(string id, string n, int c, int cap, string sch, string instrID);

    string getCourseID();
    string getCourseName();
    int getCredits();
    int getCapacity();
    int getEnrolledCount();
    bool isAvailable();
    void addStudent(string studentID);
    void removeStudent(string studentID);
    void setInstructor(string professorID);
    string getSchedule();
};

#endif