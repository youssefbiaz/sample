#ifndef USER_H
#define USER_H
#include <string>

class User {
public:
  User();
  User(std::string ID, std::string name);
  /* constructor that generates a user with given ID and name. ID should be unique.*/
  User(const User &other); //copy constructor
  ~User(); //destructor
  std::string getID() const;
  std::string getName() const;

private:
  std::string id, userName;
};

#endif
