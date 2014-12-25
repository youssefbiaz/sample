#include <iomanip>
#include "movie.h"
#include "user.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

//function prototypes for reading in data from user's files
string readMainFile(ifstream &userDataFile, ifstream &movieDataFile, int argc, char* argv);
void readUserData(ifstream &userDataFile, Map<string, User*> &users);
void readMovieData(ifstream &movieDataFile, Map<string, Movie*> &movies, Map< string, Set<Movie*>* > &keywords);

//menus and search f'ns
int logInMenu();
string requestID(Map<string, User*> &users, bool &userFound);
void userOptions(Map<string, Movie*> &movies, Map< string, Set<Movie*>* > &keywords);
void searchMovies(Map<string, Movie*> &movies);
void searchByKeyword(Map<string, Movie*> &movies, Map< string, Set<Movie*>* > &keywords);
void keywordQueryMenu(Set<Movie*> &matches, int numMatches);
void createUser(string userID, Map<string, User*> &users, string userDataFilename);

//other helpful f'ns
bool equalStrings(string str1, string str2);

//  main function
int main(int argc, char* argv[]) {
  //declare maps of users, movies, and keywords
  Map<string, User*> users;
  Map<string, Movie*> movies;
  Map< string, Set<Movie*>* > keywords;

  //declare user and movie data files and read in all data into appropriate places
  ifstream userDataFile, movieDataFile;
  string userDataFilename =  readMainFile(userDataFile, movieDataFile, argc, argv[1]);
  readUserData(userDataFile, users);
  readMovieData(movieDataFile, movies, keywords);

  //this while loop simulates the first logInMenu and reacts to appropriate commands
  while (true) {
    int command = logInMenu();
    if (command == 1) {
      bool found = false;
      string userID = requestID(users, found);
      if (found) {
	cout << "Login successful!" << endl;
	userOptions(movies, keywords);
      } else {
	cout << "Sorry, that is not a valid ID. Please try again. Returning to main..." << endl;
      }
      continue;
    } else if (command == 2) {
      bool found = false;
      string userID;
      while (!found) {
	userID = requestID(users, found);
	if (found) {
	  cout << "Sorry, that user already exists. Please enter another ID." << endl;
	  found = false;
	} else {
	  break;
	}
      }
      createUser(userID, users, userDataFilename);
    } else {
      cout << "\nThanks for using this application!" << endl << endl;
      break;
    }
  }

  //the rest of this deletes the allocated data from users, movies, and keywords
  bool thereIsNext = true;
  // delete the pointers associated with users (in their value)
  try {
    users.first();
  } catch (NoSuchElementException &e) {
    thereIsNext = false;
  }
  while (thereIsNext) {
    delete users.getCurrentValue();
    try {
      users.next();
    } catch (NoSuchElementException &e) {
      thereIsNext = false;
    }
  }

  // delete the pointers associated with movies (in their value)
  thereIsNext = true;
  try {
    movies.first();
  } catch (NoSuchElementException &e) {
    thereIsNext = false;
  }
  while (thereIsNext) {
    delete movies.getCurrentValue();
    try {
      movies.next();
    } catch (NoSuchElementException &e) {
      thereIsNext = false;
    }
  }

  // delete the pointers associated with keywords (in their value)
  thereIsNext = true;
  try {
    keywords.first();
  } catch (NoSuchElementException &e) {
    thereIsNext = false;
  }
  while(thereIsNext) {
    delete keywords.getCurrentValue();
    try {
      keywords.next();
    } catch (NoSuchElementException &e) {
      thereIsNext = false;
    }
  }
  return 0;
}

// this f'n creates a new user based on userInput
void createUser(string userID, Map<string, User*> &users, string userDataFilename) {
  cout << "\nEnter your name: ";
  string userName;
  getline(cin, userName);

  cout << "\nCreating user... " << endl;
  bool thisIsTheFirstUser = false;
  try {
    users.first();
  } catch (NoSuchElementException &e) {
    thisIsTheFirstUser = true;
  }

  User* newUser = new User(userID, userName);
  users.add( userID , newUser );
  ofstream writeToFile;
  writeToFile.open(userDataFilename.c_str(), ios::out | ios::app);

  if (!thisIsTheFirstUser) {
    writeToFile << "\n";
  }

  writeToFile << "BEGIN " << userID;
  writeToFile << "\nNAME: " << userName;
  writeToFile << "\nEND";

  writeToFile.close();

  cout << "User created! Returning to main..." << endl;
}


//this algorithm searches by keyword and calls the menu that the user has following
// the display of the first match
void searchByKeyword(Map<string, Movie*> &movies, Map< string, Set<Movie*>* > &keywords) {
  Set<Movie*> matches;

  cout << "\nEnter the keyword: ";
  string key;
  getline( cin, key );
  int numMatches = 0;
  bool thereAreMovies = true;

  try {
    movies.first();
  } catch (NoSuchElementException &e) {
    thereAreMovies = false;
  }

  while (thereAreMovies) {
    bool found = equalStrings( movies.getCurrentValue()->getTitle() , key );
    if (found) {
      numMatches++;
      matches.add(movies.getCurrentValue());
      break;
    } else {
      try {
	movies.next();
      } catch (NoSuchElementException &e) {
	break;
      }
    }
  } 

  bool thereAreKeywords = true;
  try {
    keywords.first();
  } catch (NoSuchElementException &e) {
    thereAreKeywords = false;
  }

  while (thereAreKeywords) {
    bool found = equalStrings( key , keywords.getCurrentKey() );
    if (found) {
      keywords.getCurrentValue()->first();
      while (true) {
	bool notNewMatch = matches.contains(keywords.getCurrentValue()->getCurrent());
	if (!notNewMatch) {
	  matches.add(keywords.getCurrentValue()->getCurrent());
	  numMatches++;
	}
	try {
	  keywords.getCurrentValue()->next();
	} catch (NoSuchElementException &e) {
	  try {
	    keywords.next();
	  } catch (NoSuchElementException &e) {
	    thereAreKeywords = false;
	  }
	  break;
	}
      }
    } else {
      try {
	keywords.next();
      } catch (NoSuchElementException &e) {
	break;
      }
    }
  }

  if (numMatches == 0) {
    cout << "No match" << endl << endl;
  } else {
    cout << "\nNumber of matches: " << numMatches << endl;
    keywordQueryMenu(matches, numMatches);
  }

}

//this function is called by searchKeywords to output the first match to the keyword
// searched by the user and then request the user's next command
void keywordQueryMenu(Set<Movie*> &matches, int numMatches) {
  matches.first();
  bool userWantsToExit = false;
  while (!userWantsToExit) {
    cout << "\n" << matches.getCurrent()->getTitle() << endl;
    Set<string> allKeywords = matches.getCurrent()->getAllKeywords();
    allKeywords.first();
    while (true) {
      cout << allKeywords.getCurrent() << endl;
      try {
	allKeywords.next();
      } catch (NoSuchElementException &e) {
	break;
      }
    }

    bool thereIsNext = true;
    try {
      matches.next();
    } catch (NoSuchElementException &e) {
      thereIsNext = false;
    }

    bool userWantsNextFilm = false;
    while (!userWantsToExit && !userWantsNextFilm) {
      if (thereIsNext) {
	cout << "\n" << endl;
	cout << "Choose your next option: " << endl;
	cout << setw(4) << "1" << ". " << matches.getCurrent()->getTitle() << endl;
	cout << setw(4) << "2" << ". Return to menu" << endl;
	cout << "\nInput your command: ";
	int command;
	cin >> command;
	if (cin.fail()) {
	  cin.clear();
	  cin.ignore(1000, '\n');
	  cout << "Sorry, your command was not valid" << endl;
	  continue;
	} 
	cin.clear();
	cin.ignore(1000, '\n');
	cout << endl;
	if (command == 2) {
	  userWantsToExit = true;
	  cout << "Thanks for using the keyword search query!" << endl;
	} else if (command == 1) {
	  userWantsNextFilm = true;
	} else {
	  cout << "Sorry, that was not a valid command number" << endl;
	  continue;
	}
	  
      } else {
        cout << "\n" << endl;
        cout << "Choose your next option: " << endl;
        cout << setw(4) << "1" << ". Return to menu" << endl;
        cout << "\nInput your command: ";
        int command;
        cin >> command;
        if (cin.fail()) {
          cin.clear();
          cin.ignore(1000, '\n');
          cout << "Sorry, your command was not valid" << endl;
          continue;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout <<endl;
	if (command == 1) {
	  userWantsToExit = true;
	  cout << "\nThanks for using the keyword search query!" << endl;
	} else {
	  cout << "Sorry, that was not a valid command. Please try again" << endl;
	  continue;
	}
      }

      if (userWantsToExit) {
	break;
      }
    }
  }
}

/* this f'n takes two strings in as parameters and determines
   whether they are equal while ignoring capitalization.
   It iterates through each character of the string after forcing
   them all to lowercase
 */
bool equalStrings(string str1, string str2) {
  bool stringsAreEqual = false;
  int size1 = str1.size();
  int size2 = str2.size();
  if (size1 == size2) {
    stringsAreEqual = true;
    for (int i = 0; i < size1; i++) {
      char c1 = str1[i];
      char c2 = str2[i];
      if (tolower(c1) != tolower(c2)) {
	stringsAreEqual = false;
      }
    }
  } 
  return stringsAreEqual;
}

/* this f'n searches through the movies map for a film the user is looking for. 
   if found, it returns all the keywords in that movie's set of keywords
 */
void searchMovies(Map<string, Movie*> &movies) {
  cout << "\nEnter the movie title: ";
  string movieTitle;
  getline(cin, movieTitle);

  bool thereIsNext = true;
  bool thereAreMovies = true;
  try {
    movies.first();
  } catch (NoSuchElementException &e) {
    cout << "No movies found" << endl;
    thereAreMovies = false;
  }
  while (thereAreMovies) {
    bool found =  equalStrings(movies.getCurrentValue()->getTitle() , movieTitle );
    if (found) {
      cout << "\n" << movies.getCurrentValue()->getTitle() << endl;
      Set<string> allKeywords = movies.getCurrentValue()->getAllKeywords();
      try {
	allKeywords.first();
      } catch (NoSuchElementException &e) {
	cout << "no keywords associated with this film" << endl;
	break;
      }
      while (thereIsNext) {
	cout << allKeywords.getCurrent() << endl;
	try {
	  allKeywords.next();
	} catch (NoSuchElementException &e) {
	  cout << "\n" << endl;
	  break;
	}
      }
      break;
    } else {
      try {
	movies.next();
      } catch (NoSuchElementException &e) {
	cout << "No movie found" << endl;
	break;
      }
    }
  }
}

// this function simulates the menu after a user has logged in 
// based on the command requested, it calls appropriate f'ns for searches
void userOptions(Map<string, Movie*> &movies, Map< string, Set<Movie*>* > &keywords) {
  cout << "\nWelcome! ";
  while (true) {
    cout << "Here are your options: " << endl << endl;
    cout << setw(4) << "1" << ". Search for a movie by title" << endl;
    cout << setw(4) << "2" << ". Search for a movie by keyword" << endl;
    cout << setw(4) << "3" << ". Log out" << endl;
    cout << "\nEnter the corresponding command number of your command: ";
    int command;
    cin >> command;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Sorry, your command was not valid. Please try again..." << endl;
      continue;
    }
    cin.clear();
    cin.ignore(1000, '\n');
    cout << endl;

    if (command == 1) {
      searchMovies(movies);
    } else if (command == 2) {
      searchByKeyword(movies, keywords);
    } else if (command == 3) {
      cout << "Bye now, hope to see you again soon! Returning to main..." << endl;
      break; 
    } else {
      cout << "Sorry, you did not enter a valid command number. Please try again." << endl;
      continue;
    }
  }
}

// this function requests an ID from a user and returns the string (whatever they wrote)
// however, the f'n also modifies a boolean value to notify whether the userID was indeed found
string requestID(Map<string, User*> &users, bool &found) {
  found = true;
  cout << "\nUser ID: ";
  string IDrequested;
  cin >> IDrequested;
  try {
    users.get(IDrequested);
  } catch (NoSuchElementException &e) {
    found = false;
  }
  cin.clear();
  cin.ignore(1000, '\n');
  return IDrequested;
}

// first menu for log in options--return's user's command if valid
int logInMenu() {
  int command;
  while (true) {
    cout << "\nWelcome to the main menu! Here are your options:" << endl << endl;
    cout << setw(4) << "1" << ". Log in" << endl;
    cout << setw(4) << "2" << ". Create a new user" << endl;
    cout << setw(4) << "3" << ". Quit" << endl;
    cout << "\nEnter the corresponding command number of your command: ";
    cin >> command;
    cout << endl;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Sorry, your command was not valid. Please try again. Returning to main..." << endl;
      continue;
    }

    if (command <= 3 && command >= 1) {
      break;
    } else {
      cout << "Sorry, your command was not a valid option on the menu. Returning to main..." << endl;
      continue;
    }
  }
  return command;
}

// read in data from the movie 
void readMovieData(ifstream &movieDataFile, Map<string, Movie*> &movies, Map< string, Set<Movie*>* > &keywords) {
  string currentLine;
  while (getline(movieDataFile, currentLine)) {
    string command;
    stringstream ss;
    ss << currentLine;
    ss >> command;
    if (command == "BEGIN") {
      string movieTitle;
      string temp;
      ss >> temp;
      movieTitle = temp;
      while (ss >> temp) {
	movieTitle = movieTitle + " " + temp;
      }
      bool movieAlreadyExists = true;
      try {
	movies.get(movieTitle);
      } catch (NoSuchElementException &e) {
	movieAlreadyExists = false;
      }
      Movie* newMovie = NULL;
      if (!movieAlreadyExists) {
	newMovie = new Movie(movieTitle);
	movies.add(movieTitle, newMovie);
      }
      while (getline(movieDataFile, currentLine)) {
	stringstream ss2;
	ss2 << currentLine;
	ss2 >> command;
	if (command == "KEYWORD:") {
	  string keyword;
	  string temp;
	  ss2 >> temp;
	  keyword = temp;
	  while (ss2 >> temp) {
	    keyword = keyword + " " + temp;
	  }
	  if (newMovie != NULL) {
	    newMovie->addKeyword(keyword);
	  }
	  bool success = true;
	  try {
	    keywords.get(keyword);
	  } catch (NoSuchElementException &e) {
	    success = false;
	    Set<Movie*>* newMovieSet = new Set<Movie*>();
	    newMovieSet->add(newMovie);
	    keywords.add(keyword, newMovieSet);
	  }
	  if (success) {
	    keywords.get(keyword)->add(newMovie);
	  }
	} else if (command == "END") {
	  break;
	}
      }
    }
  }
}

// read in data from the user file
void readUserData(ifstream &userDataFile, Map<string, User*> &users) {
  string currentLine;
  while (getline(userDataFile, currentLine)) {
    string command;
    stringstream ss;
    ss << currentLine;
    ss >> command;
    if (command == "BEGIN") {
      string userID;
      ss >> userID;
      while (getline(userDataFile, currentLine)) {
	stringstream ss2;
	ss2 << currentLine;
	ss2 >> command;
	if (command == "NAME:") {
	  string userName = "";
	  string temp;
	  ss2 >> temp;
	  userName = temp;
	  while (ss2 >> temp) {
	    userName = userName + " " + temp;
	  }
	  bool userAlreadyExists = true; 
	  try {
	    users.get(userName);
	  } catch (NoSuchElementException &e) {
	    userAlreadyExists = false;
	  }
	  if (!userAlreadyExists) {
	    User* newUser = new User(userID, userName);
	    users.add(userID, newUser);
	  }
	} else if (command == "END") {
	  break;
	} else {
	  cout << "User data file not valid. Please logout and revisit your file." << endl;
	}
      }
    }
  }
}

//reads in the main file passed in from the command line
string readMainFile(ifstream &userDataFile, ifstream &movieDataFile, int argc, char* argv) {
  if (argc < 2) {
    cout << "Sorry, too few args--need the main data file." << endl;
    exit(0);
  }

  ifstream mainDataFile;
  mainDataFile.open(argv);
  if (mainDataFile.fail()) {
    cout << "Sorry, invalid file" << endl;
    exit(0);
  }
  
  string userDataFilename, movieDataFilename;
  getline(mainDataFile, userDataFilename);
  getline(mainDataFile, movieDataFilename);

  userDataFile.open(userDataFilename.c_str());
  if (userDataFile.fail()) {
    cout << "Sorry, invalid filename for users" << endl;
    exit(0);
  }

  movieDataFile.open(movieDataFilename.c_str());
  if (movieDataFile.fail()) {
    cout << "Sorry, invalid filename for movie data" << endl;
    exit(0);
  }
  return userDataFilename;
}
