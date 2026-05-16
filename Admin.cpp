#include "Admin.h"
using namespace std;

Admin::Admin(string id, string user, string pwd, string e)
    : adminID(id), username(user), password(pwd), email(e) {}

string Admin::getAdminID()   { return adminID; }
string Admin::getUsername()  { return username; }
string Admin::getPassword()  { return password; }
string Admin::getEmail()     { return email; }
bool   Admin::validatePassword(string pwd) { return password == pwd; }
//git