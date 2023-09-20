#include"Data.h"
#include<vector>

void PrintIteration(Data data, std::ofstream& in, int i) {
    in << i << " " << data.x1 <<  " " << data.x2 << " " << data.f1 << " " << data.f2 << " " << data.left << " " << 
    data.right << " " << data.right - data.left << " " << data.temp / (data.right-data.left) << std::endl; 
} 

void Dichotomy() {
    Data data;
    int i = 0;
    std::ofstream in("result/Dichotomy.txt");
    while (fabs(data.left - data.right) > (2*data.eps)) {
        data.x1 = (data.left + data.right - data.delta) / 2.0;
        data.x2 = (data.left + data.right + data.delta) / 2.0;
        data.f1 = data.GetFunction(data.x1);
        data.f2 = data.GetFunction(data.x2);
        data.count += 2;
        i++;
        if (data.f1 > data.f2) {
            data.left = data.x1;
        } else {
            data.right = data.x2;
        }
        PrintIteration(data, in, i);
        data.temp = data.right - data.left;
    }
    in.close();
    std::cout << data.count << std::endl;
}

void GoldenSech() {
    double num = 0.381966011;
    int i = 0;
    Data data;
    data.x1 = data.left + num *(data.right - data.left);
    data.x2 = data.right - num *(data.right - data.left);
    data.f1 = data.GetFunction(data.x1);
    data.f2 = data.GetFunction(data.x2);
    data.count += 2;
    std::ofstream in("result/GoldenSech.txt");
    while (fabs(data.right - data.left) > data.eps) {
        if (data.f1 < data.f2) {
            data.right = data.x2;
            data.x2 = data.x1;
            data.x1 = data.left + num *(data.right - data.left);
            data.f2 = data.f1;
            data.f1 = data.GetFunction(data.x1);
            data.count+=2;
        } else {
            data.left = data.x1;
            data.x1 = data.x2;
            data.x2 = data.right - num*(data.right - data.left);
            data.f1 = data.f2;
            data.f2 = data.GetFunction(data.x2);
            data.count+=2;
        }
        i++;
        PrintIteration(data, in, i);
        data.temp = data.right - data.left;
    }
    in.close();
    std::cout << data.count << std::endl;
}

void Fibbonacci() {
    std::vector<double>NumFib = {1, 1, 2};
    Data data;
    int ind = 1;


    while ((data.right - data.left) / data.eps >= NumFib[ind + 1]) {
        NumFib.push_back(NumFib[ind] + NumFib[ind + 1]);
        ind++;
    }

    data.x1 = data.left + (NumFib[ind - 1]/NumFib[ind + 1]) * (data.right - data.left);
    data.x2 = data.right + (NumFib[ind]/NumFib[ind+1]) * (data.right-data.left);
    data.count+= 2;

    std::ofstream in("result/Fibonacci.txt");

    int i = 1;
    for (int j = 2; j <= ind; j++) {
        data.temp = data.right - data.left;
        if (data.f1 < data.f2) {
            data.right = data.x2;
            data.x2 = data.x1;
            data.x1 = data.left + NumFib[ind - j] / NumFib[ind-j+2] *(data.right - data.left);
            data.f2 = data.f1;
            data.f1 = data.GetFunction(data.x1);
            data.count++;
        } else {
            data.left = data.x1;
            data.x1 = data.x2;
            data.x2 = data.left + NumFib[ind - j + 1] / NumFib[ind-j+2] *(data.right - data.left);
            data.f1 = data.f2;
            data.f2 = data.GetFunction(data.x2);
            data.count++;
        }
        PrintIteration(data, in, i);
        data.temp = data.right - data.left;
        i++;
    }
    in.close();
    std::cout << data.count << std::endl;
}

int SearchMinimal(double& left, double& right, double delta) {
    bool flag = true;
    Data data;
    double x0 = left;
    data.count = 0;
    double h;
    std::ofstream out("result/SearchMinimal.txt");
    if (data.GetFunction(x0) >= data.GetFunction(x0+delta)) {
        data.x1 = x0 +delta;
        h = delta;
    } else if (data.GetFunction(x0) > data.GetFunction(x0 - delta)) {
        data.x1 = x0 - delta;
        h = -delta;
    }
    data.f1 = data.GetFunction(data.x1);
    while (true) {
        data.count++;
        h *= 2;
        data.x2 = data.x1 + h;
        data.f2 = data.GetFunction(data.x2);
        if ( data.f1 > data.f2 ) {
            x0 = data.x1;
            data.x1 = data.x2;
            data.f1 = data.f2;
        } else {
            out << data.count << " " << data.GetFunction(data.x2) << std::endl;
            break;
        }
        out << data.count << " " << data.x1 << " " << data.GetFunction(data.x1) << std::endl;
    }
    if (x0 > data.x2) {
        left = data.x2;
        right = x0;
    } else {
        left = x0;
        right = data.x2;
    }
    out.close();
    return data.count;
}

int main() {

    int c;
    std::cout << "enter number method" << std::endl;
    std::cout << "1 - dihotomy" << std::endl;
    std::cout << "2 - golden sech" << std::endl;
    std::cout << "3 - fibonacci" << std::endl;
    std::cout << "4 - function search minimal" << std::endl;
    std::cin >> c;
    switch(c)
    {
    case(1):
        Dichotomy();
        break;
    case(2):
        GoldenSech();
        break;
    case(3):
        Fibbonacci();
        break;
    case(4):
        double x1; double x2;
        double delt;
        std::cout << "enter interval" << std::endl;
        std::cin >> x1;
        std::cin >> x2;
        std::cout << "enter delta" << std::endl;
        std::cin >> delt;
        std::cout << SearchMinimal(x1, x2,  delt) << std::endl;
        std::cout << "left: " << x1 << std::endl;
        std::cout << "right: " << x2 << std::endl;
        break;
    default:
        break;
    }
    //Dichotomy();
    //GoldenSech();
    //Fibbonacci();
    return 0;
}