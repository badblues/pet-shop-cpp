#pragma once
#include <vector>
#include <sql.h>
#include <sqlext.h>
#include <iomanip>
#include "../models/Gender.h"

class DatabaseGateway {

  protected:

    void handleSQLReturn(SQLRETURN sqlReturn, SQLHSTMT hStmt) {
      if (sqlReturn != SQL_SUCCESS) {
        SQLCHAR sqlState[6];
        SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError;
        SQLSMALLINT errorMsgLen;

        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, errorMsg, SQL_MAX_MESSAGE_LENGTH, &errorMsgLen);

        string errorMessage = "PostgreSQL Error [" + string(reinterpret_cast<char*>(sqlState)) + "]: " + string(reinterpret_cast<char*>(errorMsg));
        throw runtime_error(errorMessage);
      }
    }

    vector<vector<string>> fetchData(SQLHSTMT hStmt) {
      vector<vector<string>> result;
      SQLCHAR columnName[256];
      SQLLEN columnNameLen;
      SQLSMALLINT columnCount = 0;

      SQLNumResultCols(hStmt, &columnCount);

      if (columnCount > 0) {
        while (SQLFetch(hStmt) == SQL_SUCCESS) {
          vector<string> row;
          for (int i = 1; i <= columnCount; i++) {
            SQLGetData(hStmt, i,
                      SQL_C_CHAR, columnName, sizeof(columnName), &columnNameLen);
            row.emplace_back(reinterpret_cast<char *>(columnName));
          }
          result.push_back(row);
        }
      }
      return result;
    }

    string getDateString(tm date) {
      stringstream dateStream;
      dateStream << put_time(&date, "%Y-%m-%d");
      return dateStream.str();
    }

    string getGenderString(Gender gender) {
      return gender == Gender::male ? "'male'" : "'female'";
    }

};