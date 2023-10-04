#pragma once
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <optional>
#include "Gender.h"

class Animal {

  public:

    Animal(int id, string name, optional<int> age, Gender gender, int breedId, string exterior,
           string pedigree, string veterinarian, optional<int> ownerId) {
      this->id = id;
      this->name = name;
      this->age = age;
      this->gender = gender;
      this->breedId = breedId;
      this->exterior = exterior;
      this->pedigree = pedigree;
      this->veterinarian = veterinarian;
      this->ownerId = ownerId;
    }

    string toString() {
      ostringstream oss;
      string genderStr = gender == Gender::male ? "male" : "female";
      oss << "[id = " << id <<
              "; name = " << name <<
              "; age = " << (age.has_value() ? to_string(age.value()) : "") <<
              "; gender = " << genderStr <<
              "; breedId = " << breedId <<
              "; exterior = " << exterior <<
              "; pedigree = " << pedigree <<
              "; veterinarian = " << veterinarian <<
              "; ownerId = " << (ownerId.has_value() ? to_string(ownerId.value()) : "") << "];}";
      return oss.str();
    }

    int getId() const {
        return id;
    }

    void setId(int value) {
        id = value;
    }

    const string getName() const {
        return name;
    }

    void setName(string name) {
        this->name = name;
    }

    optional<int> getAge() const {
        return age;
    }

    void setAge(const optional<int>& value) {
        age = value;
    }

    Gender getGender() const {
        return gender;
    }

    void setGender(Gender value) {
        gender = value;
    }

    int getBreedId() const {
        return breedId;
    }

    void setBreedId(int value) {
        breedId = value;
    }

    string getExterior() const {
        return exterior;
    }

    void setExterior(const string& value) {
        exterior = value;
    }

    string getPedigree() const {
        return pedigree;
    }

    void setPedigree(const string& value) {
        pedigree = value;
    }

    string getVeterinarian() const {
        return veterinarian;
    }

    void setVeterinarian(const string& value) {
        veterinarian = value;
    }

    optional<int> getOwnerId() const {
        return ownerId;
    }

    void setOwnerId(const optional<int>& value) {
        ownerId = value;
    }

  private:
    int id;
    string name;
    optional<int> age;
    Gender gender;
    int breedId;
    string exterior;
    string pedigree;
    string veterinarian;
    optional<int> ownerId;

};