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
  }
}

void Menu::updateClient() {
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  vector<Client> clients = clientGateway.findByName(name);
  if (clients.size() == 0) {
    cout << "Client with that name wasn't found\n";
    getchar();
    return;
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
  }
}

void Menu::removeClient() {
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  vector<Client> clients = clientGateway.findByName(name);
  if (clients.size() == 0) {
    cout << "Client with that name wasn't found\n";
    getchar();
    return;
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
  try {
    clientGateway.remove(id);
    cout << "Success!\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
  }
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
  }
}

void Menu::updateBreed() {
  string name;
  cout << "Enter name of the breed:\n";
  getline(cin, name);
  vector<Breed> breeds = breedGateway.findByName(name);
  if (breeds.size() == 0) {
    cout << "Breed with that name wasn't found\n";
    getchar();
    return;
  }
  int id = breeds[0].getId();
  cout << "Enter name:\n";
  getline(cin, name);
  try {
    Breed updatedBreed = breedGateway.update(id, name);
    cout << "Success, new data: " << updatedBreed.getName() << "\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
  }
}

void Menu::removeBreed() {
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  vector<Breed> breeds = breedGateway.findByName(name);
  if (breeds.size() == 0) {
    cout << "Breed with that name wasn't found\n";
    getchar();
    return;
  }
  int id = breeds[0].getId();
  try {
    breedGateway.remove(id);
    cout << "Success!\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
  }
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
  }
}

void Menu::updateEmployee() {
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  vector<Employee> employees = employeeGateway.findByName(name);
  if (employees.size() == 0) {
    cout << "Employee with that name wasn't found\n";
    getchar();
    return;
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
  }
}

void Menu::removeEmployee() {
  string name;
  cout << "Enter name:\n";
  getline(cin, name);
  vector<Employee> employees = employeeGateway.findByName(name);
  if (employees.size() == 0) {
    cout << "Employee with that name wasn't found\n";
    getchar();
    return;
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
  try {
    employeeGateway.remove(id);
    cout << "Success!\n";
    getchar();
  } catch(const exception& e) {
    cout << e.what() << "\n";
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
  }
}
