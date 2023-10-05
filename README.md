Программа делает:
- подключается к бд
- создает таблицы
- отображает разработанные классы с помощью архитектурного шаблона Table Data Gateway
- операции в соответствии с семантикой варианта, без привязки к реляционной модели

TODO:
  создание таблиц из кода (не забыть про not null поля)
  модели:
    - client *
    - animal *
    - application *
    - breed *
    - employee *
    - competition
  шлюзы:
    - client *
    - animal
    - application *
    - breed *
    - employee *
    - competition
  меню:

  функции меню:
    - изменение животного
    - удаление животного
    - поиск по породе, полу и т.п.
    - добавление соревнования
    - просмотр соревнования животного
    - изменение соревнования
    - удаление соревнования

CREATE TABLE Applications (
  id SERIAL PRIMARY KEY,
  client_id INT NOT NULL REFERENCES clients(id),
  employee_id INT NOT NULL REFERENCES employees(id),
  breed_id INT NOT NULL REFERENCES breeds(id),
  gender varchar(255),
  application_date DATE NOT NULL,
  completed BOOLEAN NOT NULL
);


CREATE TABLE Animals (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL,
  age int,
  gender varchar(255) NOT NULL,
  breed_id int REFERENCES breeds(id) NOT NULL,
  exterior_description varchar(255) NOT NULL,
  pedigree varchar(255) NOT NULL,
  veterinarian varchar(255) NOT NULL,
  owner_id int REFERENCES clients(id)
);