#include "Menu.h"

//CLIENTS

void Menu::listClients() {
  try {
    vector<Client> clients = clientGateway.getAll();
    cout << "\033[2J\033[H";
    printClients(clients);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::addClient() {
  string name;
  string address;
  cout << "Enter name:\n";
  getline(cin, name);
  cout << "Enter address:\n";
  getline(cin, address);
  try {
    Client client = clientGateway.create(name, address);
    cout << "Success!\n";
    printClient(client);
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeClientName() {
  int id = inputClientId();
  if (id == -1)
    return;
  string name;
  cout << "Enter new name:\n";
  getline(cin, name);
  try {
    Client oldClient = clientGateway.get(id);
    string address = oldClient.getAddress();
    Client updatedClient = clientGateway.update(id, name, address);
    cout << "Success!\n";
    printClient(updatedClient);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeClientAddress() {
  int id = inputClientId();
  if (id == -1)
    return;
  string address;
  cout << "Enter new address:\n";
  getline(cin, address);
  try {
    Client oldClient = clientGateway.get(id);
    string name = oldClient.getName();
    Client updatedClient = clientGateway.update(id, name, address);
    cout << "Success!\n";
    printClient(updatedClient);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::removeClient() {
  int id = inputClientId();
  if (id == -1)
    return;
  try {
    deleteClient(id);
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::deleteClient(int id) {
  vector<Animal> animals = animalGateway.findByClient(id);
  for (int i = 0; i < animals.size(); i++)
    deleteAnimal(animals[i].getId());
  vector<Application> applications = applicationGateway.findByClient(id);
  for (int i = 0; i < applications.size(); i++)
    deleteApplication(applications[i].getId());
  clientGateway.remove(id);
  cout << "Success!\n";
  getchar();
}

void Menu::printClients(vector<Client> clients) {
  for (int i = 0; i < clients.size(); i++) {
    cout << i + 1 << ". " << clients[i].getName() << ", address: " << clients[i].getAddress() << "\n";
  }
}

void Menu::printClient(Client client) {
  cout << "name: " << client.getName() << ", address: " << client.getAddress() << "\n";
}

int Menu::inputClientId() {
  string name;
  cout << "Enter client's name (leave empty to go back):\n";
  getline(cin, name);
  if (name == "") {
    return -1;
  }
  vector<Client> clients = clientGateway.findByName(name);
  if (clients.size() == 0) {
    cout << "Client with that name wasn't found\n";
    getchar();
    return -1;
  }
  int id;
  for (int i = 0; i < clients.size(); i++) {
    cout << i + 1 << ". " << clients[i].getName() << ", address: " << clients[i].getAddress() << "\n";
  }
  cout << "What client?\n";
  int choice = getChoice(1, clients.size());
  id = clients[choice - 1].getId();
  return id;
}

//BREEDS

void Menu::listBreeds() {
  try {
    vector<Breed> breeds = breedGateway.getAll();
    cout << "\033[2J\033[H";
    printBreeds(breeds);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::addBreed() {
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  try {
    Breed breed = breedGateway.create(name);
    cout << "Success!\n";
    printBreed(breed);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeBreedName() {
  int id = inputBreedId();
  if (id == -1)
    return;
  string name;
  cout << "Enter new name:\n";
  getline(cin, name);
  try {
    Breed updatedBreed = breedGateway.update(id, name);
    cout << "Success!\n";
    printBreed(updatedBreed);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::removeBreed() {
  int id = inputBreedId();
  if (id == -1)
    return;
  try {
    deleteBreed(id);
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::deleteBreed(int id) {
  vector<Animal> animals = animalGateway.findByBreed(id);
  vector<Application> applications = applicationGateway.findByBreed(id);
  if (animals.size() > 0 || applications.size() > 0) {
    cout << "Cannot be deleted, there are animals or applications with that breed\n";
    getchar();
    return;
  }
  breedGateway.remove(id);
  cout << "Success!\n";
  getchar();
}

void Menu::printBreeds(vector<Breed> breeds) {
  for (int i = 0; i < breeds.size(); i++) {
    cout << i + 1 << ". " << breeds[i].getName() << "\n";
  }
}

void Menu::printBreed(Breed breed) {
  cout << "name: " << breed.getName() << "\n";
}

int Menu::inputBreedId() {
  string name;
  cout << "Enter name of the breed (leave empty to go back):\n";
  getline(cin, name);
  if (name == "")
    return -1;
  vector<Breed> breeds = breedGateway.findByName(name);
  if (breeds.size() == 0) {
    cout << "Breed with that name wasn't found\n";
    getchar();
    return -1;
  }
  return breeds[0].getId();
}

//EMPLOYEES

void Menu::listEmployees() {
  try {
    vector<Employee> employees = employeeGateway.getAll();
    cout << "\033[2J\033[H";
    printEmployees(employees);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::addEmployee() {
  string name;
  string address;
  string position;
  double salary;
  cout << "Enter name:\n";
  getline(cin, name);
  cout << "Enter address:\n";
  getline(cin, address);
  cout << "Enter position:\n";
  getline(cin, position);
  cout << "Enter salary:\n";
  cin >> salary;
  try {
    Employee employee = employeeGateway.create(name, address, position, salary);
    cout << "Success!\n";
    printEmployee(employee);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeEmployeeName() {
  int id = inputEmployeeId();
  if (id == -1)
    return;
  string name;
  cout << "Enter new name:\n";
  getline(cin, name);
  try {
    Employee oldEmployee = employeeGateway.get(id);
    string address = oldEmployee.getAddress();
    string position = oldEmployee.getPosition();
    double salary = oldEmployee.getSalary();
    Employee updatedEmployee = employeeGateway.update(id, name, address, position, salary);
    cout << "Success!\n";
    printEmployee(updatedEmployee);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeEmployeeAddress() {
  int id = inputEmployeeId();
  if (id == -1)
    return;
  string address;
  cout << "Enter new address:\n";
  getline(cin, address);
  try {
    Employee oldEmployee = employeeGateway.get(id);
    string name = oldEmployee.getName();
    string position = oldEmployee.getPosition();
    double salary = oldEmployee.getSalary();
    Employee updatedEmployee = employeeGateway.update(id, name, address, position, salary);
    cout << "Success!\n";
    printEmployee(updatedEmployee);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeEmployeePosition() {
  int id = inputEmployeeId();
  if (id == -1)
    return;
  string position;
  cout << "Enter new position:\n";
  getline(cin, position);
  try {
    Employee oldEmployee = employeeGateway.get(id);
    string name = oldEmployee.getName();
    string address = oldEmployee.getAddress();
    double salary = oldEmployee.getSalary();
    Employee updatedEmployee = employeeGateway.update(id, name, address, position, salary);
    cout << "Success!\n";
    printEmployee(updatedEmployee);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeEmployeeSalary() {
  int id = inputEmployeeId();
  if (id == -1)
    return;
  double salary;
  cout << "Enter new salary:\n";
  cin >> salary;
  try {
    Employee oldEmployee = employeeGateway.get(id);
    string name = oldEmployee.getName();
    string address = oldEmployee.getAddress();
    string position = oldEmployee.getPosition();
    Employee updatedEmployee = employeeGateway.update(id, name, address, position, salary);
    cout << "Success!\n";
    printEmployee(updatedEmployee);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::removeEmployee() {
  int id = inputEmployeeId();
  if (id == -1)
    return;
  try {
    deleteEmployee(id);
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::deleteEmployee(int id) {
  vector<Application> applications = applicationGateway.findByEmployee(id);
  for (int i = 0; i < applications.size(); i++)
    applicationGateway.removeEmployeeFromApplication(applications[i].getId());
  employeeGateway.remove(id);
  cout << "Success!\n";
  getchar();
}

void Menu::findEmployeeByPosition() {
  try {
    string position;
    cout << "Enter position:\n";
    getline(cin, position);
    vector<Employee> employees = employeeGateway.findByPosition(position);
    cout << "\033[2J\033[H";
    printEmployees(employees);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::printEmployees(vector<Employee> employees) {
  for (int i = 0; i < employees.size(); i++) {
    cout << i + 1 << ". " << employees[i].getName()
          << ", address: " << employees[i].getAddress()
          << ", position: " << employees[i].getPosition()
          << ", salary: " << employees[i].getSalary() << "$"
          << "\n";
  }
}

void Menu::printEmployee(Employee employee) {
  cout << "name: " << employee.getName()
       << ", address: " << employee.getAddress()
       << ", position: " << employee.getPosition()
       << ", salary: " << employee.getSalary() << "$"
       << "\n";
}

int Menu::inputEmployeeId() {
  string name;
  cout << "Enter employee's name (leave empty to go back):\n";
  getline(cin, name);
  if (name == "")
    return -1;
  vector<Employee> employees = employeeGateway.findByName(name);
  if (employees.size() == 0) {
    cout << "Employee with that name wasn't found\n";
    getchar();
    return -1;
  }
  int id;
  for (int i = 0; i < employees.size(); i++) {
    cout << i + 1 << ". " << employees[i].getName()
           << ", address: " << employees[i].getAddress()
           << ", position: " << employees[i].getPosition()
           << ", salary: " << employees[i].getSalary() << "$"
           << "\n";
  }
  cout << "What employee?\n";
  int choice = getChoice(1, employees.size());
  id = employees[choice - 1].getId();
  return id;
}

//APPLICATIONS

void Menu::listApplications() {
  try {
    vector<Application> applications = applicationGateway.getAll();
    cout << "\033[2J\033[H";
    printApplications(applications);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::addApplication() {
  int clientId = inputClientId();
  if (clientId == -1)
    return;
  int employeeId = inputEmployeeId();
  if (employeeId == -1)
    return;
  int breedId = inputBreedId();
  if (breedId == -1)
    return;
  optional<Gender> gender = inputGender();
  time_t currentTime = time(nullptr);
  tm applicationDate = *localtime(&currentTime);

  try {
    Application application = applicationGateway.create(clientId, employeeId, breedId, gender, applicationDate, false);
    cout << "Success!\n";
    printApplication(application);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeApplicationBreed() {
  int id = inputApplicationId();
  if (id == -1)
    return;
  int employeeId = inputEmployeeId();
  if (employeeId == -1)
    return;
  try {
    Application oldApplication = applicationGateway.get(id);
    if (oldApplication.getCompleted()) {
      cout << "Cannot update completed application\n";
      getchar();
      return;
    }
    int clientId = oldApplication.getClientId();
    bool completed = oldApplication.getCompleted();
    int breedId = inputBreedId();
    if (breedId == -1)
      return;
    optional<Gender> gender = oldApplication.getGender();
    tm applicationDate = oldApplication.getApplicationDate();
    Application updatedApplication = applicationGateway.update(id, clientId, employeeId, breedId, gender, applicationDate, completed);
    cout << "Success!\n";
    printApplication(updatedApplication);
    getchar();
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeApplicationGender() {
  int id = inputApplicationId();
  if (id == -1)
    return;
  int employeeId = inputEmployeeId();
  if (employeeId == -1)
    return;
  try {
    Application oldApplication = applicationGateway.get(id);
    if (oldApplication.getCompleted()) {
      cout << "Cannot update completed application\n";
      getchar();
      return;
    }
    int clientId = oldApplication.getClientId();
    bool completed = oldApplication.getCompleted();
    int breedId = oldApplication.getBreedId();
    optional<Gender> gender = inputGender();
    tm applicationDate = oldApplication.getApplicationDate();
    Application updatedApplication = applicationGateway.update(id, clientId, employeeId, breedId, gender, applicationDate, completed);
    cout << "Success!\n";
    printApplication(updatedApplication);
    getchar();
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::closeApplication() {
  int id = inputApplicationId();
  if (id == -1)
    return;
  int employeeId = inputEmployeeId();
  if (employeeId == -1)
    return;
  try {
    Application oldApplication = applicationGateway.get(id);
    if (oldApplication.getCompleted()) {
      cout << "Already closed\n";
      getchar();
      return;
    }
    int clientId = oldApplication.getClientId();
    int breedId = oldApplication.getBreedId();
    optional<Gender> gender = oldApplication.getGender();
    tm date = oldApplication.getApplicationDate();
    Application updatedApplication = applicationGateway.update(id, clientId, employeeId, breedId, gender, date, true);
    cout << "Success!\n";
    printApplication(updatedApplication);
    getchar();
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::removeApplication() {
  int id = inputApplicationId();
  if (id == -1)
    return;
  try {
    deleteApplication(id);
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::deleteApplication(int id) {
  applicationGateway.remove(id);
  cout << "Success!\n";
  getchar();
}

void Menu::listApplicationsByClient() {
  int id = inputClientId();
  try {
    vector<Application> applications = applicationGateway.findByClient(id);
    cout << "\033[2J\033[H";
    printApplications(applications);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}


void Menu::listApplicationsByEmployee() {
  int id = inputEmployeeId();
  try {
    vector<Application> applications = applicationGateway.findByEmployee(id);
    cout << "\033[2J\033[H";
    printApplications(applications);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::listApplicationsByBreed() {
  int id = inputBreedId();
  try {
    vector<Application> applications = applicationGateway.findByBreed(id);
    cout << "\033[2J\033[H";
    printApplications(applications);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::printApplications(vector<Application> applications) {
  for (int i = 0; i < applications.size(); i++) {
    string clientName = clientGateway.get(applications[i].getClientId()).getName();
    string employeeName = "";
    optional<int> employeeId = applications[i].getEmployeeId();
    if (employeeId.has_value())
    employeeName = employeeGateway.get(employeeId.value()).getName();
    string breed = breedGateway.get(applications[i].getBreedId()).getName();
    string genderStr = "";
    optional<Gender> gender = applications[i].getGender();
    if (gender.has_value())
      genderStr = gender.value() == Gender::male ? "male" : "female";
    tm applicationDate = applications[i].getApplicationDate();
    stringstream dateStream;
    dateStream << put_time(&applicationDate, "%Y-%m-%d");
    cout << i + 1 << ". client: " << clientName
          << ", employee: " << employeeName
          << ", breed: " << breed
          << ", gender: " << genderStr
          << ", applicationDate: " << dateStream.str()
          << ", completed: " << applications[i].getCompleted()
          << "\n";
    }
}

void Menu::printApplication(Application application) {
  string clientName = clientGateway.get(application.getClientId()).getName();
  string employeeName = "";
  optional<int> employeeId = application.getEmployeeId();
  if (employeeId.has_value())
    employeeName = employeeGateway.get(employeeId.value()).getName();
  string breed = breedGateway.get(application.getBreedId()).getName();
  string genderStr = "";
  optional<Gender> gender = application.getGender();
  if (gender.has_value())
      genderStr = gender.value() == Gender::male ? "male" : "female";
  tm applicationDate = application.getApplicationDate();
  stringstream dateStream;
  dateStream << put_time(&applicationDate, "%Y-%m-%d");
  cout  << "client name: " << clientName
        << ", employee: " << employeeName
        << ", breed: " << breed
        << ", gender: " << genderStr
        << ", applicationDate: " << dateStream.str()
        << ", completed: " << application.getCompleted()
        << "\n";
}

int Menu::inputApplicationId() {
  cout << "Find application by:\n"
       << "1. Client\n"
       << "2. Employee\n"
       << "3. Breed\n"
       << "0. Go back\n";

  int choice = getChoice(0, 3);
  int id;
  vector<Application> applications;
  try {
    switch(choice) {
      case 1:
        id = inputClientId();
        applications = applicationGateway.findByClient(id);
        break;
      case 2:
        id = inputEmployeeId();
        applications = applicationGateway.findByEmployee(id);
        break;
      case 3:
        id = inputBreedId();
        applications = applicationGateway.findByBreed(id);
        break;
      case 0:
        return -1;
        break;
    }
    if (id == -1)
      return -1;
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return -1;
  }
  if (applications.size() == 0) {
    cout << "Applications not found\n";
    getchar();
    return -1;
  }
  cout << "Applications:\n";
  printApplications(applications);
  cout << "Which one?\n";
  choice = getChoice(1, applications.size());
  id = applications[choice - 1].getId();
  return id;
}

optional<Gender> Menu::inputGender() {
  string answer;
  cout << "Enter gender: (male/female/not important)\n";
  getline(cin, answer);
  while(answer != "male" && answer != "female" && answer != "not important"){
    cout << "Invalid input, try again:\n";
    getline(cin, answer);
  }
  optional<Gender> gender;
  if (answer != "not important")
    gender = answer == "male" ? Gender::male : Gender::female;
  return gender;
}

// ANIMALS

void Menu::listAnimals() {
  try {
    vector<Animal> animals = animalGateway.getAll();
    cout << "\033[2J\033[H";
    printAnimals(animals);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::addAnimal() {
  int choice;
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  optional<int> age = inputAge();
  cout << "Gender:\n1.male\n2.female\n";
  choice = getChoice(1, 2);
  Gender gender = choice == 1 ? Gender::male : Gender::female;
  int breedId = inputBreedId();
  if (breedId == -1)
    return;
  string exterior = inputOptionalString("Exterior description");
  string pedigree = inputOptionalString("Pedigree");
  string veterinarian = inputOptionalString("Veterinarian");
  optional<int> ownerId;
  cout << "Owner:\n1.Has\n2.Has no\n";
  choice = getChoice(1, 2);
  if (choice == 1) {
    ownerId = inputClientId();
    if (ownerId == -1)
      return;
  }
  try {
    Animal animal = animalGateway.create(name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(animal);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::changeAnimalName() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  try {
    Animal oldAnimal = animalGateway.get(id);
    optional<int> age;
    if (oldAnimal.getAge().has_value())
      age = oldAnimal.getAge().value();
    Gender gender = oldAnimal.getGender(); 
    int breedId = oldAnimal.getBreedId();
    string exterior = oldAnimal.getExterior();
    string pedigree = oldAnimal.getPedigree();
    string veterinarian = oldAnimal.getVeterinarian();
    optional<int> ownerId;
    if (oldAnimal.getOwnerId().has_value())
      ownerId = oldAnimal.getOwnerId().value();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeAnimalAge() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  optional<int> age = inputAge();
  try {
    Animal oldAnimal = animalGateway.get(id);
    string name = oldAnimal.getName();
    Gender gender = oldAnimal.getGender(); 
    int breedId = oldAnimal.getBreedId();
    string exterior = oldAnimal.getExterior();
    string pedigree = oldAnimal.getPedigree();
    string veterinarian = oldAnimal.getVeterinarian();
    optional<int> ownerId;
    if (oldAnimal.getOwnerId().has_value())
      ownerId = oldAnimal.getOwnerId().value();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeAnimalGender() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  cout << "Gender:\n1.male\n2.female\n";
  int choice = getChoice(1, 2);
  Gender gender = choice == 1 ? Gender::male : Gender::female;
  try {
    Animal oldAnimal = animalGateway.get(id);
    string name = oldAnimal.getName(); 
    optional<int> age;
    if (oldAnimal.getAge().has_value())
      age = oldAnimal.getAge().value();
    int breedId = oldAnimal.getBreedId();
    string exterior = oldAnimal.getExterior();
    string pedigree = oldAnimal.getPedigree();
    string veterinarian = oldAnimal.getVeterinarian();
    optional<int> ownerId;
    if (oldAnimal.getOwnerId().has_value())
      ownerId = oldAnimal.getOwnerId().value();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeAnimalBreed() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  int breedId = inputBreedId();
  try {
    Animal oldAnimal = animalGateway.get(id);
    string name = oldAnimal.getName();
    optional<int> age;
    if (oldAnimal.getAge().has_value())
      age = oldAnimal.getAge().value();
    Gender gender = oldAnimal.getGender(); 
    string exterior = oldAnimal.getExterior();
    string pedigree = oldAnimal.getPedigree();
    string veterinarian = oldAnimal.getVeterinarian();
    optional<int> ownerId;
    if (oldAnimal.getOwnerId().has_value())
      ownerId = oldAnimal.getOwnerId().value();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeAnimalExterior() {
int id = inputAnimalId();
  if (id == -1)
    return;
  string exterior = inputOptionalString("Exterior description");
  try {
    Animal oldAnimal = animalGateway.get(id);
    optional<int> age;
    if (oldAnimal.getAge().has_value())
      age = oldAnimal.getAge().value();
    Gender gender = oldAnimal.getGender(); 
    int breedId = oldAnimal.getBreedId();
    string name = oldAnimal.getName();
    string pedigree = oldAnimal.getPedigree();
    string veterinarian = oldAnimal.getVeterinarian();
    optional<int> ownerId;
    if (oldAnimal.getOwnerId().has_value())
      ownerId = oldAnimal.getOwnerId().value();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeAnimalPedigree() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  string pedigree = inputOptionalString("Pedigree");
  try {
    Animal oldAnimal = animalGateway.get(id);
    string name = oldAnimal.getName();
    optional<int> age;
    if (oldAnimal.getAge().has_value())
      age = oldAnimal.getAge().value();
    Gender gender = oldAnimal.getGender(); 
    int breedId = oldAnimal.getBreedId();
    string exterior = oldAnimal.getExterior();
    string veterinarian = oldAnimal.getVeterinarian();
    optional<int> ownerId;
    if (oldAnimal.getOwnerId().has_value())
      ownerId = oldAnimal.getOwnerId().value();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeAnimalVeterinarian() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  string veterinarian = inputOptionalString("Veterinarian");
  try {
    Animal oldAnimal = animalGateway.get(id);
    string name = oldAnimal.getName();
    optional<int> age;
    if (oldAnimal.getAge().has_value())
      age = oldAnimal.getAge().value();
    Gender gender = oldAnimal.getGender(); 
    int breedId = oldAnimal.getBreedId();
    string exterior = oldAnimal.getExterior();
    string pedigree = oldAnimal.getPedigree();
    optional<int> ownerId;
    if (oldAnimal.getOwnerId().has_value())
      ownerId = oldAnimal.getOwnerId().value();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeAnimalOwner() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  optional<int> ownerId;
  cout << "Owner:\n1.Has\n2.Has no\n";
  int choice = getChoice(1, 2);
  if (choice == 1) {
    ownerId = inputClientId();
    if (ownerId == -1)
      return;
  }
  try {
    Animal oldAnimal = animalGateway.get(id);
    string name = oldAnimal.getName();
    optional<int> age;
    if (oldAnimal.getAge().has_value())
      age = oldAnimal.getAge().value();
    Gender gender = oldAnimal.getGender(); 
    int breedId = oldAnimal.getBreedId();
    string exterior = oldAnimal.getExterior();
    string pedigree = oldAnimal.getPedigree();
    string veterinarian = oldAnimal.getVeterinarian();
    Animal updatedAnimal = animalGateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    cout << "Success!\n";
    printAnimal(updatedAnimal);
    getchar();
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::removeAnimal() {
  int id = inputAnimalId();
  if (id == -1)
    return;
  try {
    deleteAnimal(id);
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::deleteAnimal(int id) {
  vector<Competition> competitions = competitionGateway.findByAnimal(id);
  for (int i = 0; i < competitions.size(); i++)
    deleteCompetition(competitions[i].getId());
  animalGateway.remove(id);
  cout << "Success!\n";
  getchar();
}

void Menu::listAnimalsByBreed() {
  int breedId = inputBreedId();
  if (breedId == -1)
    return;
  try {
    vector<Animal> animals = animalGateway.findByBreed(breedId);
    cout << "\033[2J\033[H";
    printAnimals(animals);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::listAnimalsByOwner() {
  optional<int> ownerId;
  cout << "Owner:\n1.Has\n2.Has no\n";
  int choice = getChoice(1, 2);
  if (choice == 1) {
    ownerId = inputClientId();
    if (ownerId == -1)
      return;
  }
  try {
    vector<Animal> animals = animalGateway.findByClient(ownerId);
    cout << "\033[2J\033[H";
    printAnimals(animals);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::printAnimal(Animal animal) {
  string genderStr = animal.getGender() == Gender::male ? "male" : "female";
  string breedStr = breedGateway.get(animal.getBreedId()).getName();
  optional<int> ownerId = animal.getOwnerId();
  string ownerStr = ownerId.has_value() ? clientGateway.get(ownerId.value()).getName() : "";
  cout << "name: " << animal.getName()
       << ", age: " << (animal.getAge().has_value() ? to_string(animal.getAge().value()) : "")
       << ", gender: " << genderStr
       << ", breed: " << breedStr
       << ", exterior: " << animal.getExterior()
       << ", pedigree: " << animal.getPedigree()
       << ", veterinarian: " << animal.getVeterinarian()
       << ", owner: " << ownerStr;
}

void Menu::printAnimals(vector<Animal> animals) {
  for (int i = 0; i < animals.size(); i++) {
    string genderStr = animals[i].getGender() == Gender::male ? "male" : "female";
    string breedStr = breedGateway.get(animals[i].getBreedId()).getName();
    optional<int> ownerId = animals[i].getOwnerId();
    string ownerStr = ownerId.has_value() ? clientGateway.get(ownerId.value()).getName() : "";
    cout << i + 1 << ". name: " << animals[i].getName()
        << ", age: " << (animals[i].getAge().has_value() ? to_string(animals[i].getAge().value()) : "")
        << ", gender: " << genderStr
        << ", breed: " << breedStr
        << ", exterior: " << animals[i].getExterior()
        << ", pedigree: " << animals[i].getPedigree()
        << ", veterinarian: " << animals[i].getVeterinarian()
        << ", owner: " << ownerStr << "\n";
  }
}

optional<int> Menu::inputAge() {
  optional<int> age;
  int intAge;
  cout << "Animal age:\n1.known\n2.unknown\n";
  int choice = getChoice(1, 2);
  if (choice == 1) {
    cout << "Enter age:\n";
    cin >> intAge;
    age = intAge;
  }
  return age;
}

string Menu::inputOptionalString(string parameter) {
  string str = "";
  cout << parameter << ":\n1.Input\n2.Skip\n";
  int choice = getChoice(1,2);
  if (choice == 1) {
    cout << "Enter " << parameter << ":\n";
    getline(cin, str);
  }
  return str;
}

int Menu::inputAnimalId() {
  cout << "Find animal by:\n"
       << "1. Client\n"
       << "2. Breed\n"
       << "3. Name\n"
       << "0. Go back\n";
  int choice = getChoice(0, 3);
  int id;
  string name;
  vector<Animal> animals;
  try {
    switch(choice) {
      case 1:
        id = inputClientId();
        animals = animalGateway.findByClient(id);
        break;
      case 2:
        id = inputBreedId();
        animals = animalGateway.findByBreed(id);
        break;
      case 3:
        cout << "Enter name:\n";
        getline(cin, name);
        animals = animalGateway.findByName(name);
        break;
      case 0:
        return -1;
        break;
    }
    if (id == -1)
      return -1;
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return -1;
  }
  if (animals.size() == 0) {
    cout << "Animals not found\n";
    getchar();
    return -1;
  }
  cout << "Animals:\n";
  printAnimals(animals);
  cout << "Which one?\n";
  choice = getChoice(1, animals.size());
  id = animals[choice - 1].getId();
  return id;
}

//Competitions

void Menu::listCompetitions() {
  try {
    vector<Competition> competition = competitionGateway.getAll();
    cout << "\033[2J\033[H";
    printCompetitions(competition);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::addCompetition() {
  int animalId = inputAnimalId();
  if (animalId == -1)
    return;
  string name;
  cout << "Enter name of the competition:\n";
  getline(cin, name);
  string location;
  cout << "Enter location of the competition:\n";
  getline(cin, location);
  optional<tm> date = inputDate();
  if (!date.has_value())
    return;
  string award;
  cout << "Enter award of the animal:\n";
  getline(cin, award);
  try {
    Competition competitions = competitionGateway.create(animalId, name, location, date.value(), award);
    cout << "Success!\n";
    printCompetition(competitions);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  } 
}

void Menu::changeCompetitionName() {
  int id = inputCompetitionId();
  if (id == -1)
    return;
  try {
    string name;
    cout << "Enter name of the competition:\n";
    getline(cin, name);
    Competition oldCompetition = competitionGateway.get(id);
    int animalId = oldCompetition.getAnimalId();
    string location = oldCompetition.getLocation();
    tm date = oldCompetition.getDate();
    string award = oldCompetition.getAward();
    Competition updatedCompetition = competitionGateway.update(id, animalId, name, location, date, award);
    cout << "Success!\n";
    printCompetition(updatedCompetition);
    getchar();
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeCompetitionLocation() {
  int id = inputCompetitionId();
  if (id == -1)
    return;
  try {
    string location;
    cout << "Enter location of the competition:\n";
    getline(cin, location);
    Competition oldCompetition = competitionGateway.get(id);
    int animalId = oldCompetition.getAnimalId();
    string name = oldCompetition.getName();
    tm date = oldCompetition.getDate();
    string award = oldCompetition.getAward();
    Competition updatedCompetition = competitionGateway.update(id, animalId, name, location, date, award);
    cout << "Success!\n";
    printCompetition(updatedCompetition);
    getchar();
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeCompetitionDate() {
  int id = inputCompetitionId();
  if (id == -1)
    return;
  try {
    optional<tm> date = inputDate();
    if (!date.has_value())
      return;
    Competition oldCompetition = competitionGateway.get(id);
    int animalId = oldCompetition.getAnimalId();
    string name = oldCompetition.getName();
    string location = oldCompetition.getLocation();
    string award = oldCompetition.getAward();
    Competition updatedCompetition = competitionGateway.update(id, animalId, name, location, date.value(), award);
    cout << "Success!\n";
    printCompetition(updatedCompetition);
    getchar();
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::changeCompetitionAward() {
  int id = inputCompetitionId();
  if (id == -1)
    return;
  try {
    string award;
    cout << "Enter award of the animal:\n";
    getline(cin, award);
    Competition oldCompetition = competitionGateway.get(id);
    int animalId = oldCompetition.getAnimalId();
    string name = oldCompetition.getName();
    string location = oldCompetition.getLocation();
    tm date = oldCompetition.getDate();
    Competition updatedCompetition = competitionGateway.update(id, animalId, name, location, date, award);
    cout << "Success!\n";
    printCompetition(updatedCompetition);
    getchar();
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}
void Menu::removeCompetition() {
  int id = inputCompetitionId();
  if (id == -1)
    return;
  try {
    competitionGateway.remove(id);
  } catch(exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::deleteCompetition(int id) {
  competitionGateway.remove(id);
  cout << "Success!\n";
  getchar();
}

void Menu::findCompetitionByAnimal() {
  int animalId = inputAnimalId();
  if (animalId == -1)
    return;
  try {
    vector<Competition> competitions = competitionGateway.findByAnimal(animalId);
    cout << "\033[2J\033[H";
    cout << "Competitions:\n";
    printCompetitions(competitions);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::findCompetitionByAward() {
  string award;
  cout << "Enter award:\n";
  getline(cin, award);
  try {
    vector<Competition> competitions = competitionGateway.findByAward(award);
    cout << "\033[2J\033[H";
    cout << "Competitions:\n";
    printCompetitions(competitions);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::findCompetitionByName() {
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  try {
    vector<Competition> competitions = competitionGateway.findByName(name);
    cout << "\033[2J\033[H";
    cout << "Competitions:\n";
    printCompetitions(competitions);
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

void Menu::printCompetitions(vector<Competition> competitions) {
  for (int i = 0; i < competitions.size(); i++) {
    stringstream dateStream;
    tm date = competitions[i].getDate();
    dateStream << put_time(&date, "%Y-%m-%d");
    Animal animal = animalGateway.get(competitions[i].getAnimalId());
    Breed breed = breedGateway.get(animal.getBreedId());
    cout << i + 1 << ". animal name = " << animal.getName() <<
            ", animal breed = " << breed.getName() <<
            ", competition name = " << competitions[i].getName() <<
            ", location = " << competitions[i].getLocation() <<
            ", date = " << dateStream.str() <<
            ", award = " << competitions[i].getAward() << "\n";
  }
}

void Menu::printCompetition(Competition competition) {
  stringstream dateStream;
  tm date = competition.getDate();
  dateStream << put_time(&date, "%Y-%m-%d");
  Animal animal = animalGateway.get(competition.getAnimalId());
  Breed breed = breedGateway.get(animal.getBreedId());
  cout << "animal name = " << animal.getName() <<
          ", animal breed = " << breed.getName() <<
          ", competition name = " << competition.getName() <<
          ", location = " << competition.getLocation() <<
          ", date = " << dateStream.str() <<
          ", award = " << competition.getAward() << "\n";
}

int Menu::inputCompetitionId() {
  cout << "Find competition by:\n"
       << "1. Animal\n"
       << "2. Name\n"
       << "0. Go back\n";

  int choice = getChoice(0, 2);
  int id;
  string name;
  vector<Competition> competitions;
  try {
    switch(choice) {
      case 1:
        id = inputAnimalId();
        competitions = competitionGateway.findByAnimal(id);
        break;
      case 2:
        name;
        cout << "Enter name:\n";
        getline(cin, name);
        competitions = competitionGateway.findByName(name);
        break;
      case 0:
        return -1;
        break;
    }
    if (id == -1)
      return -1;
  } catch (exception& e) {
    cout << e.what() << "\n";
    getchar();
    return -1;
  }
  if (competitions.size() == 0) {
    cout << "Competitions not found\n";
    getchar();
    return -1;
  }
  cout << "Competitions:\n";
  printCompetitions(competitions);
  cout << "Which one?\n";
  choice = getChoice(1, competitions.size());
  id = competitions[choice - 1].getId();
  return id;
}

optional<tm> Menu::inputDate() {
  string dateStr;
  cout << "Enter date (YYYY-MM-DD):\n";
  tm date = {};
  getline(cin, dateStr);
  istringstream ss(dateStr);
  ss >> get_time(&date, "%Y-%m-%d");
  if (ss.fail()) {
    cout << "Wrong format, going back.\n" << endl;
    getchar();
    optional<tm> emptyDate;
    return emptyDate;
  }
  return date;
}