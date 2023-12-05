import csv

with open('result_matrix.txt', 'r') as txtfile:
    # Читаем содержимое файла
    data = txtfile.readlines()

# Открываем файл .csv для записи
with open('output.csv', 'w', newline='') as csvfile:
    # Создаем объект для записи в файл .csv
    csvwriter = csv.writer(csvfile)

    # Записываем каждую строку из файла .txt в файл .csv и убираем нули
    for line in data:
        # Разделяем строку на столбцы, если это необходимо, и убираем нули
        columns = [col.strip() for col in line.split('\t') if col.strip()]  # Например, если столбцы разделены табуляцией
        csvwriter.writerow(columns)