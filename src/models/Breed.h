#include <string>


class Breed {

  public:

    Breed(int id, string name) {
      this->id = id;
      this->name = name;
    }

    string toString() {
        std::ostringstream oss;
        oss << "[id = " << id <<
               "'; name = '" << name << "'];}";
        return oss.str();
    }

    int getId() {
      return id;
    }

    string getName() {
      return name;
    }

    void setName(string name) {
      this->name = name;
    }

  private:
    int id;
    string name;

};