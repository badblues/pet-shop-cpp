#pragma once
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

class Competition {

  public:

    Competition(int id, int animalId, string name, string location, tm date, string award) {
      this->id = id;
      this->animalId = animalId;
      this->name = name;
      this->location = location;
      this->date = date;
      this->award = award;
    }

    string toString() {
      ostringstream oss;
      stringstream dateStream;
      dateStream << put_time(&date, "%Y-%m-%d");
      oss << "[id = " << id <<
              "; animalId = " << animalId <<
              "; name = " << name <<
              "; location = " << location <<
              "; date = " << dateStream.str() <<
              "; award = " << award << "];}";
      return oss.str();
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    int getAnimalId() {
      return animalId;
    }

    void setAnimalId(int animalId) {
      this->animalId = animalId;
    }

    string getName() {
        return name;
    }

    void setName(string name) {
        this->name = name;
    }

    string getLocation() {
      return location;
    }

    void setLocation(string location) {
      this->location = location;
    }

    tm getDate() {
      return date;
    }

    void setDate(tm date) {
      this->date = date;
    }

    string getAward() {
      return award;
    }

    void setAward(string award) {
      this->award = award;
    }



  private:
    int id;
    int animalId;
    string name;
    string location;
    tm date;
    string award;
};