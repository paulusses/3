#include"translator.h"
#include<string>
#include<sstream>



void Translator::AddToken(int t, int p, std::ofstream ifs) {
    ifs << "(" << t << ", " << p << ")" << std::endl;;
}

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
    std::string tmp;
    bool Error = false;
    int line = 1;
    while(!Source.eof() && !Error) {
        getline(Source, str);
        std::stringstream ss(str);
        while(ss >> tmp) {
            Error = !(AnalysisStr(tmp));
            if (Error) {
                ErrorLog << "Error: in string " << line << std::endl;
            }
        }
        line++;
    }
    Source.close();
    TokenLog.close();
    ErrorLog.close();
    return !Error;
}

bool Translator::AnalysisStr(std::string str) {
    if (str.size() != 0) {
        bool Error = false;
        std::string symbol;
        std::string next;
        std::string thisstr = str;
        symbol = str[0];
        bool flag = false;
        
        int check = AnalusisSym(symbol);
        switch (check)
        {
        case 1: {
            int i;
            for (i = 1; i < str.size() && !flag; i++) {
                symbol = str[i];
                flag = (!letters.CheckInTable(symbol) || numbers.CheckInTable(symbol));
            }
            if (flag)
                i--;
            next = str.substr(i);
            str = str.substr(0, i);
            int pos = word.FindInTable(str);
            if (pos != -1) {
                TokenLog << "(" << 1 << ", " << pos << ")" << std::endl;;
            } else {
                Ident.insert(str, 0, 0);
                TokenLog << "(" << 7 << ", " << Ident.CheckInTable(str) << ")" << std::endl;;
            }
            flag = !AnalysisStr(next);
        }
            break;
        
        case 2: {
            int i;
            std::string symbol;
            for (i = 1; i < str.size() && !flag; i++) {
                symbol = str[i];
                flag = !(numbers.CheckInTable(symbol) || symbol == ".");
            }
            if (flag)
                i--;
            flag = false;
            next = str.substr(i);
            str = str.substr(0, i);
            int points = str.find_last_of('.') - str.find_first_of('.');
            if (points != 0) {
                flag = true;
            }
            if (!flag) {
                size_t pos = str.find(".");
                if (pos != std::string::npos) {
                    float tmp = std::stod(str);
                    const_float.insert(3, tmp);
                    TokenLog << "(" << 10 << ", " << const_float.CheckInTable(tmp) << ")" << std::endl;;
                    flag = !AnalysisStr(next);
                } else {
                    int tmp = std::stoi(str);
                    const_int.insert(2, tmp);
                    TokenLog << "(" << 9 << ", " << const_int.CheckInTable(tmp) << ")" << std::endl;;
                    flag = !AnalysisStr(next);
                }
            } 
        }
            break;

        case 3: {
            if (str[0] != '/') {
            int i;
            std::string oper = str.substr(0, 2);
            flag = operat.FindInTable(oper);
            int funct;
            if (flag == false) {
                oper =  str.substr(0,1);
                flag = operat.FindInTable(oper);
                funct = 1;
            } else {
                if (str.size() == 1) {
                    funct = 1;
                } else {
                    funct = 2;
                }
            }
            if (flag) {
                int n = operat.FindInTable(oper);
                TokenLog << "(" << 4 << ", " << n << ")" << std::endl;;
            }
            } else {
                if (str.size() > 1) {
                    switch (str[1])
                    {
                    case '*': {
                        std::string comment;
                        bool end = false;
                        while(!end && !Source.eof()) {
                            Source >> comment;
                            int last = comment.find("*/");
                            if (last != -1) {
                                end = true;
                            }
                        }
                        if (!Source.eof()) {
                            int last = comment.find_first_of("*/");
                            next = comment.substr(last+2);
                            flag = !AnalysisStr(next);
                        } else {
                            flag = true;
                            ErrorLog << "Error: incorrect comment!" << std::endl;
                        }
                    }
                        break;
                    
                    case '/': {
                        std::string comment;
                        getline(Source, comment);
                    }
                        break;

                    default: {
                     ErrorLog << "Error: '/' dont close comment!" << std::endl;
                     flag = true;                     
                    }
                        break;
                    }
                }  else {
                    ErrorLog << "Error: '/' dont close comment!" << std::endl;
                    flag = true;
                }
            }   
        }

        case 4: {
            next = str.substr(1);
            symbol = str[0];
            int pos = separators.FindInTable(symbol);
            TokenLog << "(" << 2 << ", " << pos << ")" << std::endl;;
            flag = !AnalysisStr(next);
        }
            break;

        default:  {
            ErrorLog << "Error: what is firts symbol " << std::endl;
            Error = true;
        }
            break;
        };
        if (flag) {
            ErrorLog << "Error: in " << thisstr << std::endl;
        }
        return !(Error || flag);
    } else {
        return true;
    }
}

int Translator::AnalusisSym(std::string symbol) {
   // letters.ShowTable();
    if (letters.CheckInTable(symbol)) {
        return 1;
    } else if(separators.CheckInTable(symbol)) {
        return 4;
    } else if(operat.CheckInTable(symbol)) {
        return 3;
    } else if(numbers.CheckInTable(symbol)) {
        return 2;
    } else {
        return 0;
    }
}



int main() {
    Translator T;
    T.StateMachine("code.txt", "errors.txt", "tokens.txt");
    return 0;
}