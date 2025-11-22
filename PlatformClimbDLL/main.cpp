#include "main.h"

std::string GetFileContents(HANDLE file);
std::vector<std::string> TokeniseFileContent(std::string content);
std::vector<int> GetScores(std::vector<std::string> content);
int GetRecordScore(std::string record);
std::vector<std::string> GetNames(std::vector<std::string> content);

void DLL_EXPORT EnterLeaderboardRecord(std::string name, int score)
{
    for (int i = 0; i < name.size(); i++) {
        if (name[i] == ':')
            name[i] = ' ';
    }

    HANDLE file = CreateFile("Leaderboard.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE) {
        std::string fileContent = GetFileContents(file);
        std::vector<std::string> contentLines = TokeniseFileContent(fileContent);
        std::vector<int> scores = GetScores(contentLines);

        int lineToWriteIn = scores.size() + 1;
        for (int i = 0; i < scores.size(); i++) {
            if (score > scores[i]) {
                lineToWriteIn = i + 1;
                break;
            }
        }

        int indexToWriteIn = 0;
        for (int i = 0; i < lineToWriteIn; i++) {
            indexToWriteIn += contentLines[i].size();
        }

        SetFilePointer(file, indexToWriteIn, NULL, FILE_BEGIN);

        std::string record = name + ": " + std::to_string(score) + '\n';

        DWORD bytesWritten;
        WriteFile(file, record.c_str(), record.size(), &bytesWritten, NULL);

        int remainingFileSize = fileContent.size() - indexToWriteIn;
        WriteFile(
            file,
            fileContent.substr(indexToWriteIn, remainingFileSize).c_str(),
            remainingFileSize,
            &bytesWritten,
            NULL
        );

        CloseHandle(file);
    }
}

int DLL_EXPORT GetPersonalBest(std::string name)
{
    for (int i = 0; i < name.size(); i++) {
        if (name[i] == ':')
            name[i] = ' ';
    }

    HANDLE file = CreateFile("Leaderboard.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE) {
        std::string fileContent = GetFileContents(file);
        std::vector<std::string> contentLines = TokeniseFileContent(fileContent);
        std::vector<std::string> names = GetNames(contentLines);
        CloseHandle(file);

        for (int i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                return GetRecordScore(contentLines[i + 1]);
            }
        }
    }
    return -1;
}

int DLL_EXPORT GetHighscore()
{
    HANDLE file = CreateFile("Leaderboard.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE) {
        std::string fileContent = GetFileContents(file);
        std::vector<std::string> contentLines = TokeniseFileContent(fileContent);
        CloseHandle(file);

        if (contentLines.size() > 1)
            return GetRecordScore(contentLines[1]);
    }
    return -1;
}

std::string GetFileContents(HANDLE file)
{
    DWORD bytes_read = 0;
    int bufferSize = 2048;
    CHAR buffer[2048] = {0};
    std::string fileContent = "";
    while(ReadFile(file, buffer, bufferSize - 1, &bytes_read, NULL) && bytes_read > 0){
        fileContent += std::string(buffer);
    }
    if (fileContent[fileContent.size() - 1] != '\n') {
        fileContent += '\n';
        WriteFile(file, "\n", 1, NULL, NULL);
    }

    return fileContent;
}

std::vector<std::string> TokeniseFileContent(std::string content)
{
    std::vector<std::string> contentLines;
    std::string currentLine = "";
    int lineStart = 0;
    for (int i = 0; i < content.size(); i++) {
        if (content[i] == '\n') {
            contentLines.push_back(content.substr(lineStart, i + 1 - lineStart));
            lineStart = i + 1;
        }
    }
    return contentLines;
}

std::vector<int> GetScores(std::vector<std::string> content)
{
    std::vector<int> scores;

    for (int i = 1; i < content.size(); i++) {
        scores.push_back(GetRecordScore(content[i]));
    }

    return scores;
}

int GetRecordScore(std::string record)
{
    int startIndex = 0;
    while (record[startIndex] != ':')
        startIndex++;

    startIndex += 2;
    return std::stoi(record.substr(startIndex, (record.size() - startIndex - 1)));
}

std::vector<std::string> GetNames(std::vector<std::string> content)
{
    std::vector<std::string> names;

    for (int i = 1; i < content.size(); i++) {
        int endIndex = 0;
        while (content[i][endIndex] != ':')
            endIndex++;

        names.push_back(content[i].substr(0, endIndex));
    }

    return names;
}


extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
