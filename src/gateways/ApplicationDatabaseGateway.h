#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstring>
#include <optional>
#include "./DatabaseGateway.h"

using namespace std;

class ApplicationDatabaseGateway : DatabaseGateway {

  public:

    ApplicationDatabaseGateway() {
      hDbc = nullptr;
    }

    ApplicationDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    vector<vector<string>> create(int clientId, int employeeId, int breedId, optional<Gender> gender, tm applicationDate, bool completed) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string genderStr = gender.has_value() ? getGenderString(gender.value()) : "NULL";
      string dateStr = getDateString(applicationDate);
      string completedStr = completed ? "true" : "false";
      string insertQuery = "INSERT INTO applications (client_id, employee_id, breed_id, gender, application_date, completed) VALUES ("
                         + to_string(clientId) + ", " + to_string(employeeId) + ", " + to_string(breedId) +", " + genderStr + ", '"
                         + dateStr + "', " + completedStr +") RETURNING id, client_id, employee_id, breed_id, gender, application_date, completed";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)insertQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);
      
      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, client_id, employee_id, breed_id, gender, application_date, completed FROM applications WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByClientId(int clientId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, client_id, employee_id, breed_id, gender, application_date, completed FROM applications WHERE client_id = " + to_string(clientId);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByEmployeeId(optional<int> employeeId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string employeeIdStr = employeeId.has_value() ? to_string(employeeId.value()) : "NULL";
      string selectQuery = "SELECT id, client_id, employee_id, breed_id, gender, application_date, completed FROM applications WHERE employee_id = " + employeeIdStr;
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByBreedId(int breedId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, client_id, employee_id, breed_id, gender, application_date, completed FROM applications WHERE breed_id = " + to_string(breedId);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, client_id, employee_id, breed_id, gender, application_date, completed FROM applications";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> update(int id, int clientId, int employeeId, int breedId, optional<Gender> gender, tm applicationDate, bool completed) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string genderStr = gender.has_value() ? getGenderString(gender.value()) : "NULL";
      string dateStr = getDateString(applicationDate);
      string completedStr = completed ? "true" : "false";
      string updateQuery = "UPDATE applications SET client_id = " + to_string(clientId) + ", employee_id = " + to_string(employeeId)
                         + ", breed_id = " + to_string(breedId) + ", gender = " + genderStr + ", application_date = '" + dateStr
                         + "', completed = " + completedStr + " WHERE id = " + to_string(id)
                         + " RETURNING id, client_id, employee_id, breed_id, gender, application_date, completed";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> removeEmployeeFromApplication(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string updateQuery = "UPDATE applications SET employee_id = NULL WHERE id = " + to_string(id)
                         + " RETURNING id, client_id, employee_id, breed_id, gender, application_date, completed";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string deleteQuery = "DELETE FROM applications WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

  private:
    SQLHDBC hDbc;
};