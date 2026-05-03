#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <string>
#include <vector>
using namespace std;

#include "Admin.h"
#include "Student.h"
#include "Professor.h"
#include "CourseManager.h"

class AdminDashboard : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminDashboard(Admin *admin,
                            vector<Student> *students,
                            vector<Professor> *professors,
                            CourseManager *cm,
                            QWidget *parent = nullptr);

private slots:
    void onAddStudentClicked();
    void onDeleteStudentClicked();
    void onCreateCourseClicked();
    void onAssignInstructorClicked();
    void onLogout();

private:
    Admin           *admin;
    vector<Student>   *students;
    vector<Professor> *professors;
    CourseManager     *courseManager;

    QStackedWidget *stack;

    // Add student
    QLineEdit *addStudentID, *addStudentName, *addStudentEmail, *addStudentPwd;
    QLabel    *addStudentStatus;

    // Delete student
    QLineEdit *delStudentID;
    QLabel    *delStudentStatus;

    // Create course
    QLineEdit *courseID, *courseName, *courseSchedule, *courseInstructor;
    QSpinBox  *courseCredits, *courseCapacity;
    QLabel    *createCourseStatus;

    // Assign instructor
    QLineEdit *assignCourseID, *assignProfID;
    QLabel    *assignStatus;

    QWidget *makeHomePage();
    QWidget *makeStudentsPage();
    QWidget *makeAddStudentPage();
    QWidget *makeDeleteStudentPage();
    QWidget *makeCreateCoursePage();
    QWidget *makeAssignInstructorPage();
    QPushButton* makeNavButton(const QString &text);
};

#endif