#pragma once
#include <iostream>
#include <stdexcept>
#include <sql.h>
#include "./models/Client.h"
#include "./gateways/ClientDatabaseGateway.h"
#include "./gateways/BreedDatabaseGateway.h"
#include "./gateways/EmployeeDatabaseGateway.h"
#include "./gateways/ApplicationDatabaseGateway.h"

using namespace std;

class Menu {

  public:

    Menu(SQLHDBC hDbc) {
      this-> hDbc;
      this->clientGateway = ClientDatabaseGateway(hDbc);
      this->breedGateway = BreedDatabaseGateway(hDbc);
      this->employeeGateway = EmployeeDatabaseGateway(hDbc);
      this->applicationGateway = ApplicationDatabaseGateway(hDbc);
    }

    void showMenu() {
      int choice;
      while (menuRunning) {
        innerMenuRunning = true;
        cout << "\033[2J\033[H";
        cout << "Menu:\n" <<
                "1. To clients base\n" <<
                "2. To breeds base\n" <<
                "3. To employees base\n" <<
                "4. To applications\n" <<
                "0. Exit\n";
        choice = getChoice(0, 4);
        switch(choice){
          case 1:  //clients menu
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "Clients menu:\n" << 
                      "1. List clients\n" <<
                      "2. New client\n" <<
                      "3. Update client data\n" <<
                      "4. Delete client\n" <<
                      "0. Back\n";
              choice = getChoice(0, 4);
              switch(choice) {
                case 1:
                  printClients();
                  break;
                case 2:
                  addClient();
                  break;
                case 3:
                  updateClient();
                  break;
                case 4:
                  removeClient();
                  break;
                case 0:
                  innerMenuRunning = false;
                  break;
              }
            }
            break;
          case 2: //breeds menu
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "Breed menu:\n" << 
                      "1. List breeds\n" <<
                      "2. New breed\n" <<
                      "3. Update breed data\n" <<
                      "4. Delete breed\n" <<
                      "0. Back\n";
              choice = getChoice(0, 4);
              switch(choice) {
                case 1:
                  printBreeds();
                  break;
                case 2:
                  addBreed();
                  break;
                case 3:
                  updateBreed();
                  break;
                case 4:
                  removeBreed();
                  break;
                case 0:
                  innerMenuRunning = false;
                  break;
              }
            }
            break;
          case 3:
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "Employees menu:\n" << 
                      "1. List employees\n" <<
                      "2. New employee\n" <<
                      "3. Update employee data\n" <<
                      "4. Delete employee\n" <<
                      "5. Find by position\n" <<
                      "0. Back\n";
              choice = getChoice(0, 5);
              switch(choice) {
                case 1:
                  printEmployees();
                  break;
                case 2:
                  addEmployee();
                  break;
                case 3:
                  updateEmployee();
                  break;
                case 4:
                  removeEmployee();
                  break;
                case 5:
                  findEmployeeByPosition();
                  break;
                case 0:
                  innerMenuRunning = false;
                  break;
              }
            }
            break;
          case 4:
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "Applications menu:\n" << 
                      "1. List applications\n" <<
                      "2. New application\n" <<
                      "0. Back\n";
              choice = getChoice(0, 2);
              switch(choice) {
                case 1:
                  printApplications();
                  break;
                case 2:
                  addApplication();
                  break;
                // case 3:
                //   updateClient();
                //   break;
                // case 4:
                //   removeClient();
                //   break;
                case 0:
                  innerMenuRunning = false;
                  break;
              }
            }
            break;
          case 0:
            menuRunning = false;
            break;
        }
      }
    }

  private:
    SQLHDBC hDbc;
    ClientDatabaseGateway clientGateway;
    BreedDatabaseGateway breedGateway;
    EmployeeDatabaseGateway employeeGateway;
    ApplicationDatabaseGateway applicationGateway;
    bool menuRunning = true;
    bool innerMenuRunning = true;

    int getChoice(int low, int high) {
      int choice = 0;
      cin >> choice;
      while (choice > high || choice < low) {
        cout << "Invalid input, try again\n";
        cin >> choice;
      }
      getchar();
      return choice;
    }

    void printClients();
    void addClient();
    void updateClient();
    void removeClient();
    int inputClientId();

    void printBreeds();
    void addBreed();
    void updateBreed();
    void removeBreed();
    int inputBreedId();

    void printEmployees();
    void addEmployee();
    void updateEmployee();
    void removeEmployee();
    void findEmployeeByPosition();
    int inputEmployeeId();

    void printApplications();
    void addApplication();

    Gender* inputGender();
};