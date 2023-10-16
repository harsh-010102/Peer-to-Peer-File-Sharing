#include "headers.h"
string ipAddress, portNo;
unordered_map<string, User> userDetails;
unordered_map<int, string> userHelper;
unordered_map<string, Group> groupDetails;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Argument should be <tracker info file name> and <tracker_number>\n";
        return 0;
    }
    processArgs(argc, argv);
    int opt = 1;
    int trackerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (trackerSocket == -1)
    {
        cerr << "Error creating trackerSocket" << endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_port = htons(stoi(portNo));
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    if (setsockopt(trackerSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cerr << "setsockopt";
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0)
    {
        cerr << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (bind(trackerSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Error binding socket" << endl;
        return 1;
    }
    if (listen(trackerSocket, 10) == -1)
    {
        cerr << "Error listening for connections" << endl;
        close(trackerSocket);
        return 1;
    }
    loggerHelper("Tracker is currently running on port : " + portNo + '\n');
    thread t2 = thread(handleTracker, trackerSocket);
    t2.detach();
    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(trackerSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);

        if (clientSocket == -1)
        {
            cerr << "Error accepting client connection" << endl;
            return 1;
        }

        char clientIP[INET_ADDRSTRLEN]; 
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);

        thread t1 = thread(handleReq, clientSocket, (string)clientIP, clientPort);
        t1.detach();
    }
    close(trackerSocket);
    return 0;
}