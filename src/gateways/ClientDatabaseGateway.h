#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include "../models/Client.h"

class ClientDatabaseGateway {

  public:

    ClientDatabaseGateway() {
      hDbc = nullptr;
    }

    ClientDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    Client create(string name, string address) {
        SQLHSTMT hStmt;
        SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        SQLCHAR* insertQuery = (SQLCHAR*)"INSERT INTO clients (name, address) VALUES (?, ?)";
        SQLPrepare(hStmt, insertQuery, SQL_NTS);

        // Задайте параметры для вставки
        const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
        const SQLCHAR* sqlAddress = reinterpret_cast<const SQLCHAR*>(address.c_str());
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAddress, 0, NULL);
        
        if (SQLExecute(hStmt) != SQL_SUCCESS) {
          SQLCHAR sqlState[6];
          SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
          SQLINTEGER nativeError;
          SQLSMALLINT errorMsgLen;

          SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

          // Construct a runtime_error with the PostgreSQL error message
          string errorMessage = "PostgreSQL Error [" + string(reinterpret_cast<char*>(sqlState)) + "]: " + string(reinterpret_cast<char*>(errorMsg));
          throw runtime_error(errorMessage);
        }

        SQLINTEGER id;
        SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
        SQLFetch(hStmt);

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return Client(id, name, address);
    }

    Client get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM clients WHERE id = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        string errorMessage = "PostgreSQL Error [" + string(reinterpret_cast<char*>(sqlState)) + "]: " + string(reinterpret_cast<char*>(errorMsg));
        throw runtime_error(errorMessage);
      } 

      SQLCHAR name[256];
      SQLCHAR address[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);

      // Извлекаем данные из результирующего набора
      SQLFetch(hStmt);
      string strName(reinterpret_cast<char*>(name));
      string strAddress(reinterpret_cast<char*>(address));

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Client(id, strName, strAddress);
    }

    vector<Client> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM clients WHERE name = ?";
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
        string errorMessage = "PostgreSQL Error [" + string(reinterpret_cast<char*>(sqlState)) + "]: " + string(reinterpret_cast<char*>(errorMsg));
        throw runtime_error(errorMessage);
      } 

      vector<Client> clients;

      SQLINTEGER id;
      SQLCHAR sdlName[256];
      SQLCHAR address[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, sdlName, sizeof(sdlName), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);

      // Извлекаем данные из результирующего набора
      while(SQLFetch(hStmt) == SQL_SUCCESS) {
        string strName(reinterpret_cast<char*>(sdlName));
        string strAddress(reinterpret_cast<char*>(address));
        Client client = Client(id, strName, strAddress);
        clients.push_back(client);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return clients;
    }


    vector<Client> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM clients;";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        string errorMessage = "PostgreSQL Error [" + string(reinterpret_cast<char*>(sqlState)) + "]: " + string(reinterpret_cast<char*>(errorMsg));
        throw runtime_error(errorMessage);
      } 

      vector<Client> clients;

      SQLINTEGER id;
      SQLCHAR name[256];
      SQLCHAR address[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, address, sizeof(address), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        string strName(reinterpret_cast<char*>(name));
        string strAddress(reinterpret_cast<char*>(address));
        Client client = Client(id, strName, strAddress);
        clients.push_back(client);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return clients;
    }

    Client update(int id, string name, string address) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* updateQuery = (SQLCHAR*)"UPDATE clients SET name = ?, address = ? WHERE id = ?";
      SQLPrepare(hStmt, updateQuery, SQL_NTS);

      // Задайте параметры для обновления
      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      const SQLCHAR* sqlAddress = reinterpret_cast<const SQLCHAR*>(address.c_str());
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAddress, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        string errorMessage = "PostgreSQL Error [" + string(reinterpret_cast<char*>(sqlState)) + "]: " + string(reinterpret_cast<char*>(errorMsg));
        throw runtime_error(errorMessage);
      } 


      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Client(id, name, address);
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* deleteQuery = (SQLCHAR*)"DELETE FROM clients WHERE id = ?";
      SQLPrepare(hStmt, deleteQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

      if (SQLExecute(hStmt) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        string errorMessage = "PostgreSQL Error [" + string(reinterpret_cast<char*>(sqlState)) + "]: " + string(reinterpret_cast<char*>(errorMsg));
        throw runtime_error(errorMessage);
      } 

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      
    }


  private:
    SQLHDBC hDbc;
};