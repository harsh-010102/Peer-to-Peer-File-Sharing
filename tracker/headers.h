#include <bits/stdc++.h>
#include <bits/stdc++.h>
#include <iostream>
#include <thread>
#include <sys/socket.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
class User
{
public:
    string userId;
    string password;
    bool isLogin;
    string ip;
    string groupID;
    int port;
    unordered_set<string> fileName;
    User(string userId, string password);
};

class File
{
public:
    string fileName;
    string SHA;
    int bytes;
    unordered_set<string> address;
    File(string filename, string SHA, int bytes);
};

class Group
{
public:
    string groupID;
    string admin;
    unordered_set<string> pendingRequest;
    unordered_set<string> participants;
    unordered_map<string, File> fileInfo;
    Group(string groupID, string admin);
};

extern string ipAddress;
extern string portNo;
extern unordered_map<string, User> userDetails;
extern unordered_map<int, string> userHelper;
extern unordered_map<string, Group> groupDetails;

void handleTracker(int trackerSocket);

void processArgs(int argc, char *argv[]);
void handleReq(int clientSocket, string clientIP, int clientPort);
void loggerHelper(string str);
void loggerClear();
void handleTracker(int trackSocket);

string handleCommand(vector<string>, int clientSocket, string clientIP, int clientPort);
void printUserDetails();

// user function
bool createUser(string username, string password);
void login(string username, string password, string &output, int clientSocket, string clientIP, int clientPort);
void logout(int clientSocket, string &output);

// group
void createGroup(string groupId, string &output, int clientSocket);
void joinGroup(string groupId, string &output, int clientSocket);
void printGroupDetails();
void leaveGroup(string groupId, string &output, int clientSocket);
void listPendingRequest(string groupId, string &output, int clientSocket);
void acceptRequest(string groupId, string &output, int clientSocket, string userID);
void listGroup(int clientSocket, string &output);

// files handle

void removeFilesOnLogout(string groupId, int clinetSocket, string address);

void listFiles(int clientSocket, string &output, string groupId);
void downloadFiles(int clientSocket, string &output, string groupId, string fileName);
void addChunk(string groupId, string &output, int clientSocket, string filename, string SHA, int fileSize);
void uploadFiles(string groupId, string &output, int clientSocket, string filename, string SHA, int fileSize);