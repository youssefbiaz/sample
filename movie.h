#ifndef MOVIE_H
#define MOVIE_H
#include <string>
#include "lib/set.h"

class Movie {
public:
  Movie(); // default constructor
  Movie(std::string title); // constructor for a movie w a given title
  Movie(const Movie &other); // copy constructor
  ~Movie(); // destructor
  std::string getTitle () const; //returns the title of a movie
  void addKeyword (std::string keyword);
  /*Adds the (free-form) keyword to this movie. 
If the exact same keyword (up to capitalization) was already associated
with the movie, then the keyword is not added again.*/

  Set<std::string> getAllKeywords () const;
private:
  Set<std::string> keyWords;
  std::string movieTitle;
};

#endif
