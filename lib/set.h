#ifndef SET_H
#define SET_H
#include "map.h"

template <class T>
class Set {
public:
  Set();
  Set(const Set<T> &other); // deep copy constructor
  ~Set(); // destructor
  const Set<T> &operator=(const Set<T> &other); // assignment operator overload
  int size () const; // returns # of elems in set
  void add (const T &item); // adds the item to the set
  void remove (const T &item); // removes
  bool contains(const T item) const;
  void merge (const Set<T> &other);
  void first();
  void next();
  const T & getCurrent();
  Set<T> setUnion(const Set<T> &other) const;
  Set<T> setIntersection(const Set<T> &other) const;
private:
  Map<T, int> internalStorage;
};

#include "setImpl.h"

#endif
