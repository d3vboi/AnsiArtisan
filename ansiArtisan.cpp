#include <iostream>
#include <vector>
#include <string>
#include <tabulate/table.hpp>

using namespace tabulate;

void addColorRow(Table& table, const std::vector<std::string>& codes, const std::vector<std::string>& colors, const std::string& type) {
    Table::Row_t row_data;
    row_data.push_back(type);

    for (size_t i = 0; i < codes.size(); ++i) {
        std::string color_code = "\e[" + codes[i] + "m";
        std::string reset_code = "\e[0m";

        // Adjust text color to black for bright backgrounds (codes ending in 0-7)
        if (codes[i].find("10") == 1 || codes[i].find("10") == 2) {
            color_code = "\e[1;90;" + codes[i].substr(1) + "m";  // Set text to black for bright backgrounds
        }

        row_data.push_back(color_code + colors[i] + " (" + codes[i] + ")" + reset_code);
    }

    table.add_row(row_data);
}

int main() {
    Table table;

    // Setting up table properties
    table.format()
        .font_style({FontStyle::bold})
        .border_top("")
        .border_bottom("")
        .border_left("")
        .border_right("")
        .column_separator("");

    // Header row
    table.add_row({"Color Type", "Black", "Red", "Green", "Yellow", "Blue", "Purple", "Cyan", "White"});

    // Colors and Codes
    const std::vector<std::string> colors = {"Black", "Red", "Green", "Yellow", "Blue", "Purple", "Cyan", "White"};
    const std::vector<std::string> regularCodes = {"0;30", "0;31", "0;32", "0;33", "0;34", "0;35", "0;36", "0;37"};
    const std::vector<std::string> boldCodes = {"1;30", "1;31", "1;32", "1;33", "1;34", "1;35", "1;36", "1;37"};
    const std::vector<std::string> underlineCodes = {"4;30", "4;31", "4;32", "4;33", "4;34", "4;35", "4;36", "4;37"};
    const std::vector<std::string> backgroundCodes = {"40", "41", "42", "43", "44", "45", "46", "47"};
    const std::vector<std::string> highIntensityCodes = {"0;90", "0;91", "0;92", "0;93", "0;94", "0;95", "0;96", "0;97"};
    const std::vector<std::string> boldHighIntensityCodes = {"1;90", "1;91", "1;92", "1;93", "1;94", "1;95", "1;96", "1;97"};
    const std::vector<std::string> highIntensityBackgroundCodes = {"0;100", "0;101", "0;102", "0;103", "0;104", "0;105", "0;106", "0;107"};

    // Adding rows for each color type
    addColorRow(table, regularCodes, colors, "Regular");
    addColorRow(table, boldCodes, colors, "Bold");
    addColorRow(table, underlineCodes, colors, "Underline");
    addColorRow(table, backgroundCodes, colors, "Background");
    addColorRow(table, highIntensityCodes, colors, "High Intensity");
    addColorRow(table, boldHighIntensityCodes, colors, "Bold High Intensity");
    addColorRow(table, highIntensityBackgroundCodes, colors, "High Intensity Background");

    // Print the table
    std::cout << table << std::endl;

    return 0;
}
