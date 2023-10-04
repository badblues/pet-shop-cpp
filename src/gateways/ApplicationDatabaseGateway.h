#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstring>
#include "../models/Application.h"

using namespace std;

class ApplicationDatabaseGateway {

  public:

    ApplicationDatabaseGateway() {
      hDbc = nullptr;
    }

    ApplicationDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    Application create(int clientId, int employeeId, int breedId, optional<Gender> gender, tm applicationDate, bool completed) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* insertQuery = (SQLCHAR*)"INSERT INTO applications (client_id, employee_id, breed_id, gender, application_date, completed) VALUES (?, ?, ?, ?, ?, ?)";
      SQLPrepare(hStmt, insertQuery, SQL_NTS);

      // Задайте параметры для вставки
      SQLINTEGER sqlClientId = static_cast<int>(clientId);
      SQLINTEGER sqlEmployeeId = static_cast<int>(employeeId);
      SQLINTEGER sqlBreedId = static_cast<int>(breedId);
      SQLCHAR* sqlGender = nullptr;
      if (gender.has_value()) {
        string strGender = getGenderString(gender.value());
        sqlGender = (SQLCHAR*)(reinterpret_cast<const SQLCHAR*>(strGender.c_str()));
      }
      const char* dateString = getDateString(applicationDate).c_str();
      SQLCHAR* sqlApplicationDate = const_cast<SQLCHAR*>(reinterpret_cast<const SQLCHAR*>(dateString));
      SQLINTEGER sqlCompleted = completed ? 1 : 0;
      
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlClientId, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlEmployeeId, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlBreedId, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, sqlGender, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, sqlApplicationDate, 0, NULL);
      SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlCompleted, 0, NULL);
      
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

      return Application(id, clientId, employeeId, breedId, gender, applicationDate, completed);
    }

    // Application get(int id) {
    //   SQLHSTMT hStmt;
    //   SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    //   SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM applications WHERE id = ?";
    //   SQLPrepare(hStmt, selectQuery, SQL_NTS);

    //   SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    //   if (SQLExecute(hStmt) != SQL_SUCCESS) {
    //     SQLCHAR sqlState[6];
    //     SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
    //     SQLINTEGER nativeError;
    //     SQLSMALLINT errorMsgLen;

    //     SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

    //     // Construct a runtime_error with the PostgreSQL error message
    //     std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
    //     throw std::runtime_error(errorMessage);
    //   } 

    //   SQLCHAR name[256];
    //   SQLCHAR address[256];
    //   SQLCHAR position[256];
    //   SQLINTEGER salary;

    //   // Привязываем столбцы результата к переменным
    //   SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
    //   SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);
    //   SQLBindCol(hStmt, 4, SQL_C_CHAR, position, sizeof(position), NULL);
    //   SQLBindCol(hStmt, 5, SQL_C_LONG, &salary, sizeof(salary), NULL);

    //   // Извлекаем данные из результирующего набора
    //   SQLFetch(hStmt);
    //   std::string strName(reinterpret_cast<char*>(name));
    //   std::string strAddress(reinterpret_cast<char*>(address));
    //   std::string strPosition(reinterpret_cast<char*>(position));
    //   double doubleSalary = static_cast<double>(salary) / 100;

    //   SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    //   return Application(id, strName, strAddress, strPosition, doubleSalary);
    // }

    // vector<Application> findByName(string name) {
    //   SQLHSTMT hStmt;
    //   SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    //   SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM applications WHERE name = ?";
    //   SQLPrepare(hStmt, selectQuery, SQL_NTS);

    //   const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
    //   SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);

    //   if (SQLExecute(hStmt) != SQL_SUCCESS) {
    //     SQLCHAR sqlState[6];
    //     SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
    //     SQLINTEGER nativeError;
    //     SQLSMALLINT errorMsgLen;

    //     SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

    //     // Construct a runtime_error with the PostgreSQL error message
    //     std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
    //     throw std::runtime_error(errorMessage);
    //   } 

    //   vector<Application> applications;

    //   SQLINTEGER id;
    //   SQLCHAR address[256];
    //   SQLCHAR position[256];
    //   SQLINTEGER salary;

    //   // Привязываем столбцы результата к переменным
    //   SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
    //   SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);
    //   SQLBindCol(hStmt, 4, SQL_C_CHAR, position, sizeof(position), NULL);
    //   SQLBindCol(hStmt, 5, SQL_C_LONG, &salary, sizeof(salary), NULL);


    //   // Извлекаем данные из результирующего набора
    //   while(SQLFetch(hStmt) == SQL_SUCCESS) {
    //     std::string strAddress(reinterpret_cast<char*>(address));
    //     std::string strPosition(reinterpret_cast<char*>(position));
    //     double doubleSalary = static_cast<double>(salary) / 100;
    //     Application application = Application(id, name, strAddress, strPosition, doubleSalary);
    //     applications.push_back(application);
    //   }

    //   SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    //   return applications;
    // }

    

    vector<Application> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM applications;";
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

      vector<Application> applications;

      SQLINTEGER id;
      SQLINTEGER clientId;
      SQLINTEGER employeeId;
      SQLINTEGER breedId;
      SQLCHAR sqlGender[256];
      SQLCHAR sqlApplicationDate[256];
      SQLINTEGER sqlCompleted;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &clientId, sizeof(clientId), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &employeeId, sizeof(employeeId), NULL);
      SQLBindCol(hStmt, 4, SQL_C_SLONG, &breedId, sizeof(breedId), NULL);
      SQLBindCol(hStmt, 5, SQL_C_CHAR, sqlGender, sizeof(sqlGender), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlApplicationDate, sizeof(sqlApplicationDate), NULL);
      SQLBindCol(hStmt, 7, SQL_C_SLONG, &sqlCompleted, sizeof(sqlCompleted), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        optional<Gender> gender;
        if (sqlGender[0] != '\0')
          gender = getGender(reinterpret_cast<char*>(sqlGender));
        tm date = getDate(reinterpret_cast<char*>(sqlApplicationDate));
        bool completed = sqlCompleted == 1;
        Application application = Application(id, clientId, employeeId, breedId, gender, date, completed);
        applications.push_back(application);
      }

      SQLRETURN fetchResult = SQLFetch(hStmt);
      if (fetchResult != SQL_NO_DATA && fetchResult != SQL_SUCCESS) {
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
      return applications;
    }

    // Application update(int id, string name, string address, string position, double salary) {
    //   SQLHSTMT hStmt;
    //   SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    //   SQLCHAR* updateQuery = (SQLCHAR*)"UPDATE applications SET name = ?, address = ?, position = ?, salary = ? WHERE id = ?";
    //   SQLPrepare(hStmt, updateQuery, SQL_NTS);

    //   // Задайте параметры для обновления
    //   const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
    //   const SQLCHAR* sqlAddress = reinterpret_cast<const SQLCHAR*>(address.c_str());
    //   const SQLCHAR* sqlPosition = reinterpret_cast<const SQLCHAR*>(position.c_str());
    //   SQLINTEGER sqlSalary = static_cast<int>(salary * 100); //Scale by 100 to simulate decimal(10,2)

    //   SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
    //   SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAddress, 0, NULL);
    //   SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlPosition, 0, NULL);
    //   SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &sqlSalary, 0, NULL);
    //   SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    //   if (SQLExecute(hStmt) != SQL_SUCCESS) {
    //     SQLCHAR sqlState[6];
    //     SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
    //     SQLINTEGER nativeError;
    //     SQLSMALLINT errorMsgLen;

    //     SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

    //     // Construct a runtime_error with the PostgreSQL error message
    //     std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
    //     throw std::runtime_error(errorMessage);
    //   } 


    //   SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    //   return Application(id, name, address, position, salary);
    // }

    // void remove(int id) {
    //   SQLHSTMT hStmt;
    //   SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    //   SQLCHAR* deleteQuery = (SQLCHAR*)"DELETE FROM applications WHERE id = ?";
    //   SQLPrepare(hStmt, deleteQuery, SQL_NTS);

    //   SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

    //   if (SQLExecute(hStmt) != SQL_SUCCESS) {
    //     SQLCHAR sqlState[6];
    //     SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
    //     SQLINTEGER nativeError;
    //     SQLSMALLINT errorMsgLen;

    //     SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

    //     // Construct a runtime_error with the PostgreSQL error message
    //     std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
    //     throw std::runtime_error(errorMessage);
    //   } 

    //   SQLFreeHandle(SQL_HANDLE_STMT, hStmt); 
    // }

  private:
    SQLHDBC hDbc;

    string getDateString(tm date) {
      stringstream dateStream;
      dateStream << put_time(&date, "%Y-%m-%d");
      return dateStream.str();
    }

    tm getDate(string dateStr) {
      tm date = {};
      istringstream dateStream(dateStr.c_str());
      dateStream >> get_time(&date, "%Y-%m-%d");
      if (dateStream.fail()) {
          throw std::invalid_argument("Invalid date string");
      }
      return date;
    }

    string getGenderString(Gender gender) {
      return gender == Gender::male ? "male" : "female";
    }

    Gender getGender(string gender) {
      return gender == "male" ? Gender::male : Gender::female;
    }
};