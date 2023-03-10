#include "WordTable.h"

bool WordTable::LoadTable(std::string file) {
    std::ifstream in(file);
    if (in.is_open()) {
        std::string line;
        while(getline(in, line)) {
            if (CheckInTable(line)) {
                return false;
            } else {
                words.push_back(line);
            }
        }
        in.close();
        return true;
    } else {
        return false;
    }
}

bool WordTable::CheckInTable(std::string word) {
    auto it = std::find(words.begin(), words.end(), word);
    if (it != words.end()) {
        return true;
    } else {
        return false;
    }
}

bool WordTable::insert(std::string word) {
    if (!CheckInTable(word)) {
        words.push_back(word);
        return true;
    } else {
        return false;
    }
}

bool WordTable::SaveTable(std::string file) {
    std::ofstream out(file);
    if (out.is_open()) {
        for (auto elem: words) {
            out << elem << std::endl;
        }
        out.close();
        return true;
    } else {
        return false;
    }
}