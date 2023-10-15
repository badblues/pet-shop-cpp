#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>

class EmployeeDatabaseGateway : DatabaseGateway {

  public:

    EmployeeDatabaseGateway() {
      hDbc = nullptr;
    }

    EmployeeDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    vector<vector<string>> create(string name, string address, string position, double salary) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string insertQuery = "INSERT INTO employees (name, address, position, salary) VALUES ('"
                         + name + "', '" + address + "', '" + position + "', " + to_string(salary * 100)
                         + ") RETURNING id, name, address, position, salary, position, salary";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)insertQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);
      
      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, address, position, salary, position, salary FROM employees WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, address, position, salary FROM employees WHERE name = '" + name + "'";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByPosition(string position) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, address, position, salary FROM employees WHERE position = '" + position + "'";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, address, position, salary FROM employees";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> update(int id, string name, string address, string position, double salary) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string updateQuery = "UPDATE employees SET name = '" + name + "', address = '" + address
                         + "', position = '" + position + "', salary = " + to_string(salary * 100)
                         + " WHERE id = " + to_string(id)
                         + " RETURNING id, name, address, position, salary";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string deleteQuery = "DELETE FROM employees WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

  private:
    SQLHDBC hDbc;
};