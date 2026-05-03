#ifndef UI_H
#define UI_H

#include <string>
#include <vector>

using namespace std;

class UI {
public:
    void clearScreen();
    void printMainMenu();
    void printLoginMenu();
    void printAdminMenu();
    void printStudentMenu();
    void printProfessorMenu();
    void printAddStudentMenu();
    void printDeleteStudentMenu();
    void printCreateCourseMenu();
    void printEditCourseMenu();
    void printMessage(string msg);
    void printError(string msg);
    void printSuccess(string msg);
    string promptInput(string label);
    void displayTable(vector<string> headers, vector<vector<string>> data);
};

#endif