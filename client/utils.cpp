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
    cout << trackerIp << " "<< trackerPort<<endl;
    vector<string> clientAddress = str_tokenize(address, ':');
    pair<pair<string, int>, pair<string, int>> ans;
    ans.first.first = clientAddress[0];
    ans.first.second = stoi(clientAddress[1]);
    ans.second.first = trackerIp;
    ans.second.second = trackerPort;
    return ans;
}

pair<int, string> findPieceSelection(vector<unordered_set<string>> &chunk_info, vector<pair<int, int>> &chunk_status)
{
    sort(chunk_status.begin(), chunk_status.end());
    cout << 1 << endl;

    int seeders = chunk_status.begin()->first;
    cout << 2 << endl;

    int chunk_no = chunk_status.begin()->second;
    cout << 3 << endl;

    auto r = rand() % seeders;
    cout << 4 << endl;

    auto it = chunk_info[chunk_no].begin();
    cout << 5 << endl;

    advance(it, r);
    string address = *it;
    cout << 6 << endl;

    chunk_status.erase(chunk_status.begin());
    cout << 7 << endl;
    cout << "Piece wise and address " << endl;
    cout << chunk_no << " " << address << endl;
    cout << 8 << endl;

    return {chunk_no, address};
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
