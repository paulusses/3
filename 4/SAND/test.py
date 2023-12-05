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
    # Определите границы интервалов, используя выборочные квантили
    quantiles = np.quantile(data, [0, 0.2, 0.4, 0.6, 0.8, 1])
    # Используйте np.unique, чтобы удалить повторяющиеся значения и получить уникальные границы интервалов
    bins = np.unique(quantiles)
    # Выполните дискретизацию, присваивая каждому значению соответствующий интервал
    discretized_indices = np.digitize(data, bins, right=True)
    
    # Создаем новый столбец в DataFrame с дискретизированными значениями
    df[column_name] = discretized_indices
    
    # Создаем словарь для соответствия интервалов значений и интервалов квантилей
    interval_mapping = dict(zip(data, pd.cut(data, bins)))

    return df, interval_mapping

# Применяем функцию и получаем результат
df, interval_mapping = discretize_column(df, 'A11')

# Выводим результат
for value, interval in interval_mapping.items():
    print(f"Value {value} in Interval {interval}")



#### Удалить повторяющиеся интвервалы, добавить открытые границы 