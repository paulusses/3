#include<vector>
#include<iostream>
#include<math.h>
#include<fstream>

struct Data {
    double x1, x2;
    double left = -2;
    double right = 20;
    double eps = 1e-7;
    double GetFunction(double x) {
        return pow(x-1, 2);
    }
    double delta = 1e-9;
    double f1, f2;
    double temp = right - left;
    int count = 0;
};