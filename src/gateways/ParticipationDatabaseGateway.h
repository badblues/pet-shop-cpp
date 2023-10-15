#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include "./DatabaseGateway.h"

class ParticipationDatabaseGateway : DatabaseGateway {

  public:

    ParticipationDatabaseGateway() {
      hDbc = nullptr;
    }

    ParticipationDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    vector<vector<string>> create(int animalId, int competitionId, string award) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string insertQuery = "INSERT INTO participations (animal_id, competition_id, award) VALUES ("
                          + to_string(animalId) + ", " + to_string(competitionId) + ", '" + award
                          + "') RETURNING animal_id, competition_id, award";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)insertQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);
      
      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> get(int animalId, int competitionId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT animal_id, competition_id, award FROM participations WHERE animal_id = "
                           + to_string(animalId) + ", competition_id = " + to_string(competitionId);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByAward(string award) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT animal_id, competition_id, award FROM participations WHERE award = '" + award + "'";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByAnimalId(int animalId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT animal_id, competition_id, award FROM participations WHERE animal_id = " + to_string(animalId);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByCompetitionId(int competitionId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT animal_id, competition_id, award FROM participations WHERE competition_id = " + to_string(competitionId);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }


    vector<vector<string>> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT animal_id, competition_id, award FROM participations";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> update(int animalId, int competitionId, string award) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string updateQuery = "UPDATE participations SET award = '" + award + "' WHERE animal_id = " + to_string(animalId)
                         + ", competition_id = " + to_string(competitionId) + " RETURNING animal_id, competition_id, award";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    void remove(int animalId, int competitionId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string deleteQuery = "DELETE FROM participations WHERE animal_id = " + to_string(animalId) + " AND competition_id = " + to_string(competitionId);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }


  private:
    SQLHDBC hDbc;
};