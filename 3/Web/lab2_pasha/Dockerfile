# Используем образ Node.js в качестве базового образа
FROM node:14

# Устанавливаем рабочую директорию внутри контейнера
WORKDIR /app



# Копируем файлы package.json и package-lock.json в рабочую директорию
COPY package*.json ./

COPY init.sql /docker-entrypoint-initdb.d/

# Устанавливаем зависимости
RUN npm install

# Копируем остальные файлы проекта в рабочую директорию
COPY . .

# Открываем порт, который будет использоваться внутри контейнера
EXPOSE 3000

# Запускаем команду для запуска приложения
CMD [ "node", "server.js" ]