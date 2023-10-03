#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include "../models/Breed.h"

class BreedDatabaseGateway {

  public:

    BreedDatabaseGateway() {
      hDbc = nullptr;
    }

    BreedDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    Breed create(string name) {
        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        SQLCHAR* insertQuery = (SQLCHAR*)"INSERT INTO breeds (name) VALUES (?)";
        SQLPrepare(hStmt, insertQuery, SQL_NTS);

        // Задайте параметры для вставки
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

        SQLINTEGER id;
        SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
        SQLFetch(hStmt);

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return Breed(id, name);
    }

    Breed get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM breeds WHERE id = ?";
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

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);

      // Извлекаем данные из результирующего набора
      SQLFetch(hStmt);
      std::string strName(reinterpret_cast<char*>(name));

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Breed(id, strName);
    }

    vector<Breed> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM breeds WHERE name = ?";
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

      vector<Breed> breeds;

      SQLINTEGER id;
      SQLCHAR sdlName[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, sdlName, sizeof(sdlName), NULL);

      // Извлекаем данные из результирующего набора
      while(SQLFetch(hStmt) == SQL_SUCCESS) {
        std::string strName(reinterpret_cast<char*>(sdlName));
        Breed breed = Breed(id, strName);
        breeds.push_back(breed);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return breeds;
    }


    vector<Breed> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM breeds;";
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

      vector<Breed> breeds;

      SQLINTEGER id;
      SQLCHAR name[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        std::string strName(reinterpret_cast<char*>(name));
        Breed breed = Breed(id, strName);
        breeds.push_back(breed);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return breeds;
    }

    Breed update(int id, string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* updateQuery = (SQLCHAR*)"UPDATE breeds SET name = ? WHERE id = ?";
      SQLPrepare(hStmt, updateQuery, SQL_NTS);

      // Задайте параметры для обновления
      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

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

      return Breed(id, name);
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* deleteQuery = (SQLCHAR*)"DELETE FROM breeds WHERE id = ?";
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