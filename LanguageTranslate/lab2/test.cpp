#include<fstream>
#include<string>
#include<iostream>

int main() {
    std::ifstream in("a.txt");
    int i = 0;
    while(!in.eof()) {
        std::string s;
        getline(in, s); 
        std::string tmp = s;
        std::cout << std::endl << i;
        int a = s.find_first_of(' ');
        while (tmp != "" && a != -1) {
            tmp.erase(0, a+1);
            s.substr(a);
            std::cout << tmp;
            a = s.find_first_of(' ');
            tmp = s;
        }
        i++;
    }
    return 0;
}