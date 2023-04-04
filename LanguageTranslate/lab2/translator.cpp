#include"translator.h"
#include<string>

Translator::Translator() {
    word.LoadTable("./table/words.txt"); // t1
    separators.LoadTable("./table/separators.txt"); // t2
    operat.LoadTable("./table/operat.txt"); // t3
    numbers.LoadTable("./table/numbers.txt"); // t4
    letters.LoadTable("./table/letters.txt"); // t5
}

bool Translator::StateMachine(std::string code, std::string errors, std::string tokens) {
    Source.open(code);
    TokenLog.open(tokens);
    ErrorLog.open(errors);
    std::string str;
    bool notError = true;
    while(!Source.eof() && notError) {
        Source >> str;
        notError = AnalysisStr(str);
    }

    return notError;
}

bool Translator::AnalysisStr(std::string str) {
    bool notError = true;
    if (str.size() != 0) {
        std::string symbol;
        std::string next;
        symbol = str[0];
        bool flag = true;
        int check = AnalusisSym(symbol);
        if (check == 1) {
            CheckIdent(str, flag);
        }
    }
}

int Translator::AnalusisSym(std::string symbol) {
    if (letters.CheckInTable(symbol)) {
        return 1;
    } else if(separators.CheckInTable(symbol)) {
        return 2;
    } else if(operat.CheckInTable(symbol)) {
        return 3;
    } else if(numbers.CheckInTable(symbol)) {
        return 4;
    } else {
        return 0;
    }
}

void Translator::CheckIdent(std::string str, bool& flag) {
    std::string symbol;
    std::string next;
    int i;
    for (i = 1; i < str.size() && flag; i++) {
        symbol = str[i];
        flag = (letters.CheckInTable(symbol) || numbers.CheckInTable(symbol));
    }
    if (!flag)
        i--;
    next = str.substr(i);
    str = str.substr(0, i);
    int pos = word.FindInTable(str);
    if (pos != -1) {
        // добавляем токен (1, pos)
    } else {
        Ident.insert(str, 0, 0);
        // добавляем токент
    }
    flag = AnalysisStr(next);
}

