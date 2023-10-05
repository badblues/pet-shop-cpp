Программа делает:
- подключается к бд
- создает таблицы
- отображает разработанные классы с помощью архитектурного шаблона Table Data Gateway
- операции в соответствии с семантикой варианта, без привязки к реляционной модели

TODO:
  - создание таблиц из кода (не забыть про not null поля)
  - сортировка заявок по дате



-- Create table Employees
CREATE TABLE Employees (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL,
  address varchar(255) NOT NULL,
  position varchar(255) NOT NULL,
  salary decimal(10, 2) NOT NULL
);

-- Create table Animals
CREATE TABLE Animals (
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

-- Create table Clients
CREATE TABLE Clients (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL,
  address varchar(255) NOT NULL
);

-- Create table Applications
CREATE TABLE Applications (
  id serial PRIMARY KEY,
  client_id int REFERENCES Clients(id) NOT NULL,
  employee_id int REFERENCES Employees(id),
  breed_id int REFERENCES Breeds(id) NOT NULL,
  gender varchar(255),
  application_date date NOT NULL,
  completed boolean NOT NULL
);

-- Create table Competitions
CREATE TABLE Competitions (
  id serial PRIMARY KEY,
  animal_id int REFERENCES Animals(id) NOT NULL,
  name varchar(255) NOT NULL,
  location varchar(255) NOT NULL,
  date date NOT NULL,
  award varchar(255) NOT NULL
);

-- Create table Breeds
CREATE TABLE Breeds (
  id serial PRIMARY KEY,
  name varchar(255) NOT NULL
);

