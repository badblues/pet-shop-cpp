#pragma once
#include <string>
#include <sstream>

using namespace std;

class Employee {

  public:

    Employee(int id, string name, string address, string position, double salary) {
      this->id = id;
      this->name = name;
      this->address = address;
      this->position = position;
      this->salary = salary;
    }

    string toString() {
        std::ostringstream oss;
        oss << "[id = " << id <<
               "; name = '" << name <<
               "'; address = '" << address <<
               "'; position = '" << position <<
               "'; salary = " << salary << "];}";
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

    string getPosition() {
      return position;
    }

    void setPosition(string position) {
      this->position = position;
    }

    double getSalary() {
      return salary;
    }

    void setSalary(double salary) {
      this->salary = salary;
    }

  private:
    int id;
    string name;
    string address;
    string position;
    double salary;
    
};