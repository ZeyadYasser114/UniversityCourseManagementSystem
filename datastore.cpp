#include "DataStore.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

// ── Helpers ───────────────────────────────────────────────────────────────────

QString DataStore::filePath() {
    return QCoreApplication::applicationDirPath() + "/data.json";
}

map<string,int> DataStore::buildCreditsMap() {
    map<string,int> m;
    for (auto &c : courseManager.listAllCourses())
        m[c.getCourseID()] = c.getCredits();
    return m;
}

// ── Seed (first run only) ─────────────────────────────────────────────────────

void DataStore::seedDefaults() {
    courseManager.addCourse(Course("CS101", "Introduction to Programming", 3, 30, "MWF 9:00-10:00",  "P001"));
    courseManager.addCourse(Course("CS201", "Data Structures",             4, 25, "TTh 10:00-11:30", "P002"));
    courseManager.addCourse(Course("CS301", "Algorithms",                  3, 28, "MWF 11:00-12:00", "P001"));

    Student s1("S001", "John Smith",  "john@uni.edu",  "pass123");
    s1.registerForCourse("CS101");
    s1.registerForCourse("CS201");
    s1.setBalance(500.00);
    s1.setScholarshipStatus(true);
    students.push_back(s1);

    Student s2("S002", "Jane Doe", "jane@uni.edu", "pass456");
    s2.registerForCourse("CS101");
    students.push_back(s2);

    Professor p1("P001", "Dr. Robert Johnson", "rj@uni.edu",  "prof123", "Computer Science");
    p1.assignCourse("CS101");
    p1.assignCourse("CS301");
    professors.push_back(p1);

    Professor p2("P002", "Dr. Sarah Williams", "sw@uni.edu",  "prof456", "Computer Science");
    p2.assignCourse("CS201");
    professors.push_back(p2);

    admins.push_back(Admin("A001", "admin_user", "admin123", "admin@uni.edu"));

    // Sync course enrolled-student lists
    for (auto &s : students) {
        for (auto &cid : s.getEnrolledCourses()) {
            Course *c = courseManager.findCourse(cid);
            if (c) c->addStudent(s.getStudentID());
        }
    }
}

// ── Save ──────────────────────────────────────────────────────────────────────

void DataStore::save() {
    QJsonObject root;

    // ── Students ──
    QJsonArray studentsArr;
    for (auto &s : students) {
        QJsonObject obj;
        obj["id"]          = QString::fromStdString(s.getStudentID());
        obj["name"]        = QString::fromStdString(s.getName());
        obj["email"]       = QString::fromStdString(s.getEmail());
        obj["password"]    = QString::fromStdString(s.getPassword());
        obj["gpa"]         = s.getGPA();
        obj["balance"]     = s.getBalance();
        obj["scholarship"] = s.hasScholarship();

        QJsonArray enrolled;
        for (auto &cid : s.getEnrolledCourses())
            enrolled.append(QString::fromStdString(cid));
        obj["enrolledCourses"] = enrolled;

        QJsonObject gradesObj;
        for (auto &pair : s.getCourseGrades())
            gradesObj[QString::fromStdString(pair.first)] =
                QString::fromStdString(pair.second);
        obj["grades"] = gradesObj;

        studentsArr.append(obj);
    }
    root["students"] = studentsArr;

    // ── Professors ──
    QJsonArray profsArr;
    for (auto &p : professors) {
        QJsonObject obj;
        obj["id"]          = QString::fromStdString(p.getProfessorID());
        obj["name"]        = QString::fromStdString(p.getName());
        obj["email"]       = QString::fromStdString(p.getEmail());
        obj["password"]    = QString::fromStdString(p.getPassword());
        obj["department"]  = QString::fromStdString(p.getDepartment());
        obj["officeHours"] = QString::fromStdString(p.getOfficeHours());

        QJsonArray courses;
        for (auto &cid : p.getAssignedCourses())
            courses.append(QString::fromStdString(cid));
        obj["assignedCourses"] = courses;

        profsArr.append(obj);
    }
    root["professors"] = profsArr;

    // ── Admins ──
    QJsonArray adminsArr;
    for (auto &a : admins) {
        QJsonObject obj;
        obj["id"]       = QString::fromStdString(a.getAdminID());
        obj["username"] = QString::fromStdString(a.getUsername());
        obj["password"] = QString::fromStdString(a.getPassword());
        obj["email"]    = QString::fromStdString(a.getEmail());
        adminsArr.append(obj);
    }
    root["admins"] = adminsArr;

    // ── Courses ──
    QJsonArray coursesArr;
    for (auto &c : courseManager.listAllCourses()) {
        QJsonObject obj;
        obj["id"]           = QString::fromStdString(c.getCourseID());
        obj["name"]         = QString::fromStdString(c.getCourseName());
        obj["credits"]      = c.getCredits();
        obj["capacity"]     = c.getCapacity();
        obj["schedule"]     = QString::fromStdString(c.getSchedule());
        obj["instructorID"] = QString::fromStdString(c.getInstructorID());

        QJsonArray enrolled;
        for (auto &sid : c.getEnrolledStudents())
            enrolled.append(QString::fromStdString(sid));
        obj["enrolledStudents"] = enrolled;

        coursesArr.append(obj);
    }
    root["courses"] = coursesArr;

    QJsonDocument doc(root);
    QFile file(filePath());
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

// ── Load ──────────────────────────────────────────────────────────────────────

void DataStore::load() {
    QFile file(filePath());
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        seedDefaults();
        save();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull() || !doc.isObject()) {
        seedDefaults();
        save();
        return;
    }

    QJsonObject root = doc.object();

    // Courses first — students reference them
    for (auto val : root["courses"].toArray()) {
        QJsonObject obj = val.toObject();
        Course c(
            obj["id"].toString().toStdString(),
            obj["name"].toString().toStdString(),
            obj["credits"].toInt(),
            obj["capacity"].toInt(),
            obj["schedule"].toString().toStdString(),
            obj["instructorID"].toString().toStdString()
            );
        for (auto sid : obj["enrolledStudents"].toArray())
            c.addStudent(sid.toString().toStdString());
        courseManager.addCourse(c);
    }

    // Students
    for (auto val : root["students"].toArray()) {
        QJsonObject obj = val.toObject();
        Student s(
            obj["id"].toString().toStdString(),
            obj["name"].toString().toStdString(),
            obj["email"].toString().toStdString(),
            obj["password"].toString().toStdString()
            );
        s.setGPA(obj["gpa"].toDouble());
        s.setBalance(obj["balance"].toDouble());
        s.setScholarshipStatus(obj["scholarship"].toBool());

        for (auto cid : obj["enrolledCourses"].toArray())
            s.registerForCourse(cid.toString().toStdString());

        // Restore saved grades (don't recalculate — trust saved GPA)
        QJsonObject gradesObj = obj["grades"].toObject();
        for (auto key : gradesObj.keys()) {
            string cid   = key.toStdString();
            string grade = gradesObj[key].toString().toStdString();
            if (!grade.empty()) {
                // Directly insert into the map without triggering GPA recalc
                auto creditsMap = buildCreditsMap();
                s.setGradeForCourse(cid, grade, creditsMap);
            }
        }
        // Override with saved GPA (recalc may differ if credits changed)
        s.setGPA(obj["gpa"].toDouble());

        students.push_back(s);
    }

    // Professors
    for (auto val : root["professors"].toArray()) {
        QJsonObject obj = val.toObject();
        Professor p(
            obj["id"].toString().toStdString(),
            obj["name"].toString().toStdString(),
            obj["email"].toString().toStdString(),
            obj["password"].toString().toStdString(),
            obj["department"].toString().toStdString()
            );
        p.updateOfficeHours(obj["officeHours"].toString().toStdString());
        for (auto cid : obj["assignedCourses"].toArray())
            p.assignCourse(cid.toString().toStdString());
        professors.push_back(p);
    }

    // Admins
    for (auto val : root["admins"].toArray()) {
        QJsonObject obj = val.toObject();
        admins.push_back(Admin(
            obj["id"].toString().toStdString(),
            obj["username"].toString().toStdString(),
            obj["password"].toString().toStdString(),
            obj["email"].toString().toStdString()
            ));
    }
}