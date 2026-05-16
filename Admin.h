#ifndef ADMIN_H
#define ADMIN_H

#include <string>
using namespace std;

class Admin {
private:
    string adminID;
    string username;
    string password;
    string email;

public:
    Admin(string id, string user, string pwd, string e);

    string getAdminID();
    string getUsername();
    string getPassword();
    string getEmail();
    bool   validatePassword(string pwd);
};
//git
#endif