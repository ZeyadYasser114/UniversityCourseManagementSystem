#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <functional>
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
                            vector<Student>   *students,
                            vector<Professor> *professors,
                            CourseManager     *cm,
                            function<void()>   saveCallback,
                            QWidget *parent = nullptr);

private slots:
    void onAddStudentClicked();
    void onDeleteStudentClicked();
    void onCreateCourseClicked();
    void onAssignInstructorClicked();
    void onLogout();

private:
    Admin             *admin;
    vector<Student>   *students;
    vector<Professor> *professors;
    CourseManager     *courseManager;
    function<void()>   save;

    QStackedWidget *stack;

    // Live stat labels (home page)
    QLabel *statStudents;
    QLabel *statProfessors;
    QLabel *statCourses;

    // Live table (students page)
    QTableWidget *studentsTable;

    // Add student inputs
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

    void refreshHomePage();
    void refreshStudentsPage();

    QPushButton *makeNavButton(const QString &text);
};
//git
#endif