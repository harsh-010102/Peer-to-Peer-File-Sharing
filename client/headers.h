#include <bits/stdc++.h>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/sha.h>

using namespace std;

string findSHA(const char *file_path);

void handleTracker(int trackerSocket);

pair<pair<string, int>, pair<string, int>> processArgs(int argc, char *argv[]);
void loggerHelper(string str);
void loggerClear();

vector<string> str_tokenize(string str, char a);
string execute(vector<string> tokens, int client_socket, int trackerPort, int clientPorts);

void handleCommand(int clientSocket, string clientIP, int trackerPort, int clientPort);
void handleLeecherReq(int clientSocket);
// user function
string createUser(int client_socket, string username, string password);
string login(int client_socket, string username, string password, int port);
string logout(int client_socket);

// group function
string createGroup(int client_socket, string groupId);
string joinGroup(int client_socket, string groupId);
string leaveGroup(int client_socket, string groupId);
string listRequest(int client_socket, string groupId);
string acceptRequest(int client_socket, string groupId, string username);
string listGroup(int client_socket);

// file function
string downloadFile(int client_socket, string groupId, string filename);
string listFiles(int client_socket, string groupId);
string upload_file(int client_socket, string filepath, string groupId, string SHA, int bytes);

int fileSize(const char *file_path);