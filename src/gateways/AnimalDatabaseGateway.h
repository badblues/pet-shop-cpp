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

class AnimalDatabaseGateway : DatabaseGateway {

  public:

    AnimalDatabaseGateway() {
      hDbc = nullptr;
    }

    AnimalDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    vector<vector<string>> create(string name, optional<int> age, Gender gender, int breedId,
                                  string exterior, string pedigree, string veterinarian,
                                  optional<int> ownerId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string ageStr = age.has_value() ? to_string(age.value()) : "NULL";
      string genderStr = getGenderString(gender);
      string ownerIdStr = ownerId.has_value() ? to_string(ownerId.value()) : "NULL";
      string insertQuery = "INSERT INTO animals (name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id)"
                           "VALUES ('" + name + "', " + ageStr + ", " + genderStr + ", " + to_string(breedId) + ", '" + exterior + "', '"
                         + pedigree + "', '" + veterinarian + "', " + ownerIdStr + ") "
                           " RETURNING id, name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)insertQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);
      
      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id "
                           " FROM animals WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id "
                           " FROM animals WHERE name = '" + name + "'";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByClientId(optional<int> ownerId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string ownerIdStr = ownerId.has_value() ? "= " + to_string(ownerId.value()) : "IS NULL";
      string selectQuery = "SELECT id, name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id "
                           " FROM animals WHERE owner_id " + ownerIdStr;            
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> findByBreedId(int breedId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id "
                           " FROM animals WHERE breed_id = " + to_string(breedId); 
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string selectQuery = "SELECT id, name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id FROM animals";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)selectQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    vector<vector<string>> update(int id, string name, optional<int> age, Gender gender,
                                  int breedId, string exterior, string pedigree,
                                  string veterinarian, optional<int> ownerId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string ageStr = age.has_value() ? to_string(age.value()) : "NULL";
      string genderStr = getGenderString(gender);
      string ownerIdStr = ownerId.has_value() ? to_string(ownerId.value()) : "NULL";
      string updateQuery = "UPDATE animals SET name = '" + name + "', age = " + ageStr + ", gender = " + genderStr + ", "
                           " breed_id = " + to_string(breedId) + ", exterior_description = '" + exterior
                         + "', pedigree = '" + pedigree + "', veterinarian = '" + veterinarian + "', owner_id = " + ownerIdStr
                         + "  WHERE id = " + to_string(id) + " RETURNING id, name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id";
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)updateQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      vector<vector<string>> result = fetchData(hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return result;
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string deleteQuery = "DELETE FROM animals WHERE id = " + to_string(id);
      SQLRETURN sqlReturn = SQLExecDirect(hStmt, (SQLCHAR*)deleteQuery.c_str(), SQL_NTS);
      handleSQLReturn(sqlReturn, hStmt);

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

  private:
    SQLHDBC hDbc;
};