#include "CourseManager.h"
using namespace std;

void CourseManager::addCourse(Course course) {
    courses.push_back(course);
}

void CourseManager::removeCourse(string courseID) {
    for (int i = 0; i < (int)courses.size(); i++) {
        if (courses[i].getCourseID() == courseID) {
            courses.erase(courses.begin() + i);
            break;
        }
    }
}

Course* CourseManager::findCourse(string courseID) {
    for (int i = 0; i < (int)courses.size(); i++)
        if (courses[i].getCourseID() == courseID)
            return &courses[i];
    return nullptr;
}

vector<Course> CourseManager::listAllCourses() {
    return courses;
}
//git