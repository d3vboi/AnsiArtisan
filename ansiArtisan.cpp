#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <termios.h>
#include <unistd.h>

void addColorRow(std::vector<std::vector<std::string>>& table, const std::vector<std::string>& codes, const std::string& type, bool isBackground) {
    std::vector<std::string> row_data;
    row_data.push_back(type);

    for (const auto& code : codes) {
        std::string color_code = "\033[" + code + "m";
        std::string reset_code = "\033[0m";

        if (isBackground) {
            if (code == "40" || code == "0;100" || code == "0;101" || code == "41" || code == "45") {
                color_code = "\033[" + code + ";97m";
            } else {
                color_code = "\033[" + code + ";90m";
            }
        } else if (code == "0;30" || code == "1;30" || code == "4;30") {
            color_code = "\033[" + code + ";100m";
        }

        row_data.push_back(color_code + code + reset_code);
    }

    table.push_back(row_data);
}

void printTable(const std::vector<std::vector<std::string>>& table) {
    for (const auto& row : table) {
        for (const auto& cell : row) {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }
}

void displayTable() {
    std::vector<std::vector<std::string>> table;

    // Header row
    const std::vector<std::string> color_names = {"Color Type", "Black", "Red", "Green", "Yellow", "Blue", "Purple", "Cyan", "White"};
    table.push_back(color_names);

    const std::vector<std::string> regularCodes = {"0;30", "0;31", "0;32", "0;33", "0;34", "0;35", "0;36", "0;37"};
    const std::vector<std::string> boldCodes = {"1;30", "1;31", "1;32", "1;33", "1;34", "1;35", "1;36", "1;37"};
    const std::vector<std::string> underlineCodes = {"4;30", "4;31", "4;32", "4;33", "4;34", "4;35", "4;36", "4;37"};
    const std::vector<std::string> backgroundCodes = {"40", "41", "42", "43", "44", "45", "46", "47"};
    const std::vector<std::string> highIntensityCodes = {"0;90", "0;91", "0;92", "0;93", "0;94", "0;95", "0;96", "0;97"};
    const std::vector<std::string> boldHighIntensityCodes = {"1;90", "1;91", "1;92", "1;93", "1;94", "1;95", "1;96", "1;97"};
    const std::vector<std::string> highIntensityBackgroundCodes = {"0;100", "0;101", "0;102", "0;103", "0;104", "0;105", "0;106", "0;107"};

    addColorRow(table, regularCodes, "Regular\t", false);
    addColorRow(table, boldCodes, "Bold\t", false);
    addColorRow(table, underlineCodes, "Underline", false);
    addColorRow(table, backgroundCodes, "Background", true);
    addColorRow(table, highIntensityCodes, "High Intensity", false);
    addColorRow(table, boldHighIntensityCodes, "Bold High Int", false);
    addColorRow(table, highIntensityBackgroundCodes, "High Int Back", true);

    printTable(table);
    std::cout << "Example: " << "\033[1;31m\\033[1;31mThis Is Bold Red!\\033[0m\033[0m" << std::endl;
}

void setInputMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void resetInputMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void interactiveMode() {
    setInputMode();

    int fgColorIndex = 0;
    int bgColorIndex = 0;
    bool isBold = false;
    bool isFgHighIntensity = false;
    bool isBgHighIntensity = false;

    const std::vector<std::string> colorCodes = {"30", "31", "32", "33", "34", "35", "36", "37"};
    const std::vector<std::string> highIntensityColorCodes = {"90", "91", "92", "93", "94", "95", "96", "97"};
    const std::vector<std::string> backgroundColorCodes = {"40", "41", "42", "43", "44", "45", "46", "47"};
    const std::vector<std::string> highIntensityBackgroundColorCodes = {"100", "101", "102", "103", "104", "105", "106", "107"};

    while (true) {
        std::cout << "\033[H\033[J"; // Clear screen
        displayTable();

        std::string fgCode = isFgHighIntensity ? highIntensityColorCodes[fgColorIndex] : colorCodes[fgColorIndex];
        std::string bgCode = isBgHighIntensity ? highIntensityBackgroundColorCodes[bgColorIndex] : backgroundColorCodes[bgColorIndex];
        std::string boldCode = isBold ? "1" : "0";

        std::string ansiSequence = "\033[" + boldCode + ";" + fgCode + ";" + bgCode + "m";
        std::cout << "Current ANSI Sequence: " << ansiSequence << "\\033[" << boldCode << ";" << fgCode << ";" << bgCode << "m" << "\\033[0m" << std::endl;
        std::cout << ansiSequence << "Sample Text\033[0m" << std::endl;

        std::cout << "\nUse 'w'/'s' to change foreground color, 'a'/'d' to change background color.\n";
        std::cout << "'b' to toggle bold, 'f' to toggle foreground high intensity, 'g' to toggle background high intensity.\n";
        std::cout << "'q' to quit.\n";

        char c;
        read(STDIN_FILENO, &c, 1);

        if (c == 'w') {
            fgColorIndex = (fgColorIndex + 1) % colorCodes.size();
        } else if (c == 's') {
            fgColorIndex = (fgColorIndex - 1 + colorCodes.size()) % colorCodes.size();
        } else if (c == 'a') {
            bgColorIndex = (bgColorIndex - 1 + backgroundColorCodes.size()) % backgroundColorCodes.size();
        } else if (c == 'd') {
            bgColorIndex = (bgColorIndex + 1) % backgroundColorCodes.size();
        } else if (c == 'b') {
            isBold = !isBold;
        } else if (c == 'f') {
            isFgHighIntensity = !isFgHighIntensity;
        } else if (c == 'g') {
            isBgHighIntensity = !isBgHighIntensity;
        } else if (c == 'q') {
            break;
        }
    }

    resetInputMode();
}

int main(int argc, char *argv[]) {
    bool interactive = false;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i],"-i") == 0 || strcmp(argv[i], "--interactive") == 0) {
            interactive = true;
        }
    }
    if (interactive) {
        interactiveMode();
    } else {
        displayTable();
    }
    return 0;
}
