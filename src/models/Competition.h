#pragma once
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

class Competition {

  public:

    Competition(int id, string name, string location, tm date) {
      this->id = id;
      this->name = name;
      this->location = location;
      this->date = date;
    }

    string toString() {
      ostringstream oss;
      stringstream dateStream;
      dateStream << put_time(&date, "%Y-%m-%d");
      oss << "[id = " << id <<
              "; name = " << name <<
              "; location = " << location <<
              "; date = " << dateStream.str() << "];}";
      return oss.str();
    }

    int getId() const {
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

  private:
    int id;
    string name;
    string location;
    tm date;
};