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
            vector<string> nameTokens = str_tokenize(filepath, '/');
            string fileName = nameTokens[nameTokens.size() - 1];
            return upload_file(client_socket, fileName, filepath, groupId, SHA, bytes);
        }
    }
    else if (tokens[0] == "download_file")
    {
        if (tokens.size() != 4)
        {
            return "Invalid arg. It should be download_file  <group_id> <file_path> <destination_path>\n";
        }
        else
        {
            string groupId = tokens[1];
            string filename = tokens[2];
            string destination_path = tokens[3];
            return downloadFile(client_socket, groupId, filename, destination_path);
        }
    }
    else if (tokens[0] == "exit")
    {
        if (tokens.size() != 1)
        {
            return "Invalid arg. \n";
        }
        return exitClient(client_socket);
    }
    return "abc";
}
string exitClient(int client_socket)
{
    string command = "exit";
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while exit\n";
    }
    else
    {
        char output[1024];
        memset(output, 0, 1024);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for exit\n";
        }
        // cout << output << " " << sizeof(output) << endl;
        string a = output;
        if (a == "bye")
        {
            close(client_socket);
            cout << a << endl;
            exit(0);
        }
        else
        {
            cout << "can't exit error\n";
        }
    }
}

string downloadFile(int client_socket, string groupId, string filename, string destination_path)
{
    string command = "download_file " + groupId + " " + filename;
    // cout << command;
    if (write(client_socket, command.c_str(), command.size()) < 0)
    {
        return "Error while downloading files\n";
    }
    else
    {
        char output[10240];
        memset(output, 0, 10240);
        if (read(client_socket, output, sizeof(output)) < 0)
        {
            return "Error while reading for downloading files\n";
        }
        else
        {

            return handleDownloadOfFile(output, filename, destination_path);
        }
    }
    return "abc";
}
string handleDownloadOfFile(char output[10240], string filename, string destination_path)
{
    string message = output;
    vector<string> tokens = str_tokenize(message, ' ');

    string SHA = tokens[0];
    int size = stoi(tokens[1]);

    tokens.erase(tokens.begin());
    tokens.erase(tokens.begin());
    vector<string> addr = tokens;
    cout << "sha and sizze" << endl;
    cout << SHA << " " << size << endl;
    for (auto it : addr)
    {
        cout << it << " ";
    }
    cout << endl;

    int no_of_chunks = ceil(size * 1.0 / chunk_size);
    int temp = no_of_chunks;
    cout << "no_of chun " << no_of_chunks << endl;

    vector<bool> v(no_of_chunks, false);

    File new_file(destination_path, filename, SHA, size);
    new_file.available_chunk = v;

    filesInGroup.insert({filename, new_file});

    vector<unordered_set<string>> chunk_info(no_of_chunks);

    for (auto it : addr)
    {
        vector<string> address = str_tokenize(it, ':');
        string ipAddr = address[0];
        int port = stoi(address[1]);

        cout << "Ip and port----" << ipAddr << " " << port << endl;

        int seederSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (seederSocket == -1)
        {
            cerr << "Error creating socket at tracker" << endl;
            return "";
        }

        struct sockaddr_in tracker_addr;
        tracker_addr.sin_family = AF_INET;
        tracker_addr.sin_port = htons(port);
        tracker_addr.sin_addr.s_addr = INADDR_ANY;
        const char *trackerIp = ipAddr.c_str();

        if (inet_pton(AF_INET, trackerIp, &tracker_addr.sin_addr) <= 0)
        {
            cerr << "\nInvalid address/ Address not supported \n";
            close(seederSocket);
            return "";
        }

        if (connect(seederSocket, (struct sockaddr *)&tracker_addr, sizeof(tracker_addr)) == -1)
        {
            cerr << "Error connecting to seeder\n";
            close(seederSocket);
            return "";
        }

        string exec_command = "get_chunk_info " + filename;
        cout << exec_command << "\n";

        if (write(seederSocket, exec_command.c_str(), exec_command.size()) < 0)
        {
            close(seederSocket);
            cerr << "Error getting chuck info\n";
            return "";
        }

        char output[BUFF];
        memset(output, 0, BUFF);

        if (read(seederSocket, output, sizeof(output)) < 0)
        {
            cerr << "Error reading chuck info\n";
            return "";
        }

        cout << "output from seeder------" << endl;
        string message = output;
        cout << message << endl;

        vector<string> message_tokens = str_tokenize(message, ' ');
        cout << "message tokens size " << message_tokens.size() << endl;
        if (message_tokens[0] == "Error")
        {
            cerr << "Error fetching IP\n";
            return "";
        }
        else
        {
            for (int i = 0; i < message_tokens.size(); i++)
            {
                cout << message_tokens[i] << endl;
            }
            if (message_tokens[0] == "Success")
            {
                for (int i = 1; i < message_tokens.size(); i++)
                {
                    chunk_info[stoi(message_tokens[i])].insert(it);
                }

                cout << "printing chuck info vec.." << endl;
                int c = 0;
                for (auto it2 : chunk_info)
                {
                    cout << c++ << " ";
                    for (auto it3 : it2)
                    {
                        cout << it3 << " ";
                    }
                    cout << endl;
                }
            }
        }
        close(seederSocket);
    }

    vector<pair<int, int>> chunk_status;

    bool completeFile = true;
    int chunk_no = 0;

    for (auto it : chunk_info)
    {
        chunk_status.push_back({it.size(), chunk_no});
        if (it.size() == 0)
        {
            completeFile = false;
            break;
        }
        chunk_no++;
    }

    cout << "file status " << boolalpha << completeFile << endl;

    if (!completeFile)
    {
        cerr << "complete file is unavailable at tracker\n"
             << endl;
        return "";
    }
    else
    {

        int output_fd = open(destination_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
        if (output_fd < 0)
        {
            cerr << "Error on opening file\n";
            return "";
        }
        else
        {
            char buffer[chunk_size];
            memset(buffer, 0, chunk_size);
            cout << "No of chunks at writing part " << no_of_chunks << endl;
            for (int i = 0; i < no_of_chunks - 1; i++)
            {
                if (write(output_fd, buffer, chunk_size) < 0)
                {
                    cerr << "Error writing file\n";
                    return "";
                }
            }
            cout << "here0 " << no_of_chunks << endl;

            int mod = size % chunk_size;
            cout << "here1 " << no_of_chunks << endl;
            char buffer1[mod];
            cout << "here2 " << no_of_chunks << endl;

            bzero(buffer1, mod);
            cout << "here3 " << no_of_chunks << endl;
            
            if (write(output_fd, buffer1, chunk_size) < 0)
            {
                cerr << "Error writing file\n";
                return "";
            }
            cout << "here4 " << no_of_chunks << endl;
        }
        close(output_fd);

        cout << "no_of_chunks " << temp << endl;
        for (int i = 0; i < no_of_chunks; i++)
        {
            pair<int, string> chunk = findPieceSelection(chunk_info, chunk_status);
            cout << "chunk --------------"
                 << " " << chunk.first << " " << chunk.second << endl;
            // std::stringstream ss;

            // ss << "get_chunk " << filename << " " << chunk.first;
            // std::string new_command1 = ss.str();

            vector<string> address2 = str_tokenize(chunk.second, ':');
            string ip = address2[0];
            int port = stoi(address2[1]);
            cout << "ip and port after piece selection "
                 << " " << ip << " " << port << endl;

            int seeder_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (seeder_socket == -1)
            {
                cerr << "Error creating socket at seeder" << endl;
                return "";
            }

            struct sockaddr_in tracker_addr;
            tracker_addr.sin_family = AF_INET;
            tracker_addr.sin_port = htons(port);
            tracker_addr.sin_addr.s_addr = INADDR_ANY;
            const char *trackIp = ip.c_str();
            if (inet_pton(AF_INET, trackIp, &tracker_addr.sin_addr) <= 0)
            {
                cerr << "\nInvalid address/ Address not supported \n";
                close(seeder_socket);
                return "";
            }

            if (connect(seeder_socket, (struct sockaddr *)&tracker_addr, sizeof(tracker_addr)) == -1)
            {
                cerr << "Error connecting to seeder\n";
                close(seeder_socket);
                return "";
            }

            cout << "chunk --------------"
                 << " " << chunk.first << " " << chunk.second << endl;
            cout << "-------------------1111111111------------------" << endl;
            cout << filename << endl;
            string a = to_string(chunk.first);
            string new_command1 = "get_chunk " + filename + " " + a;
            cout << new_command1 << endl;
            cout << "-------------------2222222222------------------" << endl;

            if (write(seeder_socket, new_command1.c_str(), new_command1.size()) < 0)
            {
                cerr << "Error at write to seeder\n";
                return "";
            }

            char output[chunk_size];
            memset(output, 0, chunk_size);

            if (read(seeder_socket, output, chunk_size) < 0)
            {
                return "Error at reading from socket";
            }
            string ans = output;
            vector<string> ans_token = str_tokenize(ans, ' ');

            if (ans_token[0] == "Error")
            {
                cerr << "Error fetching chunkInfo\n";
                return "";
            }
            else
            {
                int output_fd1 = open(destination_path.c_str(), O_RDWR, S_IRWXU);
                if (output_fd1 < 0)
                {
                    cerr << "Error while writing file\n";
                    return "";
                }
                else
                {
                    int chunk_no = chunk.first;
                    if (chunk_no == no_of_chunks - 1)
                    {
                        int pos = chunk_no * chunk_size;

                        int size_of_last = size % chunk_size;
                        lseek(output_fd1, pos, SEEK_SET);

                        if (write(output_fd1, output, size_of_last)<0)
                        {
                            cerr << "error";
                            return "";
                        }
                    }
                    else
                    {
                        int pos = chunk_no * chunk_size;

                        cout << "while final writing ----- " << chunk_no << " " << pos << endl;

                        lseek(output_fd1, pos, SEEK_SET);

                        if (write(output_fd1, output, chunk_size)<0)
                        {
                            cerr << "return error";

                            return "";
                        }
                    }

                    close(output_fd1);

                    auto it = filesInGroup.find(filename);
                    it->second.available_chunk[chunk_no] = true;
                }
            }
            close(seeder_socket);
        }
    }
    return "";
}
string upload_file(int client_socket, string file_name, string filepath, string groupId, string SHA, int bytes)
{
    // cout << "filepath" << filepath;
    File new_file(filepath, file_name, SHA, bytes);
    int no_of_chunks = ceil(bytes * 1.0 / chunk_size);

    vector<bool> v;
    for (int i = 0; i < no_of_chunks; i++)
    {
        v.push_back(true);
    }
    new_file.available_chunk = v;
    filesInGroup.insert({file_name, new_file});
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

void handleLeecherReq(int leecher_socket)
{
    char input[1024] = {0};
    memset(input, 0, sizeof(input));

    if (read(leecher_socket, input, sizeof(input)) <= 0)
    {
        close(leecher_socket);
        return;
    }

    string msg = input;

    vector<string> input_tokens = str_tokenize(msg, ' ');

    cout << "Client " << to_string(leecher_socket) << " sends message : " << msg;

    string output = handleCommandofLeecher(input_tokens, leecher_socket);

    if (write(leecher_socket, output.c_str(), output.size()) <= 0)
    {
        loggerHelper("Client socket number" + to_string(leecher_socket) + "Write failed or zero" + '\n');
        close(leecher_socket);
        return;
    }
}
string handleCommandofLeecher(vector<string> input_tokens, int leecher_socket)
{
    if (input_tokens.size() == 0)
    {
        return "Invalid command";
    }
    else
    {
        if (input_tokens[0] == "get_chunk_info")
        {
            if (input_tokens.size() != 2)
            {
                return "Error Invalid command get_chuck_info";
            }
            else
            {
                string file = input_tokens[1];
                if (filesInGroup.find(file) == filesInGroup.end())
                {
                    return "Success ";
                }
                else
                {
                    string res = "Success ";
                    int count = 0;
                    for (auto it : filesInGroup.find(file)->second.available_chunk)
                    {
                        if (it)
                        {
                            res += to_string(count);
                            res += " ";
                        }
                        count++;
                    }
                    return res;
                }
            }
        }

        if (input_tokens[0] == "get_chunk")
        {
            if (input_tokens.size() != 3)
            {
                return "Error Invalid argument\n";
            }
            else
            {
                string file_name = input_tokens[1];
                int chunk_no = stoi(input_tokens[2]);
                cout << "-------------------";
                cout << "file_name" << file_name << " " << chunk_no << endl;
                int pos = chunk_no * chunk_size;
                cout << "pos in get_chuck call--" << endl;
                cout << pos << endl;

                // auto itx = filesInGroup.find(file_name);

                string dest_path = filesInGroup.find(file_name)->second.file_path;

                cout << "File path " << dest_path << endl;

                int fd = open(dest_path.c_str(), O_RDONLY, S_IRWXU);

                lseek(fd, pos, SEEK_SET);

                char output[chunk_size];

                if (read(fd, output, chunk_size) < 0)
                {
                    return "Error while reading file\n";
                }

                close(fd);
                return output;
            }
        }
    }
    return "abc";
}