#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>
#include <sys/types.h>
 #include <sys/wait.h>



int my_exp(double x, double sigma) {
    FILE *fp;
    if ((fp = fopen("count.txt", "w+")) == NULL) {
        printf("Не удалось открыть файл\n");
        return 0;
    } 
    double result = 1.0;
    double term = 1.0;
    int n = 1;
    while (fabs(term) > 1e-10) {
        term *= -x*x/(2*sigma*sigma*n);
        result += term;
        n++;
    }
    fprintf(fp, "%lf", result);
    fclose(fp);
    return 1;
}

int result(double x, double sigma) {
    FILE *in;
    if ((in = fopen("count.txt", "r")) == NULL) {
        printf("Не удалось открыть файл\n");
        return 1;
    }
    double my_exp;
    fscanf(in, "%lf", &my_exp);
    fclose(in);
    printf("Result: %lf\n", (x/sigma*sigma)*my_exp);
    return 0;

}



int main() {
    int count = 0;
    pid_t pid;
    double x = 0; double sigma = 0;
    double EXP;
    printf("Введите значение X и значение SIGMA\n");
    scanf("%lf %lf", &x, &sigma);
    if (sigma <= 0) {
        printf("Значение sigma должно быть положительно!\n");
        return 2;
    }
    pid = fork();
    if (pid == -1) {
        printf("Ошибка при вызове fork()\n");
        return 1;
    } else if (pid == 0) { // дочерний процесс
        printf("Дочерний процесс выполняется\n");
        int c = my_exp(x, sigma);
        exit(c);
    } else {   // родительский процес 
        wait(&count);
        printf("Родительский процесс выполненяется\n");
        if (WEXITSTATUS(count) == 1) {
            if (result(x, sigma) == 1)
                printf("Ошибка подсчета\n");
        } else {
            printf("Ошибка подсчета\n");
        }
    }
    return 0;
}