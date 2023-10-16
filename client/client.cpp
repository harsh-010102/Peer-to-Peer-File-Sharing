#include "headers.h"
int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        cerr << "Argument should be <tracker info file name> and <tracker_number>\n";
        return 0;
    }
    pair<pair<string, int>, pair<string, int>> address = processArgs(argc, argv);
    pair<string, int> clientDetails = address.first;
    pair<string, int> trackerDetails = address.second;


    int seeder_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (seeder_socket == -1)
    {
        cerr << "Error creating seeder_socket" << endl;
        return 1;
    }
    int opt = 1;
    struct sockaddr_in serverAddr;
    serverAddr.sin_port = htons(clientDetails.second);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    if (setsockopt(seeder_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cerr << "setsockopt";
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET, clientDetails.first.c_str(), &serverAddr.sin_addr) <= 0)
    {
        cerr << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if (bind(seeder_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Error binding socket" << endl;
        return 1;
    }
    if (listen(seeder_socket, 10) == -1)
    {
        cerr << "Error listening for connections" << endl;
        close(seeder_socket);
        return 1;
    }
    loggerHelper("Seeder is currently running on port : " + to_string(clientDetails.second) + '\n');
    thread t2 = thread(handleCommand, seeder_socket, trackerDetails.first, trackerDetails.second, clientDetails.second);
    t2.detach();
    while (true)
    {
        struct sockaddr_in leeder_addr;
        socklen_t leeder_addrSize = sizeof(leeder_addr);
        int leecher_socket = accept(seeder_socket, (struct sockaddr *)&leeder_addr, &leeder_addrSize);

        if (leecher_socket == -1)
        {
            cerr << "Error accepting client connection" << endl;
            return 1;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(leeder_addr.sin_addr), clientIP, INET_ADDRSTRLEN);
        // int clientPort = ntohs(leeder_addr.sin_port);

        thread t1 = thread(handleLeecherReq, leecher_socket);
        t1.detach();
    }
    return 0;
}