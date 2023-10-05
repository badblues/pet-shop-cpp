#pragma once
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <optional>
#include "Gender.h"

class Application {

  public:

    Application(int id, int clientId, optional<int> employeeId, int breedId, optional<Gender> gender, tm applicationDate, bool completed) {
      this->id = id;
      this->clientId = clientId;
      this->employeeId = employeeId;
      this->breedId = breedId;
      this->gender = gender;
      this->applicationDate = applicationDate;
      this->completed = completed;
    }

    string toString() {
      ostringstream oss;
      string genderStr = "";
      if (gender.has_value())
        genderStr = gender.value() == Gender::male ? "male" : "female";
      stringstream dateStream;
      dateStream << put_time(&applicationDate, "%Y-%m-%d");
      oss << "[id = " << id <<
              "; clientId = " << clientId <<
              "; employeeId = " << (employeeId.has_value() ? to_string(employeeId.value()) : "") <<
              "; breedId = " << breedId <<
              "; gender = " << genderStr <<
              "; applicationDate = " << dateStream.str() <<
              "; completed = " << completed << "];}";
      return oss.str();
    }

    int getId() {
      return id;
    }

    int getClientId() {
      return clientId;
    }

    void setClientId(int clientId) {
      this->clientId = clientId;
    }

    optional<int> getEmployeeId() {
      return employeeId;
    }

    void setEmployeeId(optional<int> employeeId) {
      this->employeeId = employeeId;
    }

    int getBreedId() {
      return breedId;
    }

    void setBreedId(int breedId) {
      this->breedId = breedId;
    }

    optional<Gender> getGender() {
      return gender;
    }

    void setGender(optional<Gender> gender) {
      this->gender = gender;
    }

    tm getApplicationDate() {
      return applicationDate;
    }

    void setApplicationDate(tm applicationDate) {
      this->applicationDate = applicationDate;
    }

    bool getCompleted() {
      return completed;
    }

    void setCompleted(bool completed) {
      this->completed = completed;
    }

  private:
    int id;
    int clientId;
    optional<int> employeeId;
    int breedId;
    optional<Gender> gender;
    tm applicationDate;
    bool completed;
    
};