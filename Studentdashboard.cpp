#include "studentdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QFrame>

StudentDashboard::StudentDashboard(Student *s, CourseManager *cm, QWidget *parent)
    : QMainWindow(parent), student(s), courseManager(cm) {

    setWindowTitle("UCMS - Student Dashboard");
    setMinimumSize(800, 550);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #0f1923;");
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // ── Sidebar ──
    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet("background-color: #111d2b;");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setSpacing(6);
    sideLayout->setContentsMargins(12, 20, 12, 20);

    QLabel *logo = new QLabel("UCMS");
    logo->setStyleSheet("color: #4e9eff; font-size: 22px; font-weight: bold; padding: 10px 0;");
    QLabel *role = new QLabel("Student Portal");
    role->setStyleSheet("color: #5a7a99; font-size: 11px; margin-bottom: 20px;");

    QPushButton *btnHome       = makeNavButton("🏠  Home");
    QPushButton *btnTranscript = makeNavButton("📋  Transcript");
    QPushButton *btnSchedule   = makeNavButton("📅  Schedule");
    QPushButton *btnRegister   = makeNavButton("➕  Register Course");
    QPushButton *btnDrop       = makeNavButton("➖  Drop Course");
    QPushButton *btnLogout     = makeNavButton("🚪  Logout");
    btnLogout->setStyleSheet(btnLogout->styleSheet() +
                             "color: #ff6b6b;");

    sideLayout->addWidget(logo);
    sideLayout->addWidget(role);
    sideLayout->addWidget(btnHome);
    sideLayout->addWidget(btnTranscript);
    sideLayout->addWidget(btnSchedule);
    sideLayout->addWidget(btnRegister);
    sideLayout->addWidget(btnDrop);
    sideLayout->addStretch();
    sideLayout->addWidget(btnLogout);

    // ── Stack ──
    stack = new QStackedWidget();
    stack->addWidget(makeHomePage());       // 0
    stack->addWidget(makeTranscriptPage()); // 1
    stack->addWidget(makeSchedulePage());   // 2
    stack->addWidget(makeRegisterPage());   // 3
    stack->addWidget(makeDropPage());       // 4

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);
    setCentralWidget(central);

    connect(btnHome,       &QPushButton::clicked, [this]{ stack->setCurrentIndex(0); });
    connect(btnTranscript, &QPushButton::clicked, this, &StudentDashboard::showTranscript);
    connect(btnSchedule,   &QPushButton::clicked, this, &StudentDashboard::showSchedule);
    connect(btnRegister,   &QPushButton::clicked, this, &StudentDashboard::showRegisterCourse);
    connect(btnDrop,       &QPushButton::clicked, this, &StudentDashboard::showDropCourse);
    connect(btnLogout,     &QPushButton::clicked, this, &StudentDashboard::onLogout);
}

QPushButton* StudentDashboard::makeNavButton(const QString &text) {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(
        "QPushButton { background: transparent; color: #a0b8cc; "
        "text-align: left; padding: 10px 12px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #1a2d3d; color: #ffffff; }"
        );
    return btn;
}

QWidget* StudentDashboard::makeHomePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *welcome = new QLabel(QString("Welcome back, %1")
                                     .arg(QString::fromStdString(student->getName())));
    welcome->setStyleSheet("color: #ffffff; font-size: 24px; font-weight: bold;");

    QLabel *sub = new QLabel(QString("Student ID: %1")
                                 .arg(QString::fromStdString(student->getStudentID())));
    sub->setStyleSheet("color: #5a7a99; font-size: 13px;");

    // Stats row
    QHBoxLayout *stats = new QHBoxLayout();
    stats->setSpacing(16);

    auto makeCard = [](const QString &title, const QString &value) {
        QWidget *card = new QWidget();
        card->setStyleSheet(
            "background-color: #111d2b; border-radius: 10px; padding: 10px;");
        card->setFixedHeight(90);
        QVBoxLayout *cl = new QVBoxLayout(card);
        QLabel *t = new QLabel(title);
        t->setStyleSheet("color: #5a7a99; font-size: 11px;");
        QLabel *v = new QLabel(value);
        v->setStyleSheet("color: #4e9eff; font-size: 26px; font-weight: bold;");
        cl->addWidget(t);
        cl->addWidget(v);
        return card;
    };

    stats->addWidget(makeCard("GPA",
                              QString::number(student->getGPA(), 'f', 2)));
    stats->addWidget(makeCard("Enrolled Courses",
                              QString::number(student->getEnrolledCourses().size())));
    stats->addWidget(makeCard("Balance",
                              "$" + QString::number(student->getBalance(), 'f', 2)));
    stats->addWidget(makeCard("Scholarship",
                              student->hasScholarship() ? "Active" : "None"));

    layout->addWidget(welcome);
    layout->addWidget(sub);
    layout->addLayout(stats);
    layout->addStretch();
    return page;
}

QWidget* StudentDashboard::makeTranscriptPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("Academic Transcript");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Course ID", "Course Name", "Status"});
    table->setStyleSheet(
        "QTableWidget { background-color: #111d2b; color: #c0d0e0; "
        "gridline-color: #1a2d3d; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #0d1821; color: #4e9eff; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }"
        );
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto courses = student->getEnrolledCourses();
    table->setRowCount(courses.size());
    for (int i = 0; i < (int)courses.size(); i++) {
        string cid = courses[i];
        Course *c  = courseManager->findCourse(cid);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(cid)));
        table->setItem(i, 1, new QTableWidgetItem(
                                 c ? QString::fromStdString(c->getCourseName()) : "N/A"));
        table->setItem(i, 2, new QTableWidgetItem("Enrolled"));
    }

    QLabel *gpaLabel = new QLabel(
        QString("Cumulative GPA: %1").arg(student->getGPA(), 0, 'f', 2));
    gpaLabel->setStyleSheet("color: #4e9eff; font-size: 15px; font-weight: bold;");

    layout->addWidget(title);
    layout->addWidget(table);
    layout->addWidget(gpaLabel);
    return page;
}

QWidget* StudentDashboard::makeSchedulePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("My Schedule");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Course ID", "Course Name", "Schedule"});
    table->setStyleSheet(
        "QTableWidget { background-color: #111d2b; color: #c0d0e0; "
        "gridline-color: #1a2d3d; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #0d1821; color: #4e9eff; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }"
        );
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto courses = student->getEnrolledCourses();
    table->setRowCount(courses.size());
    for (int i = 0; i < (int)courses.size(); i++) {
        string cid = courses[i];
        Course *c  = courseManager->findCourse(cid);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(cid)));
        table->setItem(i, 1, new QTableWidgetItem(
                                 c ? QString::fromStdString(c->getCourseName()) : "N/A"));
        table->setItem(i, 2, new QTableWidgetItem(
                                 c ? QString::fromStdString(c->getSchedule()) : "N/A"));
    }

    layout->addWidget(title);
    layout->addWidget(table);
    layout->addStretch();
    return page;
}

QWidget* StudentDashboard::makeRegisterPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Register for a Course");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    QLabel *avail = new QLabel("Available Courses:");
    avail->setStyleSheet("color: #5a7a99; font-size: 13px;");

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Name", "Credits", "Seats Left"});
    table->setStyleSheet(
        "QTableWidget { background-color: #111d2b; color: #c0d0e0; "
        "gridline-color: #1a2d3d; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #0d1821; color: #4e9eff; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }"
        );
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto all = courseManager->listAllCourses();
    table->setRowCount(all.size());
    for (int i = 0; i < (int)all.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(
                                 QString::fromStdString(all[i].getCourseID())));
        table->setItem(i, 1, new QTableWidgetItem(
                                 QString::fromStdString(all[i].getCourseName())));
        table->setItem(i, 2, new QTableWidgetItem(
                                 QString::number(all[i].getCredits())));
        table->setItem(i, 3, new QTableWidgetItem(
                                 QString::number(all[i].getCapacity() - all[i].getEnrolledCount())));
    }

    registerCourseInput = new QLineEdit();
    registerCourseInput->setPlaceholderText("Enter Course ID to register");
    registerCourseInput->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Register");
    btn->setStyleSheet(
        "background-color: #4e9eff; color: white; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    registerStatus = new QLabel("");
    registerStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(avail);
    layout->addWidget(table);
    layout->addWidget(registerCourseInput);
    layout->addWidget(btn);
    layout->addWidget(registerStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &StudentDashboard::onRegisterClicked);
    return page;
}

QWidget* StudentDashboard::makeDropPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Drop a Course");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    dropCourseInput = new QLineEdit();
    dropCourseInput->setPlaceholderText("Enter Course ID to drop");
    dropCourseInput->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Drop Course");
    btn->setStyleSheet(
        "background-color: #ff6b6b; color: white; padding: 9px; "
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

void StudentDashboard::showTranscript()     { stack->setCurrentIndex(1); }
void StudentDashboard::showSchedule()       { stack->setCurrentIndex(2); }
void StudentDashboard::showRegisterCourse() { stack->setCurrentIndex(3); }
void StudentDashboard::showDropCourse()     { stack->setCurrentIndex(4); }

void StudentDashboard::onRegisterClicked() {
    string cid = registerCourseInput->text().toStdString();
    Course *c  = courseManager->findCourse(cid);
    if (!c) {
        registerStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        registerStatus->setText("Course not found.");
        return;
    }
    if (!c->isAvailable()) {
        registerStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        registerStatus->setText("Course is full.");
        return;
    }
    student->registerForCourse(cid);
    c->addStudent(student->getStudentID());
    registerStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
    registerStatus->setText("Successfully registered for " +
                            QString::fromStdString(c->getCourseName()) + "!");
    registerCourseInput->clear();
}

void StudentDashboard::onDropClicked() {
    string cid = dropCourseInput->text().toStdString();
    auto enrolled = student->getEnrolledCourses();
    bool found = false;
    for (auto &e : enrolled) if (e == cid) { found = true; break; }

    if (!found) {
        dropStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        dropStatus->setText("You are not enrolled in this course.");
        return;
    }
    student->dropCourse(cid);
    Course *c = courseManager->findCourse(cid);
    if (c) c->removeStudent(student->getStudentID());
    dropStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
    dropStatus->setText("Course dropped successfully.");
    dropCourseInput->clear();
}

void StudentDashboard::onLogout() {
    close();
}

QString StudentDashboard::baseStyle() { return ""; }