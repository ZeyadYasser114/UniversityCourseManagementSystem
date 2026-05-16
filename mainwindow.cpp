#include "mainwindow.h"
#include "studentdashboard.h"
#include "professordashboard.h"
#include "admindashboard.h"

#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    store.load();

    setWindowTitle("Academic Information Management System");
    setMinimumSize(420, 340);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(50, 40, 50, 40);
    layout->setSpacing(12);

    QLabel *title = new QLabel("A I M S");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #C8B89A; font-size: 32px;");

    QLabel *sub = new QLabel("Academic Information Management System");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color: #8A8478; font-size: 12px; margin-bottom: 10px;");

    roleSelector = new QComboBox();
    roleSelector->addItem("Student");
    roleSelector->addItem("Professor");
    roleSelector->addItem("Admin");
    roleSelector->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    idInput = new QLineEdit();
    idInput->setPlaceholderText("ID / Username");
    idInput->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *loginBtn = new QPushButton("Login");
    loginBtn->setStyleSheet(
        "background-color: #C8B89A; color: #1C1C1A; padding: 10px; "
        "border-radius: 6px; font-size: 14px; font-weight: bold;");

    statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #C0745A; font-size: 12px;");

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

void MainWindow::onLoginClicked() {
    string id   = idInput->text().toStdString();
    string pwd  = passwordInput->text().toStdString();
    string role = roleSelector->currentText().toStdString();

    if (role == "Student") {
        for (auto &s : store.students) {
            if (s.getStudentID() == id && s.validatePassword(pwd)) {
                auto *dash = new StudentDashboard(&s, &store.courseManager,
                                                  [this]{ store.save(); });
                dash->show();
                hide();
                return;
            }
        }
    } else if (role == "Professor") {
        for (auto &p : store.professors) {
            if (p.getProfessorID() == id && p.validatePassword(pwd)) {
                auto *dash = new ProfessorDashboard(&p, &store.courseManager,
                                                    &store.students,
                                                    [this]{ store.save(); });
                dash->show();
                hide();
                return;
            }
        }
    } else if (role == "Admin") {
        for (auto &a : store.admins) {
            if (a.getUsername() == id && a.validatePassword(pwd)) {
                auto *dash = new AdminDashboard(&a, &store.students,
                                                &store.professors,
                                                &store.courseManager,
                                                [this]{ store.save(); });
                dash->show();
                hide();
                return;
            }
        }
    }

    statusLabel->setText("Invalid ID or password.");
}
//git