#pragma once
#include "../gateways/ClientDatabaseGateway.h"
#include "../models/Client.h"

class ClientRepository {

  public:

    ClientRepository(ClientDatabaseGateway clientDatabaseGateway) {
      gateway = clientDatabaseGateway;
    }

    ClientRepository() {
      gateway = nullptr;
    }

    Client add(string name, string address) {
      vector<vector<string>> clientStrings = gateway.create(name, address);
      return parseClient(clientStrings[0]);
    }

    Client get(int id) {
      vector<vector<string>> clientStrings = gateway.get(id);
      return parseClient(clientStrings[0]);
    } 

    Client update(int id, string name, string address) {
      vector<vector<string>> clientStrings = gateway.update(id, name, address);
      return parseClient(clientStrings[0]);
    }

    void remove(int id) {
      gateway.remove(id);
    }

    vector<Client> getAll() {
      vector<vector<string>> result = gateway.getAll();
      return parseClients(result);
    }

    vector<Client> findByName(string name) {
      vector<vector<string>> result = gateway.findByName(name);
      return parseClients(result);
    }

  private:
    ClientDatabaseGateway gateway;

    vector<Client> parseClients(vector<vector<string>> clientsStrings) {
      vector<Client> clients;
      for (int i = 0; i < clientsStrings.size(); i++) {
        clients.push_back(parseClient(clientsStrings[i]));
      }
      return clients;
    }

    Client parseClient(vector<string> strings) {
      int id = atoi(strings[0].c_str());
      string name = strings[1];
      string address = strings[2];
      return Client(id, name, address);
    }
     
};