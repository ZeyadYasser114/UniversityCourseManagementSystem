#include "mainwindow.h"
#include "studentdashboard.h"
#include "professordashboard.h"
#include "admindashboard.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    seedData();

    setWindowTitle("University Course Management System");
    setMinimumSize(420, 340);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(50, 40, 50, 40);
    layout->setSpacing(12);

    QLabel *title = new QLabel("UCMS");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #4e9eff; font-size: 32px; font-weight: bold;");

    QLabel *sub = new QLabel("University Course Management System");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color: #5a7a99; font-size: 12px; margin-bottom: 10px;");

    roleSelector = new QComboBox();
    roleSelector->addItem("Student");
    roleSelector->addItem("Professor");
    roleSelector->addItem("Admin");
    roleSelector->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    idInput = new QLineEdit();
    idInput->setPlaceholderText("ID / Username");
    idInput->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *loginBtn = new QPushButton("Login");
    loginBtn->setStyleSheet(
        "background-color: #4e9eff; color: white; padding: 10px; "
        "border-radius: 6px; font-size: 14px; font-weight: bold;");

    statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");

    layout->addWidget(title);
    layout->addWidget(sub);
    layout->addWidget(roleSelector);
    layout->addWidget(idInput);
    layout->addWidget(passwordInput);
    layout->addWidget(loginBtn);
    layout->addWidget(statusLabel);

    setCentralWidget(central);

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
}

void MainWindow::seedData() {
    // Seed courses
    courseManager.addCourse(Course("CS101", "Introduction to Programming", 3, 30, "MWF 9:00-10:00", "P001"));
    courseManager.addCourse(Course("CS201", "Data Structures", 4, 25, "TTh 10:00-11:30", "P002"));
    courseManager.addCourse(Course("CS301", "Algorithms", 3, 28, "MWF 11:00-12:00", "P001"));

    // Seed students (enrolled in some courses)
    Student s1("S001", "John Smith",  "john@uni.edu", "pass123");
    s1.registerForCourse("CS101");
    s1.registerForCourse("CS201");
    s1.setGPA(3.85);
    s1.setBalance(500.00);
    s1.setScholarshipStatus(true);
    students.push_back(s1);

    Student s2("S002", "Jane Doe", "jane@uni.edu", "pass456");
    s2.registerForCourse("CS101");
    s2.setGPA(3.50);
    students.push_back(s2);

    // Seed professors
    Professor p1("P001", "Dr. Robert Johnson", "rj@uni.edu", "prof123", "Computer Science");
    p1.assignCourse("CS101");
    p1.assignCourse("CS301");
    professors.push_back(p1);

    Professor p2("P002", "Dr. Sarah Williams", "sw@uni.edu", "prof456", "Computer Science");
    p2.assignCourse("CS201");
    professors.push_back(p2);

    // Seed admin
    admins.push_back(Admin("A001", "admin_user", "admin123", "admin@uni.edu"));
}

void MainWindow::onLoginClicked() {
    string id   = idInput->text().toStdString();
    string pwd  = passwordInput->text().toStdString();
    string role = roleSelector->currentText().toStdString();

    if (role == "Student") {
        for (auto &s : students) {
            if (s.getStudentID() == id && s.validatePassword(pwd)) {
                StudentDashboard *dash = new StudentDashboard(&s, &courseManager);
                dash->show();
                hide();
                return;
            }
        }
    } else if (role == "Professor") {
        for (auto &p : professors) {
            if (p.getProfessorID() == id && p.validatePassword(pwd)) {
                ProfessorDashboard *dash = new ProfessorDashboard(
                    &p, &courseManager, &students);
                dash->show();
                hide();
                return;
            }
        }
    } else if (role == "Admin") {
        for (auto &a : admins) {
            if (a.getUsername() == id && a.validatePassword(pwd)) {
                AdminDashboard *dash = new AdminDashboard(
                    &a, &students, &professors, &courseManager);
                dash->show();
                hide();
                return;
            }
        }
    }

    statusLabel->setText("Invalid ID or password.");
}