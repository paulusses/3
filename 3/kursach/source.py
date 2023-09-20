# Инициализация пустого массива
setka = []
time = []

with open('data.txt', 'r') as file:
    lines = file.readlines()
    for line in lines:
        # Разделение строки на отдельные числа
        values = line.split()
        for value in values:
            # Преобразование числа во float и добавление в массив
            setka.append(float(value))

with open("koef.txt", "r") as file1:
    lines = file1.readlines()

hi = float(lines[0].strip())
lamb = float(lines[1].strip())
sigma = float(lines[2].strip())


with open("time.txt", "r") as file2:
    lines = file2.readlines()
    for line in lines:
        # Разделение строки на отдельные числа
        values = line.split()
        for value in values:
            # Преобразование числа во float и добавление в массив
            time.append(float(value))

def u_f(r, t):
    return r*r+t*t

def funct(r,t):
    return -4+2*t

import numpy as np
num_nodes = len(setka)
A = np.zeros((num_nodes, num_nodes))
M = np.zeros((num_nodes, num_nodes))
G = np.zeros((num_nodes,num_nodes))
b = [0] * num_nodes
q = [[[0] * num_nodes for _ in range(num_nodes)] for _ in range(3)]




def MassMatrix():
    for i in range(len(setka) - 1):
        h = float(setka[i+1] - setka[i])
        r = float(setka[i])
        local_matrix = (h/60.0)*np.array([[(r*r*20.0 + h*r*10.0+h*h*2.0),(r*r*10.0+h*r*10.0+h*h*3.0)],
                                           [r*r*10.0 + h*r*10.0+h*h*3.0, r*r*20.0+ h*r*30.0+h*h*12.0]])
        M[i:i+2, i:i+2] += local_matrix
    return M

def ZhestMatrix():
    for i in range(len(setka) - 1):
        h = float(setka[i+1] - setka[i])
        r = float(setka[i])
        G[i:i+2, i:i+2] += (1.0/h)*np.array([[(((h*h)/3.0)+h*r+r*r), -1.0*(((h*h)/3.0)+h*r+r*r)],
                            [-1.0*(((h*h)/3.0)+h*r+r*r), (((h*h)/3.0)+h*r+r*r)]])
    return G


def b_j(t):
    b = np.zeros(num_nodes)
    for i in range(len(setka) - 1):
        loc = [0,0]
        h = float(setka[i+1] - setka[i])
        r = float(setka[i])
        f = [funct(setka[i],t), funct(setka[i+1],t)]
        local_b = [0,0]
        loc = (h*1/60.0)*np.array([[r*r*20.0 + h*r*10.0+h*h*2.0,r*r*10.0+h*r*10.0+h*h*3.0],
                            [r*r*10.0 + h*r*10.0+h*h*3.0, r*r*20.0 + h*r*30.0+h*h*12.0]])
        for l in range(2):
            for j in range(2):
                local_b[l] += loc[l][j] * f[j]
        b[i:i+2]  += local_b
        
    return b

def kraev1(A, d,t_j):
    A[0][0] = 10e+60
    d[0] = 10e+60*u_f(setka[0],t_j)
    A[num_nodes-1][num_nodes-1] = 10e+60
    d[num_nodes-1] = 10e+60*u_f(setka[num_nodes-1],t_j)


def gauss_elimination(A, b):
    n = len(A)
    
    # Прямой ход метода Гаусса
    for i in range(n):
        pivot = A[i][i]
        if pivot == 0:
            raise ValueError("Деление на ноль невозможно")
        
        for j in range(i+1, n):
            ratio = A[j][i] / pivot
            for k in range(i, n):
                A[j][k] -= ratio * A[i][k]
            b[j] -= ratio * b[i]
    
    # Обратный ход метода Гаусса
    x = [0] * n
    for i in range(n-1, -1, -1):
        x[i] = b[i]
        for j in range(i+1, n):
            x[i] -= A[i][j] * x[j]
        x[i] /= A[i][i]
    
    return x

def output(q,t_j):
    with open("out.txt", "a") as file:
    # Запись заголовков столбцов
        file.write(f"Результат при  t_j = {t_j}\n")
        file.write("setka\t    q\t        u\t         q - u\n")
        # Запись данных в файл
        nev = 0.0
        for i in range(len(setka)):
            data = "{:.8f}\t{:.8f}\t{:.8f}\t{:.8f}\n".format(setka[i], q[i], u_f(setka[i], t_j), q[i] - u_f(setka[i], t_j))
            nev += pow(q[i] - u_f(setka[i], t_j),2)
            file.write(data)
        file.write(f"Норма погрешности решения: {(nev)**0.5}\n\n")

# b = b_j(1)


# print(A)
# print(b)


# print(np.dot(A, t_q))
# print("\n")
# print(b)


# q[0] = gauss_elimination(A, b)

# print(q[0])

M = MassMatrix()
G = ZhestMatrix()


for k in range(len(setka)):
    q[0][k] = u_f(setka[k], time[0])
    q[1][k] = u_f(setka[k], time[1]) 

with open("out.txt", "w") as file:
    file.truncate()

for j in range(2, len(time)):
    q[2] = {0}
    t_j = time[j]
    t_j_1 = time[j-1]
    t_j_2 = time[j-2]
    dt0  = t_j - t_j_1
    dt1 = t_j_1 - t_j_2
    dt2 = t_j - t_j_2
    A = np.zeros((num_nodes, num_nodes))
    d = [0] * num_nodes
    b = [0] * num_nodes
    b = b_j(t_j)
    A = ((2.0/(dt2*dt0)))*hi*M + ((dt2+dt0)/(dt2*dt0))*sigma*M + lamb*G
    d = b +np.dot((((-2.0/(dt1*dt2))))*hi*M,q[0]) + np.dot(((2.0/(dt1*dt0)))*hi*M,q[1]) + np.dot((-dt0/(dt1*dt2))*sigma*M,q[0]) + np.dot((dt2/(dt1*dt0))*sigma*M,q[1])
    kraev1(A, d, t_j)
    q[2] = gauss_elimination(A, d)
    output(q[2], t_j)
    q[0] = q[1]
    q[1] = q[2]

