#include "movie.h"

//default constructor
Movie::Movie() {
  movieTitle = "";
}

//constructor for movie with a given title
Movie::Movie(std::string title) {
  movieTitle = title;
}

//copy constructor
Movie::Movie(const Movie &other) {
  this->keyWords = other.keyWords;
  this->movieTitle = other.movieTitle;
}

//destructor
Movie::~Movie() {}

//returns title of a movie
std::string Movie::getTitle() const {
  return movieTitle;
}

// adds the keyword to this movie
void Movie::addKeyword(std::string keyword) {
  keyWords.add(keyword);
}

//gets all the keywords and returns them to a set
Set<std::string> Movie::getAllKeywords() const {
  return keyWords;
}

