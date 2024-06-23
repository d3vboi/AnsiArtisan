#include <iostream>
#include <vector>
#include <string>

void addColorRow(std::vector<std::vector<std::string>>& table, const std::vector<std::string>& codes, const std::string& type, bool isBackground) {
    std::vector<std::string> row_data;
    row_data.push_back(type);

    for (const auto& code : codes) {
        std::string color_code = "\e[" + code + "m";
        std::string reset_code = "\e[0m";

        if (isBackground) {
            if (code == "40" || code == "0;100" || code == "0;101" || code == "41" || code == "45") {
                color_code = "\e[" + code + ";97m";
            } else {
                color_code = "\e[" + code + ";90m";
            }
        } else if (code == "0;30" || code == "1;30" || code == "4;30") {
            color_code = "\e[" + code + ";100m";
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

int main() {
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

    return 0;
}
