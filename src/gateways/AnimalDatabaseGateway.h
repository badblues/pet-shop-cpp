#pragma once
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstring>
#include "../models/Animal.h"

using namespace std;

class AnimalDatabaseGateway {

  public:

    AnimalDatabaseGateway() {
      hDbc = nullptr;
    }

    AnimalDatabaseGateway(SQLHDBC hDbc) {
      this->hDbc = hDbc;
    }

    Animal create(string name, optional<int> age, Gender gender, int breedId, string exterior,
                  string pedigree, string veterinarian, optional<int> ownerId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string genders_str = "";
      genders_str = getGenderString(gender);
      string age_str = age.has_value() ? to_string(age.value()) : "NULL";
      string ownderId_str = ownerId.has_value() ? to_string(ownerId.value()) : "NULL";
      string query = "INSERT INTO animals (name, age, gender, breed_id, exterior_description, pedigree, veterinarian, owner_id) VALUES (?, " + age_str + ", '" + genders_str + "', ?, ?, ?, ?, " + ownderId_str + ")";
      SQLCHAR* insertQuery = new SQLCHAR[query.size() + 1];
      strcpy(reinterpret_cast<char*>(insertQuery), query.c_str());
      SQLPrepare(hStmt, insertQuery, SQL_NTS);

      // Задайте параметры для вставки
      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      SQLINTEGER sqlBreedId = breedId;
      const SQLCHAR* sqlExterior = reinterpret_cast<const SQLCHAR*>(exterior.c_str());
      const SQLCHAR* sqlPedigree = reinterpret_cast<const SQLCHAR*>(pedigree.c_str());
      const SQLCHAR* sqlVeterinarian = reinterpret_cast<const SQLCHAR*>(veterinarian.c_str());
      
      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlBreedId, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlExterior, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlPedigree, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlVeterinarian, 0, NULL);
    
      
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

      return Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    }

    Animal get(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM animals WHERE id = ?";
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

      SQLCHAR sqlName[256];
      SQLINTEGER sqlAge;
      SQLLEN ageIndicator;
      SQLCHAR sqlGender[256];
      SQLINTEGER breedId;
      SQLCHAR sqlExterior[256];
      SQLCHAR sqlPedigree[256];
      SQLCHAR sqlVeterinarian[256];
      SQLINTEGER sqlOwnerId;
      SQLLEN ownerIdIndecator;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 2, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &sqlAge, sizeof(sqlAge), &ageIndicator);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlGender, sizeof(sqlGender), NULL);
      SQLBindCol(hStmt, 5, SQL_C_SLONG, &breedId, sizeof(breedId), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlExterior, sizeof(sqlExterior), NULL);
      SQLBindCol(hStmt, 7, SQL_C_CHAR, sqlPedigree, sizeof(sqlPedigree), NULL);
      SQLBindCol(hStmt, 8, SQL_C_CHAR, sqlVeterinarian, sizeof(sqlVeterinarian), NULL);
      SQLBindCol(hStmt, 9, SQL_C_SLONG, &sqlOwnerId, sizeof(sqlOwnerId), &ownerIdIndecator);

      // Извлекаем данные из результирующего набора
      SQLFetch(hStmt);
      string name = reinterpret_cast<char*>(sqlName);
      optional<int> age;
      if (ageIndicator != SQL_NULL_DATA)
        age = sqlAge;
      Gender gender = getGender(reinterpret_cast<char*>(sqlGender));
      string exterior = reinterpret_cast<char*>(sqlExterior);
      string pedigree = reinterpret_cast<char*>(sqlPedigree);
      string veterinarian = reinterpret_cast<char*>(sqlVeterinarian);
      optional<int> ownerId;
      if (ownerIdIndecator != SQL_NULL_DATA)
        ownerId = sqlOwnerId;

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

      return Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    }

    vector<Animal> findByName(string name) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM animals WHERE name = ?";
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

      vector<Animal> animals;

      SQLINTEGER id;
      SQLINTEGER sqlAge;
      SQLLEN ageIndicator;
      SQLCHAR sqlGender[256];
      SQLINTEGER breedId;
      SQLCHAR sqlExterior[256];
      SQLCHAR sqlPedigree[256];
      SQLCHAR sqlVeterinarian[256];
      SQLINTEGER sqlOwnerId;
      SQLLEN ownerIdIndecator;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &sqlAge, sizeof(sqlAge), &ageIndicator);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlGender, sizeof(sqlGender), NULL);
      SQLBindCol(hStmt, 5, SQL_C_SLONG, &breedId, sizeof(breedId), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlExterior, sizeof(sqlExterior), NULL);
      SQLBindCol(hStmt, 7, SQL_C_CHAR, sqlPedigree, sizeof(sqlPedigree), NULL);
      SQLBindCol(hStmt, 8, SQL_C_CHAR, sqlVeterinarian, sizeof(sqlVeterinarian), NULL);
      SQLBindCol(hStmt, 9, SQL_C_SLONG, &sqlOwnerId, sizeof(sqlOwnerId), &ownerIdIndecator);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        optional<int> age;
        if (ageIndicator != SQL_NULL_DATA)
          age = sqlAge;
        Gender gender = getGender(reinterpret_cast<char*>(sqlGender));
        string exterior = reinterpret_cast<char*>(sqlExterior);
        string pedigree = reinterpret_cast<char*>(sqlPedigree);
        string veterinarian = reinterpret_cast<char*>(sqlVeterinarian);
        optional<int> ownerId;
        if (ownerIdIndecator != SQL_NULL_DATA)
          ownerId = sqlOwnerId;
        Animal animal = Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
        animals.push_back(animal);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return animals;
    }

    vector<Animal> findByClient(optional<int> ownerId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string ownderId_str = ownerId.has_value() ? " = " + to_string(ownerId.value()) : "IS NULL";
      string query = "SELECT * FROM animals WHERE owner_id " + ownderId_str + ";";
      SQLCHAR* selectQuery = new SQLCHAR[query.size() + 1];
      strcpy(reinterpret_cast<char*>(selectQuery), query.c_str());
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

      vector<Animal> animals;

      SQLINTEGER id;
      SQLCHAR sqlName[256];
      SQLINTEGER sqlAge;
      SQLLEN ageIndicator;
      SQLCHAR sqlGender[256];
      SQLINTEGER breedId;
      SQLCHAR sqlExterior[256];
      SQLCHAR sqlPedigree[256];
      SQLCHAR sqlVeterinarian[256];
      SQLINTEGER sqlOwnerId;
      SQLLEN ownerIdIndecator;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 2, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &sqlAge, sizeof(sqlAge), &ageIndicator);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlGender, sizeof(sqlGender), NULL);
      SQLBindCol(hStmt, 5, SQL_C_SLONG, &breedId, sizeof(breedId), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlExterior, sizeof(sqlExterior), NULL);
      SQLBindCol(hStmt, 7, SQL_C_CHAR, sqlPedigree, sizeof(sqlPedigree), NULL);
      SQLBindCol(hStmt, 8, SQL_C_CHAR, sqlVeterinarian, sizeof(sqlVeterinarian), NULL);
      SQLBindCol(hStmt, 9, SQL_C_SLONG, &sqlOwnerId, sizeof(sqlOwnerId), &ownerIdIndecator);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        string name = reinterpret_cast<char*>(sqlName);
        optional<int> age;
        if (ageIndicator != SQL_NULL_DATA)
          age = sqlAge;
        Gender gender = getGender(reinterpret_cast<char*>(sqlGender));
        string exterior = reinterpret_cast<char*>(sqlExterior);
        string pedigree = reinterpret_cast<char*>(sqlPedigree);
        string veterinarian = reinterpret_cast<char*>(sqlVeterinarian);
        optional<int> ownerId;
        if (ownerIdIndecator != SQL_NULL_DATA)
          ownerId = sqlOwnerId;
        Animal animal = Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
        animals.push_back(animal);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return animals;
    }

    

    vector<Animal> findByBreed(int breedId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM animals WHERE breed_id = ?";
      SQLPrepare(hStmt, selectQuery, SQL_NTS);

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &breedId, 0, NULL);

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

      vector<Animal> animals;

      SQLINTEGER id;
      SQLCHAR sqlName[256];
      SQLINTEGER sqlAge;
      SQLLEN ageIndicator;
      SQLCHAR sqlGender[256];
      SQLCHAR sqlExterior[256];
      SQLCHAR sqlPedigree[256];
      SQLCHAR sqlVeterinarian[256];
      SQLINTEGER sqlOwnerId;
      SQLLEN ownerIdIndecator;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &sqlAge, sizeof(sqlAge), &ageIndicator);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlGender, sizeof(sqlGender), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlExterior, sizeof(sqlExterior), NULL);
      SQLBindCol(hStmt, 7, SQL_C_CHAR, sqlPedigree, sizeof(sqlPedigree), NULL);
      SQLBindCol(hStmt, 8, SQL_C_CHAR, sqlVeterinarian, sizeof(sqlVeterinarian), NULL);
      SQLBindCol(hStmt, 9, SQL_C_SLONG, &sqlOwnerId, sizeof(sqlOwnerId), &ownerIdIndecator);

      // Извлекаем данные из результирующего набора
      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        string name = reinterpret_cast<char*>(sqlName);
        optional<int> age;
        if (ageIndicator != SQL_NULL_DATA)
          age = sqlAge;
        Gender gender = getGender(reinterpret_cast<char*>(sqlGender));
        string exterior = reinterpret_cast<char*>(sqlExterior);
        string pedigree = reinterpret_cast<char*>(sqlPedigree);
        string veterinarian = reinterpret_cast<char*>(sqlVeterinarian);
        optional<int> ownerId;
        if (ownerIdIndecator != SQL_NULL_DATA)
          ownerId = sqlOwnerId;
        Animal animal = Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
        animals.push_back(animal);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return animals;
    }

    vector<Animal> getAll() {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* selectQuery = (SQLCHAR*)"SELECT * FROM animals;";
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

      vector<Animal> animals;

      SQLINTEGER id;
      SQLCHAR sqlName[256];
      SQLINTEGER sqlAge;
      SQLLEN ageIndicator;
      SQLCHAR sqlGender[256];
      SQLINTEGER breedId;
      SQLCHAR sqlExterior[256];
      SQLCHAR sqlPedigree[256];
      SQLCHAR sqlVeterinarian[256];
      SQLINTEGER sqlOwnerId;
      SQLLEN ownerIdIndecator;

      // Привязываем столбцы результата к переменным
      SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, sizeof(id), NULL);
      SQLBindCol(hStmt, 2, SQL_C_CHAR, sqlName, sizeof(sqlName), NULL);
      SQLBindCol(hStmt, 3, SQL_C_SLONG, &sqlAge, sizeof(sqlAge), &ageIndicator);
      SQLBindCol(hStmt, 4, SQL_C_CHAR, sqlGender, sizeof(sqlGender), NULL);
      SQLBindCol(hStmt, 5, SQL_C_SLONG, &breedId, sizeof(breedId), NULL);
      SQLBindCol(hStmt, 6, SQL_C_CHAR, sqlExterior, sizeof(sqlExterior), NULL);
      SQLBindCol(hStmt, 7, SQL_C_CHAR, sqlPedigree, sizeof(sqlPedigree), NULL);
      SQLBindCol(hStmt, 8, SQL_C_CHAR, sqlVeterinarian, sizeof(sqlVeterinarian), NULL);
      SQLBindCol(hStmt, 9, SQL_C_SLONG, &sqlOwnerId, sizeof(sqlOwnerId), &ownerIdIndecator);

      while (SQLFetch(hStmt) == SQL_SUCCESS) {
        string name = reinterpret_cast<char*>(sqlName);
        optional<int> age;
        if (ageIndicator != SQL_NULL_DATA)
          age = sqlAge;
        Gender gender = getGender(reinterpret_cast<char*>(sqlGender));
        string exterior = reinterpret_cast<char*>(sqlExterior);
        string pedigree = reinterpret_cast<char*>(sqlPedigree);
        string veterinarian = reinterpret_cast<char*>(sqlVeterinarian);
        optional<int> ownerId;
        if (ownerIdIndecator != SQL_NULL_DATA)
          ownerId = sqlOwnerId;
        Animal animal = Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
        animals.push_back(animal);
      }

      SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
      return animals;
    }

    Animal update(int id, string name, optional<int> age, Gender gender, int breedId, string exterior,
                  string pedigree, string veterinarian, optional<int> ownerId) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      string genders_str = "";
      genders_str = getGenderString(gender);
      string age_str = age.has_value() ? to_string(age.value()) : "NULL";
      string ownderId_str = ownerId.has_value() ? to_string(ownerId.value()) : "NULL";

      string query = "UPDATE animals SET name = ?, age = " + age_str + ", gender = '" + genders_str +
                     "', breed_id = ?, exterior_description = ?, pedigree = ?, veterinarian = ?, owner_id = " + ownderId_str + 
                     " WHERE id = ?;";
      SQLCHAR* updateQuery = new SQLCHAR[query.size() + 1];
      strcpy(reinterpret_cast<char*>(updateQuery), query.c_str());
      SQLPrepare(hStmt, updateQuery, SQL_NTS);

      // Задайте параметры для обновления
      const SQLCHAR* sqlName = reinterpret_cast<const SQLCHAR*>(name.c_str());
      SQLINTEGER sqlBreedId = breedId;
      const SQLCHAR* sqlExterior = reinterpret_cast<const SQLCHAR*>(exterior.c_str());
      const SQLCHAR* sqlPedigree = reinterpret_cast<const SQLCHAR*>(pedigree.c_str());
      const SQLCHAR* sqlVeterinarian = reinterpret_cast<const SQLCHAR*>(veterinarian.c_str());

      SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlName, 0, NULL);
      SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &sqlBreedId, 0, NULL);
      SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlExterior, 0, NULL);
      SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlPedigree, 0, NULL);
      SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)sqlVeterinarian, 0, NULL);
      SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

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

      return Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    }

    void remove(int id) {
      SQLHSTMT hStmt;
      SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

      SQLCHAR* deleteQuery = (SQLCHAR*)"DELETE FROM animals WHERE id = ?";
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

    string getGenderString(Gender gender) {
      return gender == Gender::male ? "male" : "female";
    }

    Gender getGender(string gender) {
      return gender == "male" ? Gender::male : Gender::female;
    }
};