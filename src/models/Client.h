#pragma once
#include <string>
#include <sstream>

using namespace std;

class Client {

  public:

    Client(int id, string name, string address) {
      this->id = id;
      this->name = name;
      this->address = address;
    }

    string toString() {
        std::ostringstream oss;
        oss << "[id = " << id <<
               "; name = '" << name <<
               "'; address = '" << address << "'];}";
        return oss.str();
    }

    int getId() {
      return id;
    }

    string getName() {
      return name;
    }

    void setName(string name) {
      this->name = name;
    }

    string getAddress() {
      return address;
    }

    void setAddress(string address) {
      this->address = address;
    }

  private:
    int id;
    string name;
    string address;
    
};