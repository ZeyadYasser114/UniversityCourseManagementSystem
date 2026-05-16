#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include "Course.h"
#include <vector>
using namespace std;

class CourseManager {
private:
    vector<Course> courses;

public:
    void    addCourse(Course course);
    void    removeCourse(string courseID);
    Course* findCourse(string courseID);
    vector<Course> listAllCourses();
};
//git
#endif