#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include "../models/Employee.h"

class EmployeeDatabaseGateway {

  public:

    EmployeeDatabaseGateway() {
      hDbc = nullptr;
    }

    EmployeeDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    Employee create(string name, string address, string position, double salary) {
        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        SQLCHAR* insertQuery = (SQLCHAR*)"INSERT INTO employees (name, address, position, salary) VALUES (?, ?, ?, ?)";
        SQLPrepare(hStmt, insertQuery, SQL_NTS);

        // Задайте параметры для вставки
        const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
        const SQLCHAR* sqlAddress = reinterpret_cast<const SQLCHAR*>(address.c_str());
        const SQLCHAR* sqlPosition = reinterpret_cast<const SQLCHAR*>(position.c_str());
        SQLINTEGER sqlSalary = static_cast<int>(salary * 100); //Scale by 100 to simulate decimal(10,2)
        
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAddress, 0, NULL);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlPosition, 0, NULL);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &sqlSalary, 0, NULL);
        
        if (SQLExecute(hStmt) != SQL_SUCCESS) {
          SQLCHAR sqlState[6];
          SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
          SQLINTEGER nativeError;
          SQLSMALLINT errorMsgLen;

          SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

          // Construct a runtime_error with the PostgreSQL error message
          std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
          throw std::runtime_error(errorMessage);
        }

        SQLINTEGER id;
        SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
        SQLFetch(hStmt);

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return Employee(id, name, address, position, salary);
    }

    Employee get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM employees WHERE id = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
        throw std::runtime_error(errorMessage);
      } 

      SQLCHAR name[256];
      SQLCHAR address[256];
      SQLCHAR position[256];
      SQLINTEGER salary;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, position, sizeof(position), NULL);
      SQLBindCol(hStmt, 5, SQL_C_LONG, &salary, sizeof(salary), NULL);

      // Извлекаем данные из результирующего набора
      SQLFetch(hStmt);
      std::string strName(reinterpret_cast<char*>(name));
      std::string strAddress(reinterpret_cast<char*>(address));
      std::string strPosition(reinterpret_cast<char*>(position));
      double doubleSalary = static_cast<double>(salary) / 100;

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Employee(id, strName, strAddress, strPosition, doubleSalary);
    }

    vector<Employee> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM employees WHERE name = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
        throw std::runtime_error(errorMessage);
      } 

      vector<Employee> employees;

      SQLINTEGER id;
      SQLCHAR address[256];
      SQLCHAR position[256];
      SQLINTEGER salary;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, position, sizeof(position), NULL);
      SQLBindCol(hStmt, 5, SQL_C_LONG, &salary, sizeof(salary), NULL);


      // Извлекаем данные из результирующего набора
      while(SQLFetch(hStmt) == SQL_SUCCESS) {
        std::string strAddress(reinterpret_cast<char*>(address));
        std::string strPosition(reinterpret_cast<char*>(position));
        double doubleSalary = static_cast<double>(salary) / 100;
        Employee employee = Employee(id, name, strAddress, strPosition, doubleSalary);
        employees.push_back(employee);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return employees;
    }

    vector<Employee> findByPosition(string position) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM employees WHERE position = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      const SQLCHAR* sqlPosition = reinterpret_cast<const SQLCHAR*>(position.c_str());
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlPosition, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
        throw std::runtime_error(errorMessage);
      } 

      vector<Employee> employees;

      SQLINTEGER id;
      SQLCHAR name[256];
      SQLCHAR address[256];
      SQLINTEGER salary;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);
      SQLBindCol(hStmt, 5, SQL_C_LONG, &salary, sizeof(salary), NULL);


      // Извлекаем данные из результирующего набора
      while(SQLFetch(hStmt) == SQL_SUCCESS) {
        std::string strName(reinterpret_cast<char*>(name));
        std::string strAddress(reinterpret_cast<char*>(address));
        double doubleSalary = static_cast<double>(salary) / 100;
        Employee employee = Employee(id, strName, strAddress, position, doubleSalary);
        employees.push_back(employee);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return employees;
    }

    vector<Employee> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM employees;";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
        throw std::runtime_error(errorMessage);
      } 

      vector<Employee> employees;

      SQLINTEGER id;
      SQLCHAR name[256];
      SQLCHAR address[256];
      SQLCHAR position[256];
      SQLINTEGER salary;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, position, sizeof(position), NULL);
      SQLBindCol(hStmt, 5, SQL_C_LONG, &salary, sizeof(salary), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::string strName(reinterpret_cast<char*>(name));
        std::string strAddress(reinterpret_cast<char*>(address));
        std::string strPosition(reinterpret_cast<char*>(position));
        double doubleSalary = static_cast<double>(salary) / 100;
        Employee employee = Employee(id, strName, strAddress, strPosition, doubleSalary);
        employees.push_back(employee);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return employees;
    }

    Employee update(int id, string name, string address, string position, double salary) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* updateQuery = (SQLCHAR*)"UPDATE employees SET name = ?, address = ?, position = ?, salary = ? WHERE id = ?";
      SQLPrepare(hStmt, updateQuery, SQL_NTS);

      // Задайте параметры для обновления
      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      const SQLCHAR* sqlAddress = reinterpret_cast<const SQLCHAR*>(address.c_str());
      const SQLCHAR* sqlPosition = reinterpret_cast<const SQLCHAR*>(position.c_str());
      SQLINTEGER sqlSalary = static_cast<int>(salary * 100); //Scale by 100 to simulate decimal(10,2)

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAddress, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlPosition, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &sqlSalary, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
        throw std::runtime_error(errorMessage);
      } 


      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Employee(id, name, address, position, salary);
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* deleteQuery = (SQLCHAR*)"DELETE FROM employees WHERE id = ?";
      SQLPrepare(hStmt, deleteQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
        throw std::runtime_error(errorMessage);
      } 

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt); 
    }

  private:
    SQLHDBC hDbc;
};