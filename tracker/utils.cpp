#include "headers.h"
void processArgs(int argc, char *argv[])
{
    loggerClear();
    string trackerFile = argv[1];
    const char *tracker_file_info = trackerFile.data();
    char temp[1024];
    string tracker_number = argv[2];
    long long fd = open(tracker_file_info, O_RDONLY);
    if (fd < 0)
    {
        cout << "Error while opening file\n";
        exit(0);
    }
    long long size = lseek(fd, 0, SEEK_END) - 1;
    lseek(fd, 0, SEEK_SET);
    int no = read(fd, temp, size);
    string str = "";
    vector<string> tracker_info;
    for (int i = 0; i < no; i++)
    {
        if (temp[i] != '\n')
        {
            str += temp[i];
        }

        else
        {
            tracker_info.push_back(str);
            str = "";
        }
    }
    tracker_info.push_back(str);
    if (tracker_number == "1")
    {
        ipAddress = tracker_info[0];
        portNo = tracker_info[1];
    }
    else
    {
        ipAddress = tracker_info[2];
        portNo = tracker_info[3];
    }
    return;
}

void printUserDetails()
{
    for (auto x : userDetails)
    {
        cout << x.first << " ";
        cout << x.second.password << " ";
        cout << boolalpha << x.second.isLogin << " ";
        cout << x.second.ip << " ";
        cout << x.second.port << " ";
        cout << x.second.groupID << " ";
        cout << endl;
    }
    return;
}

void printGroupDetails()
{
    for (auto it : groupDetails)
    {
        cout << "Group ID" << it.first << " ";
        cout << "Admin is" << it.second.admin << endl;
        cout << "Pending request ";
        for (auto it2 : it.second.pendingRequest)
        {
            cout << it2 << " ";
        }
        cout << endl;

        cout << "Participants ";
        for (auto it3 : it.second.participants)
        {
            cout << it3 << " ";
        }
        cout << endl;
    }
}
