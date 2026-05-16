#include "studentdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QHeaderView>

StudentDashboard::StudentDashboard(Student *s, CourseManager *cm,
                                   function<void()> saveCallback,
                                   QWidget *parent)
    : QMainWindow(parent), student(s), courseManager(cm), save(saveCallback),
    homeGPA(nullptr), homeCourseCount(nullptr),
    homeBalance(nullptr), homeScholarship(nullptr),
    transcriptTable(nullptr), transcriptGPALabel(nullptr),
    scheduleTable(nullptr), registerTable(nullptr)
{
    setWindowTitle("AIMS - Student Dashboard");
    setMinimumSize(800, 550);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #1C1C1A;");
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet("background-color: #252523;");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setSpacing(6);
    sideLayout->setContentsMargins(12, 20, 12, 20);

    QLabel *logo = new QLabel("AIMS");
    logo->setStyleSheet("color: #C8B89A; font-size: 22px; padding: 10px 0;");
    QLabel *role = new QLabel("Student Portal");
    role->setStyleSheet("color: #8A8478; font-size: 11px; margin-bottom: 20px;");

    QPushButton *btnHome       = makeNavButton("🏠  Home");
    QPushButton *btnTranscript = makeNavButton("📋  Transcript");
    QPushButton *btnSchedule   = makeNavButton("📅  Schedule");
    QPushButton *btnRegister   = makeNavButton("➕  Register Course");
    QPushButton *btnDrop       = makeNavButton("➖  Drop Course");
    QPushButton *btnLogout     = makeNavButton("🚪  Logout");
    btnLogout->setStyleSheet(btnLogout->styleSheet() + "color: #C0745A;");

    sideLayout->addWidget(logo);
    sideLayout->addWidget(role);
    sideLayout->addWidget(btnHome);
    sideLayout->addWidget(btnTranscript);
    sideLayout->addWidget(btnSchedule);
    sideLayout->addWidget(btnRegister);
    sideLayout->addWidget(btnDrop);
    sideLayout->addStretch();
    sideLayout->addWidget(btnLogout);

    stack = new QStackedWidget();
    stack->addWidget(makeHomePage());       // 0
    stack->addWidget(makeTranscriptPage()); // 1
    stack->addWidget(makeSchedulePage());   // 2
    stack->addWidget(makeRegisterPage());   // 3
    stack->addWidget(makeDropPage());       // 4

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);
    setCentralWidget(central);

    connect(btnHome,       &QPushButton::clicked, [this]{ refreshHomePage();       stack->setCurrentIndex(0); });
    connect(btnTranscript, &QPushButton::clicked, this, &StudentDashboard::showTranscript);
    connect(btnSchedule,   &QPushButton::clicked, this, &StudentDashboard::showSchedule);
    connect(btnRegister,   &QPushButton::clicked, this, &StudentDashboard::showRegisterCourse);
    connect(btnDrop,       &QPushButton::clicked, this, &StudentDashboard::showDropCourse);
    connect(btnLogout,     &QPushButton::clicked, this, &StudentDashboard::onLogout);
}

QPushButton* StudentDashboard::makeNavButton(const QString &text) {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(
        "QPushButton { background: transparent; color: #8A8478; "
        "text-align: left; padding: 10px 12px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #3A3A36; color: #F0EDE3; }");
    return btn;
}

// ── Home ──────────────────────────────────────────────────────────────────────

QWidget* StudentDashboard::makeHomePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *welcome = new QLabel(QString("Welcome back, %1")
                                     .arg(QString::fromStdString(student->getName())));
    welcome->setStyleSheet("color: #F0EDE3; font-size: 24px; font-weight: bold;");

    QLabel *sub = new QLabel(QString("Student ID: %1")
                                 .arg(QString::fromStdString(student->getStudentID())));
    sub->setStyleSheet("color: #8A8478; font-size: 13px;");

    QHBoxLayout *stats = new QHBoxLayout();
    stats->setSpacing(16);

    auto makeCard = [](const QString &title, QLabel *&lbl) {
        QWidget *card = new QWidget();
        card->setStyleSheet("background-color: #252523; border-radius: 5px; padding: 5px;");
        card->setFixedHeight(90);
        QVBoxLayout *cl = new QVBoxLayout(card);
        QLabel *t = new QLabel(title);
        t->setStyleSheet("color: #8A8478; font-size: 11px;");
        lbl = new QLabel("—");
        lbl->setStyleSheet("color: #C8B89A; font-size: 26px; font-weight: bold;");
        cl->addWidget(t);
        cl->addWidget(lbl);
        return card;
    };

    stats->addWidget(makeCard("GPA",              homeGPA));
    stats->addWidget(makeCard("Enrolled Courses", homeCourseCount));
    stats->addWidget(makeCard("Balance",          homeBalance));
    stats->addWidget(makeCard("Scholarship",      homeScholarship));

    layout->addWidget(welcome);
    layout->addWidget(sub);
    layout->addLayout(stats);
    layout->addStretch();

    refreshHomePage();
    return page;
}

void StudentDashboard::refreshHomePage() {
    if (homeGPA)         homeGPA->setText(QString::number(student->getGPA(), 'f', 2));
    if (homeCourseCount) homeCourseCount->setText(QString::number(student->getEnrolledCourses().size()));
    if (homeBalance)     homeBalance->setText("$" + QString::number(student->getBalance(), 'f', 2));
    if (homeScholarship) homeScholarship->setText(student->hasScholarship() ? "Active" : "None");
}

// ── Transcript ────────────────────────────────────────────────────────────────

QWidget* StudentDashboard::makeTranscriptPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("Academic Transcript");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    transcriptTable = new QTableWidget();
    transcriptTable->setColumnCount(4);
    transcriptTable->setHorizontalHeaderLabels({"Course ID", "Course Name", "Credits", "Grade"});
    transcriptTable->setStyleSheet(
        "QTableWidget { background-color: #252523; color: #F0EDE3; "
        "gridline-color: #3A3A36; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #1C1C1A; color: #C8B89A; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }");
    transcriptTable->horizontalHeader()->setStretchLastSection(true);
    transcriptTable->verticalHeader()->setVisible(false);
    transcriptTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    transcriptGPALabel = new QLabel();
    transcriptGPALabel->setStyleSheet("color: #C8B89A; font-size: 15px; font-weight: bold;");

    layout->addWidget(title);
    layout->addWidget(transcriptTable);
    layout->addWidget(transcriptGPALabel);

    refreshTranscriptPage();
    return page;
}

void StudentDashboard::refreshTranscriptPage() {
    if (!transcriptTable) return;
    auto courses = student->getEnrolledCourses();
    transcriptTable->setRowCount(0);
    transcriptTable->setRowCount((int)courses.size());

    for (int i = 0; i < (int)courses.size(); i++) {
        string cid   = courses[i];
        Course *c    = courseManager->findCourse(cid);
        string grade = student->getGradeForCourse(cid);

        transcriptTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(cid)));
        transcriptTable->setItem(i, 1, new QTableWidgetItem(
                                           c ? QString::fromStdString(c->getCourseName()) : "N/A"));
        transcriptTable->setItem(i, 2, new QTableWidgetItem(
                                           c ? QString::number(c->getCredits()) : "N/A"));
        transcriptTable->setItem(i, 3, new QTableWidgetItem(
                                           grade.empty() ? "—" : QString::fromStdString(grade)));
    }

    if (transcriptGPALabel)
        transcriptGPALabel->setText(
            QString("Cumulative GPA: %1").arg(student->getGPA(), 0, 'f', 2));
}

// ── Schedule ──────────────────────────────────────────────────────────────────

QWidget* StudentDashboard::makeSchedulePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("My Schedule");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    scheduleTable = new QTableWidget();
    scheduleTable->setColumnCount(3);
    scheduleTable->setHorizontalHeaderLabels({"Course ID", "Course Name", "Schedule"});
    scheduleTable->setStyleSheet(
        "QTableWidget { background-color: #252523; color: #F0EDE3; "
        "gridline-color: #3A3A36; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #1C1C1A; color: #C8B89A; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }");
    scheduleTable->horizontalHeader()->setStretchLastSection(true);
    scheduleTable->verticalHeader()->setVisible(false);
    scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(title);
    layout->addWidget(scheduleTable);
    layout->addStretch();

    refreshSchedulePage();
    return page;
}

void StudentDashboard::refreshSchedulePage() {
    if (!scheduleTable) return;
    auto courses = student->getEnrolledCourses();
    scheduleTable->setRowCount(0);
    scheduleTable->setRowCount((int)courses.size());
    for (int i = 0; i < (int)courses.size(); i++) {
        string cid = courses[i];
        Course *c  = courseManager->findCourse(cid);
        scheduleTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(cid)));
        scheduleTable->setItem(i, 1, new QTableWidgetItem(
                                         c ? QString::fromStdString(c->getCourseName()) : "N/A"));
        scheduleTable->setItem(i, 2, new QTableWidgetItem(
                                         c ? QString::fromStdString(c->getSchedule()) : "N/A"));
    }
}

// ── Register ──────────────────────────────────────────────────────────────────

QWidget* StudentDashboard::makeRegisterPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Register for a Course");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    QLabel *avail = new QLabel("Available Courses:");
    avail->setStyleSheet("color: #8A8478; font-size: 13px;");

    registerTable = new QTableWidget();
    registerTable->setColumnCount(4);
    registerTable->setHorizontalHeaderLabels({"ID", "Name", "Credits", "Seats Left"});
    registerTable->setStyleSheet(
        "QTableWidget { background-color: #252523; color: #F0EDE3; "
        "gridline-color: #3A3A36; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #1C1C1A; color: #C8B89A; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }");
    registerTable->horizontalHeader()->setStretchLastSection(true);
    registerTable->verticalHeader()->setVisible(false);
    registerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    registerCourseInput = new QLineEdit();
    registerCourseInput->setPlaceholderText("Enter Course ID to register");
    registerCourseInput->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Register");
    btn->setStyleSheet(
        "background-color: #C8B89A; color: #1C1C1A; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    registerStatus = new QLabel("");
    registerStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(avail);
    layout->addWidget(registerTable);
    layout->addWidget(registerCourseInput);
    layout->addWidget(btn);
    layout->addWidget(registerStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &StudentDashboard::onRegisterClicked);

    refreshRegisterPage();
    return page;
}

void StudentDashboard::refreshRegisterPage() {
    if (!registerTable) return;
    auto all = courseManager->listAllCourses();
    registerTable->setRowCount(0);
    registerTable->setRowCount((int)all.size());
    for (int i = 0; i < (int)all.size(); i++) {
        registerTable->setItem(i, 0, new QTableWidgetItem(
                                         QString::fromStdString(all[i].getCourseID())));
        registerTable->setItem(i, 1, new QTableWidgetItem(
                                         QString::fromStdString(all[i].getCourseName())));
        registerTable->setItem(i, 2, new QTableWidgetItem(
                                         QString::number(all[i].getCredits())));
        registerTable->setItem(i, 3, new QTableWidgetItem(
                                         QString::number(all[i].getCapacity() - all[i].getEnrolledCount())));
    }
}

// ── Drop ──────────────────────────────────────────────────────────────────────

QWidget* StudentDashboard::makeDropPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Drop a Course");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    dropCourseInput = new QLineEdit();
    dropCourseInput->setPlaceholderText("Enter Course ID to drop");
    dropCourseInput->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Drop Course");
    btn->setStyleSheet(
        "background-color: #C0745A; color: #F0EDE3; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    dropStatus = new QLabel("");
    dropStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(dropCourseInput);
    layout->addWidget(btn);
    layout->addWidget(dropStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &StudentDashboard::onDropClicked);
    return page;
}

// ── Nav slots ─────────────────────────────────────────────────────────────────

void StudentDashboard::showTranscript()     { refreshTranscriptPage(); stack->setCurrentIndex(1); }
void StudentDashboard::showSchedule()       { refreshSchedulePage();   stack->setCurrentIndex(2); }
void StudentDashboard::showRegisterCourse() { refreshRegisterPage(); registerStatus->clear(); stack->setCurrentIndex(3); }
void StudentDashboard::showDropCourse()     { dropStatus->clear();    stack->setCurrentIndex(4); }

// ── Action slots ──────────────────────────────────────────────────────────────

void StudentDashboard::onRegisterClicked() {
    string cid = registerCourseInput->text().trimmed().toStdString();
    if (cid.empty()) {
        registerStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        registerStatus->setText("Please enter a Course ID.");
        return;
    }
    Course *c = courseManager->findCourse(cid);
    if (!c) {
        registerStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        registerStatus->setText("Course not found.");
        return;
    }
    for (auto &e : student->getEnrolledCourses()) {
        if (e == cid) {
            registerStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
            registerStatus->setText("You are already enrolled in this course.");
            return;
        }
    }
    if (!c->isAvailable()) {
        registerStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        registerStatus->setText("Course is full — no seats available.");
        return;
    }
    student->registerForCourse(cid);
    c->addStudent(student->getStudentID());
    save();
    registerStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    registerStatus->setText("Successfully registered for " +
                            QString::fromStdString(c->getCourseName()) + ".");
    registerCourseInput->clear();
    refreshRegisterPage();
}

void StudentDashboard::onDropClicked() {
    string cid = dropCourseInput->text().trimmed().toStdString();
    if (cid.empty()) {
        dropStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        dropStatus->setText("Please enter a Course ID.");
        return;
    }
    bool enrolled = false;
    for (auto &e : student->getEnrolledCourses())
        if (e == cid) { enrolled = true; break; }

    if (!enrolled) {
        dropStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        dropStatus->setText("You are not enrolled in this course.");
        return;
    }

    Course *c = courseManager->findCourse(cid);
    QString courseName = c ? QString::fromStdString(c->getCourseName())
                           : QString::fromStdString(cid);

    QMessageBox confirm(this);
    confirm.setWindowTitle("Confirm Course Drop");
    confirm.setText(QString("You are about to drop <b>%1</b>.").arg(courseName));
    confirm.setInformativeText(
        "This action is permanent and cannot be reversed. "
        "All grades and academic progress recorded for this course will be lost. "
        "You will need to re-register in a future semester if you wish to retake it.");
    confirm.setIcon(QMessageBox::Warning);
    QPushButton *confirmBtn = confirm.addButton("Drop Course", QMessageBox::DestructiveRole);
    confirm.addButton("Cancel", QMessageBox::RejectRole);
    confirm.setDefaultButton(confirmBtn);
    confirm.exec();
    if (confirm.clickedButton() != confirmBtn) return;

    student->dropCourse(cid);
    if (c) c->removeStudent(student->getStudentID());
    save();
    dropStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    dropStatus->setText("Course dropped successfully.");
    dropCourseInput->clear();
}

void StudentDashboard::onLogout() { close(); }
//git