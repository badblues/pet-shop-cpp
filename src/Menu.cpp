#include "Menu.h"

//CLIENTS

void Menu::printClients() {
  try {
    vector<Client> clients = clientGateway.getAll();
    cout << "\033[2J\033[H";
    for (int i = 0; i < clients.size(); i++) {
      cout << i + 1 << ". " << clients[i].getName() << ", address: " << clients[i].getAddress() << "\n";
    }
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
    cout << "Success, new client: " << client.getName() << ", address: " << client.getAddress() << "\n";
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
    cout << "Success, new data: " << updatedClient.getName() << ", address: " << updatedClient.getAddress() << "\n";
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

void Menu::printBreeds() {
  try {
    vector<Breed> breeds = breedGateway.getAll();
    cout << "\033[2J\033[H";
    for (int i = 0; i < breeds.size(); i++) {
      cout << i + 1 << ". " << breeds[i].getName() << "\n";
    }
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
    cout << "Success, new breed: " << breed.getName() << "\n";
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
    cout << "Success, new data: " << updatedBreed.getName() << "\n";
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

void Menu::printEmployees() {
  try {
    vector<Employee> employees = employeeGateway.getAll();
    cout << "\033[2J\033[H";
    for (int i = 0; i < employees.size(); i++) {
      cout << i + 1 << ". " << employees[i].getName()
           << ", address: " << employees[i].getAddress()
           << ", position: " << employees[i].getPosition()
           << ", salary: " << employees[i].getSalary() << "$"
           << "\n";
    }
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
    cout << "Success, new employee: "  << employee.getName()
           << ", address: " << employee.getAddress()
           << ", position: " << employee.getPosition()
           << ", salary: " << employee.getSalary() << "$"
           << "\n";
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
    cout << "Success, new data: " << updatedEmployee.getName()
           << ", address: " << updatedEmployee.getAddress()
           << ", position: " << updatedEmployee.getPosition()
           << ", salary: " << updatedEmployee.getSalary() << "$"
           << "\n";
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
    for (int i = 0; i < employees.size(); i++) {
      cout << i + 1 << ". " << employees[i].getName()
           << ", address: " << employees[i].getAddress()
           << ", position: " << employees[i].getPosition()
           << ", salary: " << employees[i].getSalary() << "$"
           << "\n";
    }
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
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

void Menu::printApplications() {
  try {
    vector<Application> applications = applicationGateway.getAll();
    cout << "got applications\n";
    getchar();
    cout << "\033[2J\033[H";
    for (int i = 0; i < applications.size(); i++) {
      string clientName = clientGateway.get(applications[i].getClientId()).getName();
      string employeeName = employeeGateway.get(applications[i].getEmployeeId()).getName();
      string breed = breedGateway.get(applications[i].getBreedId()).getName();
      string genderStr = "null";
      Gender* gender = applications[i].getGender();
      if (gender != nullptr)
        genderStr = *gender == Gender::male ? "male" : "female";
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
  Gender* gender = inputGender();
  time_t currentTime = time(nullptr);
  tm applicationDate = *localtime(&currentTime);

  try {
    Application application = applicationGateway.create(clientId, employeeId, breedId, gender, applicationDate, false);
    string clientName = clientGateway.get(application.getClientId()).getName();
    string employeeName = employeeGateway.get(application.getEmployeeId()).getName();
    string breed = breedGateway.get(application.getBreedId()).getName();
    string genderStr = "null";
    Gender* gender = application.getGender();
    if (gender != nullptr)
      genderStr = *gender == Gender::male ? "male" : "female";
    tm applicationDate = application.getApplicationDate();
    stringstream dateStream;
    dateStream << std::put_time(&applicationDate, "%Y-%m-%d");
    cout << "Success, new application: " << clientName
          << ", employee: " << employeeName
          << ", breed: " << breed
          << ", gender: " << genderStr
          << ", applicationDate: " << dateStream.str()
          << ", completed: " << application.getCompleted()
          << "\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
    getchar();
  }
}

Gender* Menu::inputGender() {
  string answer;
  cout << "Enter gender: (male/female/not important)\n";
  getline(cin, answer);
  while(answer != "male" && answer != "female" && answer != "not important"){
    cout << "Invalid input, try again:\n";
    getline(cin, answer);
  }
  if (answer == "not important")
    return nullptr;
  return answer == "male" ? new Gender(Gender::male) : new Gender(Gender::female);
}