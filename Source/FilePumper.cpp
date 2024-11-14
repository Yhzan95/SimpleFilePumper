///////////////////////////////////
// Simple File Pumper By Yzhan95 //
// @ https://github.com/Yhzan95  //
///////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <windows.h>
#include <commdlg.h>

using namespace std;

// Console Related
#pragma region ConsoleArt

enum ConsoleColor {
    White = 15,
    Blue = 9,
    Red = 12,
    Green = 10
};

void SetConsoleColor(ConsoleColor color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void PrintLine(const wstring& text, ConsoleColor color) {
    SetConsoleColor(color);
    wcout << text << endl;
    SetConsoleColor(White); // Reset to default color
}
#pragma endregion

// Add bytes function
void AddBytesToFile(const wstring& filePath, int additionalBytes) {
    try {
        ofstream file(filePath, ios::binary | ios::app);
        if (!file.is_open()) {
            throw runtime_error("Error! Can't open the target file.");
        }
        file.seekp(0, ios::end); // Move the write position cursor to the end of the file
        for (int i = 0; i < additionalBytes; ++i) {
            file.put(0); // Add a null byte
        }
        file.close();
        SetConsoleColor(Green);
        wcout << L"\nDone! Bytes have been successfully added to the file." << endl;
        SetConsoleColor(White); // Reset to default color
    }
    catch (const exception& e) {
        wcerr << L"\nError: " << e.what() << endl;
    }
}

// Open file dialog to select target file
wstring OpenFileDialog() {
    OPENFILENAMEW ofn;
    wchar_t filePath[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = sizeof(filePath);
    ofn.lpstrFilter = L"Executable Files\0*.exe\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        return wstring(filePath);
    }
    else {
        throw runtime_error("No file selected or an error occurred.");
    }
}

// Print Console
int main() {
    PrintLine(L"______ _ _     ______                                          ", Green);
    PrintLine(L"|  ___(_) |    | ___ \\                                         ", Green);
    PrintLine(L"| |_   _| | ___| |_/ /   _ _ __ ___  _ __   ___ _ __           ", Green);
    PrintLine(L"|  _| | | |/ _ \\  __/ | | | '_ ` _ \\| '_ \\ / _ \\ '__|         ", Green);
    PrintLine(L"| |   | | |  __/ |  | |_| | | | | | | |_) |  __/ |            ", Green);
    PrintLine(L"\\_|   |_|_|\\___\\_|   \\__,_|_| |_| |_| .__/ \\___|_|           ", Green);
    PrintLine(L"                                    | |                       ", Green);
    PrintLine(L"                                    |_| By Yzhan95                       ", Green);
    wcout << endl;

    wstring filePath;
    try {
        filePath = OpenFileDialog(); // Select file
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    wcout << L"Choose the unit (1 - Bytes, 2 - Kilobytes, 3 - Megabytes): "; // Choose unit
    int unitChoice;
    wcin >> unitChoice;

    // Validate unit choice
    if (unitChoice < 1 || unitChoice > 3) {
        cerr << "Invalid choice of unit!" << endl;
        return 1;
    }

    wcout << L"Enter the number of units to add: "; // Number of byte to add
    int numberOfUnits;
    wcin >> numberOfUnits;

    // Validate number of units
    if (numberOfUnits < 0) {
        cerr << "Number of units must be a positive integer!" << endl;
        return 1;
    }

    int additionalBytes = 0;
    switch (unitChoice) {
    case 1:
        additionalBytes = numberOfUnits;
        break;
    case 2:
        additionalBytes = numberOfUnits * 1024;
        break;
    case 3:
        additionalBytes = numberOfUnits * 1024 * 1024;
        break;
    }

    AddBytesToFile(filePath, additionalBytes);
    return 0;
}
