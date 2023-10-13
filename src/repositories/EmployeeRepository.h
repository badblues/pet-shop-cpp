#pragma once
#include "../gateways/EmployeeDatabaseGateway.h"
#include "../models/Employee.h"

class EmployeeRepository {

  public:

    EmployeeRepository(EmployeeDatabaseGateway employeeDatabaseGateway) {
      gateway = employeeDatabaseGateway;
    }

    EmployeeRepository() {
      gateway = nullptr;
    }

    Employee add(string name, string address, string position, double salary) {
      vector<vector<string>> employeeStrings = gateway.create(name, address, position, salary);
      return parseEmployee(employeeStrings[0]);
    }

    Employee get(int id) {
      vector<vector<string>> employeeStrings = gateway.get(id);
      return parseEmployee(employeeStrings[0]);
    } 

    Employee update(int id, string name, string address, string position, double salary) {
      vector<vector<string>> employeeStrings = gateway.update(id, name, address, position, salary);
      return parseEmployee(employeeStrings[0]);
    }

    void remove(int id) {
      gateway.remove(id);
    }

    vector<Employee> getAll() {
      vector<vector<string>> result = gateway.getAll();
      return parseEmployees(result);
    }

    vector<Employee> findByName(string name) {
      vector<vector<string>> result = gateway.findByName(name);
      return parseEmployees(result);
    }

    vector<Employee> findByPosition(string position) {
      vector<vector<string>> result = gateway.findByPosition(position);
      return parseEmployees(result);
    }

  private:
    EmployeeDatabaseGateway gateway;

    vector<Employee> parseEmployees(vector<vector<string>> employeesStrings) {
      vector<Employee> employees;
      for (int i = 0; i < employeesStrings.size(); i++) {
        employees.push_back(parseEmployee(employeesStrings[i]));
      }
      return employees;
    }

    Employee parseEmployee(vector<string> strings) {
      int id = atoi(strings[0].c_str());
      string name = strings[1];
      string address = strings[2];
      string position = strings[3];
      double salary = atof(strings[4].c_str()) / 100;
      return Employee(id, name, address, position, salary);
    }
     
};