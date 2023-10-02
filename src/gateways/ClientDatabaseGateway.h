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
          std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
          throw std::runtime_error(errorMessage);
        }

        SQLINTEGER id;
        SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
        SQLFetch(hStmt);

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        return Client(id, name, address);
    }

    Client get(int id) {
      throw runtime_error("not implemented");
    }

    vector<Client> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM clients;";
      //SQLPrepare(hStmt, selectQuery, SQL_NTS);

      // if (SQLExecute(hStmt) != SQL_SUCCESS) {
      //   SQLCHAR sqlState[6];
      //   SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
      //   SQLINTEGER nativeError;
      //   SQLSMALLINT errorMsgLen;

      //   SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

      //   // Construct a runtime_error with the PostgreSQL error message
      //   std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
      //   throw std::runtime_error(errorMessage);
      // } 
      if (SQLExecDirect(hStmt, selectQuery, SQL_NTS) != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        // Construct a runtime_error with the PostgreSQL error message
        std::string errorMessage = "PostgreSQL Error [" + std::string(reinterpret_cast<char*>(sqlState)) + "]: " + std::string(reinterpret_cast<char*>(errorMsg));
        throw std::runtime_error(errorMessage);
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
        std::string strName(reinterpret_cast<char*>(name));
        std::string strAddress(reinterpret_cast<char*>(address));
        Client client = Client(id, strName, strAddress);
        clients.push_back(client);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return clients;
    }

    Client update(Client client) {
      throw runtime_error("not implemented");
    }

    bool remove(int id) {
      throw runtime_error("not implemented");
    }


  private:
    SQLHDBC hDbc;
};