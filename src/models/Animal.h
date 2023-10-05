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

    int getId() {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    string getName() {
        return name;
    }

    void setName(string name) {
        this->name = name;
    }

    optional<int> getAge() {
        return age;
    }

    void setAge(optional<int> age) {
        this->age = age;
    }

    Gender getGender() {
        return gender;
    }

    void setGender(Gender gender) {
        this->gender = gender;
    }

    int getBreedId() {
        return breedId;
    }

    void setBreedId(int breedId) {
        this->breedId = breedId;
    }

    string getExterior() {
        return exterior;
    }

    void setExterior(string exterior) {
        this->exterior = exterior;
    }

    string getPedigree() {
        return pedigree;
    }

    void setPedigree (string pedigree) {
        this->pedigree = pedigree;
    }

    string getVeterinarian() {
        return veterinarian;
    }

    void setVeterinarian(string veterinarian) {
        this->veterinarian = veterinarian;
    }

    optional<int> getOwnerId() {
        return ownerId;
    }

    void setOwnerId(optional<int> ownerId) {
        this->ownerId = ownerId;
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