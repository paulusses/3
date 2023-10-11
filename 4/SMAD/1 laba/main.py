import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.linear_model import LinearRegression
import scipy.stats


### two factors - X1, X2
### X1 - quadratic, X2 - cubinp.ones(x1.shape)c
### l = (3, 2, 0.001, 2, 2.5, 0.001)
### X1, X2 = (-1, -0.5, 0, 0.5, 1)
### Y = 3+2*X1+2.5*X1*X1+0.001*X1*X1*X1+2*X2+2.5*X2*X2+3*X2*X2*X2+0.001*X2*X2*X2*X2


def get_u(X1, X2):
    return 1 + 0.001*X1**3 + 0.9*X1**2 + 2*X1 + 0.001*X2**4 + 0.8*X2**3 + 1.5*X2**2 + 3*X2

def model_two_factor_no_e():
    X1 = [-1, -0.5, 0, 0.5, 1]
    X2 = [0] * len(X1)  # Фиксированное значение X2
    Y_values = [get_u(x1, x2) for x1, x2 in zip(X1, X2)]
    plt.grid(True)
    plt.plot(X1, Y_values)
    plt.xlabel('X1')
    plt.ylabel('u(X1, 0)')
    plt.title('Зависимость u(X1, 0) от X1')
    #plt.savefig('u(x1,0).png')
    plt.show()
        # Фиксируем X1 на 0 и изменяем X2
    X1 = [0] * 5
    X2 = [-1, -0.5, 0, 0.5, 1]
    Y_values = [get_u(x1, x2) for x1, x2 in zip(X1, X2)]
    plt.grid(True)
    plt.plot(X2, Y_values)
    plt.xlabel('X2')
    plt.ylabel('u(0, X2)')
    plt.title('Зависимость u(0, X2) от X2')
    #plt.savefig('u(0,x2).png')
    plt.show()

def show_thett(y, x):
    plt.grid(True)
    plt.plot(y)
    plt.plot(x)
    plt.xlabel('X@thetta')
    plt.ylabel('y')
    plt.title('Сопоставление y с X*thetta')
    plt.show()
    #plt.savefig('X@thett.png')

X1 = X2 = [-1, -0.5, 0, 0.5, 1]
u_value= []
x1j=[]
x2j=[]

for x1 in X1:
    for x2 in X2:
        u_value.append(get_u(x1, x2))
        x1j.append(x1)
        x2j.append(x2)


model_two_factor_no_e()
u = np.array(u_value)
##w = np.var(u)
w = (u - np.mean(u)).T @ (u - np.mean(u)) / (np.size(u) - 1)
p = 0.05
noise = np.random.normal(0, (p*w)**0.5,np.size(u))
y = u + noise
# Создаем DataFrame из списка данных
data = {
    'x1' : x1j,
    'x2' : x2j,
    'u(x1,x2)'  : u,
    'e' : noise,
    'y' : y
}
df = pd.DataFrame(data)
df.to_csv('data.csv', index=False, sep='\t')

####### МКН
x1 = np.array(x1j)
x2 = np.array(x2j)
X = np.array([np.ones(x1.shape),x1**3, x1**2, x1, x2**4, x2**3, x2**2,x2]).T
thett = np.linalg.solve(X.T@X, X.T@y)
e = y - X@thett
data = {
    'y' : y,
    'y*': X@thett,
    'y-y*': e
}
df = pd.DataFrame(data)
df.to_csv('data2.csv', index=False,sep='\t')

show_thett(y, X@thett)

n = np.shape(X)[0]
m = np.shape(X)[1]

## Дисперсия
D = e.T@e / (n - m)
## Несмещенная дисперсия
D2 = p*w
### Проверяем адекватность
F = D/(D2)
FT = scipy.stats.f.ppf(q=1-0.05, dfn=n-m,dfd=1000000)
if F < FT: 
    print('F=', F,' FT=', FT)
    print('Model is ok')
else:
    print('F=', F,' FT=', FT)
    print('Model is not ok')

print('Dispersion: \n', D)
print('Unbiased dispersion: \n', D2)
print('thetta: ', thett)
