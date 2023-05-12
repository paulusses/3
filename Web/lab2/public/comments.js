// Отправка данных формы на серверный маршрут и обновление списка комментариев
document.getElementById('commentForm').addEventListener('submit', (event) => {
    event.preventDefault();
  
    const text = document.getElementById('text').value;
    const author = document.getElementById('author').value;
  
    fetch('/comments', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ text, author }),
    })
      .then((response) => response.json())
      .then((data) => {
        console.log(data);
        // Обновление списка комментариев после успешного добавления
        getComments();
      })
      .catch((error) => {
        console.error(error);
      });
  
    // Очистка полей формы
    document.getElementById('text').value = '';
    document.getElementById('author').value = '';
  });
  
  // Получение комментариев с сервера и их отображение
  function getComments() {
    fetch('/comments')
      .then((response) => response.json())
      .then((comments) => {
        // Генерация HTML-разметки для каждого комментария
        const commentsHTML = comments.map((comment) => {
          return `
            <div class="comment">
              <p class="comment-text">${comment.text}</p>
              <p class="comment-author">Автор: ${comment.author}</p>
            </div>
          `;
        });
  
        // Вставка HTML-разметки в контейнер комментариев на странице
        const commentsContainer = document.getElementById('commentsContainer');
        commentsContainer.innerHTML = commentsHTML.join('');
      })
      .catch((error) => {
        console.error(error);
      });
  }
  
  // Инициализация - получение и отображение комментариев при загрузке страницы
  getComments();
  