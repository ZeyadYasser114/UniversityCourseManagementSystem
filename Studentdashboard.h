#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <string>
#include <vector>
using namespace std;

#include "Student.h"
#include "CourseManager.h"

class StudentDashboard : public QMainWindow {
    Q_OBJECT

public:
    explicit StudentDashboard(Student *student, CourseManager *cm, QWidget *parent = nullptr);

private slots:
    void showTranscript();
    void showSchedule();
    void showRegisterCourse();
    void showDropCourse();
    void onRegisterClicked();
    void onDropClicked();
    void onLogout();

private:
    Student      *student;
    CourseManager *courseManager;

    QStackedWidget *stack;

    // Pages
    QWidget *makeHomePage();
    QWidget *makeTranscriptPage();
    QWidget *makeSchedulePage();
    QWidget *makeRegisterPage();
    QWidget *makeDropPage();

    // Register/Drop inputs
    QLineEdit *registerCourseInput;
    QLineEdit *dropCourseInput;
    QLabel    *registerStatus;
    QLabel    *dropStatus;

    QPushButton* makeNavButton(const QString &text);
    QString baseStyle();
};

#endif