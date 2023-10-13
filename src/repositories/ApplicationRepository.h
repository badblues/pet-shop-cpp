#pragma once
#include "../gateways/ApplicationDatabaseGateway.h"
#include "../models/Application.h"

class ApplicationRepository {

  public:

    ApplicationRepository(ApplicationDatabaseGateway applicationDatabaseGateway) {
      gateway = applicationDatabaseGateway;
    }

    ApplicationRepository() {
      gateway = nullptr;
    }

    Application add(int clientId, int employeeId, int breedId, optional<Gender> gender, tm applicationDate, bool completed) {
      vector<vector<string>> applicationStrings = gateway.create(clientId, employeeId, breedId, gender, applicationDate, completed);
      return parseApplication(applicationStrings[0]);
    }

    Application get(int id) {
      vector<vector<string>> applicationStrings = gateway.get(id);
      return parseApplication(applicationStrings[0]);
    } 

    Application update(int id, int clientId, int employeeId, int breedId, optional<Gender> gender, tm applicationDate, bool completed) {
      vector<vector<string>> applicationStrings = gateway.update(id, clientId, employeeId, breedId, gender, applicationDate, completed);
      return parseApplication(applicationStrings[0]);
    }

    void remove(int id) {
      gateway.remove(id);
    }

    Application removeEmployee(int id) {
      vector<vector<string>> applicationStrings = gateway.removeEmployeeFromApplication(id);
      return parseApplication(applicationStrings[0]);
    }

    vector<Application> getAll() {
      vector<vector<string>> result = gateway.getAll();
      return parseApplications(result);
    }

    vector<Application> findByClientId(int clientId) {
      vector<vector<string>> result = gateway.findByClientId(clientId);
      return parseApplications(result);
    }

    vector<Application> findByEmployeeId(optional<int> employeeId) {
      vector<vector<string>> result = gateway.findByEmployeeId(employeeId);
      return parseApplications(result);
    }

    vector<Application> findByBreedId(int breedId) {
      vector<vector<string>> result = gateway.findByBreedId(breedId);
      return parseApplications(result);
    }

  private:
    ApplicationDatabaseGateway gateway;

    vector<Application> parseApplications(vector<vector<string>> applicationsStrings) {
      vector<Application> applications;
      for (int i = 0; i < applicationsStrings.size(); i++) {
        applications.push_back(parseApplication(applicationsStrings[i]));
      }
      return applications;
    }

    Application parseApplication(vector<string> strings) {
      int id = atoi(strings[0].c_str());
      int clientId = atoi(strings[1].c_str());
      int employeeId = atoi(strings[2].c_str());
      int breedId = atoi(strings[3].c_str());
      optional<Gender> gender;
      if (strcmp(strings[3].c_str(), strings[4].c_str()) != 0)
        gender = strings[4] == "male" ? Gender::male : Gender::female;
      tm applicationDate = getDate(strings[5]);
      bool completed = strings[6] == "1";
      return Application(id, clientId, employeeId, breedId, gender, applicationDate, completed);
    }

    Gender getGender(string gender) {
      return gender == "male" ? Gender::male : Gender::female;
    }

    tm getDate(string dateStr) {
      tm date = {};
      istringstream dateStream(dateStr.c_str());
      dateStream >> get_time(&date, "%Y-%m-%d");
      if (dateStream.fail()) {
          throw std::invalid_argument("Invalid date string");
      }
      return date;
    }
     
};