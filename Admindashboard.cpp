#include "admindashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QHeaderView>
#include <QFormLayout>

AdminDashboard::AdminDashboard(Admin *a, vector<Student> *sts,
                               vector<Professor> *profs, CourseManager *cm,
                               QWidget *parent)
    : QMainWindow(parent), admin(a), students(sts), professors(profs), courseManager(cm) {

    setWindowTitle("UCMS - Admin Dashboard");
    setMinimumSize(900, 600);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #0f1923;");
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // ── Sidebar ──
    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(210);
    sidebar->setStyleSheet("background-color: #111d2b;");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setSpacing(6);
    sideLayout->setContentsMargins(12, 20, 12, 20);

    QLabel *logo = new QLabel("UCMS");
    logo->setStyleSheet("color: #4e9eff; font-size: 22px; font-weight: bold; padding: 10px 0;");
    QLabel *role = new QLabel("Admin Portal");
    role->setStyleSheet("color: #5a7a99; font-size: 11px; margin-bottom: 20px;");

    QPushButton *btnHome     = makeNavButton("🏠  Home");
    QPushButton *btnStudents = makeNavButton("👥  View Students");
    QPushButton *btnAdd      = makeNavButton("➕  Add Student");
    QPushButton *btnDel      = makeNavButton("🗑  Delete Student");
    QPushButton *btnCourse   = makeNavButton("📚  Create Course");
    QPushButton *btnAssign   = makeNavButton("👨‍🏫  Assign Instructor");
    QPushButton *btnLogout   = makeNavButton("🚪  Logout");
    btnLogout->setStyleSheet(btnLogout->styleSheet() + "color: #ff6b6b;");

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

    connect(btnHome,     &QPushButton::clicked, [this]{ stack->setCurrentIndex(0); });
    connect(btnStudents, &QPushButton::clicked, [this]{ stack->setCurrentIndex(1); });
    connect(btnAdd,      &QPushButton::clicked, [this]{ stack->setCurrentIndex(2); });
    connect(btnDel,      &QPushButton::clicked, [this]{ stack->setCurrentIndex(3); });
    connect(btnCourse,   &QPushButton::clicked, [this]{ stack->setCurrentIndex(4); });
    connect(btnAssign,   &QPushButton::clicked, [this]{ stack->setCurrentIndex(5); });
    connect(btnLogout,   &QPushButton::clicked, this, &AdminDashboard::onLogout);
}

QPushButton* AdminDashboard::makeNavButton(const QString &text) {
    QPushButton *btn = new QPushButton(text);
    btn->setStyleSheet(
        "QPushButton { background: transparent; color: #a0b8cc; "
        "text-align: left; padding: 10px 12px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #1a2d3d; color: #ffffff; }"
        );
    return btn;
}

QWidget* AdminDashboard::makeHomePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *welcome = new QLabel("Admin Dashboard");
    welcome->setStyleSheet("color: #ffffff; font-size: 24px; font-weight: bold;");

    QLabel *sub = new QLabel(QString("Logged in as: %1")
                                 .arg(QString::fromStdString(admin->getUsername())));
    sub->setStyleSheet("color: #5a7a99; font-size: 13px;");

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
    stats->addWidget(makeCard("Total Students",
                              QString::number(students->size())));
    stats->addWidget(makeCard("Total Professors",
                              QString::number(professors->size())));
    stats->addWidget(makeCard("Total Courses",
                              QString::number(courseManager->listAllCourses().size())));

    layout->addWidget(welcome);
    layout->addWidget(sub);
    layout->addLayout(stats);
    layout->addStretch();
    return page;
}

QWidget* AdminDashboard::makeStudentsPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("All Students");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Student ID", "Name", "Email", "GPA"});
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
    table->setRowCount(students->size());

    for (int i = 0; i < (int)students->size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(
                                 QString::fromStdString((*students)[i].getStudentID())));
        table->setItem(i, 1, new QTableWidgetItem(
                                 QString::fromStdString((*students)[i].getName())));
        table->setItem(i, 2, new QTableWidgetItem(
                                 QString::fromStdString((*students)[i].getEmail())));
        table->setItem(i, 3, new QTableWidgetItem(
                                 QString::number((*students)[i].getGPA(), 'f', 2)));
    }

    layout->addWidget(title);
    layout->addWidget(table);
    return page;
}

QWidget* AdminDashboard::makeAddStudentPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Add New Student");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    auto makeField = [](const QString &ph) {
        QLineEdit *f = new QLineEdit();
        f->setPlaceholderText(ph);
        f->setStyleSheet(
            "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return f;
    };

    addStudentID    = makeField("Student ID (e.g. S003)");
    addStudentName  = makeField("Full Name");
    addStudentEmail = makeField("Email");
    addStudentPwd   = makeField("Password");

    QPushButton *btn = new QPushButton("Add Student");
    btn->setStyleSheet(
        "background-color: #4e9eff; color: white; padding: 9px; "
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

QWidget* AdminDashboard::makeDeleteStudentPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Delete Student");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    delStudentID = new QLineEdit();
    delStudentID->setPlaceholderText("Enter Student ID to delete");
    delStudentID->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Delete Student");
    btn->setStyleSheet(
        "background-color: #ff6b6b; color: white; padding: 9px; "
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

QWidget* AdminDashboard::makeCreateCoursePage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Create New Course");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    auto makeField = [](const QString &ph) {
        QLineEdit *f = new QLineEdit();
        f->setPlaceholderText(ph);
        f->setStyleSheet(
            "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return f;
    };
    auto makeSpinBox = [](int min, int max, int def) {
        QSpinBox *s = new QSpinBox();
        s->setRange(min, max);
        s->setValue(def);
        s->setStyleSheet(
            "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
            "border-radius: 6px; padding: 8px; font-size: 13px;");
        return s;
    };

    courseID         = makeField("Course ID (e.g. CS301)");
    courseName       = makeField("Course Name");
    courseSchedule   = makeField("Schedule (e.g. MWF 10:00-11:00)");
    courseInstructor = makeField("Instructor ID (e.g. P001)");
    courseCredits    = makeSpinBox(1, 6, 3);
    courseCapacity   = makeSpinBox(5, 200, 30);

    QLabel *lCredits  = new QLabel("Credit Hours:");
    lCredits->setStyleSheet("color: #a0b8cc; font-size: 13px;");
    QLabel *lCapacity = new QLabel("Capacity:");
    lCapacity->setStyleSheet("color: #a0b8cc; font-size: 13px;");

    QPushButton *btn = new QPushButton("Create Course");
    btn->setStyleSheet(
        "background-color: #4e9eff; color: white; padding: 9px; "
        "border-radius: 6px; font-size: 13px; font-weight: bold;");

    createCourseStatus = new QLabel("");
    createCourseStatus->setStyleSheet("font-size: 13px;");

    layout->addWidget(title);
    layout->addWidget(courseID);
    layout->addWidget(courseName);
    layout->addWidget(courseSchedule);
    layout->addWidget(courseInstructor);
    layout->addWidget(lCredits);
    layout->addWidget(courseCredits);
    layout->addWidget(lCapacity);
    layout->addWidget(courseCapacity);
    layout->addWidget(btn);
    layout->addWidget(createCourseStatus);
    layout->addStretch();

    connect(btn, &QPushButton::clicked, this, &AdminDashboard::onCreateCourseClicked);
    return page;
}

QWidget* AdminDashboard::makeAssignInstructorPage() {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #0f1923;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Assign Instructor to Course");
    title->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold;");

    assignCourseID = new QLineEdit();
    assignCourseID->setPlaceholderText("Course ID (e.g. CS301)");
    assignCourseID->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    assignProfID = new QLineEdit();
    assignProfID->setPlaceholderText("Professor ID (e.g. P001)");
    assignProfID->setStyleSheet(
        "background-color: #111d2b; color: #ffffff; border: 1px solid #1a2d3d; "
        "border-radius: 6px; padding: 8px; font-size: 13px;");

    QPushButton *btn = new QPushButton("Assign");
    btn->setStyleSheet(
        "background-color: #4e9eff; color: white; padding: 9px; "
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

void AdminDashboard::onAddStudentClicked() {
    string id    = addStudentID->text().toStdString();
    string name  = addStudentName->text().toStdString();
    string email = addStudentEmail->text().toStdString();
    string pwd   = addStudentPwd->text().toStdString();

    if (id.empty() || name.empty() || email.empty() || pwd.empty()) {
        addStudentStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        addStudentStatus->setText("Please fill in all fields.");
        return;
    }
    for (auto &s : *students) {
        if (s.getStudentID() == id) {
            addStudentStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
            addStudentStatus->setText("Student ID already exists.");
            return;
        }
    }
    students->push_back(Student(id, name, email, pwd));
    addStudentStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
    addStudentStatus->setText("Student " + QString::fromStdString(name) + " added successfully.");
    addStudentID->clear(); addStudentName->clear();
    addStudentEmail->clear(); addStudentPwd->clear();
}

void AdminDashboard::onDeleteStudentClicked() {
    string id = delStudentID->text().toStdString();
    for (int i = 0; i < (int)students->size(); i++) {
        if ((*students)[i].getStudentID() == id) {
            students->erase(students->begin() + i);
            delStudentStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
            delStudentStatus->setText("Student deleted successfully.");
            delStudentID->clear();
            return;
        }
    }
    delStudentStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
    delStudentStatus->setText("Student ID not found.");
}

void AdminDashboard::onCreateCourseClicked() {
    string cid   = courseID->text().toStdString();
    string cname = courseName->text().toStdString();
    string sched = courseSchedule->text().toStdString();
    string instr = courseInstructor->text().toStdString();
    int    cred  = courseCredits->value();
    int    cap   = courseCapacity->value();

    if (cid.empty() || cname.empty()) {
        createCourseStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        createCourseStatus->setText("Course ID and Name are required.");
        return;
    }
    if (courseManager->findCourse(cid)) {
        createCourseStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        createCourseStatus->setText("Course ID already exists.");
        return;
    }
    courseManager->addCourse(Course(cid, cname, cred, cap, sched, instr));
    createCourseStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
    createCourseStatus->setText("Course " + QString::fromStdString(cname) + " created!");
    courseID->clear(); courseName->clear();
    courseSchedule->clear(); courseInstructor->clear();
}

void AdminDashboard::onAssignInstructorClicked() {
    string cid = assignCourseID->text().toStdString();
    string pid = assignProfID->text().toStdString();

    Course *c = courseManager->findCourse(cid);
    if (!c) {
        assignStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        assignStatus->setText("Course not found.");
        return;
    }
    Professor *found = nullptr;
    for (auto &p : *professors) if (p.getProfessorID() == pid) { found = &p; break; }
    if (!found) {
        assignStatus->setStyleSheet("color: #ff6b6b; font-size: 13px;");
        assignStatus->setText("Professor not found.");
        return;
    }
    c->setInstructor(pid);
    found->assignCourse(cid);
    assignStatus->setStyleSheet("color: #4e9eff; font-size: 13px;");
    assignStatus->setText("Instructor assigned successfully.");
    assignCourseID->clear(); assignProfID->clear();
}

void AdminDashboard::onLogout() { close(); }