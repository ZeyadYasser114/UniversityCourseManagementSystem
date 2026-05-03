#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include <string>
#include <vector>
using namespace std;

#include "Student.h"
#include "Professor.h"
#include "Admin.h"
#include "CourseManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onLoginClicked();

private:
    QLineEdit *idInput;
    QLineEdit *passwordInput;
    QComboBox *roleSelector;
    QLabel    *statusLabel;

    vector<Student>   students;
    vector<Professor> professors;
    vector<Admin>     admins;
    CourseManager     courseManager;

    void seedData();
};

#endif