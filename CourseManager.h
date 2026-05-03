#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include "Course.h"
#include <vector>

using namespace std;

class CourseManager {
private:
    vector<Course> courses;
    vector<Course> archivedCourses;

public:
    void addCourse(Course course);
    void removeCourse(string courseID);
    Course* findCourse(string courseID);
    vector<Course> listAllCourses();
    void updateCourseInfo(string courseID, string field, string value);
    void archiveCourse(string courseID);
    vector<Course> getArchivedCourses();
    vector<Course> searchCourses(string keyword);
};

#endif