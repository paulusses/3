#include"../lab1/WordTable.h"
#include"../lab1/ConstTable.h"
#include"../lab1/IdentificatorTable.h"



class Translator {
    public:
    
    Translator();
    bool StateMachine(std::string code, std::string errors, std::string tokens);

    private:

    WordTable word; // ключевые слова 1
    WordTable separators; // разделители 2
    WordTable assignment; // присваивание 3
    WordTable operat; //  +, -, ==, != ...... 4 
    WordTable numbers;
    WordTable letters; 

    IdentificatorTable Ident; // переменные t7
    ConstantTable<char> const_char; // константы чар; t8
    ConstantTable<int> const_int; // константы инт t9
    ConstantTable<float> const_float; // константы вещественные t10

    std::ofstream ErrorLog;
    std::ofstream TokenLog;
    std::ifstream Source;

    bool AnalysisStr(std::string str);
    int AnalusisSym(std::string symbol);
    void AddToken(int t, int p, std::ofstream ifs);


};