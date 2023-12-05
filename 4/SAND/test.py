import pandas as pd
from scipy.stats import spearmanr
import numpy as np

df = pd.read_csv('V8.csv', delimiter=';')

def colibrate_laplas(col, df):
    contingency_table = pd.crosstab(df[col], df['A16'])
    sum1 = sum(contingency_table["+"])
    sum2 = sum(contingency_table["-"])
    # Рассчитываем значения колибровки Лапласа
    laplace_values = (contingency_table["+"] + 1) / (contingency_table['+'] + 1 + (sum1/sum2)*(contingency_table['-']+1))
    
    # Создаем новый DataFrame с количеством '+' и '-'
    count_values = pd.DataFrame({
        '+': contingency_table['+'],
        '-': contingency_table['-']
    })
    laplace_values.name = 'Laplace Calibration'
    # Объединяем DataFrame с количеством и DataFrame с колибровкой Лапласа
    result_df = count_values.join(laplace_values)
    
    return result_df


def discretize_column(df, column_name):

      # Получаем столбец данных
    data = df[column_name]
    # Сортируем выборку по возрастанию
    sorted_data = np.sort(data)
# Вычисляем выборочные квантили
    quantiles = np.unique(np.quantile(sorted_data, [0, 0.2, 0.4, 0.6, 0.8, 1]))
    
    # Удаляем повторяющиеся квантили
    quantiles = np.unique(quantiles)
    
    # Создаем интервалы значений
    intervals = []
    for i in range(len(quantiles) - 1):
        intervals.append((quantiles[i], quantiles[i+1]))
    
  # Заменяем значения на номера интервалов
    discretized_data = []
    for value in data:
        for i, interval in enumerate(intervals):
            if interval[0] <= value < interval[1]:
                discretized_data.append(i)
                break
    
    return intervals, discretized_data

intervals, discretized_data = discretize_column(df,'A15')
print(intervals)
print(((np.sort(df['A15']))))
# df['A11'] = (discretized_data)
# print(df['A11'])




#### Удалить повторяющиеся интвервалы, добавить открытые границы 