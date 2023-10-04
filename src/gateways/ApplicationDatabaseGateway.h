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

      string genders_str = "";
      if (gender.has_value())
        genders_str = gender.value() == Gender::male ? "male" : "female";
      std::string query = "INSERT INTO applications (client_id, employee_id, breed_id, gender, application_date, completed) VALUES (?, ?, ?, '" + genders_str + "', ?, ?)";
      SQLCHAR* insertQuery = new SQLCHAR[query.size() + 1];
      std::strcpy(reinterpret_cast<char*>(insertQuery), query.c_str());
      SQLPrepare(hStmt, insertQuery, SQL_NTS);

      // Задайте параметры для вставки
      SQLINTEGER sqlClientId = static_cast<int>(clientId);
      SQLINTEGER sqlEmployeeId = static_cast<int>(employeeId);
      SQLINTEGER sqlBreedId = static_cast<int>(breedId);
      const char* dateString = getDateString(applicationDate).c_str();
      SQLCHAR* sqlApplicationDate = const_cast<SQLCHAR*>(reinterpret_cast<const SQLCHAR*>(dateString));
      SQLINTEGER sqlCompleted = completed ? 1 : 0;
      
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlClientId, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlEmployeeId, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlBreedId, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, sqlApplicationDate, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlCompleted, 0, NULL);
      
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

    Application get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM applications WHERE id = ?";
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

      SQLINTEGER clientId;
      SQLINTEGER employeeId;
      SQLINTEGER breedId;
      SQLCHAR sqlGender[256];
      SQLCHAR sqlApplicationDate[256];
      SQLINTEGER sqlCompleted;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &clientId, sizeof(clientId), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &employeeId, sizeof(employeeId), NULL);
      SQLBindCol(hStmt, 4, SQL_C_SLONG, &breedId, sizeof(breedId), NULL);
      SQLBindCol(hStmt, 5, SQL_C_CHAR, sqlGender, sizeof(sqlGender), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlApplicationDate, sizeof(sqlApplicationDate), NULL);
      SQLBindCol(hStmt, 7, SQL_C_SLONG, &sqlCompleted, sizeof(sqlCompleted), NULL);

      // Извлекаем данные из результирующего набора
      SQLFetch(hStmt);
      optional<Gender> gender;
      if (sqlGender[0] != '\0')
        gender = getGender(reinterpret_cast<char*>(sqlGender));
      tm date = getDate(reinterpret_cast<char*>(sqlApplicationDate));
      bool completed = sqlCompleted == 1;

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Application(id, clientId, employeeId, breedId, gender, date, completed);
    }

    vector<Application> findByClient(int clientId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM applications WHERE client_id = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &clientId, 0, NULL);

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
      SQLINTEGER employeeId;
      SQLINTEGER breedId;
      SQLCHAR sqlGender[256];
      SQLCHAR sqlApplicationDate[256];
      SQLINTEGER sqlCompleted;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
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

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return applications;
    }

    vector<Application> findByEmployee(int employeeId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM applications WHERE employee_id = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &employeeId, 0, NULL);

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
      SQLINTEGER breedId;
      SQLCHAR sqlGender[256];
      SQLCHAR sqlApplicationDate[256];
      SQLINTEGER sqlCompleted;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &clientId, sizeof(clientId), NULL);
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

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return applications;
    }

    vector<Application> findByBreed(int breedId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM applications WHERE breed_id = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &breedId, 0, NULL);

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
      SQLCHAR sqlGender[256];
      SQLCHAR sqlApplicationDate[256];
      SQLINTEGER sqlCompleted;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &clientId, sizeof(clientId), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &employeeId, sizeof(employeeId), NULL);
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

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return applications;
    }

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

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return applications;
    }

    Application update(int id, int clientId, int employeeId, int breedId, optional<Gender> gender, tm applicationDate, bool completed) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string genders_str = "";
      if (gender.has_value())
        genders_str = gender.value() == Gender::male ? "male" : "female";
      std::string query = "UPDATE applications SET client_id = ?, employee_id = ?, breed_id = ?, gender = '" + genders_str  + "', application_date = ?, completed = ? WHERE id = ?";
      SQLCHAR* updateQuery = new SQLCHAR[query.size() + 1];
      std::strcpy(reinterpret_cast<char*>(updateQuery), query.c_str());
      SQLPrepare(hStmt, updateQuery, SQL_NTS);

      // Задайте параметры для обновления
      SQLINTEGER sqlClientId = static_cast<int>(clientId);
      SQLINTEGER sqlEmployeeId = static_cast<int>(employeeId);
      SQLINTEGER sqlBreedId = static_cast<int>(breedId);
      const char* dateString = getDateString(applicationDate).c_str();
      SQLCHAR* sqlApplicationDate = const_cast<SQLCHAR*>(reinterpret_cast<const SQLCHAR*>(dateString));
      SQLINTEGER sqlCompleted = completed ? 1 : 0;

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlClientId, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlEmployeeId, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlBreedId, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, sqlApplicationDate, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlCompleted, 0, NULL);
      SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

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

      return Application(id, clientId, employeeId, breedId, gender, applicationDate, completed);
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* deleteQuery = (SQLCHAR*)"DELETE FROM applications WHERE id = ?";
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