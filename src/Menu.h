#pragma once
#include <iostream>
#include <stdexcept>
#include <sql.h>
#include "./models/Client.h"
#include "./gateways/ClientDatabaseGateway.h"
#include "./gateways/BreedDatabaseGateway.h"
#include "./gateways/EmployeeDatabaseGateway.h"
#include "./gateways/ApplicationDatabaseGateway.h"
#include "./gateways/AnimalDatabaseGateway.h"
#include "./gateways/CompetitionDatabaseGateway.h"

using namespace std;

class Menu {

  public:

    Menu(SQLHDBC hDbc) {
      this-> hDbc;
      this->clientGateway = ClientDatabaseGateway(hDbc);
      this->breedGateway = BreedDatabaseGateway(hDbc);
      this->employeeGateway = EmployeeDatabaseGateway(hDbc);
      this->applicationGateway = ApplicationDatabaseGateway(hDbc);
      this->animalGateway = AnimalDatabaseGateway(hDbc);
      this->competitionGateway = CompetitionDatabaseGateway(hDbc);
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
                "5. To animals\n" <<
                "6. To competitions\n" <<
                "0. Exit\n";
        choice = getChoice(0, 6);
        switch(choice){
          case 1:  //clients menu
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "Clients menu:\n" << 
                      "1. List clients\n" <<
                      "2. New client\n" <<
                      "3. Change client's name\n" <<
                      "4. Change client's address\n" <<
                      "5. Delete client\n" <<
                      "0. Back\n";
              choice = getChoice(0, 5);
              switch(choice) {
                case 1:
                  listClients();
                  break;
                case 2:
                  addClient();
                  break;
                case 3:
                  changeClientName();
                  break;
                case 4:
                  changeClientAddress();
                  break;
                case 5:
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
                      "3. Change breed's name\n" <<
                      "4. Delete breed\n" <<
                      "0. Back\n";
              choice = getChoice(0, 4);
              switch(choice) {
                case 1:
                  listBreeds();
                  break;
                case 2:
                  addBreed();
                  break;
                case 3:
                  changeBreedName();
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
                      "3. Change employee's name\n" <<
                      "4. Change employee's address\n" <<
                      "5. Change employee's position\n" <<
                      "6. Change employee's salary\n" <<
                      "7. Delete employee\n" <<
                      "8. Find by position\n" <<
                      "0. Back\n";
              choice = getChoice(0, 8);
              switch(choice) {
                case 1:
                  listEmployees();
                  break;
                case 2:
                  addEmployee();
                  break;
                case 3:
                  changeEmployeeName();
                  break;
                case 4:
                  changeEmployeeAddress();
                  break;
                case 5:
                  changeEmployeePosition();
                  break;
                case 6:
                  changeEmployeeSalary();
                  break;
                case 7:
                  removeEmployee();
                  break;
                case 8:
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
                      "3. Change breed in application\n" <<
                      "4. Change gender in application\n" <<
                      "5. Close application\n" <<
                      "6. Delete application\n" <<
                      "7. Find by client\n" <<
                      "8. Find by employee\n" <<
                      "9. Find by breed\n" <<
                      "0. Back\n";
              choice = getChoice(0, 9);
              switch(choice) {
                case 1:
                  listApplications();
                  break;
                case 2:
                  addApplication();
                  break;
                case 3:
                  changeApplicationBreed();
                  break;
                case 4:
                  changeApplicationGender();
                  break;
                case 5:
                  closeApplication();
                  break;
                case 6:
                  removeApplication();
                  break;
                case 7:
                  listApplicationsByClient();
                  break;
                case 8:
                  listApplicationsByEmployee();
                  break;
                case 9:
                  listApplicationsByBreed();
                  break;
                case 0:
                  innerMenuRunning = false;
                  break;
              }
            }
            break;
          case 5:
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "Animals menu:\n" << 
                      "1. List animals\n" <<
                      "2. New animal\n" <<
                      "3. Change name\n" <<
                      "4. Change age\n" <<
                      "5. Change gender\n" <<
                      "6. Change breed\n" <<
                      "7. Change exterior\n" <<
                      "8. Change pedigree\n" <<
                      "9. Change veterinarian\n" <<
                      "10. Change owner\n" <<
                      "11. Delete animal\n" <<
                      "12. List animals by breed\n" <<
                      "13. List animals by owner\n" <<
                      "0. Back\n";
              choice = getChoice(0, 13);
              switch(choice) {
                case 1:
                  listAnimals();
                  break;
                case 2:
                  addAnimal();
                  break;
                case 3:
                  changeAnimalName();
                  break;
                case 4:
                  changeAnimalAge();
                  break;
                case 5:
                  changeAnimalGender();
                  break;
                case 6:
                  changeAnimalBreed();
                  break;
                case 7:
                  changeAnimalExterior();
                  break;
                case 8:
                  changeAnimalPedigree();
                  break;
                case 9:
                  changeAnimalVeterinarian();
                  break;
                case 10:
                  changeAnimalOwner();
                  break;
                case 11:
                  removeAnimal();
                  break;
                case 12:
                  listAnimalsByBreed();
                  break;
                case 13:
                  listAnimalsByOwner();
                  break;
                case 0:
                  innerMenuRunning = false;
                  break;
              }
            }
            break;
          case 6:
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "Competitions menu:\n" << 
                      "1. List competitions\n" <<
                      "2. New competition\n" <<
                      "3. Change competition name\n" <<
                      "4. Change competition location\n" <<
                      "5. Change competition date\n" <<
                      "6. Change competition award\n" <<
                      "7. Delete competition\n" <<
                      "8. Find by animal\n" <<
                      "9. Find by award\n" <<
                      "10. Find by name\n" <<
                      "0. Back\n";
              choice = getChoice(0, 10);
              switch(choice) {
                case 1:
                  listCompetitions();
                  break;
                case 2:
                  addCompetition();
                  break;
                case 3:
                  changeCompetitionName();
                  break;
                case 4:
                  changeCompetitionLocation();
                  break;
                case 5:
                  changeCompetitionDate();
                  break;
                case 6:
                  changeCompetitionAward();
                  break;
                case 7:
                  removeCompetition();
                  break;
                case 8:
                  findCompetitionByAnimal();
                  break;
                case 9:
                  findCompetitionByAward();
                  break;
                case 10:
                  findCompetitionByName();
                  break;
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

  void createTables();

  private:
    SQLHDBC hDbc;
    ClientDatabaseGateway clientGateway;
    BreedDatabaseGateway breedGateway;
    EmployeeDatabaseGateway employeeGateway;
    ApplicationDatabaseGateway applicationGateway;
    AnimalDatabaseGateway animalGateway;
    CompetitionDatabaseGateway competitionGateway;
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

    void listClients();
    void addClient();
    void changeClientName();
    void changeClientAddress();
    void removeClient();
    void deleteClient(int id);
    void printClients(vector<Client> clients);
    void printClient(Client client);
    int inputClientId();

    void listBreeds();
    void addBreed();
    void changeBreedName();
    void removeBreed();
    void deleteBreed(int id);
    void printBreeds(vector<Breed> breeds);
    void printBreed(Breed breed);
    int inputBreedId();

    void listEmployees();
    void addEmployee();
    void changeEmployeeName();
    void changeEmployeeAddress();
    void changeEmployeePosition();
    void changeEmployeeSalary();
    void removeEmployee();
    void deleteEmployee(int id);
    void findEmployeeByPosition();
    void printEmployees(vector<Employee> employees);
    void printEmployee(Employee employee);
    int inputEmployeeId();

    void listApplications();
    void addApplication();
    void changeApplicationBreed();
    void changeApplicationGender();
    void closeApplication();
    void removeApplication();
    void deleteApplication(int id);
    void listApplicationsByClient();
    void listApplicationsByEmployee();
    void listApplicationsByBreed();
    void printApplications(vector<Application> applications);
    void printApplication(Application application);
    int inputApplicationId();
    optional<Gender> inputGender();

    void listAnimals();
    void addAnimal();
    void changeAnimalName();
    void changeAnimalAge();
    void changeAnimalGender();
    void changeAnimalBreed();
    void changeAnimalExterior();
    void changeAnimalPedigree();
    void changeAnimalVeterinarian();
    void changeAnimalOwner();
    void removeAnimal();
    void deleteAnimal(int id);
    void listAnimalsByBreed();
    void listAnimalsByOwner();
    void printAnimals(vector<Animal> animals);
    void printAnimal(Animal animal);
    optional<int> inputAge();
    string inputOptionalString(string parameter);
    int inputAnimalId();

    void listCompetitions();
    void addCompetition();
    void changeCompetitionName();
    void changeCompetitionLocation();
    void changeCompetitionDate();
    void changeCompetitionAward();
    void removeCompetition();
    void deleteCompetition(int id);
    void findCompetitionByAnimal();
    void findCompetitionByAward();
    void findCompetitionByName();
    void printCompetitions(vector<Competition> competitions);
    void printCompetition(Competition competition);
    int inputCompetitionId();
    optional<tm> inputDate();
};

