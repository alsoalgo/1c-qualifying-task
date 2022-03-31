#include "include/all.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>


int main() {
    std::string filename;
    std::cin >> filename;
    seeker s(filename);
    std::string substring;
    while (std::cin >> substring) {
        cout << s.seek(substring) << "\n";
        fflush(stdout);
    }
    return 0;
} 