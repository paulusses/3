import pandas as pd
from scipy.stats import spearmanr

# Создайте DataFrame с вашими данными
data = {
    'A': [2, 4, 94, 132],
    'B': [213, 22, 33, 22],
    'C': [21, 29, 23, 11],
    'D': [23, 11, 232, 92]
}

df = pd.DataFrame(data)

# Создайте пустую матрицу для хранения коэффициентов корреляции
corr_matrix = pd.DataFrame(index=df.columns, columns=df.columns)

# Вычислите ранговые коэффициенты корреляции Спирмена для каждой пары переменных
for col1 in df.columns:
    for col2 in df.columns:
        if col1 != col2:
            corr, _ = spearmanr(df[col1], df[col2])
            corr_matrix.at[col1, col2] = corr

# Выведите матрицу коэффициентов корреляции
print("Матрица коэффициентов корреляции Спирмена:")
print(corr_matrix)
