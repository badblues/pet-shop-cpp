#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include "Menu.h"

using namespace std;

void createTables(SQLHDBC hDbc) {
  SQLHSTMT hStmt;
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

  // SQL commands to create tables
  const char* createTablesSQL =
    "CREATE TABLE IF NOT EXISTS Employees ("
    "  id serial PRIMARY KEY,"
    "  name varchar(255) NOT NULL,"
    "  address varchar(255) NOT NULL,"
    "  position varchar(255) NOT NULL,"
    "  salary decimal(10, 2) NOT NULL"
    ");"
    "CREATE TABLE IF NOT EXISTS Clients ("
    "  id serial PRIMARY KEY,"
    "  name varchar(255) NOT NULL,"
    "  address varchar(255) NOT NULL"
    ");"
    "CREATE TABLE IF NOT EXISTS Breeds ("
    "  id serial PRIMARY KEY,"
    "  name varchar(255) NOT NULL"
    ");"
    "CREATE TABLE IF NOT EXISTS Animals ("
    "  id serial PRIMARY KEY,"
    "  name varchar(255) NOT NULL,"
    "  age int,"
    "  gender varchar(255) NOT NULL,"
    "  breed_id int REFERENCES Breeds(id) NOT NULL,"
    "  exterior_description text NOT NULL,"
    "  pedigree text NOT NULL,"
    "  veterinarian varchar(255) NOT NULL,"
    "  owner_id int REFERENCES Clients(id)"
    ");"
    "CREATE TABLE IF NOT EXISTS Competitions ("
    "  id serial PRIMARY KEY,"
    "  animal_id int REFERENCES Animals(id) NOT NULL,"
    "  name varchar(255) NOT NULL,"
    "  location varchar(255) NOT NULL,"
    "  date date NOT NULL"
    ");"
    "CREATE TABLE IF NOT EXISTS AnimalCompetitions ("
    "  animal_id int REFERENCES Animals(id) NOT NULL,"
    "  competition_id int REFERENCES Competitions(id) NOT NULL,"
    "  award varchar(255) NOT NULL,"
    "  PRIMARY KEY (animal_id, competition_id)"
    ");"
    "CREATE TABLE IF NOT EXISTS Applications ("
    "  id serial PRIMARY KEY,"
    "  client_id int REFERENCES Clients(id) NOT NULL,"
    "  employee_id int REFERENCES Employees(id),"
    "  breed_id int REFERENCES Breeds(id) NOT NULL,"
    "  gender varchar(255),"
    "  application_date date NOT NULL,"
    "  completed boolean NOT NULL"
    ");"
    ;

  SQLRETURN sqlResult = SQLExecDirect(hStmt, (SQLCHAR*)createTablesSQL, SQL_NTS);

  // Check if the statement executed correctly
  if (sqlResult != SQL_SUCCESS && sqlResult != SQL_SUCCESS_WITH_INFO) {
    SQLCHAR sqlState[6];
    SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER nativeError;
    SQLSMALLINT errorMsgLen;

    SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

    // Construct an error message with the PostgreSQL error details
    std::string errorMessage = "SQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
    std::cerr << errorMessage << std::endl;
    getchar();
  } else if (sqlResult != SQL_SUCCESS && sqlResult == SQL_SUCCESS_WITH_INFO) {
    std::cerr << "SQL statement executed with a warning." << std::endl;
    getchar();
  } else {
    std::cout << "Tables created successfully." << std::endl;
  }

  // Clean up resources
  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

int main() {

  try {

        string username = "badblues";
        string password = "root";
        string driver = "{PostgreSQL Unicode}";
        string database = "rpbd";
        string server = "localhost";
        string port = "5432";


        string connectionString = "DRIVER=" + driver + ";DATABASE=" + database + ";SERVER=" +
                                  server + ";PORT=" + port + ";UID=" + username + ";PWD=" + password + ";";

        SQLHENV hEnv;
        SQLHDBC hDbc;


        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);

        SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
        auto *connStr = (SQLCHAR *) connectionString.c_str();
        SQLRETURN sqlreturn = SQLDriverConnect(hDbc, nullptr, connStr, SQL_NTS, nullptr,
                                               0, nullptr, SQL_DRIVER_COMPLETE);

        if (sqlreturn == SQL_SUCCESS || sqlreturn == SQL_SUCCESS_WITH_INFO) {

            Menu menu = Menu(hDbc);
            createTables(hDbc);
            menu.showMenu();
            SQLDisconnect(hDbc);
        } else {
            cerr << "Connection failed\n";
        }

        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return 1;
    }


    return 0;
}



