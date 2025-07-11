#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <string_view>

std::string readFileContent(const std::string &path) {
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        std::cerr << "Не удалось открыть файл: '" << path << "'" << std::endl;
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(inputFile)),
                       std::istreambuf_iterator<char>());
}

long long parseInteger(std::string_view s) {
    std::string str(s);

    size_t start = 0;
    if (str[0] == '+' || str[0] == '-') {
        start = 1;
    }

    if (str.length() > start + 2) {
        if (str.substr(start, 2) == "0b" || str.substr(start, 2) == "0B") {
            std::string binary_part = (start > 0) ? str.substr(start + 2) : str.substr(2);
            long long val = std::stoll(binary_part, nullptr, 2);
            return (str[0] == '-') ? -val : val;
        }
    }

    return std::stoll(str, nullptr, 0);
}


int main() {
    const std::string filename = "/Users/kirill_smychok/CLionProjects/task7/input.txt";

    std::string content = readFileContent(filename);
    if (content.empty()) {
        return 1;
    }

    long long integer_sum = 0;
    double double_sum = 0.0;
    std::vector<std::string> found_strings;


    // --- ЭТАП 1: ИЗВЛЕЧЕНИЕ СТРОК ---
    const std::regex string_regex(R"(\"([^\"]*)\")");

    auto str_begin = std::sregex_iterator(content.begin(), content.end(), string_regex);
    auto str_end = std::sregex_iterator();

    for (std::sregex_iterator i = str_begin; i != str_end; ++i) {
        found_strings.push_back((*i).str());
    }

    std::string content_no_strings = std::regex_replace(content, string_regex, " ");

    // --- ЭТАП 2: ИЗВЛЕЧЕНИЕ ДЕЙСТВИТЕЛЬНЫХ ЧИСЕЛ ---
    const std::regex float_regex(R"((?:^|\s)([+-]?(?:(?:[0-9]+\.[0-9]*|\.[0-9]+)(?:[eE][+-]?[0-9]+)?|[0-9]+[eE][+-]?[0-9]+))(?=\s|$))");

    auto float_begin = std::sregex_iterator(content_no_strings.begin(), content_no_strings.end(), float_regex);
    auto float_end = std::sregex_iterator();

    for (std::sregex_iterator i = float_begin; i != float_end; ++i) {
        double_sum += std::stod((*i)[1].str());  // Берём первую группу, так как основное число в группе 1
    }

    std::string content_no_strings_or_floats = std::regex_replace(content_no_strings, float_regex, " ");


    // --- ЭТАП 3: ИЗВЛЕЧЕНИЕ ЦЕЛЫХ ЧИСЕЛ ---
    const std::regex int_regex(R"((?:^|\s)([+-]?(?:0[xX][0-9a-fA-F]+|0[bB][01]+|0[0-7]*|[1-9][0-9]*))(?=\s|$))");

    auto int_begin = std::sregex_iterator(content_no_strings_or_floats.begin(), content_no_strings_or_floats.end(),
                                          int_regex);
    auto int_end = std::sregex_iterator();

    for (std::sregex_iterator i = int_begin; i != int_end; ++i) {
        integer_sum += parseInteger((*i)[1].str());  // Берём первую группу
    }
    std::cout << "--- Результаты ---" << std::endl;

    std::cout << "Сумма целых чисел: " << integer_sum << std::endl;
    std::cout << "Сумма действительных чисел: " << double_sum + integer_sum << std::endl;

    std::cout << "Найденные строки:" << std::endl;
    if (found_strings.empty()) {
        std::cout << "(Строки не найдены)" << std::endl;
    } else {
        for (const auto &s: found_strings) {
            std::cout << "- " << s << std::endl;
        }
    }

    return 0;
}