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
        oss << "{id = " << id <<
            "; name = " << name <<
            "; address = " << address << "}";
        return oss.str();
    }

  private:
    int id;
    string name;
    string address;
    
};