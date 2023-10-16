#include "headers.h"

User::User(string userId, string password)
{
    this->userId = userId;
    this->password = password;
    this->isLogin = false;
    this->ip = "";
    this->port = -1;
    this->groupID = "";
}

File::File(string filename, string SHA, int bytes)
{
    this->fileName = fileName;
    this->bytes = bytes;
    this->SHA = SHA;
}
Group::Group(string groupID, string admin)
{
    this->groupID = groupID;
    this->admin = admin;
}