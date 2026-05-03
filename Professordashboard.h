#ifndef PROFESSORDASHBOARD_H
#define PROFESSORDASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLineEdit>
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
                                QWidget *parent = nullptr);

private slots:
    void onSubmitGradeClicked();
    void onUpdateOfficeHoursClicked();
    void onLogout();

private:
    Professor     *professor;
    CourseManager *courseManager;
    vector<Student> *students;

    QStackedWidget *stack;
    QLineEdit *gradeStudentID;
    QLineEdit *gradeCourseID;
    QLineEdit *gradeValue;
    QLabel    *gradeStatus;
    QLineEdit *officeHoursInput;
    QLabel    *officeStatus;

    QWidget *makeHomePage();
    QWidget *makeCoursesPage();
    QWidget *makeGradesPage();
    QWidget *makeOfficeHoursPage();
    QPushButton* makeNavButton(const QString &text);
};

#endif