#include <stdio.h>
#include <locale.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include<math.h>
using namespace std;

double x0, x1, x2; // координаты для формирования сетки
double yy0, yy1, yy2; // координаты для формирования сетки
double kx0, kx1, ky0, ky1; // коэффициенты разрядки
int nx0, nx1, ny0, ny1; // количество отрезков в каждой области
double hx0, hx1, hy0, hy1; // начальные шаги для каждой области
int n; // количество узлов

vector<vector<double>> grid; // сетка
vector<vector<double>> border; // массив содержаший заданное 1 или 2 краевое условие на границах и номера самих узлов
vector<double> b; // вектор правой части
vector<vector<double>> di; // матрица (7 диагоналей)

// для решения СЛАУ
double w = 0; // параметр релаксации
double e = 0; // точность
double maxiter = 0; // максимальное количество итераций
vector<double> xk;
vector<double> x00;// начальное приближение
vector<double> Ax;// для подсчета невязки 
vector<double> f_Ax;// для подсчета невязки

double u(double x, double y)
{
	return x*x;
}

double lambda(double x, double y)
{
	return 1;
}

double gamma(double x, double y)
{
	return 1;
}

double f(double x, double y)
{
	return x*x - 2;
}

double tetta(double x, double y)
{
	return 2*x;
}

// генерация сетки
void gridGeneration()
{
	if (kx0 != 1) hx0 = (x1 - x0) * (kx0 - 1) / (pow(kx0, nx0) - 1); // первый шаг по x для 0 и 1 областей
		else  hx0 = (x1 - x0) / nx0; // равномерная сетка
	if (ky0 != 1) hy0 = (yy1 - yy0) * (ky0 - 1) / (pow(ky0, ny0) - 1); // первый шаг по y для 0 области
		else  hy0 = (yy1 - yy0) / ny0; // равномерная сетка
	if (kx1 != 1) hx1 = (x2 - x1) * (kx1 - 1) / (pow(kx1, nx1) - 1); // первый шаг по x для 2 области
		else  hx1 = (x2 - x1) / nx1; // равномерная сетка
	if (ky1 != 1) hy1 = (yy2 - yy1) * (ky1 - 1) / (pow(ky1, ny1) - 1); // первый шаг по y для 1 и 2 областей
		else  hy1 = (yy2 - yy1) / ny1; // равномерная сетка

	n = (nx0 + 1) * (ny0 + 1) + (nx0 + 1) * (ny1 + 1) + (nx1 + 1) * (ny1 + 1) - (nx0 + 1) - (ny1 + 1); // отнимаем повторы
	grid.resize(n);
	for(int i = 0; i< n; i++)
		grid[i].resize(2); // номер узла - номера элемента в массиве, два числа - координаты узла

	double y = yy0;
	// узлы нулевой области
	for (int i = 0; i < ny0 + 1; i++)
	{
		double x = x0;
		for (int j = 0; j < nx0 + 1; j++)
		{
			grid[i * (nx0 + 1) + j][0] = x;
			grid[i * (nx0 + 1) + j][1] = y;
			x += hx0 * pow(kx0, j);
		}
		y += hy0;
		hy0 *= ky0;
	}

	y = yy1 + hy1;
	// узлы первой области
	for (int i = 0; i < ny1; i++)
	{
		double x = x0;
		for (int j = 0; j < nx0 + 1; j++)
		{
			grid[(nx0 + 1) * (ny0 + 1) + nx1 * (i + 1) + (nx0 + 1) * i + j][0] = x;
			grid[(nx0 + 1) * (ny0 + 1) + nx1 * (i + 1) + (nx0 + 1) * i + j][1] = y;
			x += hx0 * pow(kx0, j);
		}
		y += hy1 * pow(ky1, i + 1);
	}

	y = yy1;
	// узлы второй области
	for (int i = 0; i < ny1 + 1; i++)
	{
		double x = x1 + hx1;
		for (int j = 0; j < nx1; j++)
		{
			grid[(nx0 + 1) * (ny0 + 1) + (nx0 + 1) * i + nx1 * i + j][0] = x;
			grid[(nx0 + 1) * (ny0 + 1) + (nx0 + 1) * i + nx1 * i + j][1] = y;
			x += hx1 * pow(kx1, j + 1);
		}
		y += hy1 * pow(ky1, i);
	}
	// вывод сетки в файл
	ofstream grids("grid.txt");
	for (int i = 0; i < n; i++)
		grids << grid[i][0] << " " << grid[i][1] << endl;
}

// граничные узлы
void boundaryNodes()
{
	border.resize(6);
	ifstream input("border.txt");
	//задаем размер каждой грани + номер краевого
	border[0].resize(nx0 + 2);
	input >> border[0][0];
	border[1].resize(ny0 + 2);
	input >> border[1][0];
	border[2].resize(nx1 + 2);
	input >> border[2][0];
	border[3].resize(ny1 + 2);
	input >> border[3][0];
	border[4].resize(nx0 + nx1 + 2);
	input >> border[4][0];
	border[5].resize(ny0 + ny1 + 2);
	input >> border[5][0];

	// заполняем номерами узлов
	// первая грань
	for (int i = 1; i <= nx0 + 1; i++)
		border[0][i] = i - 1;
	// вторая грань
	for (int i = 1; i <= ny0 + 1; i++)
		border[1][i] = (i - 1) * (nx0 + 1) + nx0;
	// третья грань
	for (int i = 1; i <= nx1 + 1; i++)
		border[2][i] = (nx0 + 1) * (ny0 + 1) - 2 + i;
	// четвертая грань
	for (int i = 1; i <= ny1 + 1; i++)
		border[3][i] = (nx0 + 1) * (ny0 + 1) - 1 + nx1 + (i - 1) * (nx0 + nx1 + 1);
	// пятая грань
	for (int i = 1; i <= nx0 + nx1 + 1; i++)
		border[4][i] = n - i;
	// шестая грань
	int i = 1;
	for (; i <= ny0 + 1; i++)
		border[5][i] = (i -  1) * (nx0 + 1);
	for (int j = 1; i <= ny0 + ny1 + 1; i++, j++)
		border[5][i] = (ny0 + 1) * (nx0) + j * (nx0 + nx1 + 1);
}

// поиск узла среди граничных
int searchBoundary(int num)
{
	// проверяем каждую границу
	for (int i = 0; i < 6; i++) 
	{
		for (int j = 1; j < border[i].size(); j++) {
			if (border[i][j] == num)
				return i; // возвращаем номер границы
		}
	}
	return (-1); // узел не граничный 

}

// обработка регулярных узлов
void regularNodes(int num)
{
	// вычисляем шаги по х
	hx0 = grid[num][0] - grid[num - 1][0]; // левый шаг
	hx1 = grid[num + 1][0] - grid[num][0]; // правый шаг
	di[2][num - 1] = - 2 * lambda(grid[num][0], grid[num][1]) / (hx0 * (hx0 + hx1)); // u_i-1,j
	di[4][num] = - 2 * lambda(grid[num][0], grid[num][1]) / (hx1 * (hx0 + hx1)); // u_i+1,j

	// вычисляем шаги по y
	if (num < (nx0 + 1) * ny0) // принадлежит строго 0 области
	{
		hy0 = grid[num][1] - grid[num - nx0 - 1][1]; // нижний шаг
		hy1 = grid[num + nx0 + 1][1] - grid[num][1]; // верхний шаг
		di[1][num - nx0 - 1] = - 2 * lambda(grid[num][0], grid[num][1]) / (hy0 * (hy0 + hy1)); // u_i,j-1
		di[5][num] = - 2 * lambda(grid[num][0], grid[num][1]) / (hy1 * (hy0 + hy1)); // u_i,j+1
	}
	else if (num > (nx0 + 1) * (ny0 + 1) + nx1) // принадлежит строго 1 или 2 области
	{
		hy0 = grid[num][1] - grid[num - nx0 - nx1 - 1][1]; // нижний шаг
		hy1 = grid[num + nx0 + nx1 + 1][1] - grid[num][1]; // верхний шаг
		di[0][num - nx0 - nx1 - 1] = - 2 * lambda(grid[num][0], grid[num][1]) / (hy0 * (hy0 + hy1)); // u_i,j-1
		di[6][num] = - 2 * lambda(grid[num][0], grid[num][1]) / (hy1 * (hy0 + hy1)); // u_i,j+1
	}
	else // на стыке 0 и 1 области 
	{
		hy0 = grid[num][1] - grid[num - nx0 - 1][1]; // нижний шаг
		hy1 = grid[num + nx0 + nx1 + 1][1] - grid[num][1]; // верхний шаг
		di[1][num - nx0 - 1] = - 2 * lambda(grid[num][0], grid[num][1]) / (hy0 * (hy0 + hy1)); // u_i,j-1
		di[6][num] = - 2 * lambda(grid[num][0], grid[num][1]) / (hy1 * (hy0 + hy1)); // u_i,j+1
	}

	di[3][num] = 2 * lambda(grid[num][0], grid[num][1]) * ( 1 / (hx0 * hx1) + 1 / (hy0 * hy1)) + gamma(grid[num][0], grid[num][1]); // u_i,j
	b[num] = f(grid[num][0], grid[num][1]);
}

// учет краевых условий 
void borderNodes(int num, int edge)
{
	if (border[edge][0] == 1)// первые краевые
	{
		di[3][num] = 1;
		b[num] = u(grid[num][0], grid[num][1]);
	}
	else // вторые краевые
	{
		b[num] = tetta(grid[num][0], grid[num][1]);
		switch(edge)
		{
			// первая грань
			case 0:
				di[3][num] = lambda(grid[num][0], grid[num][1]) / (grid[num + nx0 + 1][1] - grid[num][1]);
				di[5][num] = -lambda(grid[num][0], grid[num][1]) / (grid[num + nx0 + 1][1] - grid[num][1]);
				break;
			// вторая грань
			case 1:
				di[3][num] = lambda(grid[num][0], grid[num][1]) / (grid[num][0] - grid[num - 1][0]);
				di[2][num - 1] = -lambda(grid[num][0], grid[num][1]) / (grid[num][0] - grid[num - 1][0]);
				break;
			// третья грань
			case 2:
				di[3][num] = lambda(grid[num][0], grid[num][1]) / (grid[num + +nx0 + nx1 + 1][1] - grid[num][1]);
				di[6][num] = -lambda(grid[num][0], grid[num][1]) / (grid[num + nx0 + nx1 + 1][1] - grid[num][1]);
				break;
			// четвертая грань
			case 3:
				di[3][num] = lambda(grid[num][0], grid[num][1]) / (grid[num][0] - grid[num - 1][0]);
				di[2][num - 1] = -lambda(grid[num][0], grid[num][1]) / (grid[num][0] - grid[num - 1][0]);
				break;
			// пятая грань
			case 4:
				di[3][num] = lambda(grid[num][0], grid[num][1]) / (grid[num][1] - grid[num - nx0 - nx1 - 1][1]);
				di[0][num - 1 - nx0 - nx1] = -lambda(grid[num][0], grid[num][1]) / (grid[num][1] - grid[num - nx0 - nx1 - 1][1]);
				break;
			// шестая грань
			case 5:
				di[3][num] = lambda(grid[num][0], grid[num][1]) / (grid[num + 1][0] - grid[num][0]);
				di[4][num] = -lambda(grid[num][0], grid[num][1]) / (grid[num + 1][0] - grid[num][0]);
				break;
		}		
	}
}

// сборка СЛАУ
void creationSLAU()
{
	b.resize(n);
	di.resize(7);
	di[0].resize(n - nx0 - nx1 - 1);
	di[1].resize(n - nx0 - 1);
	di[2].resize(n - 1);
	di[3].resize(n);
	di[4].resize(n - 1);
	di[5].resize(n - nx0 - 1);
	di[6].resize(n - nx0 - nx1 - 1);
	// зануляем матрицу
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < di[i].size(); j++)
			di[i][j] = 0;

	for (int i = 0; i < n; i++)
	{
		b[i] = 0;
		int edge;
		if (i < (nx0 + 1) * (ny0 + 1)) // 0 область
		{
			if(grid[i][0] != x0 && grid[i][1] != yy0 && grid[i][0] != x1) // если не на границе
				regularNodes(i); // обрабатываем как регулярный узел
			else // если на границе
			{
				edge = searchBoundary(i);
				borderNodes(i, edge); // обрабатываем как граничный узел
			}
		}
		else // 1 и 2 области
		{
			if(grid[i][0] != x2 && grid[i][1] != yy1 && grid[i][1] != yy2 && grid[i][0] != x0) // если не на границе
				regularNodes(i); // обрабатываем как регулярный узел
			else // если на границе
			{
				edge = searchBoundary(i);
				borderNodes(i, edge); // обрабатываем как граничный узел
			}
		}
	}
}

// далее идут функции для решения СЛАУ, взятые из 2 лабы ЧМ
void Iterations(vector<double>& xk, vector<double>& x00) 
{
	for (int i = 0; i < n; i++)
	{
		double sum = 0, a = 0;
		for (int j = 0; j < n; j++)
		{
			if (i == j) a = di[3][i]; // главная диагональ
			else
			{
				if (j - i == -nx0 - nx1 - 1) a = di[0][j]; // нижние диагонали
				else if (j - i == -nx0 -1) a = di[1][j];
				else if (j - i == -1) a = di[2][j];
				else if (j - i == 1) a = di[4][i];// верхние диагонали
				else if (j - i == nx0 + 1) a = di[5][i]; 
				else if (j - i == nx0 + nx1 + 1) a = di[6][i];
				else  a = 0;
			}
			sum += a * x00[j];
		}
		Ax[i] = sum;
		xk[i] = x00[i] + w / di[3][i] * (b[i] - sum);
	}
}

double Norm(vector<double>& y)
{
	double norma = 0;
	for (int i = 0; i < n; i++)
		norma += y[i] * y[i];
	return sqrt(norma);
}

void Jacobi()
{
	ifstream matrix("matrix.txt");
	matrix >> w >> e >> maxiter;
	if (w <= 0 || w > 1) return;
	if (maxiter <= 0) return;
	x00.resize(n);
	xk.resize(n);
	Ax.resize(n);
	f_Ax.resize(n);

	for (int i = 0; i < n; i++)
	{
		x00[i] = 0;
		xk[i] = x00[i];
	}

	for (int i = 1; i < maxiter; i++)
	{
		Iterations(xk, x00);
		for (int i = 0; i < n; i++)
		{
			f_Ax[i] = b[i] - Ax[i];
			x00[i] = xk[i];
		}

		double nevyazka;
		nevyazka = Norm(f_Ax) / Norm(b);
		if (nevyazka < e)
			return;
	}
}
// вывод
void Output(ofstream& out)
{
	for (int i = 0; i < n; i++)
		out << x00[i] << endl;
}

int main()
{
	ifstream input("input.txt");
	input >> x0 >> yy0 >> x1 >> yy1 >> x2 >> yy2;
	input >> kx0 >> kx1 >> ky0 >> ky1;
	input >> nx0 >> nx1 >> ny0 >> ny1;
	gridGeneration(); // генерация сетки
	boundaryNodes(); // границы
	creationSLAU(); // создание СЛАУ
	Jacobi(); // решение СЛАУ
	ofstream out("out.txt");
	Output(out);
}