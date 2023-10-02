#pragma once
#include <iostream>
#include <sql.h>
#include "./models/Client.h"
#include "./gateways/ClientDatabaseGateway.h"

using namespace std;


class Menu {

  public:

    Menu(SQLHDBC hDbc) {
      this-> hDbc;
      this->clientGateway = ClientDatabaseGateway(hDbc);
    }

    void showMenu() {
      int choice;
      cout << "menu:\n" <<
              "1. create client\n" <<
              "2. get clients\n";
      cin >> choice;
      if (choice == 1) {
        string str1;
        string str2;
        cout << "enter name:\n";
        cin >> str1;
        cout << "enter address:\n";
        cin >> str2;
        clientGateway.create(str1, str2);
      } else if (choice == 2) {
        vector<Client> clients = clientGateway.getAll();
        for (int i = 0; i < clients.size(); i++) {
          cout << clients[i].toString() << "\n";
        }
      }
    }

  private:
    SQLHDBC hDbc;
    ClientDatabaseGateway clientGateway;
};