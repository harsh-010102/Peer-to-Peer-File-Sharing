#include <bits/stdc++.h>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/sha.h>
#include <sstream>

using namespace std;
#define chunk_size 1024
#define BUFF 20480

string findSHA(const char *file_path);

void handleTracker(int trackerSocket);
string handleCommandofLeecher(vector<string> input_tokens, int leecher_socket);

pair<pair<string, int>, pair<string, int>> processArgs(int argc, char *argv[]);
void loggerHelper(string str);
void loggerClear();

class File
{
public:
    string file_name;
    int size;
    string SHA;
    vector<bool> available_chunk;
    string file_path;
    File(string file_path, string file_name, string SHA, int bytes);
};

extern unordered_map<string, File> filesInGroup;
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
string downloadFile(int client_socket, string groupId, string filename, string destination_path);
string listFiles(int client_socket, string groupId);
string upload_file(int client_socket, string file_name, string filepath, string groupId, string SHA, int bytes);
int fileSize(const char *file_path);
string handleDownloadOfFile(char output[10240], string filename, string destination_path);

// string handleDownloadOfFile(int client_socket, string file_path, string destination_path);

// pair<int, string> findPieceSelection(vector<unordered_set<string>> chunk_info, vector<pair<int, int>> chunk_status);
pair<int, string> findPieceSelection(vector<unordered_set<string>> &chunk_info, vector<pair<int, int>> &chunk_status);

string exitClient(int client_socket);