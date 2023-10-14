#pragma once
#include "../gateways/AnimalDatabaseGateway.h"
#include "../models/Animal.h"

class AnimalRepository {

  public:

    AnimalRepository(AnimalDatabaseGateway applicationDatabaseGateway) {
      gateway = applicationDatabaseGateway;
    }

    AnimalRepository() {
      gateway = nullptr;
    }

    Animal add(string name, optional<int> age, Gender gender, int breedId, string exterior, string pedigree, string veterinarian, optional<int> ownerId) {
      vector<vector<string>> applicationStrings = gateway.create(name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
      return parseAnimal(applicationStrings[0]);
    }

    Animal get(int id) {
      vector<vector<string>> applicationStrings = gateway.get(id);
      return parseAnimal(applicationStrings[0]);
    } 

    Animal update(int id, string name, optional<int> age, Gender gender, int breedId, string exterior, string pedigree, string veterinarian, optional<int> ownerId) {
      vector<vector<string>> applicationStrings = gateway.update(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
      return parseAnimal(applicationStrings[0]);
    }

    void remove(int id) {
      gateway.remove(id);
    }

    vector<Animal> getAll() {
      vector<vector<string>> result = gateway.getAll();
      return parseAnimals(result);
    }

    vector<Animal> findByClientId(optional<int> clientId) {
      vector<vector<string>> result = gateway.findByClientId(clientId);
      return parseAnimals(result);
    }

    vector<Animal> findByName(string name) {
      vector<vector<string>> result = gateway.findByName(name);
      return parseAnimals(result);
    }

    vector<Animal> findByBreedId(int breedId) {
      vector<vector<string>> result = gateway.findByBreedId(breedId);
      return parseAnimals(result);
    }

  private:
    AnimalDatabaseGateway gateway;

    vector<Animal> parseAnimals(vector<vector<string>> applicationsStrings) {
      vector<Animal> applications;
      for (int i = 0; i < applicationsStrings.size(); i++) {
        applications.push_back(parseAnimal(applicationsStrings[i]));
      }
      return applications;
    }

    Animal parseAnimal(vector<string> strings) {
      cout << "aboaba ||\n";
      for (int i = 0; i < strings.size(); i++)
        cout << strings[i] << "\n";
      cout << "aboaba ||\n";
      int id = atoi(strings[0].c_str());
      string name = strings[1];
      optional<int> age;
      if (strcmp(strings[1].c_str(), strings[2].c_str()) != 0)
        age = atoi(strings[2].c_str());;
      Gender gender = getGender(strings[3]);
      int breedId = atoi(strings[4].c_str());
      string exterior = strings[5];
      string pedigree = strings[6];
      string veterinarian = strings[7];
      optional<int> ownerId;
      if (strcmp(strings[7].c_str(), strings[8].c_str()) != 0)
        ownerId = atoi(strings[8].c_str());
      return Animal(id, name, age, gender, breedId, exterior, pedigree, veterinarian, ownerId);
    }

    Gender getGender(string gender) {
      return gender == "male" ? Gender::male : Gender::female;
    }
     
};