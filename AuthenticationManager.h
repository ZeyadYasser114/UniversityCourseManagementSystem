#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H
#include <string>
#include "Admin.h"
#include "Student.h"
#include "Professor.h"
using namespace std;
class AuthenticationManager {
public:
    bool loginAdmin(string username, string password);
    bool loginStudent(string studentID, string password);
    bool loginProfessor(string professorID, string password);
    bool registerStudent(string studentID, string name, string email, string password);
    bool registerProfessor(string professorID, string name, string email, string password, string department);
};
#endif