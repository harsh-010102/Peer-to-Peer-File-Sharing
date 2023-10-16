#include "headers.h"

pair<pair<string, int>, pair<string, int>> processArgs(int argc, char *argv[])
{
    loggerClear();
    string address = argv[1];
    string trackerFile = argv[2];
    const char *tracker_file_info = trackerFile.data();
    char temp[1024];
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
    string trackerIp = tracker_info[0];
    int trackerPort = atoi(tracker_info[1].c_str());
    vector<string> clientAddress = str_tokenize(address, ':');
    pair<pair<string, int>, pair<string, int>> ans;
    ans.first.first = clientAddress[0];
    ans.first.second = stoi(clientAddress[1]);
    ans.second.first = trackerIp;
    ans.second.second = trackerPort;
    return ans;
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
vector<string> str_tokenize(string str, char a)
{
    vector<string> tokens;
    string temp = "";
    for (int i = 0; i < (int)str.size(); i++)
    {
        if (str[i] == a)
        {
            tokens.push_back(temp);
            temp = "";
        }
        else if (i == (int)str.size() - 1)
        {

            temp += str[i];
            tokens.push_back(temp);
            temp = "";
        }
        else
        {
            temp += str[i];
        }
    }

    return tokens;
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
