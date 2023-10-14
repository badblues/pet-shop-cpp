#pragma once
#include <iostream>
#include <stdexcept>
#include <sql.h>
#include "./models/Client.h"
#include "./repositories/ClientRepository.h"
#include "./repositories/BreedRepository.h"
#include "./repositories/EmployeeRepository.h"
#include "./repositories/ApplicationRepository.h"
#include "./repositories/AnimalRepository.h"
#include "./repositories/CompetitionRepository.h"
#include "./repositories/ParticipationRepository.h"
#include "./gateways/ClientDatabaseGateway.h"
#include "./gateways/BreedDatabaseGateway.h"
#include "./gateways/EmployeeDatabaseGateway.h"
#include "./gateways/ApplicationDatabaseGateway.h"
#include "./gateways/AnimalDatabaseGateway.h"
#include "./gateways/CompetitionDatabaseGateway.h"
#include "./gateways/ParticipationDatabaseGateway.h"

using namespace std;

class Menu {

  public:

    Menu(SQLHDBC hDbc) {
      this-> hDbc;
      clientRepository = ClientRepository(ClientDatabaseGateway(hDbc));
      breedRepository = BreedRepository(BreedDatabaseGateway(hDbc));
      employeeRepository = EmployeeRepository(EmployeeDatabaseGateway(hDbc));
      applicationRepository = ApplicationRepository(ApplicationDatabaseGateway(hDbc));
      animalRepository = AnimalRepository(AnimalDatabaseGateway(hDbc));
      competitionRepository = CompetitionRepository(CompetitionDatabaseGateway(hDbc));
      participationRepository = ParticipationRepository(ParticipationDatabaseGateway(hDbc));
    }

    void showMenu() {
      int choice;
      int id;
      while (menuRunning) {
        innerMenuRunning = true;
        cout << "\033[2J\033[H";
        cout << "Menu:\n" <<
                "1. List clients\n" <<
                "2. List animals\n" <<
                "3. List employees\n" <<
                "4. List competitions\n" <<
                "5. List applications\n" <<
                "6. List breeds\n" <<
                "7. Select client\n" <<
                "8. Select animal\n" <<
                "9. Select employee\n" <<
                "10. Select competition\n" <<
                "11. Select application\n" <<
                "12. Select breed\n" <<
                "13. Add new\n" << 
                "0. Exit\n";
        choice = getChoice(0, 13);
        switch(choice) {
          case 1:
            listClients();
            break;
          case 2:
            listAnimals();
            break;
          case 3:
            listEmployees();
            break;
          case 4:
            listCompetitions();
            break;
          case 5:
            listApplications();
            break;
          case 6:
            listBreeds();
            break;
          case 7:
            id = inputClientId();
            if (id == -1)
              break;
            currentClient = clientRepository.get(id);
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              printClient(currentClient);
              cout << "1. Change name\n"
                   << "2. Change address\n"
                   << "3. List animals\n"
                   << "4. Assign animal\n"
                   << "5. List applications\n"
                   << "6. Add application\n"
                   << "7. Change gender in application\n"
                   << "8. Change breed in application\n"
                   << "9. Delete application\n"
                   << "10. Delete client (with applications and animals)\n"
                   << "0. Back\n";
              choice = getChoice(0, 8);
              switch(choice) {
                case 1:
                  changeClientName();
                  break;
                case 2:
                  changeClientAddress();
                  break;
                case 3:
                  listAnimalsByClient();
                  break;
                case 4:
                  assignAnimal();
                  break;
                case 5:
                  listApplicationsByClient();
                  break;
                case 6:
                  addApplication();
                  break;
                case 7:
                  changeApplicationGender();
                  break;
                case 8:
                  changeApplicationBreed();
                  break;
                case 9: 
                  removeApplication();
                case 10:
                  removeClient();
                  innerMenuRunning = false;
                  break;
                case 0:
                  innerMenuRunning = false;
                  break;
              }
            }
            break;
          case 13:
            while(innerMenuRunning) {
              cout << "\033[2J\033[H";
              cout << "1. Client\n"
                   << "2. Animal\n"
                   << "3. Competition\n"
                   << "4. Employee\n"
                   << "5. Breed\n"
                   << "0. Back\n";
              choice = getChoice(0, 5);
              switch(choice) {
                case 1:
                  addClient();
                  break;
                case 2:
                  addAnimal();
                  break;
                case 3:
                  addCompetition();
                  break;
                case 4:
                  addEmployee();
                  break;
                case 5:
                  addBreed();
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
    ClientRepository clientRepository;
    BreedRepository breedRepository;
    EmployeeRepository employeeRepository;
    ApplicationRepository applicationRepository;
    AnimalRepository animalRepository;
    CompetitionRepository competitionRepository;
    ParticipationRepository participationRepository;

    Client currentClient;
    Animal currentAnimal;
    Employee currentEmployee;
    Competition currentCompetition;
    Breed currentBreed;

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
    void listAnimalsByClient();
    void listApplicationsByClient();
    void assignAnimal();
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
    void printCompetitions(vector<Competition> competitions);
    void printCompetition(Competition competition);
    int inputCompetitionId();
    optional<tm> inputDate();
};


