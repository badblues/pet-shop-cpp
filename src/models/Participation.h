#pragma once
#include <string>
#include <sstream>

class Participation {

  public:

    Participation() {}

    Participation(int animalId, int competitionId,string award) {
      this->animalId = animalId;
      this->competitionId = competitionId;
      this->award = award;
    }

    string toString() {
      ostringstream oss;
      oss << "[animalId = " << animalId <<
              "; competitionId = " << competitionId <<
              "; award = " << award << "];}";
      return oss.str();
    }

    int getAnimalId() {
      return animalId;
    }

    void setAnimalId(int animalId) {
      this->animalId = animalId;
    }

    int getCompetitionId() const {
        return competitionId;
    }

    void setCompetitionId(int competitionId) {
        this->competitionId = competitionId;
    }

    string getAward() {
      return award;
    }

    void setAward(string award) {
      this->award = award;
    }

  private:
    int animalId;
    int competitionId;
    string award;
};