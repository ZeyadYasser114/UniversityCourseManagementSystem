#ifndef DATASTORE_H
#define DATASTORE_H

#include <vector>
#include <map>
#include <string>
using namespace std;

#include <QString>

#include "Student.h"
#include "Professor.h"
#include "Admin.h"
#include "CourseManager.h"

class DataStore {
public:
    vector<Student>   students;
    vector<Professor> professors;
    vector<Admin>     admins;
    CourseManager     courseManager;

    void load();
    void save();
    map<string,int> buildCreditsMap();

private:
    static QString filePath();
    void seedDefaults();
};

#endif