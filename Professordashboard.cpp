#include "professordashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QHeaderView>

ProfessorDashboard::ProfessorDashboard(Professor *p, CourseManager *cm,
                                       vector<Student> *sts,
                                       function<void()> saveCallback,
                                       QWidget *parent)
    : QMainWindow(parent), professor(p), courseManager(cm), students(sts),
    save(saveCallback), coursesTable(nullptr)
{
    setWindowTitle("AIMS - Professor Dashboard");
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

    QLabel *logo = new QLabel("A I M S");
    logo->setStyleSheet("color: #C8B89A; font-size: 22px; padding: 10px 0;");
    QLabel *role = new QLabel("Professor Portal");
    role->setStyleSheet("color: #8A8478; font-size: 11px; margin-bottom: 20px;");

    QPushButton *btnHome    = makeNavButton("🏠  Home");
    QPushButton *btnCourses = makeNavButton("📚  My Courses");
    QPushButton *btnGrades  = makeNavButton("✏️  Submit Grades");
    QPushButton *btnOffice  = makeNavButton("🕐  Office Hours");
    QPushButton *btnLogout  = makeNavButton("🚪  Logout");
    btnLogout->setStyleSheet(btnLogout->styleSheet() + "color: #C0745A;");

    sideLayout->addWidget(logo);
    sideLayout->addWidget(role);
    sideLayout->addWidget(btnHome);
    sideLayout->addWidget(btnCourses);
    sideLayout->addWidget(btnGrades);
    sideLayout->addWidget(btnOffice);
    sideLayout->addStretch();
    sideLayout->addWidget(btnLogout);

    stack = new QStackedWidget();
    stack->addWidget(makeHomePage());        // 0
    stack->addWidget(makeCoursesPage());     // 1
    stack->addWidget(makeGradesPage());      // 2
    stack->addWidget(makeOfficeHoursPage()); // 3

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);
    setCentralWidget(central);

    connect(btnHome,    &QPushButton::clicked, [this]{ stack->setCurrentIndex(0); });
    connect(btnCourses, &QPushButton::clicked, [this]{ refreshCoursesPage(); stack->setCurrentIndex(1); });
    connect(btnGrades,  &QPushButton::clicked, [this]{ stack->setCurrentIndex(2); });
    connect(btnOffice,  &QPushButton::clicked, [this]{ stack->setCurrentIndex(3); });
    connect(btnLogout,  &QPushButton::clicked, this, &ProfessorDashboard::onLogout);
}

QPushButton* ProfessorDashboard::makeNavButton(const QString &text) {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(
        "QPushButton { background: transparent; color: #8A8478; "
        "text-align: left; padding: 10px 12px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #3A3A36; color: #F0EDE3; }");
    return btn;
}

// ── Home ──────────────────────────────────────────────────────────────────────

QWidget* ProfessorDashboard::makeHomePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *welcome = new QLabel(QString("Welcome, %1")
                                     .arg(QString::fromStdString(professor->getName())));
    welcome->setStyleSheet("color: #F0EDE3; font-size: 24px; font-weight: bold;");

    QLabel *dept = new QLabel(QString("Department: %1  |  ID: %2")
                                  .arg(QString::fromStdString(professor->getDepartment()))
                                  .arg(QString::fromStdString(professor->getProfessorID())));
    dept->setStyleSheet("color: #8A8478; font-size: 13px;");

    auto makeCard = [](const QString &title, const QString &value) {
        QWidget *card = new QWidget();
        card->setStyleSheet("background-color: #252523; border-radius: 5px; padding: 5px;");
        card->setFixedHeight(90);
        QVBoxLayout *cl = new QVBoxLayout(card);
        QLabel *t = new QLabel(title);
        t->setStyleSheet("color: #8A8478; font-size: 11px;");
        QLabel *v = new QLabel(value);
        v->setStyleSheet("color: #C8B89A; font-size: 26px; font-weight: bold;");
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

// ── Courses ───────────────────────────────────────────────────────────────────

QWidget* ProfessorDashboard::makeCoursesPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("My Courses");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    coursesTable = new QTableWidget();
    coursesTable->setColumnCount(4);
    coursesTable->setHorizontalHeaderLabels({"Course ID", "Name", "Credits", "Enrolled"});
    coursesTable->setStyleSheet(
        "QTableWidget { background-color: #252523; color: #F0EDE3; "
        "gridline-color: #3A3A36; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #1C1C1A; color: #C8B89A; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }");
    coursesTable->horizontalHeader()->setStretchLastSection(true);
    coursesTable->verticalHeader()->setVisible(false);
    coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(title);
    layout->addWidget(coursesTable);
    layout->addStretch();

    refreshCoursesPage();
    return page;
}

void ProfessorDashboard::refreshCoursesPage() {
    if (!coursesTable) return;
    auto assigned = professor->getAssignedCourses();
    coursesTable->setRowCount(0);
    coursesTable->setRowCount((int)assigned.size());
    for (int i = 0; i < (int)assigned.size(); i++) {
        Course *c = courseManager->findCourse(assigned[i]);
        coursesTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(assigned[i])));
        coursesTable->setItem(i, 1, new QTableWidgetItem(
                                        c ? QString::fromStdString(c->getCourseName()) : "N/A"));
        coursesTable->setItem(i, 2, new QTableWidgetItem(
                                        c ? QString::number(c->getCredits()) : "N/A"));
        coursesTable->setItem(i, 3, new QTableWidgetItem(
                                        c ? QString::number(c->getEnrolledCount()) : "N/A"));
    }
}

// ── Grades ────────────────────────────────────────────────────────────────────

QWidget* ProfessorDashboard::makeGradesPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Submit Grades");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    QLabel *note = new QLabel(
        "Submitting a grade will immediately update the student's GPA and persist to the database.");
    note->setStyleSheet("color: #8A8478; font-size: 12px;");
    note->setWordWrap(true);

    auto makeField = [](const QString &ph) {
        QLineEdit *f = new QLineEdit();
        f->setPlaceholderText(ph);
        f->setStyleSheet(
            "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return f;
    };

    gradeStudentID = makeField("Student ID (e.g. S001)");
    gradeCourseID  = makeField("Course ID (e.g. CS101)");

    gradeValue = new QComboBox();
    gradeValue->addItems({"A", "A-", "B+", "B", "B-", "C+", "C", "C-", "F"});
    gradeValue->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Submit Grade");
    btn->setStyleSheet(
        "background-color: #C8B89A; color: #1C1C1A; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    gradeStatus = new QLabel("");
    gradeStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(note);
    layout->addWidget(gradeStudentID);
    layout->addWidget(gradeCourseID);
    layout->addWidget(gradeValue);
    layout->addWidget(btn);
    layout->addWidget(gradeStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &ProfessorDashboard::onSubmitGradeClicked);
    return page;
}

// ── Office Hours ──────────────────────────────────────────────────────────────

QWidget* ProfessorDashboard::makeOfficeHoursPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Update Office Hours");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    officeHoursInput = new QLineEdit();
    officeHoursInput->setPlaceholderText("e.g. MW 2:00-4:00 PM, Room 305");
    officeHoursInput->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Update");
    btn->setStyleSheet(
        "background-color: #C8B89A; color: #1C1C1A; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    officeStatus = new QLabel("");
    officeStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(officeHoursInput);
    layout->addWidget(btn);
    layout->addWidget(officeStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &ProfessorDashboard::onUpdateOfficeHoursClicked);
    return page;
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void ProfessorDashboard::onSubmitGradeClicked() {
    string sid   = gradeStudentID->text().trimmed().toStdString();
    string cid   = gradeCourseID->text().trimmed().toStdString();
    string grade = gradeValue->currentText().toStdString();

    if (sid.empty() || cid.empty()) {
        gradeStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        gradeStatus->setText("Please fill in Student ID and Course ID.");
        return;
    }

    // Validate the professor actually teaches this course
    bool teaches = false;
    for (auto &c : professor->getAssignedCourses())
        if (c == cid) { teaches = true; break; }
    if (!teaches) {
        gradeStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        gradeStatus->setText("You are not the instructor for this course.");
        return;
    }

    // Find the course for its credits
    Course *course = courseManager->findCourse(cid);
    if (!course) {
        gradeStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        gradeStatus->setText("Course not found.");
        return;
    }

    // Find the student and confirm they are enrolled
    Student *target = nullptr;
    for (auto &s : *students)
        if (s.getStudentID() == sid) { target = &s; break; }

    if (!target) {
        gradeStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        gradeStatus->setText("Student not found.");
        return;
    }

    bool enrolled = false;
    for (auto &e : target->getEnrolledCourses())
        if (e == cid) { enrolled = true; break; }
    if (!enrolled) {
        gradeStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        gradeStatus->setText("Student is not enrolled in this course.");
        return;
    }

    // Build credits map from courseManager for GPA calculation
    map<string,int> creditsMap;
    for (auto &c : courseManager->listAllCourses())
        creditsMap[c.getCourseID()] = c.getCredits();

    target->setGradeForCourse(cid, grade, creditsMap);
    save();

    gradeStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    gradeStatus->setText(QString("Grade %1 submitted for %2 in %3. New GPA: %4")
                             .arg(QString::fromStdString(grade))
                             .arg(QString::fromStdString(sid))
                             .arg(QString::fromStdString(cid))
                             .arg(target->getGPA(), 0, 'f', 2));
    gradeStudentID->clear();
    gradeCourseID->clear();
}

void ProfessorDashboard::onUpdateOfficeHoursClicked() {
    string hours = officeHoursInput->text().trimmed().toStdString();
    if (hours.empty()) {
        officeStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        officeStatus->setText("Please enter office hours.");
        return;
    }
    professor->updateOfficeHours(hours);
    save();
    officeStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    officeStatus->setText("Office hours updated successfully.");
    officeHoursInput->clear();
}

void ProfessorDashboard::onLogout() { close(); }
//git