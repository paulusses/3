const express = require('express');
const bodyParser = require('body-parser');
const { Pool } = require('pg');
const path = require('path');

const app = express();
const pool = new Pool({
  user: 'postgres',
  host: 'db',
  database: 'COM',
  password: 'master',
  port: 5432, // Порт PostgreSQL
});

app.use(bodyParser.json());

app.use(express.static(__dirname));
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.static(path.join(__dirname, 'public/js'), { 'Content-Type': 'text/javascript' }));



// Маршрут для создания нового комментария
app.post('/comments', (req, res) => {
    const { text, author } = req.body;
  
    // Проверка наличия значения для поля 'name'
    if (!author) {
      return res.status(400).json({ error: "Поле 'name' не может быть пустым" });
    }
  
    const query = "INSERT INTO comments (text, author, created_at) VALUES ($1, $2, current_timestamp) RETURNING *";
    const values = [text, author];
  
    pool.query(query, values)
      .then((result) => {
        res.json(result.rows[0]);
      })
      .catch((error) => {
        console.error('Error inserting comment:', error);
        res.status(500).json({ error: 'Ошибка сервера' });
      });
  });

// Маршрут для получения всех комментариев
app.get('/comments', (req, res) => {
  const query = 'SELECT * FROM comments';

  pool.query(query, (err, result) => {
    if (err) {
      console.error('Error retrieving comments:', err);
      res.status(500).json({ error: 'An error occurred while retrieving comments' });
    } else {
      res.json(result.rows);
    }
  });
});

// Обработчик маршрута "/"
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
  });

// Запуск сервера
app.listen(3000, () => {
  console.log('Server is running on port 3000');
});
