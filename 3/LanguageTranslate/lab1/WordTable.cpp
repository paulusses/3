#include "WordTable.h"

void WordTable::ShowTable() {
    	for (int i = 0; i < words.size(); i++) {
    		std::cout << words[i] << std::endl;
    	}
}

bool WordTable::LoadTable(std::string file) {
    std::ifstream in(file);
    while (!in.eof()) {
        std::string line;
        in >> line;
            if (CheckInTable(line)) {
                return false;
            } else {
                words.push_back(line);
            }
        }
        in.close();
        return true;
}

bool WordTable::CheckInTable(std::string word) {
    auto it = std::find(words.begin(), words.end(), word);
    if (it != words.end()) {
        return true;
    } else {
        return false;
    }
}

int WordTable::FindInTable(std::string word) {
    //std::cout << words.size();
    for (int i = 0; i < words.size(); i++) {
        if (word.compare(words[i]) == 0) {
            return i;
        }
    }
    return -1;
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
