#ifndef WORDTABLE_H
#define WORDTABLE_H

#include <fstream>
#include <iostream>
#include <vector>
#include<algorithm>

class WordTable {
    public:

    bool LoadTable(std::string file); // загрузить таблицу в файл

    bool CheckInTable(std::string word);  // найти в таблице

    bool insert(std::string word); //  ввести слово

    int FindInTable(std::string word);

    bool SaveTable(std::string file);  // сохранить таблицу в файл
    
    void ShowTable();

    private:

    std::vector<std::string> words;
};

#endif // WORDTABLE_H
