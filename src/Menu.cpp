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

void Menu::updateClient() {
  int id = inputClientId();
  if (id == -1)
    return;
  string name;
  string address;
  cout << "Enter name:\n";
  getline(cin, name);
  cout << "Enter address:\n";
  getline(cin, address);
  try {
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
    clientGateway.remove(id);
    cout << "Success!\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
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
  if (clients.size() > 1) {
    cout << "What client?\n";
    int choice = getChoice(1, clients.size());
    id = clients[choice - 1].getId();
  } else {
    id = clients[0].getId();
  }
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

void Menu::updateBreed() {
  int id = inputBreedId();
  if (id == -1)
    return;
  string name;
  cout << "Enter name:\n";
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
    breedGateway.remove(id);
    cout << "Success!\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
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

void Menu::updateEmployee() {
  int id = inputEmployeeId();
  if (id == -1)
    return;
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
    employeeGateway.remove(id);
    cout << "Success!\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
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
  if (employees.size() > 1) {
    cout << "What employee?\n";
    int choice = getChoice(1, employees.size());
    id = employees[choice - 1].getId();
  } else {
    id = employees[0].getId();
  }
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

void Menu::updateApplication() {
  int id = inputApplicationId();
  if (id == -1)
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
    int employeeId = inputEmployeeId();
    if (employeeId == -1)
      return;
    int breedId = inputBreedId();
    if (breedId == -1)
      return;
    optional<Gender> gender = inputGender();
    time_t currentTime = time(nullptr);
    tm applicationDate = *localtime(&currentTime);
    Application updatedApplication = applicationGateway.update(id, clientId, employeeId, breedId, gender, applicationDate, completed);
    cout << "Success!\n";
    printApplication(updatedApplication);
    getchar();
  } catch(std::exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
}

void Menu::closeApplication() {
  int id = inputApplicationId();
  if (id == -1)
    return;
  try {
    Application oldApplication = applicationGateway.get(id);
    if (oldApplication.getCompleted()) {
      cout << "Already closed\n";
      getchar();
      return;
    }
    int clientId = oldApplication.getClientId();
    int employeeId = oldApplication.getEmployeeId();
    int breedId = oldApplication.getBreedId();
    optional<Gender> gender = oldApplication.getGender();
    tm date = oldApplication.getApplicationDate();
    Application updatedApplication = applicationGateway.update(id, clientId, employeeId, breedId, gender, date, true);
    cout << "Success!\n";
    printApplication(updatedApplication);
    getchar();
  } catch(std::exception& e) {
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
    applicationGateway.remove(id);
    cout << "Success!\n";
    getchar();
  } catch(std::exception& e) {
    cout << e.what() << "\n";
    getchar();
    return;
  }
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
    string employeeName = employeeGateway.get(applications[i].getEmployeeId()).getName();
    string breed = breedGateway.get(applications[i].getBreedId()).getName();
    string genderStr = "";
    optional<Gender> gender = applications[i].getGender();
    if (gender.has_value())
      genderStr = gender.value() == Gender::male ? "male" : "female";
    tm applicationDate = applications[i].getApplicationDate();
    stringstream dateStream;
    dateStream << std::put_time(&applicationDate, "%Y-%m-%d");
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
  string employeeName = employeeGateway.get(application.getEmployeeId()).getName();
  string breed = breedGateway.get(application.getBreedId()).getName();
  string genderStr = "";
  optional<Gender> gender = application.getGender();
  if (gender.has_value())
      genderStr = gender.value() == Gender::male ? "male" : "female";
  tm applicationDate = application.getApplicationDate();
  stringstream dateStream;
  dateStream << std::put_time(&applicationDate, "%Y-%m-%d");
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
  } catch (std::exception& e) {
    cout << e.what() << "\n";
    getchar;
    return -1;
  }
  cout << "Applications:\n";
  printApplications(applications);
  if (applications.size() > 1) {
    cout << "Which one?\n";
    choice = getChoice(1, applications.size());
    id = applications[choice - 1].getId();
  } else {
    id = applications[0].getId();
  }
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