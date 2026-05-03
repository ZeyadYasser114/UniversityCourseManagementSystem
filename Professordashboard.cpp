#include "professordashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QHeaderView>

ProfessorDashboard::ProfessorDashboard(Professor *p, CourseManager *cm,
                                       vector<Student> *sts, QWidget *parent)
    : QMainWindow(parent), professor(p), courseManager(cm), students(sts) {

    setWindowTitle("UCMS - Professor Dashboard");
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
    QLabel *role = new QLabel("Professor Portal");
    role->setStyleSheet("color: #5a7a99; font-size: 11px; margin-bottom: 20px;");

    QPushButton *btnHome    = makeNavButton("🏠  Home");
    QPushButton *btnCourses = makeNavButton("📚  My Courses");
    QPushButton *btnGrades  = makeNavButton("✏️  Submit Grades");
    QPushButton *btnOffice  = makeNavButton("🕐  Office Hours");
    QPushButton *btnLogout  = makeNavButton("🚪  Logout");
    btnLogout->setStyleSheet(btnLogout->styleSheet() + "color: #ff6b6b;");

    sideLayout->addWidget(logo);
    sideLayout->addWidget(role);
    sideLayout->addWidget(btnHome);
    sideLayout->addWidget(btnCourses);
    sideLayout->addWidget(btnGrades);
    sideLayout->addWidget(btnOffice);
    sideLayout->addStretch();
    sideLayout->addWidget(btnLogout);

    stack = new QStackedWidget();
    stack->addWidget(makeHomePage());      // 0
    stack->addWidget(makeCoursesPage());   // 1
    stack->addWidget(makeGradesPage());    // 2
    stack->addWidget(makeOfficeHoursPage()); // 3

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);
    setCentralWidget(central);

    connect(btnHome,    &QPushButton::clicked, [this]{ stack->setCurrentIndex(0); });
    connect(btnCourses, &QPushButton::clicked, [this]{ stack->setCurrentIndex(1); });
    connect(btnGrades,  &QPushButton::clicked, [this]{ stack->setCurrentIndex(2); });
    connect(btnOffice,  &QPushButton::clicked, [this]{ stack->setCurrentIndex(3); });
    connect(btnLogout,  &QPushButton::clicked, this, &ProfessorDashboard::onLogout);
}

QPushButton* ProfessorDashboard::makeNavButton(const QString &text) {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(
        "QPushButton { background: transparent; color: #a0b8cc; "
        "text-align: left; padding: 10px 12px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #1a2d3d; color: #ffffff; }"
        );
    return btn;
}

QWidget* ProfessorDashboard::makeHomePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *welcome = new QLabel(QString("Welcome, %1")
                                     .arg(QString::fromStdString(professor->getName())));
    welcome->setStyleSheet("color: #ffffff; font-size: 24px; font-weight: bold;");

    QLabel *dept = new QLabel(QString("Department: %1  |  ID: %2")
                                  .arg(QString::fromStdString(professor->getDepartment()))
                                  .arg(QString::fromStdString(professor->getProfessorID())));
    dept->setStyleSheet("color: #5a7a99; font-size: 13px;");

    auto makeCard = [](const QString &title, const QString &value) {
        QWidget *card = new QWidget();
        card->setStyleSheet("background-color: #111d2b; border-radius: 10px; padding: 10px;");
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

    QHBoxLayout *stats = new QHBoxLayout();
    stats->setSpacing(16);
    stats->addWidget(makeCard("Assigned Courses",
                              QString::number(professor->getAssignedCourses().size())));
    stats->addWidget(makeCard("Department",
                              QString::fromStdString(professor->getDepartment())));

    layout->addWidget(welcome);
    layout->addWidget(dept);
    layout->addLayout(stats);
    layout->addStretch();
    return page;
}

QWidget* ProfessorDashboard::makeCoursesPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("My Courses");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Course ID", "Name", "Credits", "Enrolled"});
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

    auto assigned = professor->getAssignedCourses();
    table->setRowCount(assigned.size());
    for (int i = 0; i < (int)assigned.size(); i++) {
        Course *c = courseManager->findCourse(assigned[i]);
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(assigned[i])));
        table->setItem(i, 1, new QTableWidgetItem(
                                 c ? QString::fromStdString(c->getCourseName()) : "N/A"));
        table->setItem(i, 2, new QTableWidgetItem(
                                 c ? QString::number(c->getCredits()) : "N/A"));
        table->setItem(i, 3, new QTableWidgetItem(
                                 c ? QString::number(c->getEnrolledCount()) : "N/A"));
    }

    layout->addWidget(title);
    layout->addWidget(table);
    layout->addStretch();
    return page;
}

QWidget* ProfessorDashboard::makeGradesPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Submit Grades");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    auto makeField = [](const QString &ph) {
        QLineEdit *f = new QLineEdit();
        f->setPlaceholderText(ph);
        f->setStyleSheet(
            "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return f;
    };

    gradeStudentID = makeField("Student ID (e.g. S001)");
    gradeCourseID  = makeField("Course ID (e.g. CS101)");
    gradeValue     = makeField("Grade (e.g. A, B+, C)");

    QPushButton *btn = new QPushButton("Submit Grade");
    btn->setStyleSheet(
        "background-color: #4e9eff; color: white; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    gradeStatus = new QLabel("");
    gradeStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(gradeStudentID);
    layout->addWidget(gradeCourseID);
    layout->addWidget(gradeValue);
    layout->addWidget(btn);
    layout->addWidget(gradeStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &ProfessorDashboard::onSubmitGradeClicked);
    return page;
}

QWidget* ProfessorDashboard::makeOfficeHoursPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Update Office Hours");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    officeHoursInput = new QLineEdit();
    officeHoursInput->setPlaceholderText("e.g. MW 2:00-4:00 PM, Room 305");
    officeHoursInput->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Update");
    btn->setStyleSheet(
        "background-color: #4e9eff; color: white; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    officeStatus = new QLabel("");
    officeStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(officeHoursInput);
    layout->addWidget(btn);
    layout->addWidget(officeStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked,
            this, &ProfessorDashboard::onUpdateOfficeHoursClicked);
    return page;
}

void ProfessorDashboard::onSubmitGradeClicked() {
    string sid   = gradeStudentID->text().toStdString();
    string cid   = gradeCourseID->text().toStdString();
    string grade = gradeValue->text().toStdString();

    if (sid.empty() || cid.empty() || grade.empty()) {
        gradeStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        gradeStatus->setText("Please fill in all fields.");
        return;
    }
    professor->submitGrade(sid, cid, grade);
    gradeStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
    gradeStatus->setText(QString("Grade %1 submitted for student %2 in %3.")
                             .arg(QString::fromStdString(grade))
                             .arg(QString::fromStdString(sid))
                             .arg(QString::fromStdString(cid)));
    gradeStudentID->clear();
    gradeCourseID->clear();
    gradeValue->clear();
}

void ProfessorDashboard::onUpdateOfficeHoursClicked() {
    string hours = officeHoursInput->text().toStdString();
    if (hours.empty()) {
        officeStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        officeStatus->setText("Please enter office hours.");
        return;
    }
    professor->updateOfficeHours(hours);
    officeStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
    officeStatus->setText("Office hours updated successfully.");
    officeHoursInput->clear();
}

void ProfessorDashboard::onLogout() { close(); }