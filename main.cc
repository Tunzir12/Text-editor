#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>

void performOperation(const std::string& flag, const std::string& parameter, std::vector<std::string>& text, const std::string& filename) {

    if (flag == "--print") {
        std::for_each(text.begin(), text.end(), [](const auto& word) {
            std::cout << word << ' ';
        });
        std::cout << std::endl;
    }
    else if (flag == "--frequency") {
        std::map<std::string, int> frequencyTable;
    std::for_each(text.begin(), text.end(), [&frequencyTable](const auto& word) {
        frequencyTable[word]++;
    });

    // Convert map to vector of pairs
    std::vector<std::pair<std::string, int>> sortedFrequency(
        frequencyTable.begin(), frequencyTable.end());

    // Sort vector based on frequency
    std::sort(sortedFrequency.begin(), sortedFrequency.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs.second > rhs.second;
              });

    std::cout << std::setw(15) << std::left << "Word" << std::setw(10) << "Frequency" << std::endl;
    std::cout << std::string(25, '-') << std::endl;

    std::for_each(sortedFrequency.begin(), sortedFrequency.end(), [](const auto& entry) {
        std::cout << std::setw(15) << std::left << entry.first
                  << std::setw(10) << entry.second << std::endl;
    });
    } 
    else if (flag == "--table") {
        std::map<std::string, int> frequencyTable;
        std::for_each(text.begin(), text.end(), [&frequencyTable](const auto& word) {
            frequencyTable[word]++;
        });

        std::cout << std::setw(15) << std::left << "Word" << std::setw(10) << "Frequency" << std::endl;
        std::cout << std::string(25, '-') << std::endl;

        std::for_each(frequencyTable.begin(), frequencyTable.end(), [](const auto& entry) {
            std::cout << std::setw(15) << std::left << entry.first
                      << std::setw(10) << entry.second << std::endl;
        });
    } 
    else if (flag == "--substitute") {
        size_t plusPos = parameter.find('+');
        std::string oldStr = (plusPos != std::string::npos) ? parameter.substr(0, plusPos) : parameter;
        std::string newStr = (plusPos != std::string::npos) ? parameter.substr(plusPos + 1) : "";

        std::for_each(text.begin(), text.end(), [&oldStr, &newStr](auto& word) {
            size_t found = 0;
            while ((found = word.find(oldStr, found)) != std::string::npos) {
                if ((found == 0 || !std::isalnum(word[found - 1])) &&
                    (found + oldStr.length() == word.length() || !std::isalnum(word[found + oldStr.length()]))) {
                    word.replace(found, oldStr.length(), newStr);
                    found += newStr.length();
                } else {
                    found++;
                }
            }
        });

        std::ofstream outFile(filename);
        std::for_each(text.begin(), text.end(), [&outFile](const auto& word) {
            outFile << word << ' ';
        });
    } 
    else if (flag == "--remove") {
        std::string wordToRemove = parameter;
        text.erase(std::remove_if(text.begin(), text.end(), [&wordToRemove](const auto& word) {
            return word == wordToRemove;
        }), text.end());

        std::ofstream outFile(filename);
        std::for_each(text.begin(), text.end(), [&outFile](const auto& word) {
            outFile << word << ' ';
        });
    } else {
        std::cerr << "Unknown flag: " << flag << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Check if there are at least two arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> [additional arguments...]" << std::endl;
        return 1; // Exit with an error code
    }

    // Extract the filename from the command-line arguments
    const std::string filename = argv[1];

    // Open the file
    std::ifstream file(filename);

    // Check if the file is successfully opened
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1; // Exit with an error code
    }

    // Check if the file is empty
    if (file.peek() == std::ifstream::traits_type::eof()) {
    std::cerr << "Error: File is empty." << std::endl;
    return 1; // Exit with an error code
    }
    
    std::cout << "File opened successfully: " << filename << std::endl;

    // Create a vector to store the remaining command-line arguments
    std::vector<std::string> arguments;

    // Copy the remaining arguments to the vector 
    std::copy(argv + 2, argv + argc, std::back_inserter(arguments));

    // Create a vector to store the words from the file
    std::vector<std::string> text;

    // Read words from the file and store them in the vector
    std::copy(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>(), std::back_inserter(text));


    // Process each argument using std::for_each and a lambda function
    std::for_each(arguments.begin(), arguments.end(), [&](const auto& arg) {
    // Find the first occurrence of '=' in the argument
    size_t pos = arg.find('=');

    // Split the argument into flag and parameter
    std::string flag = (pos != std::string::npos) ? arg.substr(0, pos) : arg;
    std::string parameter = (pos != std::string::npos) ? arg.substr(pos + 1) : "";

    // Perform the operation based on the flag and parameter
    performOperation(flag, parameter, text, filename);
 });

    // Exit the program successfully
    return 0;
}
