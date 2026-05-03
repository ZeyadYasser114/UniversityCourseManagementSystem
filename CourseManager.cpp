#include "CourseManager.h"
#include <iostream>

using namespace std;

void CourseManager::addCourse(Course course) {
    courses.push_back(course);
}

void CourseManager::removeCourse(string courseID) {
    for (int i = 0; i < courses.size(); i++) {
        if (courses[i].getCourseID() == courseID) {
            courses.erase(courses.begin() + i);
            break;
        }
    }
}

Course* CourseManager::findCourse(string courseID) {
    for (int i = 0; i < courses.size(); i++) {
        if (courses[i].getCourseID() == courseID) {
            return &courses[i];
        }
    }
    return nullptr;
}

vector<Course> CourseManager::listAllCourses() {
    return courses;
}

void CourseManager::updateCourseInfo(string courseID, string field, string value) {
    Course* course = findCourse(courseID);
    if (course != nullptr) {
    }
}

void CourseManager::archiveCourse(string courseID) {
    Course* course = findCourse(courseID);
    if (course != nullptr) {
        archivedCourses.push_back(*course);
        removeCourse(courseID);
    }
}

vector<Course> CourseManager::getArchivedCourses() {
    return archivedCourses;
}

vector<Course> CourseManager::searchCourses(string keyword) {
    vector<Course> results;
    for (int i = 0; i < courses.size(); i++) {
        if (courses[i].getCourseName().find(keyword) != string::npos ||
            courses[i].getCourseID().find(keyword) != string::npos) {
            results.push_back(courses[i]);
            }
    }
    return results;
}