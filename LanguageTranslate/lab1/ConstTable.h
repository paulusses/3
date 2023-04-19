
#include <iostream>
#include <vector>
#include<fstream>

// Шаблонный класс для построения таблицы констант(тип, значение).
// 0 -void, 1 - int, 2 - char, 3 - float


template<typename T>
class ConstantTable {
public:
    void insert(int type, T value) { // функция добавления в таблицу
        table.push_back(std::make_pair(type, value));
    }

    bool Search(T value) { // функция поиска константы по значению
        for (auto& row: table) {
            if (row.second == value) {
                return true;
            }
        }
        return false;
    }

    int CheckInTable(T value) {
        for (int i = 0; i < table.size();i++) {
            if (table[i].second == value) {
                return i;
            }
        }
        return -1;
    }
    bool ChangeValue(T value, T NewValue) { //  функция изменения значения, поиск по значению
        for (auto& row: table) {
            if (row.second == value) {
                row.second = NewValue;
                return true;
            }
        }
        return false;
    } 

    bool ChangeType(T value, int type) { //  функция изменения типа, поиск по значению
        if (type >= 0 && type <= 3) {
            for (auto& row: table) {
                if (row.second == value) {
                    row.first = type;
                    return true;
                }
            }
            return false;
        } else {
            return false;
        }
    }

    bool PrintTable(std::string file) { // функция вывода таблицы в файл
        std::ofstream out(file);
        if(out.is_open()) {
            for (auto row: table) {
                out << "TYPE: " << row.first << "| VALUE: " << row.second << std::endl;
            }
            out.close();
            return true;
        } else {
            return false;
        }
    }

private:
    std::vector<std::pair<int, T>> table; //  пара (тип, значение)
};

