#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstring>
#include "../models/Competition.h"

using namespace std;

class CompetitionDatabaseGateway {

  public:

    CompetitionDatabaseGateway() {
      hDbc = nullptr;
    }

    CompetitionDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    Competition create(int animalId, string name, string location, tm date, string award) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* insertQuery = (SQLCHAR*)"INSERT INTO competitions (animal_id, name, location, date, award) VALUES (?, ?, ?, ?, ?)";
      SQLPrepare(hStmt, insertQuery, SQL_NTS);

      const char* dateString = getDateString(date).c_str();
      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      const SQLCHAR* sqlLocation = reinterpret_cast<const SQLCHAR*>(location.c_str());
      SQLCHAR* sqlDate = const_cast<SQLCHAR*>(reinterpret_cast<const SQLCHAR*>(dateString));
      const SQLCHAR* sqlAward = reinterpret_cast<const SQLCHAR*>(award.c_str());

      
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &animalId, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlLocation, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlDate, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAward, 0, NULL);
      
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

      return Competition(id, animalId, name, location, date, award);
    }

    Competition get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM competitions WHERE id = ?";
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

      SQLINTEGER animalId;
      SQLCHAR sqlName[256];
      SQLCHAR sqlLocation[256];
      SQLCHAR sqlDate[256];
      SQLCHAR sqlAward[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &animalId, sizeof(animalId), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlLocation, sizeof(sqlLocation), NULL);
      SQLBindCol(hStmt, 5, SQL_C_CHAR, sqlDate, sizeof(sqlDate), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlAward, sizeof(sqlAward), NULL);

      // Извлекаем данные из результирующего набора
      SQLFetch(hStmt);
      string name(reinterpret_cast<char*>(sqlName));
      tm date = getDate(reinterpret_cast<char*>(sqlDate));
      string location(reinterpret_cast<char*>(sqlLocation));
      string award(reinterpret_cast<char*>(sqlAward));

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Competition(id, animalId, name, location, date, award);
    }

    vector<Competition> findByAnimal(int animalId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM competitions WHERE animal_id = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &animalId, 0, NULL);

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

      vector<Competition> competitions;

      SQLINTEGER id;
      SQLCHAR sqlName[256];
      SQLCHAR sqlLocation[256];
      SQLCHAR sqlDate[256];
      SQLCHAR sqlAward[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(animalId), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlLocation, sizeof(sqlLocation), NULL);
      SQLBindCol(hStmt, 5, SQL_C_CHAR, sqlDate, sizeof(sqlDate), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlAward, sizeof(sqlAward), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        string name(reinterpret_cast<char*>(sqlName));
        tm date = getDate(reinterpret_cast<char*>(sqlDate));
        string location(reinterpret_cast<char*>(sqlLocation));
        string award(reinterpret_cast<char*>(sqlAward));
        Competition competition = Competition(id, animalId, name, location, date, award);
        competitions.push_back(competition);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return competitions;
    }

    vector<Competition> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM competitions WHERE name = ?";
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

      vector<Competition> competitions;

      SQLINTEGER id;
      SQLINTEGER animalId;
      SQLCHAR sqlLocation[256];
      SQLCHAR sqlDate[256];
      SQLCHAR sqlAward[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &animalId, sizeof(animalId), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlLocation, sizeof(sqlLocation), NULL);
      SQLBindCol(hStmt, 5, SQL_C_CHAR, sqlDate, sizeof(sqlDate), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlAward, sizeof(sqlAward), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        tm date = getDate(reinterpret_cast<char*>(sqlDate));
        string location(reinterpret_cast<char*>(sqlLocation));
        string award(reinterpret_cast<char*>(sqlAward));
        Competition competition = Competition(id, animalId, name, location, date, award);
        competitions.push_back(competition);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return competitions;
    }

    vector<Competition> findByAward(string award) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM competitions WHERE award = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      const SQLCHAR* sqlAward = reinterpret_cast<const SQLCHAR*>(award.c_str());

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAward, 0, NULL);

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

      vector<Competition> competitions;

      SQLINTEGER id;
      SQLINTEGER animalId;
      SQLCHAR sqlLocation[256];
      SQLCHAR sqlDate[256];
      SQLCHAR sqlName[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &animalId, sizeof(animalId), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlLocation, sizeof(sqlLocation), NULL);
      SQLBindCol(hStmt, 5, SQL_C_CHAR, sqlDate, sizeof(sqlDate), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        string name(reinterpret_cast<char*>(sqlName));
        tm date = getDate(reinterpret_cast<char*>(sqlDate));
        string location(reinterpret_cast<char*>(sqlLocation));
        Competition competition = Competition(id, animalId, name, location, date, award);
        competitions.push_back(competition);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return competitions;
    }

    vector<Competition> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM competitions;";
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

      vector<Competition> competitions;

      SQLINTEGER id;
      SQLINTEGER animalId;
      SQLCHAR sqlName[256];
      SQLCHAR sqlLocation[256];
      SQLCHAR sqlDate[256];
      SQLCHAR sqlAward[256];

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_SLONG, &animalId, sizeof(animalId), NULL);
      SQLBindCol(hStmt, 3, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlLocation, sizeof(sqlLocation), NULL);
      SQLBindCol(hStmt, 5, SQL_C_CHAR, sqlDate, sizeof(sqlDate), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlAward, sizeof(sqlAward), NULL);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        string name(reinterpret_cast<char*>(sqlName));
        tm date = getDate(reinterpret_cast<char*>(sqlDate));
        string location(reinterpret_cast<char*>(sqlLocation));
        string award(reinterpret_cast<char*>(sqlAward));
        Competition competition = Competition(id, animalId, name, location, date, award);
        competitions.push_back(competition);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return competitions;
    }

    Competition update(int id, int animalId, string name, string location, tm date, string award) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* updateQuery = (SQLCHAR*) "UPDATE competitions SET animal_id = ?, name = ?, location = ?, date = ?, award = ? WHERE id = ?";
      SQLPrepare(hStmt, updateQuery, SQL_NTS);

      const char* dateString = getDateString(date).c_str();
      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      const SQLCHAR* sqlLocation = reinterpret_cast<const SQLCHAR*>(location.c_str());
      SQLCHAR* sqlDate = const_cast<SQLCHAR*>(reinterpret_cast<const SQLCHAR*>(dateString));
      const SQLCHAR* sqlAward = reinterpret_cast<const SQLCHAR*>(award.c_str());

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &animalId, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlLocation, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlDate, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlAward, 0, NULL);
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

      return Competition(id, animalId, name, location, date, award);
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* deleteQuery = (SQLCHAR*)"DELETE FROM competitions WHERE id = ?";
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
};