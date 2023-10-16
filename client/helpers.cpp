#include "headers.h"
void handleCommand(int seederSocket, string trackerIp, int trackerPort, int clientPort)
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        cerr << "Error creating socket at tracker" << endl;
        exit(0);
    }
    struct sockaddr_in tracker_addr;
    tracker_addr.sin_family = AF_INET;
    tracker_addr.sin_port = htons(trackerPort);
    tracker_addr.sin_addr.s_addr = INADDR_ANY;

    if (inet_pton(AF_INET, trackerIp.c_str(), &tracker_addr.sin_addr) <= 0)
    {
        cerr << "\nInvalid address/ Address not supported \n";
        close(client_socket);
        exit(0);
    }
    if (connect(client_socket, (struct sockaddr *)&tracker_addr, sizeof(tracker_addr)) == -1)
    {
        cerr << "Error connecting to tracker\n";
        close(client_socket);
        exit(0);
    }
    while (true)
    {
        string input;
        getline(cin, input);
        vector<string> str_tokens = str_tokenize(input, ' ');

        string output = execute(str_tokens, client_socket, trackerPort, clientPort);
        cout << output << endl;
    }
}
string execute(vector<string> tokens, int client_socket, int trackerPort, int clientPort)
{
    if (tokens[0] == "create_user")
    {
        if (tokens.size() != 3)
        {
            return "Invalid arg. It should be create_user <username> <password>\n";
        }
        else
        {
            string user_name = tokens[1];
            string password = tokens[2];
            return createUser(client_socket, user_name, password);
        }
    }

    else if (tokens[0] == "login")
    {
        if (tokens.size() != 3)
        {
            return "Invalid arg. It should be login <username> <password>\n";
        }
        else
        {
            string user_name = tokens[1];
            string password = tokens[2];
            int port = clientPort;
            return login(client_socket, user_name, password, port);
        }
    }
    else if (tokens[0] == "logout")
    {
        if (tokens.size() != 1)
        {
            return "Invalid arg. It should be logout\n";
        }
        else
        {
            return logout(client_socket);
        }
    }
    else if (tokens[0] == "create_group")
    {
        if (tokens.size() != 2)
        {
            return "Invalid arg. It should be create_group <group_id>\n";
        }
        else
        {
            string groupId = tokens[1];
            return createGroup(client_socket, groupId);
        }
    }
    else if (tokens[0] == "join_group")
    {
        if (tokens.size() != 2)
        {
            return "Invalid arg. It should be create_group <group_id>\n";
        }
        else
        {
            string groupId = tokens[1];
            return joinGroup(client_socket, groupId);
        }
    }
    else if (tokens[0] == "leave_group")
    {
        if (tokens.size() != 2)
        {
            return "Invalid arg. It should be leave_group <group_id>\n";
        }
        else
        {
            string groupId = tokens[1];
            return leaveGroup(client_socket, groupId);
        }
    }
    else if (tokens[0] == "list_requests")
    {
        if (tokens.size() != 2)
        {
            return "Invalid arg. It should be list_requests <group_id>\n";
        }
        else
        {
            string groupId = tokens[1];
            return listRequest(client_socket, groupId);
        }
    }
    else if (tokens[0] == "accept_request")
    {
        if (tokens.size() != 3)
        {
            return "Invalid arg. It should be list_requests <group_id>\n";
        }
        else
        {
            string groupId = tokens[1];
            string username = tokens[2];
            return acceptRequest(client_socket, groupId, username);
        }
    }
    else if (tokens[0] == "list_groups")
    {
        if (tokens.size() != 1)
        {
            return "Invalid arg. It should be list_groups\n";
        }
        else
        {
            return listGroup(client_socket);
        }
    }
    else if (tokens[0] == "list_files")
    {
        if (tokens.size() != 2)
        {
            return "Invalid arg. It should be list_files <group_id>\n";
        }
        else
        {
            string groupId = tokens[1];
            return listFiles(client_socket, groupId);
        }
    }
    else if (tokens[0] == "upload_file")
    {
        if (tokens.size() != 3)
        {
            return "Invalid arg. It should be upload_file <file_path> <group_id>\n";
        }
        else
        {
            string filepath = tokens[1];
            string groupId = tokens[2];
            int bytes = fileSize(filepath.c_str());
            string SHA = findSHA(filepath.c_str());

            return upload_file(client_socket, filepath, groupId, SHA, bytes);
        }
    }
    else if (tokens[0] == "download_file")
    {
        if (tokens.size() != 4)
        {
            return "Invalid arg. It should be download_file <file_path> <group_id>\n";
        }
        else
        {
            string groupId = tokens[1];
            string filename = tokens[2];
            string destination_path = tokens[3];
            // return upload_file(client_socket, groupId, filename);
        }
    }
}
string downloadFile(int client_socket, string groupId, string filename)
{
    string command = "download_file " + groupId + " " + "filename";
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while downloading files\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for downloading files\n";
        }
        else
        {
            return output;
        }
    }
}
string upload_file(int client_socket, string filepath, string groupId, string SHA, int bytes)
{
    string command = "upload_file " + filepath + " " + groupId + " " + SHA + " " + to_string(bytes);
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while uploading files\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for uploading files\n";
        }
        else
        {
            return output;
        }
    }
}
string listFiles(int client_socket, string groupId)
{
    string command = "list_files " + groupId;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while listing files\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for listing files\n";
        }
        else
        {
            return output;
        }
    }
}

string listGroup(int client_socket)
{
    string command = "list_group";
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while listing requests\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for listing group\n";
        }
        else
        {
            return output;
        }
    }
}
string acceptRequest(int client_socket, string groupId, string username)
{
    string command = "accept_request " + groupId + " " + username;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while accepting requests\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for accepting group\n";
        }
        else
        {
            return output;
        }
    }
}
string listRequest(int client_socket, string groupId)
{
    string command = "list_requests " + groupId;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while listing requests\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for listing request group\n";
        }
        else
        {
            return output;
        }
    }
}
string leaveGroup(int client_socket, string groupId)
{
    string command = "leave_group " + groupId;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while leaving group\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for leaving group\n";
        }
        else
        {
            return output;
        }
    }
}
string joinGroup(int client_socket, string groupId)
{
    string command = "join_group " + groupId;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while join group\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for joining group\n";
        }
        else
        {
            return output;
        }
    }
}
string createGroup(int client_socket, string groupId)
{
    string command = "create_group " + groupId;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while logout_user\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for creating groupu\n";
        }
        else
        {
            return output;
        }
    }
}

string logout(int client_socket)
{
    string command = "logout";
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while logout_user\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for logout\n";
        }
        else
        {
            return output;
        }
    }
}

string login(int client_socket, string username, string password, int port)
{
    string command = "login " + username + " " + password + " " + to_string(port);
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while login_user\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for login_user\n";
        }
        else
        {
            return output;
        }
    }
}

string createUser(int client_socket, string user_name, string password)
{
    string command = "create_user " + user_name + " " + password;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while create_user\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for create_user\n";
        }
        else
        {
            return output;
        }
    }
}

void handleLeecherReq(int clientSocket)
{
}