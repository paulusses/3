import pandas as pd
import numpy as np
from scipy.stats import spearmanr

dataf = pd.read_csv('V8.csv', delimiter=';')


n, m = dataf.shape
print(f"Количество строк: {n}")
print(f"Количество столбцов: {m}")
a16 = {
    '+': 1,
    '-': 0
}
dataf["A16"] = dataf['A16'].replace(a16)
df = dataf ## matrix of counts data
def colibrate_column(column_name, df):
    value = df[column_name].value_counts(normalize=True)
    unique_values_count = df[column_name].nunique()
    df[column_name] = df[column_name].replace((value*n+value)/(value*n + (n-value*n)+1*unique_values_count))
    return df


def discretize_column(df, column_name):
    # Получаем столбец данных
    data = df[column_name]
    # Определите границы интервалов, используя выборочные квантили
    quantiles = np.quantile(data, [0, 0.2, 0.4, 0.6, 0.8, 1])
    # Вычислите количество интервалов (бинов)
    N = len(quantiles) - 1
    # Генерируйте равномерные интервалы на основе квантилей
    bins = quantiles
    # Выполните дискретизацию, присваивая каждому значению соответствующий интервал
    discretized_indices = np.digitize(data, bins)
    # Создайте новый столбец в DataFrame с дискретизированными значениями
    df[column_name] = discretized_indices
    return df


def matrix_to_csv(file_name, df):
    df.to_csv(file_name, index=False)

######## 0.1

colibrate_column("A1", df)
colibrate_column("A4", df)
colibrate_column("A6", df)
colibrate_column('A7', df)
colibrate_column('A9', df)
colibrate_column('A10', df)
colibrate_column('A12', df)
matrix_to_csv('matrix1.csv', df)

######## 

######## 0.2

df2 = df ### matrix of categorials data

discretize_column(df2, 'A15')
discretize_column(df2, 'A11')
discretize_column(df2, 'A14')
matrix_to_csv('matrix2.csv', df2)

########

######## 1.0

# Вычисление рангов для всех переменных
ranked_data = df.rank()
spearman_corr_matrix = ranked_data.corr(method='spearman')
matrix_to_csv('spearman_matrix.csv', spearman_corr_matrix)