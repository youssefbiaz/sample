#ifndef MAP_H
#define MAP_H
#include "nosuchelement.h"

template <class keyType, class valueType>
struct MapItem { // struct--one item in the list
  keyType key;
  valueType value;
  MapItem<keyType, valueType> *prev, *next; //pointers to either side
  MapItem<keyType, valueType> (keyType k, valueType v, MapItem* nxt, MapItem* prv) { // special constructor
    key = k;
    value = v;
    next = nxt;
    prev = prv;
  }
};

template <class keyType, class valueType>
class Map { // class that will create the linked list
public:
  Map (); // constructor
  Map(const Map<keyType, valueType> &other); //deep copy constructor
  ~Map(); // destructor 
  const Map<keyType, valueType> &operator=(const Map<keyType, valueType> &other);
  int size () const; // returns number of mapItems
  void add (const keyType &key, const valueType &value); // adds a mapItem
  void remove (const keyType &key); // removes a mapItem
  const valueType &get(const keyType &key) const; //gets an item
  void merge (const Map<keyType, valueType> &other); // merges w other list
  void first(); // resets traversor to equal head, or THROWS EXCEPTION
  void next();
  const keyType & getCurrentKey();
  const valueType & getCurrentValue();
private:
  MapItem <keyType, valueType> *head, *tail; //head and tail ptrs
  MapItem <keyType, valueType> *traversor; //ptr that traverses through list
};

#include "mapImpl.h"

#endif
