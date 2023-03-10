
#include<iostream>
#include<vector>
#include<fstream>

enum type {
    Null,
    CHAR,
    INT,
    FLOAT
};

class Identificator { // класс идентификаторов 

    public:
    void set_name(std::string Fname);
    std::string get_name();

    void set_type(int ID_Type);
    int get_type();

    void set_value(bool assign);
    bool get_value();


    private:
        std::string name;
        int TYPE;
        bool value;

};


class IdentificatorTable {
    public:
    IdentificatorTable(int size);


    // ~IdentificatorTable() {
    //     delete[] table;
    // }
    int CheckInTable(const std::string name); // есть ли значение в таблице

    bool insert(const std::string& name, int t, bool val); // добавить строку в табоицу 

    void lookup(const std::string name);  

    int ChangeTypeWithIndex(int ind, int t); // поменять тип, поиск по индексу

    int ChangeTypeWithName(std::string name, int t); // поменять тип, поиск по имени

    int ChangeNameWithIndex(int ind, std::string name);  // поменять имя поиск по индексу

    int ChangeNameWithName(std::string nameold, std::string namenew); // поменять имя по имени

    int ChangeValueWithIndex(int ind, bool ass); // поменять значение по индексу 

    int ChangeValueWithName(std::string name, bool ass);  // поменять значение по имени

    void PrintAllTable(std::string file); // вывести всю таблицу в файл

    void PrintOnePoint(Identificator p); // вывести одну строку таблицы


    private:
        std::vector<Identificator> table;
};
