#pragma once
#include "../gateways/BreedDatabaseGateway.h"
#include "../models/Breed.h"

class BreedRepository {

  public:

    BreedRepository(BreedDatabaseGateway breedDatabaseGateway) {
      gateway = breedDatabaseGateway;
    }

    BreedRepository() {
      gateway = nullptr;
    }

    Breed add(string name) {
      vector<vector<string>> breedStrings = gateway.create(name);
      return parseBreed(breedStrings[0]);
    }

    Breed get(int id) {
      vector<vector<string>> breedStrings = gateway.get(id);
      return parseBreed(breedStrings[0]);
    } 

    Breed update(int id, string name) {
      vector<vector<string>> breedStrings = gateway.update(id, name);
      return parseBreed(breedStrings[0]);
    }

    void remove(int id) {
      gateway.remove(id);
    }

    vector<Breed> getAll() {
      vector<vector<string>> result = gateway.getAll();
      return parseBreeds(result);
    }

    vector<Breed> findByName(string name) {
      vector<vector<string>> result = gateway.findByName(name);
      return parseBreeds(result);
    }

  private:
    BreedDatabaseGateway gateway;

    vector<Breed> parseBreeds(vector<vector<string>> breedsStrings) {
      vector<Breed> breeds;
      for (int i = 0; i < breedsStrings.size(); i++) {
        breeds.push_back(parseBreed(breedsStrings[i]));
      }
      return breeds;
    }

    Breed parseBreed(vector<string> strings) {
      int id = atoi(strings[0].c_str());
      string name = strings[1];
      return Breed(id, name);
    }
     
};