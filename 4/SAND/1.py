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


def colibrate_column(col, df):
    contingency_table = pd.crosstab(df[col], df['A16'])
    sum1 = sum(contingency_table[1])
    sum2 = sum(contingency_table[0])
    df[col] = df[col].replace((contingency_table[1] + 1) / (contingency_table[1] + 1 + (sum1/sum2)*(contingency_table[0]+1)))
    return df


def discretize_column(df, column_name):
    # Получаем столбец данных
    data = df[column_name]
    # Определите границы интервалов, используя выборочные квантили
    quantiles = np.quantile(data, [0, 0.2, 0.4, 0.6, 0.8, 1])
    # Используйте np.unique, чтобы удалить повторяющиеся значения и получить уникальные границы интервалов
    bins = np.unique(quantiles)
    # Выполните дискретизацию, присваивая каждому значению соответствующий интервал
    discretized_indices = np.digitize(data, bins,right=True)
    # Создайте новый столбец в DataFrame с дискретизированными значениями
    df[column_name] = discretized_indices
    return df



def matrix_to_csv(file_name, df):
    # Открываем файл для записи
    with open(file_name, 'w') as f:
        # Записываем названия столбцов в первую строку с выравниванием
        column_headers = ['{:>10}'.format(col) for col in df.columns]
        f.write('{}\n'.format(' '.join(column_headers)))
        
        # Записываем значения с выравниванием
        for index, row in df.iterrows():
            row_values = ['{:>10.3E}'.format(val) for val in row]
            f.write('{}\n'.format(' '.join(row_values)))


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

# Вычисление коэффициента Спирмена для всех переменных
ranked_data = df.rank()
spearman_corr_matrix = ranked_data.corr(method='spearman')
matrix_to_csv('spearman_matrix.csv', spearman_corr_matrix)

############

alpha = 0.05
with open("2.1.txt", "w") as file:
    # Проверьте значимость коэффициентов корреляции и запишите результаты в файл
    for i in range(len(df.columns)):
        for j in range(i + 1, len(df.columns)):
            var1 = df.columns[i]
            var2 = df.columns[j]
            spearman_corr, p_value = spearmanr(df[var1], df[var2])
            
            file.write(f"Корреляция между {var1} и {var2}:\n")
            file.write("Коэффициент корреляции Спирмена: {}\n".format(spearman_corr))
            file.write("p-значение: {}\n".format(p_value))

            # Проверка гипотезы на значимость
            if p_value < alpha:
                file.write("Гипотеза о значимости корреляции подтверждается\n")
            else:
                file.write("Гипотеза о значимости корреляции не подтверждается\n")
            
            file.write("\n")

num_permutations = 1000 

p_value_matrix = pd.DataFrame(index=df.columns, columns=df.columns)
# Матрица подтверждения гипотезы 
matrix_size = len(df.columns)
result_matrix = np.zeros((matrix_size, matrix_size), dtype=int)

with open("2.2", "w") as file:
    # Проверьте значимость коэффициентов корреляции и запишите результаты в файл
    for i in range(len(df.columns)):
        for j in range(i + 1, len(df.columns)):
            var1 = df.columns[i]
            var2 = df.columns[j]
            
            # Вычислите коэффициент корреляции Спирмена
            observed_corr, _ = spearmanr(df[var1], df[var2])
            
            # Сгенерируйте случайные перестановки и вычислите коэффициент корреляции для каждой
            random_correlations = []
            for _ in range(num_permutations):
                shuffled_var2 = np.random.permutation(df[var2])
                random_corr, _ = spearmanr(df[var1], shuffled_var2)
                random_correlations.append(random_corr)
            
            # Вычислите p-значение как долю случайных корреляций, которые больше или равны наблюдаемой
            p_value = (np.sum(np.abs(random_correlations) >= np.abs(observed_corr)) + 1) / (num_permutations + 1)
            # Заносим p-value в матрицу
            p_value_matrix.at[var1, var2] = p_value
            p_value_matrix.at[var2, var1] = p_value
            file.write(f"Корреляция между {var1} и {var2}:\n")
            file.write("Наблюдаемый коэффициент корреляции Спирмена: {}\n".format(observed_corr))
            file.write("p-значение (перестановочный критерий): {}\n".format(p_value))

            # Проверка гипотезы на значимость
            if p_value < alpha:
                file.write("Гипотеза о значимости корреляции подтверждается\n")
                result_matrix[i, j] = 1
            else:
                file.write("Гипотеза о значимости корреляции не подтверждается\n")
                result_matrix[i, j] = 0
            file.write("\n")

matrix_to_csv("p-value2.csv",p_value_matrix)
np.savetxt("result_matrix.txt", result_matrix, fmt='%d', delimiter='\t')
