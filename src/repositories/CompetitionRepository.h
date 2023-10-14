#pragma once
#include "../gateways/CompetitionDatabaseGateway.h"
#include "../models/Competition.h"

class CompetitionRepository {

  public:

    CompetitionRepository(CompetitionDatabaseGateway competitionDatabaseGateway) {
      gateway = competitionDatabaseGateway;
    }

    CompetitionRepository() {
      gateway = nullptr;
    }

    Competition add(string name, string location, tm date) {
      vector<vector<string>> competitionStrings = gateway.create(name, location, date);
      return parseCompetition(competitionStrings[0]);
    }

    Competition get(int id) {
      vector<vector<string>> competitionStrings = gateway.get(id);
      return parseCompetition(competitionStrings[0]);
    } 

    Competition update(int id, string name, string location, tm date) {
      vector<vector<string>> competitionStrings = gateway.update(id, name, location, date);
      return parseCompetition(competitionStrings[0]);
    }

    void remove(int id) {
      gateway.remove(id);
    }

    vector<Competition> getAll() {
      vector<vector<string>> result = gateway.getAll();
      return parseCompetitions(result);
    }

    vector<Competition> findByName(string name) {
      vector<vector<string>> result = gateway.findByName(name);
      return parseCompetitions(result);
    }

  private:
    CompetitionDatabaseGateway gateway;

    vector<Competition> parseCompetitions(vector<vector<string>> competitionsStrings) {
      vector<Competition> competitions;
      for (int i = 0; i < competitionsStrings.size(); i++) {
        competitions.push_back(parseCompetition(competitionsStrings[i]));
      }
      return competitions;
    }

    Competition parseCompetition(vector<string> strings) {
      int id = atoi(strings[0].c_str());
      string name = strings[1];
      string location = strings[2];
      tm date = getDate(strings[3]);
      return Competition(id, name, location, date);
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