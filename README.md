Программа делает:
- подключается к бд
- создает таблицы
- отображает разработанные классы с помощью архитектурного шаблона Table Data Gateway
- операции в соответствии с семантикой варианта, без привязки к реляционной модели

TODO:
- проверить правильно ли пробрасываются ошибки
- сделать меню более удобным (выбираешь клиента, совершаешь операции с ним)
- вывод вложенных ресурсов
- проблема удаления соревнований или животных (наверное стоит удалить и участия)

menu:
  select client -> 
    list animals
    list applications
  select animal ->
    change data
    add participation
    list participations
    delete
  select competition ->
    chagne data
    add participation
    list animals
    delete
  select employee ->
    change data
    delete
  select breed ->
    change data
    delete
  
CRUD:
  client ✓



CREATE TABLE IF NOT EXISTS Breeds (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS Employees (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL,
  address varchar(255) NOT NULL,
  position varchar(255) NOT NULL,
  salary decimal(10, 2) NOT NULL
);

CREATE TABLE IF NOT EXISTS Clients (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL,
  address varchar(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS Animals (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL,
  age int,
  gender varchar(255) NOT NULL,
  breed_id int REFERENCES Breeds(id) NOT NULL,
  exterior_description text NOT NULL,
  pedigree text NOT NULL,
  veterinarian varchar(255) NOT NULL,
  owner_id int REFERENCES Clients(id)
);

CREATE TABLE IF NOT EXISTS Competitions (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL,
  location varchar(255) NOT NULL,
  date date NOT NULL
);

CREATE TABLE IF NOT EXISTS Participations (
  animal_id int REFERENCES Animals(id) NOT NULL,
  competition_id int REFERENCES Competitions(id) NOT NULL,
  award varchar(255) not null,
  PRIMARY KEY (animal_id, competition_id)
);

CREATE TABLE IF NOT EXISTS Applications (
  id serial PRIMARY KEY,
  client_id int REFERENCES Clients(id) NOT NULL,
  employee_id int REFERENCES Employees(id),
  breed_id int REFERENCES Breeds(id) NOT NULL,
  gender varchar(255),
  application_date date NOT NULL,
  completed boolean NOT NULL
);




