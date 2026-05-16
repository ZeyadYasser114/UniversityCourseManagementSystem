#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <functional>
#include <string>
#include <vector>
using namespace std;

#include "Student.h"
#include "CourseManager.h"

class StudentDashboard : public QMainWindow {
    Q_OBJECT

public:
    explicit StudentDashboard(Student *student,
                              CourseManager *cm,
                              function<void()> saveCallback,
                              QWidget *parent = nullptr);

private slots:
    void showTranscript();
    void showSchedule();
    void showRegisterCourse();
    void showDropCourse();
    void onRegisterClicked();
    void onDropClicked();
    void onLogout();

private:
    Student          *student;
    CourseManager    *courseManager;
    function<void()>  save;

    QStackedWidget *stack;

    // Home stat labels
    QLabel *homeGPA;
    QLabel *homeCourseCount;
    QLabel *homeBalance;
    QLabel *homeScholarship;

    // Live tables
    QTableWidget *transcriptTable;
    QLabel       *transcriptGPALabel;
    QTableWidget *scheduleTable;
    QTableWidget *registerTable;

    // Inputs
    QLineEdit *registerCourseInput;
    QLineEdit *dropCourseInput;
    QLabel    *registerStatus;
    QLabel    *dropStatus;

    QWidget *makeHomePage();
    QWidget *makeTranscriptPage();
    QWidget *makeSchedulePage();
    QWidget *makeRegisterPage();
    QWidget *makeDropPage();

    void refreshHomePage();
    void refreshTranscriptPage();
    void refreshSchedulePage();
    void refreshRegisterPage();

    QPushButton *makeNavButton(const QString &text);
};
//git
#endif