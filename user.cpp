#include "user.h"

User::User() {
  id = "";
  userName = "";
}

User::User(std::string ID, std::string name) {
  id = ID;
  userName = name;
}

User::User(const User &other) {
  this->id = other.id;
  this->userName = other.userName;
}

User::~User() {}

std::string User::getID() const {
  return id;
}

std::string User::getName() const {
  return userName;
}
