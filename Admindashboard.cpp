#include "admindashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QHeaderView>

AdminDashboard::AdminDashboard(Admin *a, vector<Student> *sts,
                               vector<Professor> *profs, CourseManager *cm,
                               function<void()> saveCallback,
                               QWidget *parent)
    : QMainWindow(parent), admin(a), students(sts), professors(profs),
    courseManager(cm), save(saveCallback),
    statStudents(nullptr), statProfessors(nullptr), statCourses(nullptr),
    studentsTable(nullptr)
{
    setWindowTitle("AIMS - Admin Dashboard");
    setMinimumSize(900, 600);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #1C1C1A;");
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(210);
    sidebar->setStyleSheet("background-color: #252523;");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setSpacing(6);
    sideLayout->setContentsMargins(12, 20, 12, 20);

    QLabel *logo = new QLabel("A I M S");
    logo->setStyleSheet("color: #C8B89A; font-size: 22px; padding: 10px 0;");
    QLabel *role = new QLabel("Admin Portal");
    role->setStyleSheet("color: #8A8478; font-size: 11px; margin-bottom: 20px;");

    QPushButton *btnHome     = makeNavButton("🏠  Home");
    QPushButton *btnStudents = makeNavButton("👥  View Students");
    QPushButton *btnAdd      = makeNavButton("➕  Add Student");
    QPushButton *btnDel      = makeNavButton("🗑  Delete Student");
    QPushButton *btnCourse   = makeNavButton("📚  Create Course");
    QPushButton *btnAssign   = makeNavButton("👨‍🏫  Assign Instructor");
    QPushButton *btnLogout   = makeNavButton("🚪  Logout");
    btnLogout->setStyleSheet(btnLogout->styleSheet() + "color: #C0745A;");

    sideLayout->addWidget(logo);
    sideLayout->addWidget(role);
    sideLayout->addWidget(btnHome);
    sideLayout->addWidget(btnStudents);
    sideLayout->addWidget(btnAdd);
    sideLayout->addWidget(btnDel);
    sideLayout->addWidget(btnCourse);
    sideLayout->addWidget(btnAssign);
    sideLayout->addStretch();
    sideLayout->addWidget(btnLogout);

    stack = new QStackedWidget();
    stack->addWidget(makeHomePage());             // 0
    stack->addWidget(makeStudentsPage());         // 1
    stack->addWidget(makeAddStudentPage());       // 2
    stack->addWidget(makeDeleteStudentPage());    // 3
    stack->addWidget(makeCreateCoursePage());     // 4
    stack->addWidget(makeAssignInstructorPage()); // 5

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stack);
    setCentralWidget(central);

    connect(btnHome,     &QPushButton::clicked, [this]{ refreshHomePage();     stack->setCurrentIndex(0); });
    connect(btnStudents, &QPushButton::clicked, [this]{ refreshStudentsPage(); stack->setCurrentIndex(1); });
    connect(btnAdd,      &QPushButton::clicked, [this]{ stack->setCurrentIndex(2); });
    connect(btnDel,      &QPushButton::clicked, [this]{ stack->setCurrentIndex(3); });
    connect(btnCourse,   &QPushButton::clicked, [this]{ stack->setCurrentIndex(4); });
    connect(btnAssign,   &QPushButton::clicked, [this]{ stack->setCurrentIndex(5); });
    connect(btnLogout,   &QPushButton::clicked, this, &AdminDashboard::onLogout);
}

QPushButton* AdminDashboard::makeNavButton(const QString &text) {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(
        "QPushButton { background: transparent; color: #8A8478; "
        "text-align: left; padding: 10px 12px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #3A3A36; color: #F0EDE3; }");
    return btn;
}

// ── Home ──────────────────────────────────────────────────────────────────────

QWidget* AdminDashboard::makeHomePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *welcome = new QLabel("Admin Dashboard");
    welcome->setStyleSheet("color: #F0EDE3; font-size: 24px; font-weight: bold;");

    QLabel *sub = new QLabel(QString("Logged in as: %1")
                                 .arg(QString::fromStdString(admin->getUsername())));
    sub->setStyleSheet("color: #8A8478; font-size: 13px;");

    auto makeCard = [](const QString &title, QLabel *&lbl) {
        QWidget *card = new QWidget();
        card->setStyleSheet("background-color: #252523; border-radius: 5px; padding: 5px;");
        card->setFixedHeight(90);
        QVBoxLayout *cl = new QVBoxLayout(card);
        QLabel *t = new QLabel(title);
        t->setStyleSheet("color: #8A8478; font-size: 11px;");
        lbl = new QLabel("0");
        lbl->setStyleSheet("color: #C8B89A; font-size: 26px; font-weight: bold;");
        cl->addWidget(t);
        cl->addWidget(lbl);
        return card;
    };

    QHBoxLayout *stats = new QHBoxLayout();
    stats->setSpacing(16);
    stats->addWidget(makeCard("Total Students",   statStudents));
    stats->addWidget(makeCard("Total Professors", statProfessors));
    stats->addWidget(makeCard("Total Courses",    statCourses));

    layout->addWidget(welcome);
    layout->addWidget(sub);
    layout->addLayout(stats);
    layout->addStretch();

    refreshHomePage();
    return page;
}

void AdminDashboard::refreshHomePage() {
    if (statStudents)   statStudents->setText(QString::number(students->size()));
    if (statProfessors) statProfessors->setText(QString::number(professors->size()));
    if (statCourses)    statCourses->setText(
            QString::number(courseManager->listAllCourses().size()));
}

// ── Students ──────────────────────────────────────────────────────────────────

QWidget* AdminDashboard::makeStudentsPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("All Students");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    studentsTable = new QTableWidget();
    studentsTable->setColumnCount(4);
    studentsTable->setHorizontalHeaderLabels({"Student ID", "Name", "Email", "GPA"});
    studentsTable->setStyleSheet(
        "QTableWidget { background-color: #252523; color: #F0EDE3; "
        "gridline-color: #3A3A36; border: none; font-size: 13px; }"
        "QHeaderView::section { background-color: #1C1C1A; color: #C8B89A; "
        "padding: 8px; border: none; font-weight: bold; }"
        "QTableWidget::item { padding: 8px; }");
    studentsTable->horizontalHeader()->setStretchLastSection(true);
    studentsTable->verticalHeader()->setVisible(false);
    studentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(title);
    layout->addWidget(studentsTable);

    refreshStudentsPage();
    return page;
}

void AdminDashboard::refreshStudentsPage() {
    if (!studentsTable) return;
    studentsTable->setRowCount(0);
    studentsTable->setRowCount((int)students->size());
    for (int i = 0; i < (int)students->size(); i++) {
        studentsTable->setItem(i, 0, new QTableWidgetItem(
                                         QString::fromStdString((*students)[i].getStudentID())));
        studentsTable->setItem(i, 1, new QTableWidgetItem(
                                         QString::fromStdString((*students)[i].getName())));
        studentsTable->setItem(i, 2, new QTableWidgetItem(
                                         QString::fromStdString((*students)[i].getEmail())));
        studentsTable->setItem(i, 3, new QTableWidgetItem(
                                         QString::number((*students)[i].getGPA(), 'f', 2)));
    }
}

// ── Add Student ───────────────────────────────────────────────────────────────

QWidget* AdminDashboard::makeAddStudentPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Add New Student");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    auto makeField = [](const QString &ph) {
        QLineEdit *f = new QLineEdit();
        f->setPlaceholderText(ph);
        f->setStyleSheet(
            "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return f;
    };

    addStudentID    = makeField("Student ID (e.g. S003)");
    addStudentName  = makeField("Full Name");
    addStudentEmail = makeField("Email");
    addStudentPwd   = makeField("Password");

    QPushButton *btn = new QPushButton("Add Student");
    btn->setStyleSheet(
        "background-color: #C8B89A; color: #1C1C1A; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    addStudentStatus = new QLabel("");
    addStudentStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(addStudentID);
    layout->addWidget(addStudentName);
    layout->addWidget(addStudentEmail);
    layout->addWidget(addStudentPwd);
    layout->addWidget(btn);
    layout->addWidget(addStudentStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &AdminDashboard::onAddStudentClicked);
    return page;
}

// ── Delete Student ────────────────────────────────────────────────────────────

QWidget* AdminDashboard::makeDeleteStudentPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Delete Student");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    delStudentID = new QLineEdit();
    delStudentID->setPlaceholderText("Enter Student ID to delete");
    delStudentID->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Delete Student");
    btn->setStyleSheet(
        "background-color: #C0745A; color: #F0EDE3; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    delStudentStatus = new QLabel("");
    delStudentStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(delStudentID);
    layout->addWidget(btn);
    layout->addWidget(delStudentStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &AdminDashboard::onDeleteStudentClicked);
    return page;
}

// ── Create Course ─────────────────────────────────────────────────────────────

QWidget* AdminDashboard::makeCreateCoursePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Create New Course");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    auto makeField = [](const QString &ph) {
        QLineEdit *f = new QLineEdit();
        f->setPlaceholderText(ph);
        f->setStyleSheet(
            "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return f;
    };
    auto makeSpin = [](int mn, int mx, int def) {
        QSpinBox *s = new QSpinBox();
        s->setRange(mn, mx);
        s->setValue(def);
        s->setStyleSheet(
            "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return s;
    };

    courseID         = makeField("Course ID (e.g. CS401)");
    courseName       = makeField("Course Name");
    courseSchedule   = makeField("Schedule (e.g. MWF 10:00-11:00)");
    courseInstructor = makeField("Instructor ID (optional, e.g. P001)");
    courseCredits    = makeSpin(1, 6, 3);
    courseCapacity   = makeSpin(5, 200, 30);

    QLabel *lCred = new QLabel("Credit Hours:");
    lCred->setStyleSheet("color: #8A8478; font-size: 13px;");
    QLabel *lCap  = new QLabel("Capacity:");
    lCap->setStyleSheet("color: #8A8478; font-size: 13px;");

    QPushButton *btn = new QPushButton("Create Course");
    btn->setStyleSheet(
        "background-color: #C8B89A; color: #1C1C1A; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    createCourseStatus = new QLabel("");
    createCourseStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(courseID);
    layout->addWidget(courseName);
    layout->addWidget(courseSchedule);
    layout->addWidget(courseInstructor);
    layout->addWidget(lCred);
    layout->addWidget(courseCredits);
    layout->addWidget(lCap);
    layout->addWidget(courseCapacity);
    layout->addWidget(btn);
    layout->addWidget(createCourseStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &AdminDashboard::onCreateCourseClicked);
    return page;
}

// ── Assign Instructor ─────────────────────────────────────────────────────────

QWidget* AdminDashboard::makeAssignInstructorPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #1C1C1A;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Assign Instructor to Course");
    title->setStyleSheet("color: #F0EDE3; font-size: 22px; font-weight: bold;");

    assignCourseID = new QLineEdit();
    assignCourseID->setPlaceholderText("Course ID (e.g. CS301)");
    assignCourseID->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    assignProfID = new QLineEdit();
    assignProfID->setPlaceholderText("Professor ID (e.g. P001)");
    assignProfID->setStyleSheet(
        "background-color: #252523; color: #F0EDE3; border: 1px solid #3A3A36; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Assign");
    btn->setStyleSheet(
        "background-color: #C8B89A; color: #1C1C1A; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    assignStatus = new QLabel("");
    assignStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(assignCourseID);
    layout->addWidget(assignProfID);
    layout->addWidget(btn);
    layout->addWidget(assignStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &AdminDashboard::onAssignInstructorClicked);
    return page;
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void AdminDashboard::onAddStudentClicked() {
    string id    = addStudentID->text().trimmed().toStdString();
    string name  = addStudentName->text().trimmed().toStdString();
    string email = addStudentEmail->text().trimmed().toStdString();
    string pwd   = addStudentPwd->text().toStdString();

    if (id.empty() || name.empty() || email.empty() || pwd.empty()) {
        addStudentStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        addStudentStatus->setText("Please fill in all fields.");
        return;
    }
    for (auto &s : *students) {
        if (s.getStudentID() == id) {
            addStudentStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
            addStudentStatus->setText("Student ID already exists.");
            return;
        }
    }
    students->push_back(Student(id, name, email, pwd));
    save();
    addStudentStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    addStudentStatus->setText("Student " + QString::fromStdString(name) + " added successfully.");
    addStudentID->clear(); addStudentName->clear();
    addStudentEmail->clear(); addStudentPwd->clear();
}

void AdminDashboard::onDeleteStudentClicked() {
    string id = delStudentID->text().trimmed().toStdString();
    if (id.empty()) {
        delStudentStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        delStudentStatus->setText("Please enter a Student ID.");
        return;
    }
    int idx = -1;
    for (int i = 0; i < (int)students->size(); i++)
        if ((*students)[i].getStudentID() == id) { idx = i; break; }

    if (idx == -1) {
        delStudentStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        delStudentStatus->setText("Student ID not found.");
        return;
    }

    QString sName = QString::fromStdString((*students)[idx].getName());
    QMessageBox confirm(this);
    confirm.setWindowTitle("Confirm Deletion");
    confirm.setText(QString("You are about to permanently delete the record for <b>%1</b> (ID: %2).")
                        .arg(sName).arg(QString::fromStdString(id)));
    confirm.setInformativeText(
        "This action cannot be undone. All enrollment records and associated data "
        "for this student will be permanently removed from the system.");
    confirm.setIcon(QMessageBox::Warning);
    QPushButton *confirmBtn = confirm.addButton("Delete Permanently", QMessageBox::DestructiveRole);
    confirm.addButton("Cancel", QMessageBox::RejectRole);
    confirm.setDefaultButton(confirmBtn);
    confirm.exec();
    if (confirm.clickedButton() != confirmBtn) return;

    // Remove student from all course enrolled lists
    for (auto &cid : (*students)[idx].getEnrolledCourses()) {
        Course *c = courseManager->findCourse(cid);
        if (c) c->removeStudent(id);
    }

    students->erase(students->begin() + idx);
    save();
    delStudentStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    delStudentStatus->setText("Student record deleted successfully.");
    delStudentID->clear();
}

void AdminDashboard::onCreateCourseClicked() {
    string cid   = courseID->text().trimmed().toStdString();
    string cname = courseName->text().trimmed().toStdString();
    string sched = courseSchedule->text().trimmed().toStdString();
    string instr = courseInstructor->text().trimmed().toStdString();
    int    cred  = courseCredits->value();
    int    cap   = courseCapacity->value();

    if (cid.empty() || cname.empty()) {
        createCourseStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        createCourseStatus->setText("Course ID and Name are required.");
        return;
    }
    if (courseManager->findCourse(cid)) {
        createCourseStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        createCourseStatus->setText("Course ID already exists.");
        return;
    }
    courseManager->addCourse(Course(cid, cname, cred, cap, sched, instr));
    save();
    createCourseStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    createCourseStatus->setText("Course " + QString::fromStdString(cname) + " created successfully.");
    courseID->clear(); courseName->clear();
    courseSchedule->clear(); courseInstructor->clear();
}

void AdminDashboard::onAssignInstructorClicked() {
    string cid = assignCourseID->text().trimmed().toStdString();
    string pid = assignProfID->text().trimmed().toStdString();

    Course *c = courseManager->findCourse(cid);
    if (!c) {
        assignStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        assignStatus->setText("Course not found.");
        return;
    }
    Professor *found = nullptr;
    for (auto &p : *professors)
        if (p.getProfessorID() == pid) { found = &p; break; }
    if (!found) {
        assignStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
        assignStatus->setText("Professor not found.");
        return;
    }

    // Prevent duplicate assignment
    for (auto &existing : found->getAssignedCourses()) {
        if (existing == cid) {
            assignStatus->setStyleSheet("color: #C0745A; font-size: 13px;");
            assignStatus->setText("Professor is already assigned to this course.");
            return;
        }
    }

    c->setInstructor(pid);
    found->assignCourse(cid);
    save();
    assignStatus->setStyleSheet("color: #C8B89A; font-size: 13px;");
    assignStatus->setText("Instructor assigned successfully.");
    assignCourseID->clear(); assignProfID->clear();
}
//git
void AdminDashboard::onLogout() { close(); }