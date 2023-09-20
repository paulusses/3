#include "IdentificatorTable.h"
#include "WordTable.h"
#include "ConstTable.h"
#include<cassert>

void TestTableIdentificatorInsert() {
    IdentificatorTable Table(0);
    Table.insert("pasha", 0, 0);
    assert(Table.CheckInTable("pasha") == 0);
    Table.insert("katya", 1, 1);
    assert(Table.CheckInTable("katya") == 1);
    std::cout << "Test INSERT in table of identificatorr OK" << std::endl;
}

void TestTableIdentificatorChangeType() {
    IdentificatorTable Table(0);
    Table.insert("pasha", 1, 1);
    assert(Table.ChangeTypeWithIndex(0,2) == 1);
    assert(Table.ChangeTypeWithName("pasha", 3) == 1);
    std::cout << "Test CHANGE TYPE of identificator table OK" << std::endl;
}

void TestTableIdentificatorChangeName() {
    IdentificatorTable Table(0);
    Table.insert("pasha", 1, 0);
    Table.insert("katya", 0,0);
    assert(Table.ChangeNameWithIndex(0,"Daddy") == 1);
    assert(Table.ChangeNameWithName("katya", "catty") == 1);
    std::cout << "Test CHANGE NAME of identificator table OK" << std::endl;
}

void TestTableIdentificatorChangeValue() {
    IdentificatorTable Table(0);
    Table.insert("pasha", 1, 0);
    Table.insert("katya", 0,0);
    assert(Table.ChangeValueWithName("pasha", 1) == 1);
    assert(Table.ChangeValueWithIndex(1, 1) == 1);
    std::cout << "Test CHANGE VALUE of identificator table OK" << std::endl;
}

void TestTableIdentificatorPrintTable() {
    IdentificatorTable Table(0);
    Table.insert("pasha", 1, 0);
    Table.insert("katya", 0,0);
    Table.insert("cat", 2, 1);
    Table.insert("way", 3, 1);
    Table.insert("FOX", 1, 0);
    std::string file = "tests/table_ident.txt";
    Table.PrintAllTable(file);
    std::ifstream f(file);
    assert(f.is_open() == true);
    int flag = 1;
    if (f.peek() == EOF) flag = 0;
    assert(flag == 1);
    std::cout << "Test SAVE TABLE of identificator OK (check tests/table_ident.txt)" << std::endl;
}   


void TestTableWordInsert() {
    WordTable Table;
    assert(Table.insert("pasha") == true);
    assert(Table.insert("katya") == true);
    std::cout << std::endl << "Test INSERT TABLE WORDS OK" << std::endl;
}

void TestTableWordCheck() {
    WordTable Table;
    Table.insert("if");
    Table.insert("for");
    assert(Table.CheckInTable("if") == true);
    assert(Table.CheckInTable("for") == true);
    assert(Table.CheckInTable("f") == false);
    std::cout << "Test CHECK in WORD TABLE OK" << std::endl;
}

void TestTableWordLoadFile() {
    WordTable Table;
    assert(Table.LoadTable("in/test_word.txt")==true);
    std::cout << "Test LOAD from file(in/test_word.txt) WORD TABLE OK" << std::endl;
}

void TestTableWordSaveInFile() {
    WordTable Table;
    Table.insert("for");
    Table.insert("do");
    Table.insert("while");
    Table.insert("main");
    Table.insert("void");
    assert(Table.SaveTable("tests/test_wordTable.txt") == true);
    std::cout << "Test SAVE TABLE WORD in file OK(check tests/table_wordTable.txt)" << std::endl;
}

void TestTableConstInsertFloat() {
    ConstantTable<float> Table;
    Table.insert(3, 3.2);
    Table.insert(3, 4.2);
    assert(Table.Search(3.2) == true);
    assert(Table.Search(4.2) == true);
    std::cout << std::endl << "Test INSERT CONST FLOAT OK" << std::endl;
}

void TestTableConstInsertInt() {
    ConstantTable<int> Table;
    Table.insert(1, 3);
    Table.insert(1, 4);
    assert(Table.Search(3) == true);
    assert(Table.Search(4) == true);
    std::cout << "Test INSERT CONST INT OK" << std::endl;
}

void TestTableConstInsertChar() {
    ConstantTable<char> Table;
    Table.insert(2, 28);
    Table.insert(2, 'a');
    assert(Table.Search(28) == true);
    assert(Table.Search('a') == true);
    std::cout << "Test INSERT CONST CHAR OK" << std::endl;
}

void TestTableConstChangeTypeFloat() {
    ConstantTable<float> Table;
    Table.insert(3, 3.2);
    Table.insert(3, 4.2);
    assert(Table.ChangeType(3.2, 1) == true);
    assert(Table.ChangeType(4.2,2) == true);
    std::cout <<  "Test CHANGE TYPE CONST FLOAT OK" << std::endl;
}

void TestTableConstChangeTypeInt() {
    ConstantTable<int> Table;
    Table.insert(1, 3);
    Table.insert(1, 4);
    assert(Table.ChangeType(3, 0) == true);
    assert(Table.ChangeType(4, 2) == true);
    std::cout << "Test CHANGE TYPE  CONST INT OK" << std::endl;
}

void TestTableConstChangeTypeChar() {
    ConstantTable<char> Table;
    Table.insert(2, 28);
    Table.insert(2, 'a');
    assert(Table.ChangeType(28, 1) == true);
    assert(Table.ChangeType('a', 3) == true);
    std::cout << "Test CHANGE TYPE  CONST CHAR OK" << std::endl;
}


void TestTableConstChangeValueFloat() {
    ConstantTable<float> Table;
    Table.insert(3, 3.2);
    Table.insert(3, 4.2);
    assert(Table.ChangeValue(3.2, 1.2) == true);
    assert(Table.ChangeValue(4.2,2.3) == true);
    std::cout <<  "Test CHANGE VALUE CONST FLOAT OK" << std::endl;
}

void TestTableConstChangeValueInt() {
    ConstantTable<int> Table;
    Table.insert(1, 3);
    Table.insert(1, 4);
    assert(Table.ChangeValue(3, 5) == true);
    assert(Table.ChangeValue(4, 2) == true);
    std::cout << "Test CHANGE VALUE  CONST INT OK" << std::endl;
}

void TestTableConstChangeValueChar() {
    ConstantTable<char> Table;
    Table.insert(2, 28);
    Table.insert(2, 'a');
    assert(Table.ChangeValue(28, 's') == true);
    assert(Table.ChangeValue('a', 5) == true);
    std::cout << "Test CHANGE VALUE  CONST CHAR OK" << std::endl;
}

void TestTableConstSave() {
    ConstantTable<float> Table;
    Table.insert(3, 1.2);
    Table.insert(3, 8.29);
    Table.insert(3, 1.002);
    assert(Table.PrintTable("tests/test_const.txt") == true);
    std::cout << "Test SAVE TABLE IN FILE OK(check tests/test_const.txt)" << std::endl;
}




int main() {
    TestTableIdentificatorInsert();
    TestTableIdentificatorChangeType();
    TestTableIdentificatorChangeName();
    TestTableIdentificatorChangeValue();
    TestTableIdentificatorPrintTable();
    TestTableWordInsert();
    TestTableWordCheck();
    TestTableWordLoadFile();
    TestTableWordSaveInFile();
    TestTableConstInsertFloat();
    TestTableConstInsertInt();
    TestTableConstInsertChar();
    TestTableConstChangeTypeInt();
    TestTableConstChangeTypeFloat();
    TestTableConstChangeTypeChar();
    TestTableConstChangeValueInt();
    TestTableConstChangeValueFloat();
    TestTableConstChangeValueChar();
    TestTableConstSave();
    return 0;
}