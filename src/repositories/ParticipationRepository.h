#pragma once
#include "../gateways/ParticipationDatabaseGateway.h"
#include "../models/Participation.h"

class ParticipationRepository {

  public:

    ParticipationRepository(ParticipationDatabaseGateway participationDatabaseGateway) {
      gateway = participationDatabaseGateway;
    }

    ParticipationRepository() {
      gateway = nullptr;
    }

    Participation add(int animalId, int competitionId, string award) {
      vector<vector<string>> participationStrings = gateway.create(animalId, competitionId, award);
      return parseParticipation(participationStrings[0]);
    }

    Participation get(int animalId, int competitionId) {
      vector<vector<string>> participationStrings = gateway.get(animalId, competitionId);
      return parseParticipation(participationStrings[0]);
    } 

    Participation update(int animalId, int competitionId, string award) {
      vector<vector<string>> participationStrings = gateway.update(animalId, competitionId, award);
      return parseParticipation(participationStrings[0]);
    }

    void remove(int animalId, int competitionId) {
      gateway.remove(animalId, competitionId);
    }

    vector<Participation> getAll() {
      vector<vector<string>> result = gateway.getAll();
      return parseParticipations(result);
    }

    vector<Participation> findByAward(string award) {
      vector<vector<string>> result = gateway.findByAward(award);
      return parseParticipations(result);
    }

    vector<Participation> findByAnimalId(int animalId) {
      vector<vector<string>> result = gateway.findByAnimalId(animalId);
      return parseParticipations(result);
    }

    vector<Participation> findByCompetitionId(int competitionId) {
      vector<vector<string>> result = gateway.findByCompetitionId(competitionId);
      return parseParticipations(result);
    }

  private:
    ParticipationDatabaseGateway gateway;

    vector<Participation> parseParticipations(vector<vector<string>> participationsStrings) {
      vector<Participation> participations;
      for (int i = 0; i < participationsStrings.size(); i++) {
        participations.push_back(parseParticipation(participationsStrings[i]));
      }
      return participations;
    }

    Participation parseParticipation(vector<string> strings) {
      int animalId = atoi(strings[0].c_str());
      int competitionId = atoi(strings[1].c_str());
      string award = strings[2];
      return Participation(animalId, competitionId, award);
    }
     
};