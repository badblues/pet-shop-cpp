#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include "./DatabaseGateway.h"

class BreedDatabaseGateway : DatabaseGateway {

  public:

    BreedDatabaseGateway() {
      hDbc = nullptr;
    }

    BreedDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    vector<vector<string>> create(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string insertQuery = "INSERT INTO breeds (name) VALUES ('" + name + "') RETURNING id, name";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)insertQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);
      
      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name FROM breeds WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name FROM breeds WHERE name = '" + name + "'";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }


    vector<vector<string>> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name FROM breeds";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> update(int id, string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string updateQuery = "UPDATE breeds SET name = '" + name + "' WHERE id = " + to_string(id) + " RETURNING id, name";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);
      
      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string deleteQuery = "DELETE FROM breeds WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }


  private:
    SQLHDBC hDbc;
};