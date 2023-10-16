#include "headers.h"

vector<string> str_tokenize(string str);
void handleTracker(int trackSocket)
{
    string input;
    while (true)
    {
        cin >> input;
        if (input == "quit")
        {
            close(trackSocket);
            cout << "Tracker closed";
            loggerHelper("Tracker Close");
            exit(0);
        }
    }
}
void handleReq(int clientSocket, string clientIP, int clientPort)
{
    loggerHelper("Client socket is connected with socket number: " + to_string(clientSocket) + "\n");
    // cout << clientIP.size() << " " << clientIP << endl;
    char input[1024] = {0};
    while (true)
    {
        memset(input, 0, sizeof(input));

        if (read(clientSocket, input, sizeof(input)) <= 0)
        {
            close(clientSocket);
            return;
        }
        string a = "";
        for (int i = 0; i < 4; i++)
        {
            a += input[i];
        }

        if (a == "exit")
        {
            string output2;
            logout(clientSocket, output2);
            write(clientSocket, output2.c_str(), output2.size());
            close(clientSocket);
            return;
        }
        string msg(input);

        vector<string> input_tokens = str_tokenize(msg);
        cout << "Client " << to_string(clientSocket) << " sends message : " << msg;
        // string output = "Message Successfully sent!!\n";
        string output = handleCommand(input_tokens, clientSocket, clientIP, clientPort);
        if (write(clientSocket, output.c_str(), output.size()) <= 0)
        {
            loggerHelper("Client socket number" + to_string(clientSocket) + "Write failed or zero" + '\n');
            close(clientSocket);
            return;
        }
        loggerHelper("Client " + to_string(clientSocket) + " sends message: " + string(input) + "\n");
    }
}

string handleCommand(vector<string> input_tokens, int clientSocket, string clientIP, int clientPort)
{
    string output;
    for (int i = 0; i < input_tokens.size(); i++)
    {
        cout << input_tokens[i] << " ";
    }
    cout << input_tokens.size() << endl;
    if (input_tokens[0] == "create_user")
    {
        if (input_tokens.size() != 3)
        {
            output = "Argument to be in form create_user <user_id> <password>\n";
        }
        else
        {
            string username = input_tokens[1];
            string password = input_tokens[2];
            if (createUser(username, password))
            {
                output = "Account Created\n";
            }
            else
            {
                output = "User Already exist!!\n";
            }
        }
    }
    else if (input_tokens[0] == "login")
    {
        if (input_tokens.size() != 4)
        {
            output = "Argument to be in form login <user_id> <password>\n";
        }
        else
        {
            string username = input_tokens[1];
            string password = input_tokens[2];
            int port = stoi(input_tokens[3]);
            login(username, password, output, clientSocket, clientIP, port);
        }
    }

    else if (input_tokens[0] == "create_group")
    {
        if (input_tokens.size() != 2)
        {
            output = "Invalid argument. It should be create_group <group_id>\n";
        }
        else
        {
            string groupId = input_tokens[1];
            createGroup(groupId, output, clientSocket);
        }
    }

    else if (input_tokens[0] == "join_group")
    {
        if (input_tokens.size() != 2)
        {
            output = "Invalid argument. It should be join_group <group_id>\n";
        }
        else
        {
            string groupId = input_tokens[1];
            joinGroup(groupId, output, clientSocket);
        }
    }

    else if (input_tokens[0] == "leave_group")
    {

        if (input_tokens.size() != 2)
        {
            output = "Invalid argument. It should be leave_group <group_id>\n";
        }
        else
        {
            string groupId = input_tokens[1];
            leaveGroup(groupId, output, clientSocket);
        }
    }

    else if (input_tokens[0] == "list_requests")
    {
        if (input_tokens.size() != 2)
        {
            output = "Invalid argument. It should be list_requests <group_id>\n";
        }
        else
        {
            string groupId = input_tokens[1];
            listPendingRequest(groupId, output, clientSocket);
        }
    }

    else if (input_tokens[0] == "accept_request")
    {
        if (input_tokens.size() != 3)
        {
            output = "Invalid argument. It should be accept_request <group_id> <userId>\n";
        }
        else
        {
            string groupId = input_tokens[1];
            string userID = input_tokens[2];
            // void acceptRequest(string groupId, string &output, int clientSocket, string userID)

            acceptRequest(groupId, output, clientSocket, userID);
        }
    }
    else if (input_tokens[0] == "list_groups")
    {
        if (input_tokens.size() != 1)
        {
            output = "Invalid argument. It should be list_groups\n";
        }
        else
        {
            listGroup(clientSocket, output);
        }
    }
    else if (input_tokens[0] == "list_files")
    {
        if (input_tokens.size() != 2)
        {
            output = "Invalid argument. It should be list_files<group_id>\n";
        }
        else
        {
            string groupId = input_tokens[1];
            listFiles(clientSocket, output, groupId);
        }
    }
    else if (input_tokens[0] == "upload_file")
    {
        if (input_tokens.size() != 5)
        {
            output = "Invalid argument. It should be upload_files <file_path> <group_id> <SHA> <fileSize>\n";
        }
        else
        {
            string filePath = input_tokens[1];
            string groupId = input_tokens[2];
            string SHA = input_tokens[3];
            int fileSize = atoi(input_tokens[4].c_str());

            uploadFiles(groupId, output, clientSocket, filePath, SHA, fileSize);
        }
    }
    else if (input_tokens[0] == "download_file")
    {
        // string groupId, string fileName
        if (input_tokens.size() != 3)
        {
            output = "Invalid argument. It should be download_file  <file_path> <group_id>\n";
        }
        else
        {
            string fileName = input_tokens[1];
            string groupId = input_tokens[2];
            downloadFiles(clientSocket, output, groupId, fileName);
        }
    }
    else if (input_tokens[0] == "logout")
    {
        if (input_tokens.size() != 1)
        {
            output = "Invalid argument. It should be logout\n";
        }
        logout(clientSocket, output);
    }
    else
    {
        output = "Invalid Command...........\n";
    }

    return output;
}

void listFiles(int clientSocket, string &output, string groupId)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }

    else
    {
        string username = userHelper.find(clientSocket)->second;
        auto it = userDetails.find(username);
        string groupID = it->second.groupID;
        if (groupID != "")
        {
            if (groupId == groupID)
            {
                auto it2 = groupDetails.find(groupID);
                unordered_map<string, File> temp = it2->second.fileInfo;
                for (auto it4 : temp)
                {
                    output += it4.first;
                    output += " ";
                }
            }
            else
            {
                output = "You are part of different group\n";
            }
        }
        else
        {
            output = "You are not part of group\n";
        }
    }
}

void downloadFiles(int clientSocket, string &output, string groupId, string fileName)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "DownloadError: You need to login first\n";
    }

    else
    {
        string username = userHelper.find(clientSocket)->second;
        auto it = userDetails.find(username);
        string groupID = it->second.groupID;
        if (groupID != "")
        {
            if (groupId == groupID)
            {
                auto it2 = groupDetails.find(groupID);
                unordered_map<string, File> temp = it2->second.fileInfo;
                auto it3 = temp.find(fileName);
                output += it3->second.SHA;
                output += " ";
                output += it3->second.bytes;
                output += " ";
                if (it3 != temp.end())
                {
                    for (auto it4 : it3->second.address)
                    {
                        output += it4;
                        output += " ";
                    }
                }
                else
                {
                    output = "DownloadError: File not exists\n";
                }
            }
            else
            {
                output = "DownloadError: You are part of different group\n";
            }
        }
        else
        {
            output = "DownloadError: You are not part of group\n";
        }
    }
}
void listGroup(int clientSocket, string &output)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }
    else
    {
        for (auto it : groupDetails)
        {
            output += it.second.groupID;
            output += " ";
        }
    }
}

void acceptRequest(string groupId, string &output, int clientSocket, string userID)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }
    else
    {
        string username;
        auto it = userHelper.find(clientSocket);
        username = it->second;
        string group = userDetails.at(username).groupID;
        if (groupDetails.find(groupId) != groupDetails.end())
        {
            string admin = groupDetails.at(groupId).admin;
            if (group == groupId)
            {
                if (admin == username)
                {
                    auto it2 = userDetails.at(username);
                    it2.groupID = groupId;
                    if (groupDetails.at(groupId).pendingRequest.find(userID) != groupDetails.at(groupId).pendingRequest.end())
                    {
                        if (userDetails.at(userID).groupID == "")
                        {

                            groupDetails.at(groupId).pendingRequest.erase(userID);
                            groupDetails.at(groupId).participants.insert(userID);
                            userDetails.at(userID).groupID = groupId;

                            output = "Request accepted \n";
                        }
                        else
                        {
                            output = "User is already a part of one group\n";
                        }
                    }
                    else
                    {
                        output = "User doesn't exists\n";
                    }
                }
                else
                {
                    output = "You are not an admin.\n";
                }
            }
            else
            {
                output = "You are not part of this group \n";
            }
        }
        else
        {
            output = "You are not part of group\n";
        }
    }
}
void listPendingRequest(string groupId, string &output, int clientSocket)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }
    else
    {
        string username;
        auto it = userHelper.find(clientSocket);
        username = it->second;
        auto new_it = userDetails.find(username);
        string groupID = new_it->second.groupID;
        if (groupID == groupId)
        {
            if (groupDetails.find(groupId) != groupDetails.end())
            {
                string admin = groupDetails.at(groupId).admin;
                if (admin == username)
                {
                    unordered_set<string> temp = groupDetails.at(groupId).pendingRequest;
                    for (auto it2 : temp)
                    {
                        output += it2;
                        output += "\n";
                    }
                }
                else
                {
                    output = "You are not an admin.\n";
                }
            }
            else
            {
                output = "Group doesn't exists.\n";
            }
        }
        else
        {
            output = "You are not part of this group\n";
        }
    }
}
void removeFilesOnLogout(string groupId, int clientSocket, string address)
{
    auto it = groupDetails.find(groupId);
    auto it2 = userHelper.find(clientSocket);
    string username = it2->second;
    auto it3 = userDetails.find(username);
    unordered_set<string> file = it3->second.fileName;
    for (auto it4 : file)
    {
        string filename = it4;
        auto it5 = it->second.fileInfo.find(filename);
        auto addr = it5->second.address.find(address);

        it5->second.address.erase(addr);

        if (it5->second.address.empty())
        {
            it->second.fileInfo.erase(it5);
        }
    }
    it3->second.fileName.clear();
}

void addChunk(string groupId, string &output, int clientSocket, string filename, string SHA, int fileSize)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }
    else
    {
        string username;
        auto it = userHelper.find(clientSocket);
        username = it->second;
        auto it2 = userDetails.find(username);
        if (it2->second.groupID != "")
        {
            if (groupDetails.find(groupId) != groupDetails.end())
            {
                auto it3 = groupDetails.find(groupId);
                auto it4 = it3->second.fileInfo.find(filename);

                string ip = it2->second.ip;
                int port = it2->second.port;
                if (it4 == it3->second.fileInfo.end())
                {
                    output = "Full file not exists\n";
                }
                else
                {
                    it4->second.address.insert(ip + ":" + to_string(port));
                    it2->second.fileName.insert(filename);
                    output = "File uploaded\n";
                }
            }
            else
            {
                output = "Group doesn't exists\n";
            }
        }
        else
        {
            output = "You are not a part of any group\n";
        }
    }
}
void uploadFiles(string groupId, string &output, int clientSocket, string filename, string SHA, int fileSize)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }
    else
    {
        string username;
        auto it = userHelper.find(clientSocket);
        username = it->second;
        auto it2 = userDetails.find(username);
        if (it2->second.groupID != "")
        {
            if (groupDetails.find(groupId) != groupDetails.end())
            {
                if (it2->second.groupID == groupId)
                {
                    auto it3 = groupDetails.find(groupId);
                    auto it4 = it3->second.fileInfo.find(filename);

                    string ip = it2->second.ip;
                    int port = it2->second.port;
                    if (it4 == it3->second.fileInfo.end())
                    {
                        File newFile(filename, SHA, fileSize);
                        newFile.address.insert(ip + ":" + to_string(port));
                        it3->second.fileInfo.insert({filename, newFile});
                        output = "File uploaded\n";
                        it2->second.fileName.insert(filename);
                    }
                    else
                    {
                        it4->second.address.insert(ip + ":" + to_string(port));

                        it2->second.fileName.insert(filename);

                        output = "File uploaded\n";
                    }
                }
                else
                {
                    output = "You are not part of this group\n";
                }
            }
            else
            {
                output = "Group doesn't exists\n";
            }
        }
        else
        {
            output = "You are not a part of any group\n";
        }
    }
}

void leaveGroup(string groupId, string &output, int clientSocket)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }
    else
    {
        string username;
        auto it = userHelper.find(clientSocket);
        username = it->second;
        auto it2 = userDetails.find(username);
        string ip = it2->second.ip;
        int port = it2->second.port;
        string address = ip + ":" + to_string(port);
        if (it2->second.groupID != "")
        {
            if (groupDetails.find(groupId) != groupDetails.end())
            {
                if (groupId == it2->second.groupID)
                {
                    groupDetails.at(groupId).participants.erase(username);
                    removeFilesOnLogout(groupId, clientSocket, address);
                    it2->second.groupID = "";
                    output = "Leave group Success\n";
                }
                else
                {
                    output = "You are not part of this group\n";
                }
            }
            else
            {
                output = "Group doesn't exists\n";
            }
        }
        else
        {
            output = "You are not a part of any group\n";
        }
    }
}

void joinGroup(string groupId, string &output, int clientSocket)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }
    else
    {
        string username;
        auto it = userHelper.find(clientSocket);
        username = it->second;
        auto it2 = userDetails.find(username);
        if (it2->second.groupID == "")
        {
            if (groupDetails.find(groupId) != groupDetails.end())
            {
                if (groupDetails.at(groupId).admin == username)
                {
                    groupDetails.at(groupId).participants.insert(username);
                    it2->second.groupID = groupId;
                    output = "Welcome admin\n";
                }
                else
                {
                    groupDetails.at(groupId).pendingRequest.insert(username);
                    output = "Join request sent to admin\n";
                }
            }
            else
            {
                output = "Group doesn't exists\n";
            }
        }
        else
        {
            output = "You can be part of only one group\n";
        }
    }
}

void createGroup(string groupId, string &output, int clientSocket)
{
    if (userHelper.find(clientSocket) == userHelper.end())
    {
        output = "You need to login first\n";
    }

    else if (groupDetails.find(groupId) != groupDetails.end())
    {
        output = "Group with groupId " + groupId + " Already exists";
    }

    else
    {
        string admin;
        auto it = userHelper.find(clientSocket);
        admin = it->second;
        // auto it2 = userDetails.find(admin);

        Group new_group(groupId, admin);
        groupDetails.insert({groupId, new_group});
        printGroupDetails();
        output = "Group created successfully\n";
    }
}
bool createUser(string username, string password)
{
    if (userDetails.find(username) == userDetails.end())
    {
        User new_user(username, password);
        userDetails.insert({username, new_user});
        printUserDetails();
        return true;
    }
    else
    {
        return false;
    }
}

void login(string username, string password, string &output, int clientSocket, string clientIP, int clientPort)
{
    if (userHelper.find(clientSocket) != userHelper.end())
    {
        output = "Another user already login!!\n";
        return;
    }
    if (userDetails.find(username) == userDetails.end())
    {
        output = "Username not found! Create User first\n";
        return;
    }
    auto it = userDetails.find(username);
    if (it->second.isLogin == true)
    {
        output = "User Already login using another socket\n";
        return;
    }
    else
    {
        string pass = it->second.password;
        if (password == pass)
        {
            it->second.isLogin = true;
            it->second.port = clientPort;
            it->second.ip = clientIP;
            userHelper.insert({clientSocket, username});
            output = "Login Successful\n";
            return;
        }
        else
        {
            output = "Invalid Credentials\n";
            return;
        }
    }
}

// todo check if it is join in a group or not for removing files ..
void logout(int clientSocket, string &output)
{
    auto it = userHelper.find(clientSocket);
    if (it == userHelper.end())
    {
        output = "You are not logged in , kindly login first\n";
        return;
    }
    else
    {
        string username = it->second;
        auto it2 = userDetails.find(username);
        string groupID = it2->second.groupID;
        string address = it2->second.ip + ":" + to_string(it2->second.port);
        removeFilesOnLogout(groupID, clientSocket, address);
        it2->second.ip = "";
        it2->second.port = -1;
        it2->second.isLogin = false;
        userHelper.erase(it);
        output = "Logout Successful\n";
        return;
    }
}
vector<string> str_tokenize(string str)
{
    vector<string> tokens;
    string temp = "";
    for (int i = 0; i < (int)str.size(); i++)
    {
        if (str[i] == ' ')
        {
            tokens.push_back(temp);
            temp = "";
        }
        else
        {
            temp += str[i];
        }
    }
    tokens.push_back(temp);
    return tokens;
}

void loggerHelper(string txt)
{
    string loggerFile = "trackerLog.txt";

    const char *filePtr = loggerFile.data();

    long long fd2 = open(filePtr, O_RDWR | O_APPEND, S_IRWXU);
    if (fd2 < 0)
    {
        cout << "Error while opening logger";
        exit(0);
    }

    char temp[1024];
    for (int i = 0; i < 1024; i++)
    {
        temp[i] = txt[i];
    }
    write(fd2, temp, txt.size());
    close(fd2);
}
void loggerClear()
{
    string loggerFile = "trackerLog.txt";

    const char *filePtr = loggerFile.data();

    long long fd2 = open(filePtr, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd2 < 0)
    {
        cout << "Error while opening logger";
        exit(0);
    }
    close(fd2);
}
