#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstring>
#include "./DatabaseGateway.h"

using namespace std;

class CompetitionDatabaseGateway : DatabaseGateway {

  public:

    CompetitionDatabaseGateway() {
      hDbc = nullptr;
    }

    CompetitionDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    vector<vector<string>> create(string name, string location, tm date) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string dateStr = getDateString(date); 
      string insertQuery = "INSERT INTO competitions (name, location, date) VALUES ('" + name + "', '" + location + "', '" + dateStr + "') RETURNING id, name, location, date";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)insertQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);
      
      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, location, date FROM competitions WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, location, date FROM competitions WHERE name = '" + name + "'";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, location, date FROM competitions";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> update(int id, string name, string location, tm date) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string dateStr = getDateString(date);
      string updateQuery = "UPDATE competitions SET name = '" + name + "', location = '" + location + "', date = '" + dateStr + "' WHERE id = " + to_string(id) + " RETURNING id, name, location, date";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string deleteQuery = "DELETE FROM competitions WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

  private:
    SQLHDBC hDbc;
};