#ifndef PROFESSORDASHBOARD_H
#define PROFESSORDASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QComboBox>
#include <functional>
#include <string>
#include <vector>
using namespace std;

#include "Professor.h"
#include "CourseManager.h"
#include "Student.h"

class ProfessorDashboard : public QMainWindow {
    Q_OBJECT

public:
    explicit ProfessorDashboard(Professor *professor,
                                CourseManager *cm,
                                vector<Student> *students,
                                function<void()> saveCallback,
                                QWidget *parent = nullptr);

private slots:
    void onSubmitGradeClicked();
    void onUpdateOfficeHoursClicked();
    void onLogout();

private:
    Professor        *professor;
    CourseManager    *courseManager;
    vector<Student>  *students;
    function<void()>  save;

    QStackedWidget *stack;

    QTableWidget *coursesTable;

    QLineEdit *gradeStudentID;
    QLineEdit *gradeCourseID;
    QComboBox *gradeValue;
    QLabel    *gradeStatus;

    QLineEdit *officeHoursInput;
    QLabel    *officeStatus;

    QWidget *makeHomePage();
    QWidget *makeCoursesPage();
    QWidget *makeGradesPage();
    QWidget *makeOfficeHoursPage();

    void refreshCoursesPage();

    QPushButton *makeNavButton(const QString &text);
};
//git
#endif